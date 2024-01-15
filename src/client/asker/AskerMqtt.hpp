/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30
	
#pragma once

#ifdef HUB_USE_MQTT

#include "AskerInterface.hpp"

#include <mqtt/client.h>

//#include "net/ClientSocket.hpp"
#include "io/StreamMqtt.hpp"


namespace hub {
namespace client {

class AskerMqtt : public AskerInterface
{
  public:
    AskerMqtt(const std::string & ipv4 = io::StreamMqtt::s_defaultIpv4, int port = io::StreamMqtt::s_defaultPort);
    ~AskerMqtt();

//  protected:
    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition(const std::string & streamName) override;

  private:
    template <class T>
    void read(T& t) {
//    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );
        memcpy(reinterpret_cast<unsigned char*>(&t), m_inputMsgPtr->get_payload().data(), sizeof(T));
    }

    std::string m_hostName;
//    net::ClientSocket m_sock;
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_inputMsgPtr;
};



} // namespace client
} // namespace hub

#endif
