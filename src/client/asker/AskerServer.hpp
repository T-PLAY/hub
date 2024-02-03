/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#ifdef HUB_USE_SERVER

#    include "AskerInterface.hpp"

#    include "io/InputOutputSocket.hpp"
#    include "io/StreamServer.hpp"

namespace hub {
namespace client {

class AskerServer : public AskerInterface
{
  public:
    AskerServer( const std::string& ipv4 = io::StreamServer::s_defaultIpv4,
                 int port                = io::StreamServer::s_defaultPort );
    ~AskerServer();

    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    io::InputOutputSocket m_sock;
};

} // namespace client
} // namespace hub

#endif
