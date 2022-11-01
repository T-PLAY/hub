#pragma once

#include <functional>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "Configurations.hpp"
#include "OutputSensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

///
/// \brief The Streamer class
///
class SRC_API Streamer
{
  public:
    Streamer( const std::string& ipv4 = net::s_defaultServiceIp,
              int port                = net::s_defaultServicePort );
    ~Streamer();

    void addStream( const std::string& streamName,
                    SensorSpec&& sensorSpec,
                    const std::vector<Acquisition>& initAcqs = {} );
    void newAcquisition( const std::string& streamName, Acquisition&& acquisition );

    bool isConnected() const;

  private:
    void onServerConnected();
    void onServerDisconnected();

    std::thread m_thread;

    std::map<std::string, SensorSpec> m_streamName2sensorSpec;
    std::map<std::string, std::unique_ptr<OutputSensor>> m_streamName2outputSensor;
    std::map<std::string, std::vector<Acquisition>> m_streamName2initAcqs;
    std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>>
        m_streamName2lastLogout;
    bool m_stopThread = false;

    std::string m_ipv4;
    int m_port;

    bool m_serverConnected = false;
    std::regex m_ipv4Regex;
};

} // namespace hub
