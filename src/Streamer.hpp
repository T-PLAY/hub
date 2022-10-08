#pragma once

#include <functional>
#include <string>
#include <thread>
#include <regex>

#include "SensorSpec.hpp"
#include "Configurations.hpp"
#include "Acquisition.hpp"
#include "OutputSensor.hpp"

namespace hub {

///
/// \brief The Streamer class
///
class SRC_API Streamer
{
  public:
    Streamer(
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port = net::s_defaultServicePort);
    ~Streamer();

    void addStream(const std::string& streamName, SensorSpec && sensorSpec);
    void newAcquisition(const std::string& streamName, Acquisition && acquisition);


//    void setIpv4(const std::string & ipv4);
//    void setPort(int port);

  private:
//    std::thread m_thread;
    std::map<std::string, SensorSpec> m_streamName2sensorSpec;
    std::map<std::string, std::unique_ptr<OutputSensor>> m_streamName2outputSensor;
    std::map<std::string, std::chrono::system_clock::time_point> m_streamName2lastLogout;
//    std::map<std::string, OutputSensor> m_streamName2outputSensor;
    bool m_stopThread = false;

    std::string m_ipv4;
    int m_port;

    bool m_serverConnected = false;
    std::regex m_ipv4Regex;
};

} // namespace hub
