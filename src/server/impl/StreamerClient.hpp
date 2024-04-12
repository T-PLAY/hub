/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include "Client.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class ServerImpl;

class StreamViewerClient;

///
/// \brief The StreamerClient class
/// represents OutputSensor peer from server side
///
class SRC_API StreamerClient : public Client
{
  public:
    ///
    /// \brief StreamerClient
    /// \param server
    /// \param iClient
    /// \param sock
    /// \param streamName
    /// \param streamIpv4
    /// \param port
    ///
    StreamerClient( ServerImpl* server,
                     int iClient,
                     hub::io::InputOutputSocket&& sock,
                     std::string streamName,
                     std::string streamIpv4,
                     int port );

    ~StreamerClient();

    ///
    /// \brief headerMsg
    /// \return
    ///
    // cppcheck-suppress virtualCallinConstructor
    std::string headerMsg() const override;

    ///
    /// \brief end
    /// \param message
    ///
    void end( io::StreamBase::ServerMessage message ) override;

    ///
    /// \brief notifyInited
    ///
    void notifyInited() override;

    StreamerClient( const StreamerClient& ) = delete;
    StreamerClient( StreamerClient&& )      = delete;


    ///
    /// \brief m_streamName
    ///
    const std::string m_streamName;

    ///
    /// \brief m_streamIpv4
    ///
    const std::string m_streamIpv4;

    ///
    /// \brief m_streamPort
    ///
    const int m_streamPort;

    ///
    /// \brief m_header
    ///
    io::Header m_header;

    ///
    /// \brief getNStreamViewer
    /// \return
    ///
    int getNStreamViewer() const;

  private:
    std::thread m_thread;

    std::unique_ptr<hub::io::InputOutputSocket> m_sock;

    bool m_serverDown = false;

    int m_nStreamViewer = 0;
};

} // namespace server
} // namespace hub
