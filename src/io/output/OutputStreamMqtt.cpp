
#include "OutputStreamMqtt.hpp"

#include <iostream>
#include <typeinfo>


namespace hub {
using namespace io;
namespace output {

OutputStreamMqtt::OutputStreamMqtt(const std::string &streamName, const std::string &ipv4, int port) :
    OutputStreamInterface(streamName, ipv4, port)
    , m_client(new mqtt::client(ipv4 + ":" + std::to_string(port), "publisher", mqtt::create_options(MQTTVERSION_5)))
    , m_msgPtr(mqtt::make_message(streamName, ""))
{
    m_client->connect();
    assert(m_client->is_connected());
//    assert(m_msgPtr == nullptr);
//    m_msgPtr = mqtt::make_message(streamName, "");
    assert(m_msgPtr != nullptr);
}


//OutputStreamMqtt::OutputStreamMqtt( OutputStreamMqtt&& outputStream ) :
//    OutputStreamInterface(outputStream.m_name, outputStream.m_ipv4, outputStream.m_port)
//    , m_client(std::move(outputStream.m_client))
//    , m_msgPtr(std::move(outputStream.m_msgPtr))
//{
//    assert(m_client->is_connected());
//}

//OutputStreamMqtt::~OutputStreamMqtt() {
//}

} // namespace output
} // namespace hub
