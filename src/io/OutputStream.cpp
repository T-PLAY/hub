
#include "io/OutputStream.hpp"

#include <iostream>
#include <typeinfo>

namespace hub {
namespace io {

OutputStream::OutputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :
//    net::ClientSocket( std::move( clientSocket ) )
    m_clientSocket(std::move(clientSocket))
{

    Output::write( net::ClientSocket::Type::STREAMER );

    Output::write( streamName );

    net::ClientSocket::Message mess;
//    Input::read( mess );
//    net::ClientSocket::read( mess );
    m_clientSocket.read(mess);
    if ( mess == net::ClientSocket::Message::FOUND ) {
        throw net::Socket::exception(
            ( std::string( "stream '" ) + streamName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );
}


} // namespace io
} // namespace hub
