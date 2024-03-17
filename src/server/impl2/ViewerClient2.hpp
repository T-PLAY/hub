/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/11/27
 */

#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include "Client2.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class StreamerClient2;

///
/// \brief The ViewerClient2 class
///
class SRC_API ViewerClient2 : public Client2
{
  private:
    ViewerClient2( ServerImpl2* server, int iClient, net::ClientSocket&& sock );
    ~ViewerClient2();

    std::string headerMsg() const override;

    void notifyNewStreamer( const StreamerClient2* streamer );
    void notifyDelStreamer( const std::string& streamName );

    void end( io::StreamBase::ServerMessage message ) override;
    void notifyInited() override;

  private:
    std::thread m_thread;

    hub::io::InputOutputSocket m_socket;

    bool m_viewerClosed                   = false;
    std::atomic<bool> m_clientStreamAdded = false;

    friend class ServerImpl2;
};

} // namespace server
} // namespace hub
