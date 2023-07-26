#pragma once

#include <mutex>
#include <thread>

#include "Output.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The OutputStream class
/// Describes an output communication to the server.
///
class SRC_API OutputStream : public Output
{
  public:
    ///
    /// \brief OutputStream
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
    explicit OutputStream( const std::string& streamName,
                           net::ClientSocket&& clientSocket = net::ClientSocket() );

    OutputStream( OutputStream&& outputStream );
    ~OutputStream();

    void write( const Acquisition& acq ) override;

    // #ifdef WIN32 // msvc warning C4250
  protected:
    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;

    // #endif

  private:
    net::ClientSocket m_clientSocket;

    std::unique_ptr<std::thread> m_thread;
    bool m_moved = false;

    bool m_serverClosed   = false;
    bool m_streamerClosed = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void OutputStream::write( const Acquisition& acq ) {
    Output::write( net::ClientSocket::Message::NEW_ACQ );
    Output::write( acq );
}

inline void OutputStream::write( const unsigned char* data, size_t len ) {
    m_clientSocket.write( data, len );
}

inline void OutputStream::close() {
    std::cout << "[OutputStream] close() started" << std::endl;
    assert( m_clientSocket.isOpen() );
    if ( !m_serverClosed && !m_streamerClosed ) {
        m_clientSocket.write( net::ClientSocket::Message::OUTPUT_STREAM_CLOSED );
    }
    int iSleep = 0;
    while ( !m_serverClosed && !m_streamerClosed && iSleep < 10 ) {
        std::cout << "[OutputStream] close() waiting for server/streamer closing" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        ++iSleep;
    }
    m_clientSocket.close();
    std::cout << "[OutputStream] close() ended" << std::endl;
}

inline bool OutputStream::isOpen() const {
    return m_clientSocket.isOpen();
}

} // namespace io
} // namespace hub
