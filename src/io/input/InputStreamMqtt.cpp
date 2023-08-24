
#include "InputStreamMqtt.hpp"

#include "io/Memory.hpp"

namespace hub {
namespace input {


InputStreamMqtt::InputStreamMqtt( const std::string& streamName,
                                  const std::string& ipv4,
                                  int port ) :
    //    InputStreamInterface( streamName, ipv4, port ),
    io::StreamMqtt( streamName, ipv4, port ),
    m_client( new mqtt::client( ipv4 + ":" + std::to_string( port ),
                                std::string( "InputStreamMqtt" ) + std::to_string( (long)this ),
                                mqtt::create_options( MQTTVERSION_5 ) ) )
//    , m_msgPtr(mqtt::make_message(streamName, ""))
{
    //    mqtt::connect_options options;
    //    options.set_properties(properties);
    m_client->connect();
    assert( m_client->is_connected() );

//    mqtt::string_collection topics;
//    topics.push_back(s_topicStream + m_name);
//    topics.push_back(s_topicStream + m_name + "/header/size");
//    topics.push_back(s_topicStream + m_name + "/header/data");
//    topics.push_back(s_topicStream + m_name + "/acqs/size");
//    topics.push_back(s_topicStream + m_name + "/acqs/data0");
//    auto rsp = m_client->subscribe(topics);
//    for (const auto reasonCode : rsp.get_reason_codes()) {
//        assert(reasonCode == mqtt::ReasonCode::SUCCESS || reasonCode == mqtt::GRANTED_QOS_1);
//    }
//    assert(rsp.get_properties().empty());

//    assert(rsp.get_properties().contains(mqtt::property::RESPONSE_TOPIC));
//    assert(rsp.get_properties().size() == 1);
    m_client->start_consuming();

//    assert( m_msgPtr == nullptr );
    //    m_msgPtr.set_retained(true);
    //    m_msgPtr.set_qos(2);
#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " InputStreamMqtt()" << std::endl;
#endif
}

// InputStreamMqtt::InputStreamMqtt(InputStreamMqtt &&inputStream)
//     : io::StreamMqtt(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port),
//     m_client(std::move(inputStream.m_client))
//{
//     assert(m_client->is_connected());
// }

// InputStreamMqtt::InputStreamMqtt( InputStreamMqtt&& inputStream ) :
//     InputStreamInterface(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port)
//{
// }

// InputStreamMqtt::~InputStreamMqtt() {
// }

void InputStreamMqtt::read( Acquisition& acq ) {
    assert( m_client->is_connected() );

#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&)" << std::endl;
#endif

    //    uint64_t packetSize = 0;
    if ( m_acqSize == 0 ) {
#ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) init acqSize" << std::endl;
#endif
        subscribe(s_topicStream + m_name + "/acqs/size");
//        m_client->subscribe( m_currentTopic );
        Input::read( m_acqSize );
        //    read(packetSize);
//        m_client->unsubscribe( m_currentTopic );

#ifdef DEBUG_INPUT_STREAM
        std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) acq"
                  << std::to_string( m_iAcq ) << ", m_acqSize = " << m_acqSize << std::endl;
#endif
//        for ( int i = 0; i < s_bufferAcqSize; ++i ) {
//            m_client->subscribe( s_topicStream + m_name + "/acqs/data" + std::to_string( i ) );
//        }
        subscribe(s_topicStream + m_name + "/acqs/data0");
//        m_client->subscribe( m_currentTopic );
    }

    std::vector<char> buff( m_acqSize );
    //    auto * data = buff.data();
    //    ((uint64_t*)data)[0] = m_acqSize;
//    m_currentTopic = s_topicStream + m_name + "/acqs/data" + std::to_string( m_iAcq );
//    m_iAcq         = ( m_iAcq + 1 ) % s_bufferAcqSize;

#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM + m_name + " read(Acquisition&) subscribe to " << m_currentTopic
              << std::endl;
#endif

//        m_client->subscribe( m_currentTopic );
    //    read((unsigned char*)&data[sizeof(uint64_t)], m_acqSize);
    read( (unsigned char*)buff.data(), m_acqSize );
//        m_client->unsubscribe( m_currentTopic );

    io::Memory<decltype( buff )> memory( buff );
    memory.read( acq );

    std::cout << "\tread acq : " << acq << std::endl;
//    std::cout << "\tread acq" << m_iAcq - 1 << " : " << acq << std::endl;

    assert( buff.empty() );
    assert( memory.isEnd() );
}

void InputStreamMqtt::read( SensorSpec& sensorSpec ) {

    //    std::cout << "[InputStreamMqtt] start read(SensorSpec)" << std::endl;
    assert( m_client->is_connected() );

    subscribe(s_topicStream + m_name);
//    m_client->start_consuming();
//    bool consumed =
//        m_client->try_consume_message_for( &m_msgPtr, std::chrono::milliseconds( 100 ) );
//    assert( consumed );
//    m_client->subscribe( m_currentTopic );
    m_msgPtr = m_client->consume_message();
//    m_client->unsubscribe( m_currentTopic );
    if (m_msgPtr->get_topic() != m_currentTopic) {
            std::cout << "[InputStreamMqtt] msg topic : '" + m_msgPtr->get_topic() +
                         ", current topic : '" + m_currentTopic + "'" << std::endl;
    }
    assert(m_msgPtr->get_topic() == m_currentTopic);
    assert( m_msgPtr != nullptr );

    const auto& payload = m_msgPtr->get_payload_str();
    assert( payload == "active" );
//    m_client->stop_consuming();
    //    auto topic = m_client->get_topic(s_topicStream + m_name);
    //    assert(topic.to_string() == "active");

    uint64_t packetSize;
    subscribe(s_topicStream + m_name + "/header/size");
//    m_client->subscribe( m_currentTopic );
    Input::read( packetSize );
//    m_client->unsubscribe( m_currentTopic );

    std::vector<char> buff( sizeof( uint64_t ) + packetSize );
    //    unsigned char data[sizeof(uint64_t) + packetSize];
    auto* data             = buff.data();
    ( (uint64_t*)data )[0] = packetSize;
    subscribe(s_topicStream + m_name + "/header/data");
//    m_client->subscribe( m_currentTopic );
    read( (unsigned char*)&data[sizeof( uint64_t )], packetSize );
//    m_client->unsubscribe( m_currentTopic );

    io::Memory<decltype( buff )> memory( buff );
    memory.read( sensorSpec );
    assert( buff.empty() );
    assert( memory.isEnd() );
}

void InputStreamMqtt::subscribe(const std::string &topic)
{
    m_currentTopic = topic;
#ifdef DEBUG_INPUT_STREAM
    std::cout << DEBUG_INPUT_STREAM "subscribing to " << m_currentTopic << std::endl;
#endif
    auto rsp = m_client->subscribe(topic);
    for (const auto reasonCode : rsp.get_reason_codes()) {
        assert(reasonCode == mqtt::ReasonCode::SUCCESS || reasonCode == mqtt::GRANTED_QOS_1);
    }
    assert(rsp.get_properties().empty());

}

} // namespace input
} // namespace hub
