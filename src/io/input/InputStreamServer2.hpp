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

#include <thread>

#include "core/Configuration.hpp"
#include "core/Input.hpp"
#include "io/InputOutputSocket.hpp"
#include "io/StreamServer2.hpp"

namespace hub {
namespace input {

///
/// \brief The InputStreamServer2 class
/// Describes an input communication from the server.
/// \warning The communication is only possible if the stream (with the same name) is active within the server.
/// That implies an OutputStream communicating data through the server.
///
class SRC_API InputStreamServer2 : public Input, public io::StreamServer2
{
  public:
    using Input::read;

    ///
    /// \brief not_endable
    ///
    static struct {
    } not_endable;

    ///
    /// \brief Login stream from streamer
    /// \param streamPort [in] correspond of the streamer port you want to log in
    /// \param streamIpv4 [in] correspond of the streamer ipv4 you want to log in
    /// \exception net::Socket::exception
    /// when no streamer found (service is not running)
    ///
    explicit InputStreamServer2( int streamPort, const std::string& streamIpv4 = "127.0.0.1" );

    ///
    /// \brief Login stream from server
    /// \param streamName
    /// [in] is an unique identifier name of stream.
    /// \param serverPort
    /// [in] is a running service port you want to use
    /// \param serverIpv4
    /// [in] is a ip of running server
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to link (by their names) needs to be active in the server hub.
    /// \exception net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is not connected to the server.
    ///
    explicit InputStreamServer2( const std::string& streamName,
                        int serverPort                = HUB_SERVICE_PORT,
                        const std::string& serverIpv4 = HUB_SERVICE_IPV4 );

    InputStreamServer2( const InputStreamServer2& ) = delete;

    ///
    /// \brief Create from movable argument
    ///
    InputStreamServer2( InputStreamServer2&& )      = default;

    ///
    /// \brief Start catching information from peer
    ///
    void startStream();

    bool isOpen() const override;

    void read( Data_t* data, Size_t len ) override;

    void close() override;

    bool isEnd() const override;

    void clear() override;

    ///
    /// \brief Get header of stream
    /// \return header of stream
    /// Todo remove this when header is moved into input/output classes
    ///
    const io::Header& getHeader() const { return m_header; }

  private:
    // Todo making union because both server or streamer cannot be connected
    std::unique_ptr<hub::io::InputOutputSocket> m_serverSocket;
    std::unique_ptr<hub::io::InputOutputSocket> m_streamSocket;

    // Todo move this on ios class
    io::Header m_header;

    std::string m_streamIpv4;
    int m_streamPort;
};

//////////////////////////////////// INLINE ////////////////////////////////////

inline bool InputStreamServer2::isOpen() const {
    assert( m_streamSocket != nullptr );
    return m_streamSocket->isOpen();
}

inline void InputStreamServer2::read( Data_t* data, Size_t size ) {
    assert( m_streamSocket != nullptr );
    assert( m_header.getDataSize() == size );
    m_streamSocket->read( data, size );
}

inline void InputStreamServer2::close() {
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStreamServer2] close() started" << std::endl;
#endif

    if ( m_serverSocket != nullptr && m_serverSocket->isOpen() ) m_serverSocket->close();

#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStreamServer2] close() ended" << std::endl;
#endif
}

inline bool InputStreamServer2::isEnd() const {
    assert( m_streamSocket != nullptr );
    return m_streamSocket->isEnd();
}

} // namespace input
} // namespace hub
