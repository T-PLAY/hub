#include "ViewerMqtt.hpp"

#ifdef HUB_USE_MQTT

#    include <algorithm>
#    include <functional>
#    include <mqtt/client.h>
#    include <string>

#    include "io/input/InputStream.hpp"

namespace hub {
namespace client {

ViewerMqtt::ViewerMqtt( const std::string& name,
                        ViewerHandler&& viewerHandler,
                        const std::string& ipv4,
                        int port ) :
    ViewerInterface( name, std::move( viewerHandler ), ipv4, port ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "viewer:" ) + m_name,
                                mqtt::create_options( MQTTVERSION_5 ) ) )

{

    m_thread = std::thread( [this]() {
        DEBUG_MSG( "[ViewerMqtt] trying to connect to server at ipv4 "
                   << m_serverIpv4 << " and port " << m_serverPort );

        m_client->connect();
        assert( m_client->is_connected() );

        ////    assert(rsp.is_session_present());

        m_viewerHandler.onServerConnected( m_serverIpv4.c_str(), m_serverPort );
        m_serverConnected = true;
        printStatus();

        DEBUG_MSG( "[ViewerMqtt] connected to server at ipv4 " << m_serverIpv4 << " and port "
                                                               << m_serverPort );

        m_outputMsgPtr =
            mqtt::make_message( input::InputStreamMqtt::s_topicViewer + m_name, "active" );
        m_outputMsgPtr->set_retained( true );
        m_outputMsgPtr->set_qos( 2 );
        m_client->publish( m_outputMsgPtr );

        m_client->subscribe( input::InputStreamMqtt::s_topicEvents );

        m_client->start_consuming();
        while ( !m_stopThread ) {
            m_inputMsgPtr.reset();
            if ( m_client->try_consume_message_for( &m_inputMsgPtr,
                                                    std::chrono::milliseconds( 100 ) ) ) {
                assert( m_inputMsgPtr != nullptr );
                const auto& payload = m_inputMsgPtr->get_payload_str();

                const io::StreamBase::ServerMessage message =
                    io::StreamBase::ServerMessage( payload.at( 0 ) - '0' );

                switch ( message ) {
                case io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER: {
                    std::cout << std::endl;
                    ++m_nStreamer;
                    const std::string streamName = payload.substr( 1, payload.size() - 1 );
                    DEBUG_MSG( "[ViewerMqtt] new streamer '" << streamName << "'" );

                    sensor::SensorSpec sensorSpec;
                    try {
                        input::InputStreamMqtt inputStream(
                            streamName, m_serverPort, m_serverIpv4 );
                        inputStream.read( sensorSpec );
                    }
                    catch ( io::StreamBase::exception& ex ) {
                        DEBUG_MSG( "[ViewerMqtt] unable to connect with new stream '"
                                   << streamName << "' suddenly close after registration" );
                        continue;
                    }

                    DEBUG_MSG( "[ViewerMqtt] sensorSpec '" << sensorSpec << "'" );

                } break;
                case io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER: {
                    const std::string streamName = payload.substr( 1, payload.size() - 1 );
                    DEBUG_MSG( "[ViewerMqtt] del streamer '" << streamName << "'" );

                    delStream( streamName );

                } break;
                case io::StreamBase::ServerMessage::VIEWER_SET_PROPERTY: {
                } break;
                default:
                    assert( false );
                    break;
                }
            }

        } // while ( !m_stopThread )
        m_client->stop_consuming();

        m_outputMsgPtr->set_payload( "inactive" );
        m_client->publish( m_outputMsgPtr );
    } ); // m_thread
}

ViewerMqtt::~ViewerMqtt() {

    m_viewerHandler.onServerDisconnected( m_serverIpv4.c_str(), m_serverPort );
    m_serverConnected = false;
    printStatus();

    assert( !m_stopThread );
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
}

} // namespace client
} // namespace hub

#endif
