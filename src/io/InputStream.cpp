
#include "io/InputStream.hpp"

namespace hub {
namespace io {

InputStream::InputStream( const std::string& streamName,
                          //                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket ) :
    //                          bool mergeSyncAcqs ) :

    m_clientSocket( std::move( clientSocket ) ) {

    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );

    assert( streamName != "" );
    m_clientSocket.write( streamName );

    net::ClientSocket::Message mess;
    Input::read( mess );
    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
                                         << "' is not attached to server" );
#endif
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( mess == net::ClientSocket::Message::OK );

    //    m_clientSocket.write( syncStreamName );
    //    Input::read( mess );
    //    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
    //        throw net::ClientSocket::exception(
    //            ( std::string( "sync stream '" ) + syncStreamName + "' is not attached to server"
    //            )
    //                .c_str() );
    //    }
    //    assert( mess == net::ClientSocket::Message::OK );

    //    m_clientSocket.write( mergeSyncAcqs );
}

InputStream::InputStream( InputStream&& inputStream ) :
    m_clientSocket( std::move( inputStream.m_clientSocket ) ),
    //    m_sensorSpec( inputStream.m_sensorSpec ),
    m_streamViewerClientClosed( inputStream.m_streamViewerClientClosed ),
    m_streamerClosed(inputStream.m_streamerClosed),
    m_readAcqWaiting(inputStream.m_readAcqWaiting)
{
    inputStream.m_moved = true;
}

InputStream::~InputStream() {
    if ( !m_moved ) {
        //        std::cout << "[InputStream] ~InputStream() started" << std::endl;
        assert( !InputStream::isOpen() );
        //        if ( InputStream::isOpen() ) InputStream::close();
        //        assert( !InputStream::isOpen() );
        //        std::cout << "[InputStream] ~InputStream() ended" << std::endl;
    }
}

void InputStream::read( Acquisition& acq ) {
//    std::cout << "[InputStream] read(Acq&) started" << std::endl;
    assert(! m_readAcqWaiting);
    m_readAcqWaiting = true;

    //{
    //    m_mtxRead.lock();
    assert( !m_streamerClosed );

    //}
    // Acquisition InputStream::getAcq() {
    net::ClientSocket::Message message;
    //    do {
    //        Input::read( message );
    //        if ( message == net::ClientSocket::Message::PING ) {
    //            std::cout << "\033[32m[InputStream] receive ping from " <<
    //            m_clientSocket.getFdSock()
    //                      << "\033[0m" << std::endl;
    //        }
    //    } while ( message == net::ClientSocket::Message::PING );
    //    Input::read(message);
    //    Input & input = m_clientSocket;
    try {
        m_clientSocket.read( message );
    }
    catch ( const std::exception& ex ) {
        m_streamerClosed = true;
        m_readAcqWaiting         = false;
        throw new std::exception( ex );
    }
//    std::cout << "[InputStream] read(Acq&) message : " << message << std::endl;

    //    if (message == net::ClientSocket::Message::CLOSE) {
    //        throw net::ClientSocket::exception("[InputStream] streamer disconnected from server");
    //    }

    //    if ( message == net::ClientSocket::Message::STREAM_VIEWER_CLOSED ) {
    //        //        throw net::ClientSocket::exception("[InputStream] closing connection, all
    //        acqs
    //        //        received (streamViewer is done)");
    //        assert( !m_streamViewerClosed );
    //        m_streamViewerClosed = true;
    //        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
    //        throw net::ClientSocket::exception( "streamer closed connection" );
    //    }

    if ( message == net::ClientSocket::Message::STREAMER_CLOSED ) {
        std::cout << "[InputStream] streamer closed" << std::endl;

        m_streamerClosed = true;
        //        close();
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
        //        m_clientSocket.close();
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sreamer closed from server" );
    }
    else if ( message == net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED ) {
        std::cout << "[InputStream] stream viewer client closed" << std::endl;

//        m_streamerClosed = true;
        m_streamViewerClientClosed = true;
        //        close();
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
        //        m_clientSocket.close();
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sream viewer client closed from server" );
    }
    else if ( message == net::ClientSocket::Message::NEW_ACQ ) {
        //        assert( message == net::ClientSocket::Message::NEW_ACQ );
        //        if ( !m_clientSocket.isOpen() ) {
        //            throw net::ClientSocket::exception( "link broken by peer" );
        //        }
        assert( m_clientSocket.isOpen() );

        //    assert( message == net::ClientSocket::Message::NEW_ACQ );
        //    auto acq = Input::getAcq( );
        m_clientSocket.read( acq );
        //    auto acq = m_clientSocket.getAcq();
        //    return acq;
    }
    else { assert( false ); }

    m_readAcqWaiting = false;
    //    m_mtxRead.unlock();
//    std::cout << "[InputStream] read(Acq&) ended" << std::endl;
}

void InputStream::read( SensorSpec& sensorSpec ) {
    m_clientSocket.read( sensorSpec );
    //    m_sensorSpec = sensorSpec;
}

//    std::cout << "[InputStream] ~InputStream()" << std::endl;
//    net::ClientSocket::Message mess;
//    m_clientSocket.read( mess );
//    assert ( mess == net::ClientSocket::Message::CLOSED );
////    if ( mess != net::ClientSocket::Message::CLOSED ) {
////#ifdef DEBUG_SOCKET
////        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
////                                         << "' is not attached to server" );
////#endif
////        throw net::ClientSocket::exception(
////            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str()
///); /    } /    assert( mess == net::ClientSocket::Message::OK );

//}

// #ifdef WIN32
// void InputStream::write( const unsigned char* data, size_t len ) {
//     net::ClientSocket::write( data, len );
// }

// void InputStream::read( unsigned char* data, size_t len ) {
//     net::ClientSocket::read( data, len );
// }

// void InputStream::close() {
//     net::ClientSocket::close();
// }
// bool InputStream::isOpen() const {
//     return net::ClientSocket::isOpen();
// }
// bool InputStream::isEnd() const {
//     return net::ClientSocket::isEnd();
// }
// #endif

} // namespace io
} // namespace hub
