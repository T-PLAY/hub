
#pragma once

#include <thread>

#include "Input.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

///
/// \brief The InputStream class
/// Describes an input communication from the server.
/// The communication is only possible if the stream (with the same name) is active within the
/// server. That implies an OutputStream communicating data through the hub.
///
// class SRC_API InputStream : public Input, public net::ClientSocket
class SRC_API InputStream : public Input
{
  public:
    ///
    /// \brief InputStream
    /// is used to instantiate an InputSensor.
    /// \param streamName
    /// [in] is an unique identifier name of stream.
    /// \param clientSocket
    /// [in] is an existing connection to a hub server.
    /// \warning The hub service must be accessible, that means the
    /// server hub is running on a machine you know the ip and the oppened port of the service.
    /// \warning Stream you want to link (by their names) needs to be active in the server hub.
    /// \exception hub::net::Socket::exception
    /// when the server is not found or by loosing connection to the server.
    /// Also occur when stream you want to link is not connected to the server.
    ///
    explicit InputStream( const std::string& streamName,
                          //                          const std::string& syncStreamName = "",
                          net::ClientSocket&& clientSocket = net::ClientSocket() );
    //                          bool mergeSyncAcqs                = true );

    //    InputStream( const InputStream& inputStream ) = delete;
    //    InputStream& operator=( const InputStream& inputStream ) = delete;
    //    InputStream&& operator=( InputStream&& inputStream ) = delete;
    //    virtual ~InputStream() override;

    InputStream( InputStream&& inputStream );
    ~InputStream();

    // #ifdef WIN32 // msvc warning C4250
    //   protected:
    //     void write( const unsigned char* data, size_t len ) const override;
    //     void read( unsigned char* data, size_t len ) const override;
    //     void close() const override;
    //     bool isOpen() const override;
    //     bool isEnd() const override;
    // #endif

  protected:
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
    void read( unsigned char* data, size_t len ) override;

    ///
    /// \brief close
    ///
    void close() override;

    ///
    /// \brief isEnd
    /// \return
    ///
    bool isEnd() const override;

    //    ///
    //    /// \brief getAcquisition
    //    /// \param sensorSpec
    //    /// \return
    //    ///
    //    Acquisition getAcq() override;
    void read( Acquisition& acq ) override;
    void read( SensorSpec& sensorSpec ) override;

  private:
    net::ClientSocket m_clientSocket;
    //    SensorSpec m_sensorSpec;
    bool m_streamViewerClientClosed = false;
    bool m_streamerClosed     = false;


    bool m_moved = false;

    //    std::mutex m_mtxRead;
    bool m_readAcqWaiting = false;

    friend class InputSyncStream;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool InputStream::isOpen() const {
    return m_clientSocket.isOpen();
}

inline void InputStream::read( unsigned char* data, size_t len ) {
    m_clientSocket.read( data, len );
}


inline void InputStream::close() {
    std::cout << "[InputStream] close() started" << std::endl;
    //    assert( !m_streamViewerClosed );
    //    if ( m_streamViewerClosed ) {}
    //    else {

    // inputSensor closing, prevent server this stream is done
    if ( !m_streamerClosed && ! m_streamViewerClientClosed ) {
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
    }

    if ( m_readAcqWaiting ) {
        while ( !m_streamerClosed && ! m_streamViewerClientClosed ) {
            std::cout << "[InputStream] close() waiting for server/streamer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
    }
    else {

        net::ClientSocket::Message message;
        m_clientSocket.read( message );
        while ( message == net::ClientSocket::Message::NEW_ACQ ) {
            auto acq = m_clientSocket.get<Acquisition>();
            m_clientSocket.read( message );
        }
        assert( message == net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED );
        std::cout << "[InputStream] stream viewer client closed" << std::endl;
    }

    //        assert(m_clientSocket.isOpen());
    if ( m_clientSocket.isOpen() ) m_clientSocket.close();

    //    std::cout << "[InputStream] close()" << std::endl;
    //    m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
    //    assert( m_clientSocket.isOpen() );
    //    if (m_)
    //        net::ClientSocket::Message mess;
    //        m_clientSocket.read( mess );
    //        assert(mess == net::ClientSocket::Message::STREAM_VIEWER_CLOSED);

    //        try {
    ////            while ( true ) {
    //            while ( m_clientSocket.isOpen() ) {
    //    Acquisition acq;
    //                m_clientSocket.read(acq);
    ////                auto acq = getAcq( );
    //            }
    //        }
    //        catch ( std::exception& ex ) {
    //            //        std::cout << "[InputStream] closing connection, all acqs received
    //            //        (streamViewer is done)" << std::endl;
    //            std::cout << ex.what() << std::endl;
    //        }
    //    }

    //    while (mess == net::ClientSocket::Message::NEW_ACQ) {
    //        auto acq = m_clientSocket.getAcq(m_sensorSpec);
    //        m_clientSocket.read( mess );
    //    }
    //    m_clientSocket.read( mess );
    //    assert ( mess == net::ClientSocket::Message::CLOSED );

    //    if (m_clientSocket.isOpen())
    //        m_clientSocket.close();
    std::cout << "[InputStream] close() ended" << std::endl;
}

inline bool InputStream::isEnd() const {
    return m_clientSocket.isEnd();
}

} // namespace io
} // namespace hub
