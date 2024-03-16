#include "AskerMqtt.hpp"

#ifdef HUB_USE_MQTT

#    include "core/Utils.hpp"

namespace hub {
namespace client {

AskerMqtt::AskerMqtt( const std::string& ipv4, int port ) :
    m_hostName( utils::getHostname() ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "asker" ) + m_hostName,
                                mqtt::create_options( MQTTVERSION_5 ) ) ) {

    m_client->connect();
    assert( m_client->is_connected() );
}

AskerMqtt::~AskerMqtt() {}

std::list<std::pair<std::string, sensor::SensorSpec>> AskerMqtt::listStreams() {

    assert( m_client->is_connected() );
    m_client->start_consuming();

    m_client->subscribe( io::StreamMqtt::s_topicStream + "+" );

    std::vector<std::string> activeStreamTopics;

    while ( true ) {
        m_inputMsgPtr.reset();
        m_client->try_consume_message_for( &m_inputMsgPtr, std::chrono::milliseconds( 100 ) );
        if ( m_inputMsgPtr == nullptr ) break;
        assert( m_inputMsgPtr != nullptr );

        if ( m_inputMsgPtr->get_payload_str() == "active" ) {
            activeStreamTopics.push_back( m_inputMsgPtr->get_topic() );
        }
    }

    std::list<std::pair<std::string, sensor::SensorSpec>> ret;

    for ( const auto& activeStreamTopic : activeStreamTopics ) {

        uint64_t packetSize;
        m_client->subscribe( activeStreamTopic + "/header/size" );
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert( m_inputMsgPtr != nullptr );
        assert( m_inputMsgPtr->get_topic() == activeStreamTopic + "/header/size" );
        read( packetSize );

        std::vector<char> buff( sizeof( uint64_t ) + packetSize );
        auto* data                             = buff.data();
        reinterpret_cast<uint64_t*>( data )[0] = packetSize;

        m_client->subscribe( activeStreamTopic + "/header/data" );
        m_inputMsgPtr.reset();
        m_inputMsgPtr = m_client->consume_message();
        assert( m_inputMsgPtr != nullptr );
        assert( m_inputMsgPtr->get_topic() == activeStreamTopic + "/header/data" );
        memcpy( &data[sizeof( uint64_t )], m_inputMsgPtr->get_payload().data(), packetSize );
    }

    m_client->stop_consuming();

    return ret;
}

sensor::Acquisition AskerMqtt::getAcquisition( const std::string& streamName ) {
    assert( m_client->is_connected() );
    m_client->start_consuming();

    m_inputMsgPtr.reset();
    m_client->subscribe( io::StreamMqtt::s_topicStream + streamName );
    m_client->try_consume_message_for( &m_inputMsgPtr, std::chrono::milliseconds( 100 ) );

    if ( m_inputMsgPtr == nullptr || m_inputMsgPtr->get_payload_str() == "inactive" ) {

        throw io::StreamMqtt::exception(
            ( "[Asker] Stream '" + streamName + "' is not connected to server" ).c_str() );
    }

    assert( m_inputMsgPtr->get_payload_str() == "active" );

    uint64_t acqSize;

    m_client->subscribe( io::StreamMqtt::s_topicStream + streamName + "/acqs/size" );
    m_inputMsgPtr.reset();
    m_inputMsgPtr = m_client->consume_message();
    assert( m_inputMsgPtr != nullptr );
    assert( m_inputMsgPtr->get_topic() ==
            io::StreamMqtt::s_topicStream + streamName + "/acqs/size" );
    read( acqSize );
    std::cout << "[Asker] acqSize = " << acqSize << std::endl;

    std::vector<char> buff( acqSize );

    m_client->subscribe( io::StreamMqtt::s_topicStream + streamName + "/acqs/data0" );
    m_inputMsgPtr.reset();
    m_inputMsgPtr = m_client->consume_message();
    assert( m_inputMsgPtr != nullptr );
    assert( m_inputMsgPtr->get_topic() ==
            io::StreamMqtt::s_topicStream + streamName + "/acqs/data0" );
    memcpy( buff.data(), m_inputMsgPtr->get_payload().data(), acqSize );

    m_client->stop_consuming();

    return hub::sensor::Acquisition();
}

} // namespace client
} // namespace hub

#endif
