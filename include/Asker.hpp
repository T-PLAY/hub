#pragma once

// #include <functional>
// #include <regex>
#include <string>
// #include <thread>

// #include "Acquisition.hpp"
#include "Net/ClientSocket.hpp"
// #include "SensorSpec.hpp"
#include "Macros.hpp"

namespace hub {

class SRC_API Asker
{
  public:
    explicit Asker( const std::string& ipv4 = net::s_defaultServiceIp,
           int port                = net::s_defaultServicePort );

    ~Asker();

    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() const;
    hub::SensorSpec getSensorSpec(const std::string & streamName) const;
    hub::Acquisition getAcquisition(const std::string & streamName) const ;


    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    net::ClientSocket m_sock;
};

} // namespace hub
