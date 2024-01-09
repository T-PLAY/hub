/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27
	
#pragma once

#include <thread>

// #include "Input.hpp"
// #include "net/ClientSocket.hpp"
#include "core/Input.hpp"
#include "io/InputOutputSocket.hpp"
// #include "sensor/Acquisition.hpp"
// #include "sensor/SensorSpec.hpp"

// #include "../StreamServer.hpp"
#include "io/StreamServer2.hpp"

// #define HUB_DEBUG_INPUT_STREAM

namespace hub {
namespace input {

///
/// \brief The InputStreamServer2 class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
// class SRC_API InputStreamServer2 : public Input
class SRC_API InputStreamServer2 : public Input, public io::StreamServer2
{
  public:
    static struct {
    } not_endable;
    using Input::read;
    // using io::StreamServer2::getHeader;

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
    //    explicit InputStreamServer2( const std::string& streamName,
    //                          net::ClientSocket&& clientSocket = net::ClientSocket() );
    InputStreamServer2( int streamPort, const std::string& ipv4 = "127.0.0.1" );
    InputStreamServer2( const std::string& streamName,
                        int serverPort                = HUB_SERVICE_PORT,
                        const std::string& serverIpv4 = HUB_SERVICE_IPV4 );

    ///
    /// \brief InputStreamServer2
    /// \param inputStream
    ///
    //    InputStreamServer2( InputStreamServer2&& inputStream );

    InputStreamServer2(const InputStreamServer2 &) = delete;
    InputStreamServer2(InputStreamServer2 &&) = default;

    //    ~InputStreamServer2();

    //  protected:
    //  public:
    //    void read( sensor::Acquisition& acq );
    //    void read( sensor::SensorSpec& sensorSpec );

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

    void clear() override;

    // todo acq
    //    void read( sensor::Acquisition& acq ) override;
    //    void read( sensor::SensorSpec& sensorSpec ) override;
    const io::Header &  getHeader() const {
        return m_header;
    }

  private:
    //    net::ClientSocket m_serverSocket;
    std::unique_ptr<hub::io::InputOutputSocket> m_serverSocket;
    //    bool m_streamViewerClientClosed = false;
    //    bool m_streamerClosed           = false;
    std::unique_ptr<hub::io::InputOutputSocket> m_streamSocket;
    io::Header m_header;

    std::string m_streamIpv4;
    int m_streamPort;
    //    std::string m_ipv4;
    //    int m_port;

    //    bool m_moved = false;

    //    bool m_readAcqWaiting = false;

    //    friend class InputSyncStream;
    //    friend class InputSyncStream<InputStreamServer2>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamServer2::isOpen() const {
    // return ! m_serverSocket || m_serverSocket->isOpen();
    assert( m_streamSocket != nullptr );
    return m_streamSocket->isOpen();
}

inline void InputStreamServer2::read( Data_t* data, Size_t size ) {
    //    m_serverSocket.read( data, size );
    assert( m_streamSocket != nullptr );
    assert( m_header.getDataSize() == size );
    m_streamSocket->read( data, size );
}

inline void InputStreamServer2::close() {
    assert( false );
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStreamServer2] close() started" << std::endl;
#endif

    // inputSensor closing, prevent server this stream is done
    //    if ( !m_streamerClosed && !m_streamViewerClientClosed ) {
    //        m_serverSocket.write( io::StreamInterface::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED
    //        );
    //    }

    //    if ( m_readAcqWaiting ) {
    //        int iTry = 0;
    //        while ( !m_streamerClosed && !m_streamViewerClientClosed && iTry < 10 ) {
    //            std::cout << "[InputStreamServer2] close() waiting for server/streamer closing" <<
    //            std::endl; std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //            ++iTry;
    //        }
    //        assert(iTry < 10);
    //    }
    //    else {

    //        io::StreamInterface::ServerMessage message;
    //        m_serverSocket.read( message );
    //        while ( message == io::StreamInterface::ServerMessage::STREAM_VIEWER_NEW_ACQ ) {
    //            // todo server
    ////            auto acq = m_serverSocket.get<sensor::Acquisition>();
    //            m_clientSocket.read( message );
    //        }
    ////        assert( message == io::StreamInterface::ServerMessage::STREAM_VIEWER_CLOSED );
    //        assert( message == io::StreamInterface::ServerMessage::STREAMER_CLOSED );
    //        std::cout << "[InputStreamServer2] streamer client closed" << std::endl;
    ////        std::cout << "[InputStreamServer2] stream viewer client closed" << std::endl;
    //    }

    if ( m_serverSocket->isOpen() ) m_serverSocket->close();

#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStreamServer2] close() ended" << std::endl;
#endif
}

inline bool InputStreamServer2::isEnd() const {
    assert( m_streamSocket != nullptr );
    return m_streamSocket->isEnd();
    // return m_serverSocket->isEnd();
}

} // namespace input
} // namespace hub
