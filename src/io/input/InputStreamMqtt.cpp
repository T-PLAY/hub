
#include "InputStreamMqtt.hpp"

#include "io/Memory.hpp"

namespace hub {
using namespace io;
namespace input {

InputStreamMqtt::InputStreamMqtt(const std::string &streamName, const std::string &ipv4, int port) :
    InputStreamInterface(streamName, ipv4, port)
    , m_client(new mqtt::client(ipv4 + ":" + std::to_string(port), std::string("consumer") + std::to_string((long)this), mqtt::create_options(MQTTVERSION_5)))
//    , m_msgPtr(mqtt::make_message(streamName, ""))
{
    m_client->connect();
    assert(m_client->is_connected());
    assert(m_msgPtr == nullptr);
//    m_msgPtr.set_retained(true);
//    m_msgPtr.set_qos(2);
}


//InputStreamMqtt::InputStreamMqtt( InputStreamMqtt&& inputStream ) :
//    InputStreamInterface(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port)
//{
//}

//InputStreamMqtt::~InputStreamMqtt() {
//}

void InputStreamMqtt::read( Acquisition& acq ) {
    assert(m_client->is_connected());

    uint64_t packetSize;
    m_client->subscribe(m_name + "/acq/size");
    Input::read(packetSize);

    std::vector<char> buff(packetSize);
//    auto * data = buff.data();
//    ((uint64_t*)data)[0] = packetSize;
    m_client->subscribe(m_name + "/acq/data");
//    read((unsigned char*)&data[sizeof(uint64_t)], packetSize);
    read((unsigned char*)buff.data(), packetSize);

    io::Memory<decltype(buff)> memory(buff);
    memory.read(acq);

    assert(buff.empty());
    assert(memory.isEnd());
}

void InputStreamMqtt::read( SensorSpec& sensorSpec ) {

    std::cout << "[InputStreamMqtt] start read(SensorSpec)" << std::endl;
    assert(m_client->is_connected());

    m_client->subscribe(m_name);
    m_client->start_consuming();
    bool consumed = m_client->try_consume_message_for(&m_msgPtr, std::chrono::milliseconds(100));
    assert(consumed);
    const auto & payload = m_msgPtr->get_payload_str();
    assert(payload == "active");
    m_client->stop_consuming();
//    auto topic = m_client->get_topic(m_name);
//    assert(topic.to_string() == "active");


    uint64_t packetSize;
//    auto topic = m_client->get_topic(m_name + "/header/size");
    m_client->subscribe(m_name + "/header/size");
//    m_msgPtr->set_topic(m_name + "/acq/size");
    Input::read(packetSize);

    std::vector<char> buff(sizeof(uint64_t) + packetSize);
//    unsigned char data[sizeof(uint64_t) + packetSize];
    auto * data = buff.data();
    ((uint64_t*)data)[0] = packetSize;
    m_client->subscribe(m_name + "/header/data");
    read((unsigned char*)&data[sizeof(uint64_t)], packetSize);
//    read((unsigned char*)buff.data(), packetSize);

    io::Memory<decltype(buff)> memory(buff);
    memory.read(sensorSpec);
//    memory.write(packetSize);
//    memory.write();
    assert(buff.empty());
    assert(memory.isEnd());

    std::cout << "[InputStreamMqtt] end read(SensorSpec)" << std::endl;
}

} // namespace input
} // namespace hub
