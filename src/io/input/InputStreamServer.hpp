
#pragma once

#include <thread>

//#include "Input.hpp"
//#include "net/ClientSocket.hpp"
#include "core/Input.hpp"
#include "io/InputOutputSocket.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

//#include "../StreamServer.hpp"
#include "io/StreamServer.hpp"

namespace hub {
namespace input {

///
/// \brief The InputStreamServer class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
//class SRC_API InputStreamServer : public Input
class SRC_API InputStreamServer : public Input, public io::StreamServer
{
  public:
    ///
    /// \brief InputStreamServer
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
//    explicit InputStreamServer( const std::string& streamName,
//                          net::ClientSocket&& clientSocket = net::ClientSocket() );
    // InputStreamServer( const std::string& streamName, const std::string & ipv4 = s_defaultIpv4, int port = s_defaultPort);
    InputStreamServer( const std::string& streamName,
                       int port = s_defaultPort,
                       const std::string & ipv4 = s_defaultIpv4);

    ///
    /// \brief InputStreamServer
    /// \param inputStream
    ///
    InputStreamServer( InputStreamServer&& inputStream );


//    ~InputStreamServer();

  protected:
  public:
    void read( sensor::Acquisition& acq );
    void read( sensor::SensorSpec& sensorSpec );

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

  private:
//    net::ClientSocket m_clientSocket;
    io::InputOutputSocket m_clientSocket;
    bool m_streamViewerClientClosed = false;
    bool m_streamerClosed           = false;

    bool m_moved = false;

    bool m_readAcqWaiting = false;

//    friend class InputSyncStream;
//    friend class InputSyncStream<InputStreamServer>;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStreamServer::isOpen() const {
    return m_clientSocket.isOpen();
}

inline void InputStreamServer::read( Data_t* data, Size_t len ) {
    m_clientSocket.read( data, len );
}

inline void InputStreamServer::close() {
    std::cout << "[InputStreamServer] close() started" << std::endl;

    // inputSensor closing, prevent server this stream is done
    if ( !m_streamerClosed && !m_streamViewerClientClosed ) {
        m_clientSocket.write( io::StreamBase::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
    }

    if ( m_readAcqWaiting ) {
        int iTry = 0;
        while ( !m_streamerClosed && !m_streamViewerClientClosed && iTry < 10 ) {
            std::cout << "[InputStreamServer] close() waiting for server/streamer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTry;
        }
        assert(iTry < 10);
    }
    else {

        io::StreamBase::ServerMessage message;
        m_clientSocket.read( message );
        while ( message == io::StreamBase::ServerMessage::STREAM_VIEWER_NEW_ACQ ) {
            // todo server
//            auto acq = m_clientSocket.get<sensor::Acquisition>();
            m_clientSocket.read( message );
        }
//        assert( message == io::StreamBase::ServerMessage::STREAM_VIEWER_CLOSED );
        assert( message == io::StreamBase::ServerMessage::STREAMER_CLOSED );
        std::cout << "[InputStreamServer] streamer client closed" << std::endl;
//        std::cout << "[InputStreamServer] stream viewer client closed" << std::endl;
    }

    if ( m_clientSocket.isOpen() ) m_clientSocket.close();

    std::cout << "[InputStreamServer] close() ended" << std::endl;
}

inline bool InputStreamServer::isEnd() const {
    return m_clientSocket.isEnd();
}

} // namespace input
} // namespace hub
