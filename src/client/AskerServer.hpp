#pragma once

#include "AskerInterface.hpp"

#include "net/ClientSocket.hpp"

namespace hub {
namespace client {

class AskerMqtt : public AskerInterface
{
  public:
    AskerMqtt(const std::string & ipv4, int port);
    ~AskerMqtt();

  protected:
    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() override;
    hub::Acquisition getAcquisition(const std::string & streamName) override;

  private:
    net::ClientSocket m_sock;
};



} // namespace client
} // namespace hub
