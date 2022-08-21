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

// static constexpr std::string_view type2string[static_cast<int>( Client::Type::COUNT )] = {
//     "NONE",
//     "STREAMER",
//     "VIEWER",
//     "STREAM_VIEWER",
// };
// std::ostream& operator<<( std::ostream& os, const Client::Type& type ) {
//     os << type2string[(int)type];
//     return os;
// }

// static constexpr std::string_view message2string[static_cast<int>( Client::Message::COUNT )] = {
//     "NONE",
//     "PING",
//     "SYNC",
//     "DATA",
//     "OK",
//     "CLOSE",
//     "DEL_STREAMER",
//     "NEW_STREAMER",
//     "NOT_FOUND",
//     "FOUND",
// };
// std::ostream& operator<<( std::ostream& os, const Client::Message& msg ) {
//     os << message2string[(int)msg];
//     return os;
// }

// Client::Client( net::ClientSocket&& clientSocket ) :
//     m_socket( new net::ClientSocket( std::move( clientSocket ) ) ) {};

//// Client* Client::init( net::ClientSocket&& sock ) {

////    Type clientType;
////    sock.read( reinterpret_cast<unsigned char*>( &clientType ), sizeof( Type ) );

////    switch ( clientType ) {
////    case Type::STREAMER:
////        return new Streamer( std::move( sock ) );
////    case Type::VIEWER:
////        return nullptr;
////        //        return new Viewer( std::move( sock ) );
////    case Type::STREAM_VIEWER:
////        return new StreamViewer( std::move( sock ) );
////    default:
////        assert( false );
////        return nullptr;
////    }
////    //    //        //    m_socket( new net::ClientSocket( clientSocket  ) ) {}
////}

// void Client::close() {}

// void Client::write( const unsigned char* data, size_t len ) const {
//     m_socket->write( data, len );
// }

// void Client::read( unsigned char* data, size_t len ) const {
//     m_socket->read( data, len );
// }

////////////////////////////////////////////////////////

Streamer::Streamer( const std::string& sensorName, net::ClientSocket&& clientSocket ) :
    //    Client( std::move( clientSocket ) ) {
    net::ClientSocket( std::move( clientSocket ) ) {

    std::cout << "\t[Streamer] Streamer(string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAMER );

    Interface::write( sensorName );

    net::ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == net::ClientSocket::Message::FOUND ) {
        assert( false );
        throw net::Socket::exception(
            ( std::string( "sensor '" ) + sensorName + "' is already attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::NOT_FOUND );

    //    }
}

Streamer::Streamer( ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {}

// void Streamer::startAsyncRoutine( Server* server, int iThread ) {
//     std::cout << "[Streamer] startAsyncRoutine(Server*, int)" << std::endl;
// }

////////////////////////////////////////////////////////

StreamViewer::StreamViewer( const std::string& sensorName,
                            const std::string& syncSensorName,
                            net::ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {
    //    Client( std::move( clientSocket ) ) {

    std::cout << "\t[StreamViewer] StreamViewer(string, string, ClientSocket&&)" << std::endl;

    Interface::write( net::ClientSocket::Type::STREAM_VIEWER );

    Interface::write( sensorName );
    ClientSocket::Message mess;
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        DEBUG_MSG( getHeader( mFdSock ) << "[StreamViewer] exception sensor '" << sensorName
                                        << "' is not attached to server" );
        throw ClientSocket::exception(
            ( std::string( "sensor '" ) + sensorName + "' is not attached to server" ).c_str() );
    }
    assert( mess == ClientSocket::Message::OK );

    Interface::write( syncSensorName );
    Interface::read( mess );
    if ( mess == ClientSocket::Message::NOT_FOUND ) {
        throw ClientSocket::exception(
            ( std::string( "sync sensor '" ) + syncSensorName + "' is not attached to server" )
                .c_str() );
    }
    assert( mess == ClientSocket::Message::OK );

    std::cout << "\t[StreamViewer] StreamViewer(string, string, ClientSocket&&) end" << std::endl;
    //    DEBUG_MSG( getHeader( mFdSock ) << "[ClientSocket] connected to server" );
}

StreamViewer::StreamViewer( ClientSocket&& clientSocket ) :
    net::ClientSocket( std::move( clientSocket ) ) {}

// void Viewer::startAsyncRoutine( Server* server, int iThread ) {}

////////////////////////////////////////////////////////

// Viewer::Viewer( net::ClientSocket&& clientSocket ) :
//     net::ClientSocket( std::move( clientSocket ) ) {
//     //    Client( std::move( clientSocket ) ) {

//    Interface::write( net::ClientSocket::Type::VIEWER );

//}

// void StreamViewer::startAsyncRoutine( Server* server, int iThread ) {}

} // namespace io
} // namespace hub
