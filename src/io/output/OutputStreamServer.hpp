/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/08/17
 */

#pragma once

#ifdef HUB_USE_SERVER

#    include <mutex>
#    include <thread>

#    include "core/Output.hpp"
#    include "io/StreamServer.hpp"
#    include "sensor/Acquisition.hpp"
#    include "sensor/SensorSpec.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputStreamServer class
/// Describes an output communication to the server.
///
class SRC_API OutputStreamServer : public Output, public io::StreamServer
{
  public:
    ///
    /// \brief OutputStreamServer
    /// is used to instantiate an OutputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream
    /// \param clientSocket
    /// [in] is an existing connection to a hub server
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to start linking (by it name) needs to not exist in the server hub.
    /// Streaming with most of one sender is not allowed.
    /// \exception net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is already started in the server.
    ///
    explicit OutputStreamServer( const std::string& streamName,
                                 const std::string& ipv4 = s_defaultIpv4,
                                 int port                = s_defaultPort );
    ///
    /// \brief OutputStreamServer
    /// \param outputStream
    ///
    OutputStreamServer( OutputStreamServer&& outputStream );

    ~OutputStreamServer();

    void write( const sensor::Acquisition& acq );
    void write( const sensor::SensorSpec& sensorSpec );

  protected:
    void write( const Data_t* data, Size_t len ) override;
    void close() override;
    bool isOpen() const override;

  private:
    std::unique_ptr<io::InputOutputSocket> m_clientSocket;

    std::unique_ptr<std::thread> m_thread;
    bool m_moved = false;

    std::unique_ptr<bool> m_serverClosed   = std::make_unique<bool>( false );
    std::unique_ptr<bool> m_streamerClosed = std::make_unique<bool>( false );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void OutputStreamServer::write( const sensor::Acquisition& acq ) {
    Output::write( io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_ACQ );
    Output::write( acq );
}

inline void OutputStreamServer::write( const sensor::SensorSpec& sensorSpec ) {
    Output::write( sensorSpec );
}

inline void OutputStreamServer::write( const Data_t* data, Size_t len ) {
    m_clientSocket->write( data, len );
}

inline void OutputStreamServer::close() {
    std::cout << "[OutputStreamServer] close() started" << std::endl;
    assert( m_clientSocket->isOpen() );
    if ( !*m_serverClosed && !*m_streamerClosed ) {
        m_clientSocket->write( io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED );
    }
    int iTry = 0;
    while ( !*m_serverClosed && !*m_streamerClosed && iTry < 10 ) {
        std::cout << "[OutputStreamServer] close() waiting for server/streamer closing"
                  << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        ++iTry;
    }
    assert( iTry < 10 );

    std::cout << "[OutputStreamServer] closing connection ended" << std::endl;
    m_clientSocket->close();
    std::cout << "[OutputStreamServer] close() ended" << std::endl;
}

inline bool OutputStreamServer::isOpen() const {
    return m_clientSocket->isOpen();
}

} // namespace output
} // namespace hub

#endif
