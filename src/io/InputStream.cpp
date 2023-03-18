
#include "io/InputStream.hpp"

namespace hub {
namespace io {

InputStream::InputStream( const std::string& streamName,
                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket) :
//                          bool mergeSyncAcqs ) :

    m_clientSocket( std::move( clientSocket ) ) {

    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );

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
//            ( std::string( "sync stream '" ) + syncStreamName + "' is not attached to server" )
//                .c_str() );
//    }
//    assert( mess == net::ClientSocket::Message::OK );

//    m_clientSocket.write( mergeSyncAcqs );
}

//#ifdef WIN32
//void InputStream::write( const unsigned char* data, size_t len ) {
//    net::ClientSocket::write( data, len );
//}

//void InputStream::read( unsigned char* data, size_t len ) {
//    net::ClientSocket::read( data, len );
//}

//void InputStream::close() {
//    net::ClientSocket::close();
//}
//bool InputStream::isOpen() const {
//    return net::ClientSocket::isOpen();
//}
//bool InputStream::isEnd() const {
//    return net::ClientSocket::isEnd();
//}
//#endif

Acquisition InputStream::getAcquisition( const SensorSpec& sensorSpec ) {
    net::ClientSocket::Message message;
    do {
        Input::read( message );
        if ( message == net::ClientSocket::Message::PING ) {
            std::cout << "\033[32m[InputStream] receive ping from " << m_clientSocket.getFdSock()
                      << "\033[0m" << std::endl;
        }
    } while ( message == net::ClientSocket::Message::PING );

    assert( message == net::ClientSocket::Message::NEW_ACQ );
    auto acq = Input::getAcquisition( sensorSpec );
    return acq;
}

} // namespace io
} // namespace hub
