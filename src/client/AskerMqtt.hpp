#pragma once

#include "AskerInterface.hpp"

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
    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() override;
    hub::Acquisition getAcquisition(const std::string & streamName) override;

  private:
//    net::ClientSocket m_sock;
};



} // namespace client
} // namespace hub
