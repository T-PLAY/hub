#pragma once

#include <mutex>
#include <thread>

//#include "Output.hpp"
//#include "OutputStreamInterface.hpp"
//#include "net/ClientSocket.hpp"
#include "Output.hpp"
#include "io/StreamServer.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputStreamServer class
/// Describes an output communication to the server.
///
//class SRC_API OutputStreamServer : public Output
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
    /// \exception hub::net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is already started in the server.
    ///
//    explicit OutputStreamServer( const std::string& streamName,
//                           net::ClientSocket&& clientSocket = net::ClientSocket() );
    explicit OutputStreamServer( const std::string& streamName,
                                 const std::string & ipv4 = s_defaultIpv4, int port = s_defaultPort);
    ///
    /// \brief OutputStreamServer
    /// \param outputStream
    ///
    OutputStreamServer( OutputStreamServer&& outputStream );

    ~OutputStreamServer();

    void write( const Acquisition& acq ) override;

    // #ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;

    // #endif

  private:
    std::unique_ptr<net::ClientSocket> m_clientSocket;

    std::unique_ptr<std::thread> m_thread;
    bool m_moved = false;

    std::unique_ptr<bool> m_serverClosed   = std::make_unique<bool>(false);
    std::unique_ptr<bool> m_streamerClosed = std::make_unique<bool>(false);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void OutputStreamServer::write( const Acquisition& acq ) {
    Output::write( net::ClientSocket::Message::NEW_ACQ );
    Output::write( acq );
}

inline void OutputStreamServer::write( const unsigned char* data, size_t len ) {
    m_clientSocket->write( data, len );
}

inline void OutputStreamServer::close() {
    std::cout << "[OutputStreamServer] close() started" << std::endl;
    assert( m_clientSocket->isOpen() );
    if ( ! *m_serverClosed && ! *m_streamerClosed ) {
        m_clientSocket->write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
    }
    int iSleep = 0;
    while ( ! *m_serverClosed && ! *m_streamerClosed && iSleep < 10 ) {
        std::cout << "[OutputStreamServer] close() waiting for server/streamer closing" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        ++iSleep;
    }
    m_clientSocket->close();
    std::cout << "[OutputStreamServer] close() ended" << std::endl;
}

inline bool OutputStreamServer::isOpen() const {
    return m_clientSocket->isOpen();
}

} // namespace output
} // namespace hub
