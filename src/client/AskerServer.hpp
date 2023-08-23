#pragma once

#include "AskerInterface.hpp"

#include "net/ClientSocket.hpp"
#include "io/StreamServer.hpp"

namespace hub {
namespace client {

class AskerServer : public AskerInterface
{
  public:
    AskerServer(const std::string & ipv4 = io::StreamServer::s_defaultIpv4 , int port = io::StreamServer::s_defaultPort);
    ~AskerServer();

//  protected:
    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() override;
    hub::Acquisition getAcquisition(const std::string & streamName) override;

  private:
    net::ClientSocket m_sock;
};



} // namespace client
} // namespace hub
