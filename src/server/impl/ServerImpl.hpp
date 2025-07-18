/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "core/Configuration.hpp"
#include "io/StreamServer2.hpp"
#include "net/ServerSocket.hpp"
#include "Client.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"
#include "server/ServerI.hpp"

namespace hub {
namespace server {

static std::mutex m_mtxPrint;

///
/// \brief The ServerImpl class
/// Todo make ServerInterface class to inherit
///
class SRC_API ServerImpl : public ServerI
{
  public:
    ///
    /// \brief ServerImpl
    /// \param port
    ///
    explicit ServerImpl( int port = HUB_SERVICE_PORT );

    ~ServerImpl();

    ///
    /// \brief run
    ///
    void run() override;

    ///
    /// \brief asyncRun
    ///
    void asyncRun() override;

    ///
    /// \brief stop
    ///
    void stop() override;

    ///
    /// \brief printStatus
    ///
    void printStatus() const override;

    ///
    /// \brief running
    /// \return
    ///
    bool running() const override;

    ///
    /// \brief nStreamer
    /// \return
    ///
    int nStreamer() const override;

    ///
    /// \brief nClient
    /// \return
    ///
    int nClient() const override;

  private:
    void exitMainThread();

    std::string getStatus() const;
    std::string headerMsg() const;

    Client* initClient( hub::io::InputOutputSocket&& sock, int iClient );

    void addStreamer( StreamerClient* streamer );
    void addStreamViewer( StreamerClient* streamer );
    void delStreamViewer( StreamerClient* streamer );

    void addViewer( ViewerClient* viewer );

    void delStreamer( const StreamerClient* streamer );
    void delViewer( ViewerClient* viewer );

    void removeClient( Client* client );

  private:
    std::thread* m_thread = nullptr;

    std::map<std::string, StreamerClient*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    std::list<ViewerClient*> m_viewers;
    std::mutex m_mtxViewers;

    net::ServerSocket m_serverSock;
    std::list<Client*> m_clients;
    std::mutex m_mtxClients;

    int m_iClient       = 0;
    int m_nActiveClient = 0;

    bool m_killed  = false;
    bool m_running = false;

    int m_givingPort;

    friend class Client;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
};

} // namespace server
} // namespace hub
