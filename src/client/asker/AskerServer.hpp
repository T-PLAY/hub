#pragma once

#include "AskerInterface.hpp"

//#include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
// #include "impl/server/io/StreamServer.hpp"
#include "io/StreamServer.hpp"

namespace hub {
namespace client {

class AskerServer : public AskerInterface
{
  public:
    AskerServer(const std::string & ipv4 = io::StreamServer::s_defaultIpv4 , int port = io::StreamServer::s_defaultPort);
    ~AskerServer();

//  protected:
    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition(const std::string & streamName) override;

  private:
//    net::ClientSocket m_sock;
    io::InputOutputSocket m_sock;
};



} // namespace client
} // namespace hub
