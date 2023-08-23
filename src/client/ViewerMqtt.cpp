#include "ViewerMqtt.hpp"

// #include <regex>
// #include <sstream>
#include <algorithm>
#include <functional>
#include <mqtt/client.h>
#include <string>

// #include "InputSensor.hpp"
//#include <client/StreamViewer.hpp>
#include "io/input/InputStream.hpp"
//#include <io/input/InputStreamMqtt.hpp>

#include <utils/Utils.hpp>


namespace hub {
namespace client {
// namespace viewer {}


// const std::string TOPIC_VIEWER {"viewer"};
// const std::string TOPIC_VIEWER { "streamName" };
// const int QOS              = 1;
// const int N_RETRY_ATTEMPTS = 5;
// const std::string CLIENT_ID( "viewer" );

ViewerMqtt::ViewerMqtt(std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, int )> onServerNotFound,
    std::function<void( const char*, int )> onServerConnected,
    std::function<void( const char*, int )> onServerDisconnected,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char*, const char*, int, const Any& )> onSetProperty,
    std::function<void( const char* )> onLogMessage ,
    const std::string& ipv4,
    int port) :
    ViewerInterface(
                     onNewStreamer,
                     onDelStreamer,
                     onServerNotFound,
                     onServerConnected,
                     onServerDisconnected,
                     onNewAcquisition,
                     onSetProperty,
                     onLogMessage,
        ipv4,
        port
        ),
    m_hostName( hub::utils::getHostname() ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "viewer" ) + m_hostName,
                                mqtt::create_options( MQTTVERSION_5 ) ) )
//    m_client(new mqtt::async_client(ipv4 + ":" + std::to_string(port), CLIENT_ID +
//    std::to_string((long)this), mqtt::create_options(MQTTVERSION_5))) m_client(new
//    mqtt::async_client(ipv4 + ":" + std::to_string(port), CLIENT_ID + std::to_string((long)this)))

