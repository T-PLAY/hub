#include "ClientSocket.hpp"

namespace hub {
namespace net {

void ClientSocket::connectToServer() {
    DEBUG_MSG( "[ClientSocket] connectToServer" );

    DEBUG_MSG( getHeader( mFdSock )
               << "[ClientSocket] ClientSocket('" << mIpv4 << ", " << mPort << ")" );

    /*   struct addrinfo *result = NULL, *ptr = NULL, hints;

       ZeroMemory( &hints, sizeof( hints ) );
       hints.ai_family   = AF_UNSPEC;
       hints.ai_socktype = SOCK_STREAM;
       hints.ai_protocol = IPPROTO_TCP;*/

    // Resolve the server address and port
    /*const int iResult = getaddrinfo(mIpv4.c_str(), mPort, &hints, &result );
    if ( iResult != 0 ) {
        printf( "getaddrinfo failed: %d\n", iResult );
        WSACleanup();
        return;
    }*/

    // Socket creation
    mFdSock = socket( PF_INET, SOCK_STREAM, 0 );
    if ( mFdSock < 0 ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }
    net::sSockets.push_back( mFdSock );

    // Server address construction
    struct sockaddr_in serv_addr;
    memset( &serv_addr, 0, sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr(mIpv4.c_str()); // winsock 1.0
    inet_pton( AF_INET, mIpv4.c_str(), &serv_addr.sin_addr.s_addr ); // winsock 2.0
    // InetPton( AF_INET, (PCWSTR)mIpv4.c_str(), &serv_addr.sin_addr.s_addr ); // winsock 2.0
    serv_addr.sin_port = htons( mPort ); // Server port

    // Connect to server
    while ( connect( mFdSock, (struct sockaddr*)&serv_addr, sizeof( serv_addr ) ) < 0 ) {
        //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
        throw Socket::exception( ( ( std::string( "Failed to connect to server at address " ) +
                                     mIpv4 + " and port " + std::to_string( mPort ) ) )
                                     .c_str() );
    }

    DEBUG_MSG( getHeader( mFdSock )
               << "[ClientSocket] connected to the server, starting communication" );

    DEBUG_MSG( getHeader( mFdSock ) << "[ClientSocket] connected to server" );

    DEBUG_MSG( getHeader( mFdSock ) << "new client on socket " << mFdSock );
}

ClientSocket::ClientSocket() : mIpv4( s_defaultServiceIp ), mPort( s_defaultServicePort ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( mFdSock ) << "ClientSocket()" );
#endif

    connectToServer();
}

ClientSocket::ClientSocket( const std::string& ipv4, int port ) : mIpv4( ipv4 ), mPort( port ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( mFdSock ) << "ClientSocket(std::string ipv4, int port)" );
#endif

    connectToServer();
    // ask server
}

// ClientSocket::ClientSocket( const std::string& sensorName,
//                             const std::string& syncSensorName,
//                             const std::string ipv4,
//                             int port ) :
//     mIpv4( ipv4 ), mPort( port ) {
//#ifdef DEBUG_SOCKET
//     DEBUG_MSG( getHeader( mFdSock ) << "ClientSocket(std::string sensorName, std::string "
//                                        "syncSensorName, std::string ipv4, int port)" );
//#endif
//     DEBUG_MSG( getHeader( mFdSock ) << "[ClientSocket] ClientSocket('" << sensorName << "', '"
//                                     << syncSensorName << ", " << ipv4 << ", " << port );

//    connectToServer();

//    DEBUG_MSG( getHeader( mFdSock )
//               << "[ClientSocket] connected to the server, starting communication" );

//    Type clientType = Type::STREAM_VIEWER;
//    // ask server
//    write( clientType );

//    write( sensorName );
//    Socket::Message mess;
//    read( mess );
//    if ( mess == Socket::Message::NOT_FOUND ) {
//        DEBUG_MSG( getHeader( mFdSock ) << "[ClientSocket] exception sensor '" << sensorName
//                                        << "' is not attached to server" );
//        throw Socket::exception(
//            ( std::string( "sensor '" ) + sensorName + "' is not attached to server" ).c_str() );
//    }
//    assert( mess == Socket::Message::OK );

//    write( syncSensorName );
//    read( mess );
//    if ( mess == Socket::Message::NOT_FOUND ) {
//        throw Socket::exception(
//            ( std::string( "sync sensor '" ) + syncSensorName + "' is not attached to server" )
//                .c_str() );
//    }
//    assert( mess == Socket::Message::OK );

//    DEBUG_MSG( getHeader( mFdSock ) << "[ClientSocket] connected to server" );
//}

ClientSocket::ClientSocket( socket_fd fdSock ) {
    mPort   = -1;
    mFdSock = fdSock;
    DEBUG_MSG( getHeader( mFdSock ) << "ClientSocket(socket_fd fdSock)" );
}

ClientSocket::ClientSocket( ClientSocket&& sock ) noexcept :
    Socket(),
    mIpv4( sock.mIpv4 ),
    mPort( sock.mPort )
//    mIsServer( sock.mIsServer )
{
    mFdSock      = sock.mFdSock;
    sock.mFdSock = INVALID_SOCKET;

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( mFdSock ) << "ClientSocket(ClientSocket && sock)" );
#endif
}

