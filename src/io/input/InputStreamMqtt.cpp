
#include "InputStreamMqtt.hpp"

namespace hub {
using namespace io;
namespace input {

InputStreamMqtt::InputStreamMqtt(const std::string &streamName, const std::string &ipv4, int port) :
    InputStreamInterface(streamName, ipv4, port)
    , m_client(new mqtt::client(ipv4 + ":" + std::to_string(port), "consumer", mqtt::create_options(MQTTVERSION_5)))
//    , m_msgPtr(mqtt::make_message(streamName, ""))
{
    m_client->connect();
    assert(m_client->is_connected());
    assert(m_msgPtr == nullptr);
}


//InputStreamMqtt::InputStreamMqtt( InputStreamMqtt&& inputStream ) :
//    InputStreamInterface(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port)
//{
//}

//InputStreamMqtt::~InputStreamMqtt() {
//}

//void InputStreamMqtt::read( Acquisition& acq ) {
//}

//void InputStreamMqtt::read( SensorSpec& sensorSpec ) {
//}

} // namespace input
} // namespace hub
