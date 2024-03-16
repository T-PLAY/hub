
#include "InputStreamMqtt.hpp"

#ifdef HUB_USE_MQTT

namespace hub {
namespace input {

InputStreamMqtt::InputStreamMqtt( const std::string& streamName,
                                  int port,
                                  const std::string& ipv4 ) :
    io::StreamMqtt( streamName, ipv4, port ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "InputStreamMqtt" ) + std::to_string( (long)this ),
                                mqtt::create_options( MQTTVERSION_5 ) ) ) {
    m_client->connect();
    assert( m_client->is_connected() );

    m_client->start_consuming();

    subscribe( s_topicStream + m_name );
    bool consumed =
        m_client->try_consume_message_for( &m_msgPtr, std::chrono::milliseconds( 100 ) );
    if ( !consumed ) {
        throw io::StreamMqtt::exception(
            ( "[InputStreamMqtt] stream '" + m_name + "' is not connected to server" ).c_str() );
    }
    assert( consumed );
    assert( m_msgPtr->get_topic() == m_currentTopic );
    assert( m_msgPtr != nullptr );

    const auto& payload = m_msgPtr->get_payload_str();
    if ( payload == "inactive" ) {
        throw io::StreamMqtt::exception(
            ( "[InputStreamMqtt] stream '" + m_name + "' is inactive" ).c_str() );
    }
    assert( payload == "active" );

#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " InputStreamMqtt()" << std::endl;
#    endif
}

void InputStreamMqtt::clear() {}

void InputStreamMqtt::read( sensor::Acquisition& acq ) {
    assert( m_client->is_connected() );

#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&)" << std::endl;
#    endif

    if ( m_acqSize == 0 ) {
#    ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) init acqSize" << std::endl;
#    endif
        subscribe( s_topicStream + m_name + "/acqs/size" );
        Input::read( m_acqSize );

#    ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) acq"
                  << std::to_string( m_iAcq ) << ", m_acqSize = " << m_acqSize << std::endl;
#    endif
        subscribe( s_topicStream + m_name + "/acqs/data0" );
    }

    std::vector<char> buff( m_acqSize );

#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) subscribe to " << m_currentTopic
              << std::endl;
#    endif

    read( reinterpret_cast<unsigned char*>( buff.data() ), m_acqSize );

    assert( buff.empty() );
}

void InputStreamMqtt::read( sensor::SensorSpec& sensorSpec ) {

    assert( m_client->is_connected() );

    uint64_t packetSize;
    subscribe( s_topicStream + m_name + "/header/size" );
    Input::read( packetSize );

    std::vector<char> buff( sizeof( uint64_t ) + packetSize );
    auto* data                             = buff.data();
    reinterpret_cast<uint64_t*>( data )[0] = packetSize;
    subscribe( s_topicStream + m_name + "/header/data" );
    read( reinterpret_cast<unsigned char*>( &data[sizeof( uint64_t )] ), packetSize );
}

void InputStreamMqtt::subscribe( const std::string& topic ) {
    m_currentTopic = topic;
#    ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + "subscribing to " << m_currentTopic << std::endl;
#    endif
    auto rsp = m_client->subscribe( topic );
    for ( const auto reasonCode : rsp.get_reason_codes() ) {
        assert( reasonCode == mqtt::ReasonCode::SUCCESS || reasonCode == mqtt::GRANTED_QOS_1 );
    }
    assert( rsp.get_properties().empty() );
}

} // namespace input
} // namespace hub

#endif