ClientSocket::~ClientSocket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( mFdSock ) << "~ClientSocket()" );
#endif
    clear();
}

// void ClientSocket::close() {
//     clear();
// }

// void ClientSocket::setupOutput( const std::string& sensorName ) const {
//#ifdef DEBUG_SOCKET
//     DEBUG_MSG( "ClientSocket::setOutputName(const std::string& sensorName)" );
//#endif

//    if ( !mIsServer ) {
//        ClientSocket::Type clientType = ClientSocket::Type::STREAMER;
//        write( clientType );

//        write( sensorName );
//        Socket::Message mess;
//        read( mess );
//        if ( mess == Socket::Message::FOUND ) {
//            assert( false );
//            throw Socket::exception(
//                ( std::string( "sensor '" ) + sensorName + "' is already attached to server" )
//                    .c_str() );
//        }
//        assert( mess == Socket::Message::NOT_FOUND );
//    }

//    io::Interface::setupOutput( sensorName );
//}

// void ClientSocket::setIsServer( bool isServer ) {
//     mIsServer = isServer;
// }

void ClientSocket::write( const unsigned char* data, size_t len ) const {
    assert( len > 0 );
    size_t uploadSize = 0;
    do {
        if ( !isConnected() ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG(
                getHeader( mFdSock )
                << "write(const unsigned char* data, size_t len) : isConnected() client lost" );
#endif
            throw Socket::exception( "Client lost" );
        }
        // winsock const char * data
        // winsock int len
        int byteSent;
        try {
            byteSent = send(
                mFdSock, (const char*)data + uploadSize, static_cast<int>( len - uploadSize ), 0 );
        }
        //        catch (std::runtime_error & e) {
        catch ( std::exception& e ) {
            assert( false );
            throw e;
        }

        if ( byteSent == -1 ) {
            DEBUG_MSG( getHeader( mFdSock ) << "can't send packet " << byteSent << "/" << len );
            throw Socket::exception( "Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
            DEBUG_MSG( "byteSent == 0, sleep" );
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
        uploadSize += byteSent;
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( mFdSock )
                   << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" );
#endif
    } while ( len != uploadSize );

#ifdef DEBUG_SOCKET
    // DEBUG_MSG(getHeader(mFdSock) << "write message ");
//    for (size_t i = 0; i < std::min(10, (int)len); ++i) {
//        std::cout << (int)*(data + i) << " ";
//    }
//    std::cout << std::endl;
#endif
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocket::read( unsigned char* data, size_t len ) const {
    size_t downloadSize = 0;
    do {
        int byteRead =
            recv( mFdSock, (char*)data + downloadSize, static_cast<int>( len - downloadSize ), 0 );
        if ( byteRead == -1 ) {
            DEBUG_MSG( "byte read == -1 error" );
            throw Socket::exception( "Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            throw Socket::exception( "0 byte received, peer connection lost" );
        }

        downloadSize += byteRead;
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( mFdSock )
                   << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" );
#endif
    } while ( len != downloadSize );

#ifdef DEBUG_SOCKET
//    DEBUG_MSG(getHeader(mFdSock) << "read message ");
//
//    for (size_t i = 0; i < std::min(10, (int)len); ++i) {
//        std::cout << (int)*(data + i) << " ";
//    }
//    std::cout << std::endl;
#endif
}

// void ClientSocket::waitClose() const {
//     Socket::Message message;
//     bool wantToClose = false;
//     while ( !wantToClose ) {
//         io::Interface::read( message );

//        switch ( message ) {
//        case Message::CLOSE:
//            wantToClose = true;
//            break;

//        default:
//            DEBUG_MSG( "waitClose, sleep" );
//            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//            break;
//        }
//    }

//    io::Interface::write( Message::OK );
//}

void ClientSocket::clear() {
    if ( mFdSock != INVALID_SOCKET ) {
        DEBUG_MSG( getHeader( mFdSock ) << "close socket" );
        net::clearSocket( mFdSock );
        mFdSock = INVALID_SOCKET;
    }
}

} // namespace net
} // namespace hub
