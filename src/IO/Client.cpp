#include "Client.hpp"

#include <iostream>
#include <typeinfo>

//#include <any>
//#include <cassert>
//#include <iostream>
//#include <list>
//#include <map>
//#include <string>
//#include <typeinfo>
//#include <vector>

//#include "Macros.hpp"
//#include "SensorSpec.hpp"

namespace hub {
namespace io {

static std::string getServerHeader( int iThread ) {
    const std::string str = "\t\033[" + std::to_string( 31 + iThread % 7 ) +
                            "m[server:" + std::to_string( iThread ) + "]\033[0m ";
    return str;
}

Server::Server() {}

Server::Server( int port ) : mServerSock( port ) {}

void Server::run() {
    int iThread = 1;

    while ( true ) {

        net::ClientSocket sock = mServerSock.waitNewClient();
//        sock.setIsServer( true );
        std::cout << getServerHeader( 0 ) << "new client" << std::endl;

        io::Client * newClient = io::Client::init(std::move(sock));
        newClient->startAsyncRoutine(this, iThread);

        ++iThread;
    }
}



static constexpr std::string_view type2string[static_cast<int>( Client::Type::COUNT )] = {
    "NONE",
    "STREAMER",
    "VIEWER",
    "STREAM_VIEWER",
};
std::ostream& operator<<( std::ostream& os, const Client::Type& type ) {
    os << type2string[(int)type];
    return os;
}

static constexpr std::string_view message2string[static_cast<int>( Client::Message::COUNT )] = {
    "NONE",
    "PING",
    "SYNC",
    "DATA",
    "OK",
    "CLOSE",
    "DEL_STREAMER",
    "NEW_STREAMER",
    "NOT_FOUND",
    "FOUND",
};
std::ostream& operator<<( std::ostream& os, const Client::Message& msg ) {
    os << message2string[(int)msg];
    return os;
}

Client::Client( net::ClientSocket&& clientSocket ) :
    m_socket( new net::ClientSocket( std::move( clientSocket ) ) ) {};

Client* Client::init( net::ClientSocket&& sock ) {

    Type clientType;
    sock.read( reinterpret_cast<unsigned char*>( &clientType ), sizeof( Type ) );

    switch ( clientType ) {
    case Type::STREAMER:
        return new Streamer( std::move( sock ) );
    case Type::VIEWER:
        return new Viewer( std::move( sock ) );
    case Type::STREAM_VIEWER:
        return new StreamViewer( std::move( sock ) );
    default:
        assert( false );
        return nullptr;
    }
    //    //        //    m_socket( new net::ClientSocket( clientSocket  ) ) {}
}

void Client::close() {}

void Client::write( const unsigned char* data, size_t len ) const {
    m_socket->write( data, len );
}

void Client::read( unsigned char* data, size_t len ) const {
    m_socket->read( data, len );
}

////////////////////////////////////////////////////////

Streamer::Streamer( net::ClientSocket&& clientSocket ) : Client( std::move( clientSocket ) ) {}

void Streamer::startAsyncRoutine( Server* server, int iThread ) {
}

////////////////////////////////////////////////////////

Viewer::Viewer( net::ClientSocket&& clientSocket ) : Client( std::move( clientSocket ) ) {}

void Viewer::startAsyncRoutine( Server* server, int iThread ) {}

////////////////////////////////////////////////////////

StreamViewer::StreamViewer( net::ClientSocket&& clientSocket ) :
    Client( std::move( clientSocket ) ) {}

void StreamViewer::startAsyncRoutine( Server* server, int iThread ) {}

} // namespace io
} // namespace hub
