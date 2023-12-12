#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include "Client2.hpp"
// #include "sensor/InputSensor.hpp"
//  #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class ServerImpl2;

class StreamViewerClient2;

///
/// \brief The StreamerClient2 class
///
class StreamerClient2 : public Client2
{
  public:
    ///
    /// \brief StreamerClient2
    /// \param server
    /// \param iClient
    /// \param sock
    /// \param streamName
    ///
    StreamerClient2( ServerImpl2* server,
                     int iClient,
                     //                    net::ClientSocket&& sock,
                     hub::io::InputOutputSocket&& sock,
                     std::string streamName,
                     std::string streamIpv4,
                     int port );

    ~StreamerClient2();

    ///
    /// \brief headerMsg
    /// \return
    ///
    std::string headerMsg() const override;

    ///
    /// \brief getInputSensor
    /// \return
    ///
    //    const sensor::InputSensor *getInputSensor() const;

    ///
    /// \brief getLastAcq
    /// \return
    ///
    //    sensor::Acquisition getLastAcq() const;

    ///
    /// \brief end
    /// \param message
    ///
    void end( io::StreamBase::ServerMessage message ) override;
    void notifyInited() override;

    StreamerClient2( const StreamerClient2& ) = delete;
    StreamerClient2( StreamerClient2&& )      = delete;

    ///
    /// \brief getStreamName
    /// \return
    ///
    //    const std::string& getStreamName() const;

    ///
    /// \brief isPackedStream
    /// \return
    ///
    //    bool isPackedStream() const;

    ///
    /// \brief getPackedAcqs
    /// \return
    ///
    //    const std::set<sensor::Acquisition>& getPackedAcqs() const;

    const std::string m_streamName;
    // std::vector<hub::Data_t> m_retainedData;
    // bool m_retained;
    const std::string m_streamIpv4;
    const int m_streamPort;
    Datas_t m_header;
    // std::atomic<bool> m_fullyRetained = false;
    // std::atomic<bool> m_retainedSharedToViewer = false;

    int getNStreamViewer() const;

  private:
    std::thread m_thread;

    std::unique_ptr<hub::io::InputOutputSocket> m_sock;

    bool m_serverDown = false;

    int m_nStreamViewer = 0;

    //    std::unique_ptr<sensor::InputSensor> m_inputSensor;

    //    sensor::Acquisition m_lastAcq;

    //    int m_nAcq            = 0;
    //    bool m_isPackedStream = false;
    //    std::set<sensor::Acquisition> m_packedAcqs;
};

} // namespace server
} // namespace hub
