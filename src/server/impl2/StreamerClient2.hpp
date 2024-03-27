/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Mozilla Public License, v. 2.0
* available at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include "Client2.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class ServerImpl2;

class StreamViewerClient2;

///
/// \brief The StreamerClient2 class
///
class SRC_API StreamerClient2 : public Client2
{
  public:
    ///
    /// \brief StreamerClient2
    /// \param server
    /// \param iClient
    /// \param sock
    /// \param streamName
    /// \param streamIpv4
    /// \param port
    ///
    StreamerClient2( ServerImpl2* server,
                     int iClient,
                     hub::io::InputOutputSocket&& sock,
                     std::string streamName,
                     std::string streamIpv4,
                     int port );

    ~StreamerClient2();

    ///
    /// \brief headerMsg
    /// \return
    ///
    // cppcheck-suppress virtualCallinConstructor
    std::string headerMsg() const override;

    ///
    /// \brief getInputSensor
    /// \return
    ///

    ///
    /// \brief getLastAcq
    /// \return
    ///

    ///
    /// \brief end
    /// \param message
    ///
    void end( io::StreamBase::ServerMessage message ) override;

    ///
    /// \brief notifyInited
    ///
    void notifyInited() override;

    StreamerClient2( const StreamerClient2& ) = delete;
    StreamerClient2( StreamerClient2&& )      = delete;


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
