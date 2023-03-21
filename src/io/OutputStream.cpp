
#include "io/OutputStream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :
    m_clientSocket( std::move( clientSocket ) ) {

    Output::write( net::ClientSocket::Type::STREAMER );

    Output::write( streamName );

    net::ClientSocket::Message mess;
    m_clientSocket.read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );
}


//OutputStream::~OutputStream() {
//    std::cout << "[OutputStream] ~OutputStream()" << std::endl;
//    net::ClientSocket::Message mess;
//    if ( m_clientSocket.isOpen() ) {
//        m_clientSocket.read( mess );
//        assert( mess == net::ClientSocket::Message::CLOSED );
//    }
//    //    if ( mess != net::ClientSocket::Message::CLOSED ) {
//    // #ifdef DEBUG_SOCKET
//    //        DEBUG_MSG( getHeader( m_fdSock ) << "[OutputStream] exception sensor '" << streamName
//    //                                         << "' is not attached to server" );
//    // #endif
//    //        throw net::ClientSocket::exception(
//    //            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str()
//    //            );
//    //    }
//    //    assert( mess == net::ClientSocket::Message::OK );
//}

} // namespace io
} // namespace hub
