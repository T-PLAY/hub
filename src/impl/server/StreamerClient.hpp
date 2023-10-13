#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include "Client.hpp"
#include "sensor/InputSensor.hpp"
#include "net/ClientSocket.hpp"

namespace hub {

class Server;

namespace server {

class StreamViewerClient;

///
/// \brief The StreamerClient class
///
class StreamerClient : public Client
{
  public:
    ///
    /// \brief StreamerClient
    /// \param server
    /// \param iClient
    /// \param sock
    /// \param streamName
    ///
    StreamerClient( Server* server,
                    int iClient,
                    net::ClientSocket&& sock,
                    std::string streamName );

    ~StreamerClient();

    ///
    /// \brief headerMsg
    /// \return
    ///
    std::string headerMsg() const override;

    ///
    /// \brief getInputSensor
    /// \return
    ///
    const sensor::InputSensor *getInputSensor() const;

    ///
    /// \brief getLastAcq
    /// \return
    ///
    sensor::Acquisition getLastAcq() const;

    ///
    /// \brief end
    /// \param message
    ///
    void end( io::StreamInterface::ServerMessage message ) override;

    ///
    /// \brief getStreamName
    /// \return
    ///
    const std::string& getStreamName() const;

    ///
    /// \brief isPackedStream
    /// \return
    ///
    bool isPackedStream() const;

    ///
    /// \brief getPackedAcqs
    /// \return
    ///
    const std::set<sensor::Acquisition>& getPackedAcqs() const;

  private:
    std::thread m_thread;

    std::unique_ptr<sensor::InputSensor> m_inputSensor;
    std::string m_streamName;

    sensor::Acquisition m_lastAcq;

    int m_nAcq            = 0;
    bool m_isPackedStream = false;
    std::set<sensor::Acquisition> m_packedAcqs;
};

} // namespace server
} // namespace hub