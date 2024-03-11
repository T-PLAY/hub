/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <thread>

#include "core/Input.hpp"
#include "io/InputOutputSocket.hpp"

#include "io/StreamServer2.hpp"

namespace hub {
namespace input {

///
/// \brief The InputStreamServer2 class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
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
    /// \brief InputStreamServer2
    /// is used to instantiate an InputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream.
    /// \param clientSocket
    /// [in] is an existing connection to a hub server.
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to link (by their names) needs to be active in the server hub.
    /// \exception net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is not connected to the server.
    ///
    InputStreamServer2( int streamPort, const std::string& ipv4 = "127.0.0.1" );

    ///
    /// \brief InputStreamServer2
    /// \param streamName
    /// \param serverPort
    /// \param serverIpv4
    ///
    InputStreamServer2( const std::string& streamName,
                        int serverPort                = HUB_SERVICE_PORT,
                        const std::string& serverIpv4 = HUB_SERVICE_IPV4 );

    ///
    /// \brief InputStreamServer2
    /// \param inputStream
    ///

    InputStreamServer2( const InputStreamServer2& ) = delete;

    ///
    /// \brief InputStreamServer2
    ///
    InputStreamServer2( InputStreamServer2&& )      = default;

    ///
    /// \brief startStream
    ///
    void startStream();

    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( Data_t* data, Size_t len ) override;

    ///
    /// \brief close
    ///
    void close() override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

    ///
    /// \brief clear
    ///
    void clear() override;

    ///
    /// \brief getHeader
    /// \return
    ///
    const io::Header& getHeader() const { return m_header; }

  private:
    std::unique_ptr<hub::io::InputOutputSocket> m_serverSocket;
    std::unique_ptr<hub::io::InputOutputSocket> m_streamSocket;
    io::Header m_header;

    std::string m_streamIpv4;
    int m_streamPort;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    // assert( false );
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStreamServer2] close() started" << std::endl;
#endif

    ////            auto acq = m_serverSocket.get<sensor::Acquisition>();
    ////        assert( message == io::StreamInterface::ServerMessage::STREAM_VIEWER_CLOSED );
    ////        std::cout << "[InputStreamServer2] stream viewer client closed" << std::endl;

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
