#pragma once

#include "client/AskerInterface.hpp"

// #include "net/ClientSocket.hpp"
#include "impl/server2/io/StreamServer.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace impl2 {
namespace client {

class AskerServer : public hub::client::AskerInterface
{
  public:
    AskerServer( const std::string& ipv4 = io::StreamServer::s_defaultIpv4,
                 int port                = io::StreamServer::s_defaultPort );
    ~AskerServer();

    //  protected:
    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    //    net::ClientSocket m_sock;
    hub::io::InputOutputSocket m_sock;
};

} // namespace client
} // namespace impl2
} // namespace hub