{

    m_thread = std::thread( [this]() {
        //            try {
        DEBUG_MSG( "[ViewerMqtt] trying to connect to server at ipv4 " << m_ipv4 << " and port "
                                                                       << m_port );

        m_client->connect();
        assert( m_client->is_connected() );

        //    m_client->start_consuming();

        //    user_callback cb;
        //    m_client->set_callback(cb);

        //    auto tok = m_client->connect();
        //    auto rsp = tok->get_connect_response();
        //    if (!rsp.is_session_present())
        //        m_client->subscribe(TOPIC_VIEWER, 2)->wait();
        ////    assert(rsp.is_session_present());
        //    assert(m_client->is_connected());
        //    assert(m_msgPtr == nullptr);

        m_onServerConnected( m_ipv4.c_str(), m_port );
        m_serverConnected = true;

        DEBUG_MSG( "[ViewerMqtt] connected to server at ipv4 " << m_ipv4 << " and port "
                                                                       << m_port );

        //        const auto hostName = getHostname();
        m_outputMsgPtr = mqtt::make_message( input::InputStreamMqtt::s_topicViewer + m_hostName, "active" );
        //        m_outputMsgPtr->set_payload("active");
        //        m_client->subscribe( "viewer/" + hostName );
        m_client->publish( m_outputMsgPtr );

        m_client->subscribe( input::InputStreamMqtt::s_topicEvents );

        m_client->start_consuming();
        while ( !m_stopThread ) {
            //            while (true) {
            if ( m_client->try_consume_message_for( &m_inputMsgPtr,
                                                    std::chrono::milliseconds( 100 ) ) ) {
                //            if ( m_msgPtr != nullptr ) {
                //                assert(readed);
                //                m_msgPtr = m_client->consume_message();
                assert( m_inputMsgPtr != nullptr );
                const auto& payload = m_inputMsgPtr->get_payload_str();
                //                DEBUG_MSG( "[ViewerMqtt] receive message : '" << payload << "'" );
                //                m_inputMsgPtr.reset();

                const io::StreamInterface::Message message = io::StreamInterface::Message( payload.at( 0 ) - '0' );

                switch ( message ) {
                case io::StreamInterface::Message::NEW_STREAM: {
                    //                    m_client->subscribe( s_topicEvents + "/streamName" );
                    //                    m_inputMsgPtr = m_client->consume_message();
                    //                    assert( m_inputMsgPtr != nullptr );
                    const std::string streamName = payload.substr( 1, payload.size() - 1 );
                    //                    std::cout << "[ViewerMqtt] new streamer '" << streamName
                    //                    << "'" << std::endl;
                    DEBUG_MSG( "[ViewerMqtt] new streamer '" << streamName << "'" );

                    //                std::string streamName;
                    SensorSpec sensorSpec;
                    input::InputStreamMqtt inputStream( streamName, m_ipv4, m_port );
                    try {
                        inputStream.read( sensorSpec );
                    }
                    catch ( io::StreamInterface::exception& ex ) {
                        //                        DEBUG_MSG( "[ViewerMqtt] catch Stream exception '"
                        //                        << ex.what() << "'" );
                        DEBUG_MSG( "[ViewerMqtt] unable to connect with new stream '"
                                   << streamName << "' suddenly close after registration" );
                        continue;
                    }

                    //                    std::cout << "[ViewerMqtt] sensorSpec '" << sensorSpec <<
                    //                    "'"  << std::endl;
                    DEBUG_MSG( "[ViewerMqtt] sensorSpec '" << sensorSpec << "'" );

                    assert( m_streams.find( streamName ) == m_streams.end() );

                    if ( m_onNewStreamer ) {
                        //                    assert(m_onDelStreamer);

                        m_streams[streamName] =
                            std::make_unique<viewer::StreamViewer<input::InputStreamMqtt>>(
                                m_ipv4,
                                m_port,
                                streamName,
                                sensorSpec,
                                m_onNewStreamer,
                                m_onDelStreamer,
                                m_onNewAcquisition,
                                m_onLogMessage );
                    }

                    // prevent all son the father is comming

                    // wait for client init sensorSpec with main thread context (async)
                    // for unity side (update context different of static event function)
                    //                std::this_thread::sleep_for( std::chrono::milliseconds( 100 )
                    //                );
                } break;
                case io::StreamInterface::Message::DEL_STREAM: {
                    //                    std::string streamName;
                    const std::string streamName = payload.substr( 1, payload.size() - 1 );
                    //                    m_sock.read( streamName );
                    //                    assert( m_streams.find( streamName ) != m_streams.end() );
                    //                    const SensorSpec & sensorSpec =
                    //                    m_streams.at(streamName).getSensorSpec(); m_sock.read(
                    //                    sensorSpec );
                    DEBUG_MSG( "[ViewerServer] del streamer '" << streamName << "'" );

                    if ( m_onNewStreamer ) {
                        assert( m_onDelStreamer );
                        //                        assert( m_streams.find( streamName ) !=
                        //                        m_streams.end() );
                        if ( m_streams.find( streamName ) != m_streams.end() ) {
                            m_streams.erase( streamName );
                        }
                    }

                    // prevent all son the father is leaving

                    // wait for client init sensorSpec with main thread context (async)
                    // for unity side (update context different of static event function)
                    //                    std::this_thread::sleep_for( std::chrono::milliseconds(
                    //                    100 ) );
                } break;
                case io::StreamInterface::Message::SET_PROPERTY: {
                } break;
                default:
                    assert( false );
                    break;
                }
            }

        } // while ( !m_stopThread )
        m_client->stop_consuming();

        m_outputMsgPtr->set_payload( "inactive" );
        //        m_client->subscribe( "viewer/" + hostName );
        m_client->publish( m_outputMsgPtr );
    } ); // m_thread
}

ViewerMqtt::~ViewerMqtt() {
    //    DEBUG_MSG( "[ViewerMqtt] ~ViewerMqtt()" );

    //    m_client->unsubscribe(TOPIC_VIEWER)->wait();
    //    m_client->stop_consuming();
    //    assert(m_client->is_connected());
    //    m_client->disconnect();

    //    try {
    //        std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
    //        m_client->disconnect()->wait();
    //        std::cout << "OK" << std::endl;
    //    }
    //    catch (const mqtt::exception& exc) {
    //        std::cerr << exc << std::endl;
    //        return;
    //    }

    m_onServerDisconnected( m_ipv4.c_str(), m_port );
    m_serverConnected = false;

    assert( !m_stopThread );
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
}

} // namespace client
} // namespace hub
