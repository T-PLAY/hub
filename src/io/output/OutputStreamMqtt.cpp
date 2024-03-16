
#include "OutputStreamMqtt.hpp"

#ifdef HUB_USE_MQTT

#    include <iostream>
#    include <typeinfo>

namespace hub {
namespace output {

OutputStreamMqtt::OutputStreamMqtt( const std::string& streamName,
                                    const std::string& ipv4,
                                    int port ) :
    StreamMqtt( streamName, ipv4, port ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "publisher:" ) + streamName,
                                mqtt::create_options( MQTTVERSION_5 ) ) ),
    m_msgPtr( mqtt::make_message( streamName, "" ) ) {
    m_client->connect();
    assert( m_client->is_connected() );
    assert( m_msgPtr != nullptr );
    m_msgPtr->set_retained( true );
    m_msgPtr->set_qos( 2 );

    m_client->subscribe( s_topicStream + m_name );
    mqtt::const_message_ptr inputMsgPtr;

    m_msgPtr->set_topic( s_topicStream + m_name );
    m_msgPtr->set_payload( "active" );
    m_client->publish( m_msgPtr );

#    ifdef DEBUG_OUTPUT_STREAM
    std::cout << DEBUG_OUTPUT_STREAM + m_name + " OutputStreamMqtt()" << std::endl;
#    endif
}

void OutputStreamMqtt::write( const sensor::Acquisition& acq ) {
    assert( m_msgPtr->get_qos() == 2 );

    std::vector<char> buff;
    constexpr int maxBuffLen = 512;
    buff.reserve( maxBuffLen );

    ///////////////////////////////////////

    if ( m_acqSize == 0 ) {
        m_acqSize = buff.size();
#    ifdef DEBUG_OUTPUT_STREAM
        std::cout << DEBUG_OUTPUT_STREAM + m_name + " write(const Acquisition&) acq"
                  << std::to_string( m_iAcq ) << ", acqSize = " << m_acqSize << std::endl;
#    endif

        m_currentTopic = s_topicStream + m_name + "/acqs/size";
        m_msgPtr->set_topic( m_currentTopic );
        Output::write( m_acqSize );
    }

    m_currentTopic = s_topicStream + m_name + "/acqs/data" + std::to_string( m_iAcq );
    m_msgPtr->set_topic( m_currentTopic );

    write( reinterpret_cast<unsigned char*>( buff.data() ), m_acqSize );
}

void OutputStreamMqtt::write( const sensor::SensorSpec& sensorSpec ) {
    assert( m_msgPtr->get_qos() == 2 );

    assert( m_client->is_connected() );

    std::vector<char> buff;
    constexpr int maxBuffLen = 512;
    buff.reserve( maxBuffLen );

    ///////////////////////////////////////

    m_msgPtr->set_retained( false );
    m_msgPtr->set_topic( s_topicEvents );
    m_msgPtr->set_payload( std::to_string( (int)ClientMessage::STREAMER_CLIENT_START ) + m_name );
    m_client->publish( m_msgPtr );
    m_msgPtr->set_retained( true );
}

} // namespace output
} // namespace hub

#endif
