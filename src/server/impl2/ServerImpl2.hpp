/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "core/Configuration.hpp"
#include "io/StreamServer2.hpp"
#include "net/ServerSocket.hpp"
#include "Client2.hpp"
#include "StreamerClient2.hpp"
#include "ViewerClient2.hpp"

namespace hub {
namespace server {

static std::mutex m_mtxPrint;

///
/// \brief The ServerImpl2 class
///
class SRC_API ServerImpl2
{
  public:
    ///
    /// \brief ServerImpl2
    /// \param port
    ///
    explicit ServerImpl2( int port = HUB_SERVICE_PORT );

    ~ServerImpl2();

    ///
    /// \brief run
    ///
    void run();

    ///
    /// \brief asyncRun
    ///
    void asyncRun();

    ///
    /// \brief stop
    ///
    void stop();

    ///
    /// \brief printStatus
    ///
    void printStatus() const;

    ///
    /// \brief running
    /// \return
    ///
    bool running() const;

    ///
    /// \brief nStreamer
    /// \return
    ///
    int nStreamer() const;

    ///
    /// \brief nClient
    /// \return
    ///
    int nClient() const;

  private:
    void exitMainThread();

    std::string getStatus() const;
    std::string headerMsg() const;

    Client2* initClient( hub::io::InputOutputSocket&& sock, int iClient );

    void addStreamer( StreamerClient2* streamer );
    void addStreamViewer( StreamerClient2* streamer );
    void delStreamViewer( StreamerClient2* streamer );

    void addViewer( ViewerClient2* viewer );

    void delStreamer( const StreamerClient2* streamer );
    void delViewer( ViewerClient2* viewer );

    void removeClient( Client2* client );

  private:
    std::thread* m_thread = nullptr;

    std::map<std::string, StreamerClient2*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    std::list<ViewerClient2*> m_viewers;
    std::mutex m_mtxViewers;

    net::ServerSocket m_serverSock;
    std::list<Client2*> m_clients;
    std::mutex m_mtxClients;

    int m_iClient       = 0;
    int m_nActiveClient = 0;

    bool m_killed  = false;
    bool m_running = false;

    int m_givingPort;

    friend class Client2;
    friend class StreamerClient2;
    friend class StreamViewerClient2;
    friend class ViewerClient2;
};

} // namespace server
} // namespace hub
