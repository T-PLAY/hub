
#include "OutputStreamMqtt.hpp"

#include <iostream>
#include <typeinfo>

#include "io/Memory.hpp"

namespace hub {
namespace output {

OutputStreamMqtt::OutputStreamMqtt( const std::string& streamName,
                                    const std::string& ipv4,
                                    int port ) :
    //    OutputStreamInterface(streamName, ipv4, port)
    StreamMqtt( streamName, ipv4, port ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "publisher:" ) + streamName,
                                mqtt::create_options( MQTTVERSION_5 ) ) ),
    m_msgPtr( mqtt::make_message( streamName, "" ) )
//    m_streamName( streamName )
{
    m_client->connect();
    assert( m_client->is_connected() );
    //    assert(m_msgPtr == nullptr);
    //    m_msgPtr = mqtt::make_message(streamName, "");
    assert( m_msgPtr != nullptr );
    m_msgPtr->set_retained( true );
    m_msgPtr->set_qos( 2 );

    m_client->subscribe( s_topicStream + m_name );
    mqtt::const_message_ptr inputMsgPtr;

//    if (  m_client->try_consume_message_for( &inputMsgPtr, std::chrono::milliseconds( 100 ) )) {
//        if (inputMsgPtr->get_payload_str() == "active") {
//            throw io::StreamMqtt::exception(("OutputStream '" + m_name + "' is already connected to server").c_str());
//        }
//    }

    //    m_msgPtr->set_topic(s_topicStream + m_name);
    //    m_msgPtr->set_payload("active");
    //    m_client->publish(m_msgPtr);
}

void OutputStreamMqtt::write( const Acquisition& acq ) {
    std::vector<char> buff;
    constexpr int maxBuffLen = 512;
    buff.reserve( maxBuffLen );
    io::Memory<decltype( buff )> memory( buff );

    memory.write( acq );

    ///////////////////////////////////////

    uint64_t packetSize = buff.size();
    //    memory.read(packetSize);

    //    assert(buff.size() == packetSize);

    m_msgPtr->set_topic( s_topicStream + m_name + "/acq/size" );
    Output::write( packetSize );
    //    Output::write((unsigned char*)buff.data(), packetSize);
    m_msgPtr->set_topic( s_topicStream + m_name + "/acq/data" );
    write( (unsigned char*)buff.data(), packetSize );
}

void OutputStreamMqtt::write( const SensorSpec& sensorSpec ) {
    assert( m_client->is_connected() );
    //    std::cout << "[OutputStreamMqtt] start write(SensorSpec)" << std::endl;
    //    Output::write(sensorSpec);
    //    uint64_t packetSize;
    //    std::list<char> buff;
    std::vector<char> buff;
    constexpr int maxBuffLen = 512;
    buff.reserve( maxBuffLen );
    io::Memory<decltype( buff )> memory( buff );

    memory.write( sensorSpec );

    ///////////////////////////////////////

    uint64_t packetSize;
    memory.read( packetSize );

    assert( buff.size() == packetSize );

    m_msgPtr->set_topic( s_topicStream + m_name + "/header/size" );
    Output::write( packetSize );
    //    Output::write((unsigned char*)buff.data(), packetSize);
    m_msgPtr->set_topic( s_topicStream + m_name + "/header/data" );
    write( (unsigned char*)buff.data(), packetSize );

    m_msgPtr->set_topic( s_topicStream + m_name );
    m_msgPtr->set_payload( "active" );
    m_client->publish( m_msgPtr );

    // prevent viewers there is new streamer
    m_msgPtr->set_retained( false );
    m_msgPtr->set_topic( s_topicEvents );
    m_msgPtr->set_payload( to_string( Message::NEW_STREAM ) + m_name );
    m_client->publish( m_msgPtr );

    //    m_msgPtr->set_topic(s_topicEvents + "/streamName");
    //    m_msgPtr->set_payload(m_streamName);
    //    m_client->publish(m_msgPtr);
    m_msgPtr->set_retained( true );

    //    m_msgPtr->set_retained(false);
    //    m_msgPtr->set_qos(0);
    //    std::cout << "[OutputStreamMqtt] end write(SensorSpec)" << std::endl;
}

// void OutputStreamMqtt::write(uint64_t packetSize)
//{
//     std::cout << "[OutputStreamMqtt] write(packetSize)" << std::endl;
// }

// OutputStreamMqtt::OutputStreamMqtt( OutputStreamMqtt&& outputStream ) :
//     OutputStreamInterface(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port)
//     , m_client(std::move(outputStream.m_client))
//     , m_msgPtr(std::move(outputStream.m_msgPtr))
//{
//     assert(m_client->is_connected());
// }

// OutputStreamMqtt::~OutputStreamMqtt() {
// }

} // namespace output
} // namespace hub
