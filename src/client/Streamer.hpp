#pragma once

#include <functional>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "OutputSensor.hpp"
#include "SensorSpec.hpp"

namespace hub {
namespace client {

namespace streamer {

class Stream;

} // end namespace streamer

///
/// \brief The Streamer class
/// simplifies the management of OutputSensors.
/// Indeed when you use an OutputSensor you have to manage network interruptions
/// (loss of connection to the server) by yourself when you use a network connection for your
/// sensor. The streamer class automatically handles the disconnection and reconnection of the
/// sensor to the server hub. For this you have to set the stream to be broadcasted. When the sensor
/// is connected or reconnected, the sensor data can be shared in the network. If there is a network
/// interruption, the data retrieved from the sensor will not be able to be shared during this time,
/// waiting for the reconnection to the server.
///
class SRC_API Streamer
{
  public:
    ///
    /// \brief Streamer
    /// constructor is used when you want to share sensor data to the server hub.
    /// \param ipv4
    /// is the ip of the server hub machine.
    /// \param port
    /// is the port of the server hub service.
    ///
    explicit Streamer( const std::string& ipv4 = net::s_defaultServiceIp,
                       int port                = net::s_defaultServicePort );

    ~Streamer();

    ///
    /// \brief addStream
    /// function is used when you want to share sensor data to server.
    /// \param streamName
    /// is the unique id name of the data you want to share and retrieve from
    /// other viewer application.
    /// \param sensorSpec
    /// is the sensor spec of the data you want to share.
    /// \param initAcqs
    /// for recorded stream (fixed size).
    ///
    void addStream( const std::string& streamName,
                    const SensorSpec& sensorSpec,
                    std::vector<Acquisition>&& initAcqs = {} );

    ///
    /// \brief newAcquisition
    /// function is used when you received acquisition from sensor you want to share the data.
    /// \param streamName
    /// is the name you added before.
    /// You can add many stream per streamer instance.
    /// \param acquisition
    /// is the new acquisition received from sensor.
    /// \warning
    /// The parameter stream with the param name 'streamName' has to be added before share
    /// acquisition.
    ///
    void newAcquisition( const std::string& streamName, const Acquisition& acquisition );

    ///
    /// \brief isConnected
    /// user feature.
    /// \return
    /// true if the streamer is connected to the server.\n
    /// false otherwise.
    ///
    bool isConnected() const;

  private:
    void onServerConnected();
    void onServerDisconnected();
    void waitingForServer();

    std::thread m_thread;

    std::map<std::string, std::unique_ptr<streamer::Stream>> m_streams;

    const std::string m_ipv4;
    const int m_port;

    bool m_serverConnected = false;
    const std::regex m_ipv4Regex;

    bool m_exitThread = false;

    friend class streamer::Stream;
};

} // namespace client
} // namespace hub
