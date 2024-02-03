/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#ifdef HUB_USE_MQTT

#    include "AskerInterface.hpp"
#    include "io/StreamMqtt.hpp"
#    include <mqtt/client.h>

namespace hub {
namespace client {

class AskerMqtt : public AskerInterface
{
  public:
    AskerMqtt( const std::string& ipv4 = io::StreamMqtt::s_defaultIpv4,
               int port                = io::StreamMqtt::s_defaultPort );
    ~AskerMqtt();

    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    template <class T>
    void read( T& t ) {
        memcpy( reinterpret_cast<unsigned char*>( &t ),
                m_inputMsgPtr->get_payload().data(),
                sizeof( T ) );
    }

    std::string m_hostName;
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_inputMsgPtr;
};

} // namespace client
} // namespace hub

#endif
