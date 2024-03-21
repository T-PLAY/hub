#include "ViewerServer.hpp"

#ifdef HUB_USE_SERVER

#    include <regex>
#    include <sstream>

namespace hub {
namespace client {

////                    net::ClientSocket( m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort() ) )
///);

ViewerServer::ViewerServer( const std::string& name,
                            ViewerHandler&& viewerHandler,
                            const std::string& ipv4,
                            int port ) :

    ViewerInterface( name, std::move( viewerHandler ), ipv4, port ),
    m_sock( net::ClientSocket( ipv4, port, false ) ) {

    // m_thread = std::thread( [this]() {
    //     while ( !m_stopThread ) {
    //         try {
    //
    //             DEBUG_MSG( "[ViewerServer] trying to connect to server at ipv4 "
    //                        << m_sock.getIpv4() << " and port " << m_sock.getPort() );
    //
    //             assert( !m_sock.isOpen() );
    //             m_sock.connect();
    //             m_serverConnected = true;
    //             assert( m_sock.isOpen() );
    //
    //             m_sock.write( io::StreamBase::ClientType::VIEWER );
    //
    //             if ( m_viewerHandler.onServerConnected )
    //                 m_viewerHandler.onServerConnected( m_sock.getIpv4().c_str(), m_sock.getPort() );
    //
    //             while ( !m_stopThread ) {
    //
    //                 io::StreamBase::ServerMessage serverMessage;
    //                 m_sock.read( serverMessage );
    //
    //                 switch ( serverMessage ) {
    //
    //                 case io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER: {
    //
    //                     std::string streamName;
    //                     m_sock.read( streamName );
    //                     sensor::SensorSpec sensorSpec;
    //                     m_sock.read( sensorSpec );
    //                     std::cout << "[ViewerServer] new streamer '" << streamName << "'"
    //                               << std::endl;
    //
    //                     DEBUG_MSG( "[ViewerServer] new streamer '" << streamName << "'" );
    //
    //                     addStream( streamName, sensorSpec );
    //
    //                     //// std::make_unique<StreamViewer<input::InputStreamServer>>( *this,
    //                     /// streamName, sensorSpec ); /
    //                     /// std::make_unique<viewer::StreamViewer<input::InputStreamServer>>(
    //
    //                 } break;
    //
    //                 case io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER: {
    //                     std::string streamName;
    //                     m_sock.read( streamName );
    //                     sensor::SensorSpec sensorSpec;
    //                     m_sock.read( sensorSpec );
    //                     DEBUG_MSG( "[ViewerServer] del streamer '" << streamName << "'" );
    //
    //                     deleteStream( streamName );
    //
    //                 } break;
    //
    //                 case io::StreamBase::ServerMessage::SERVER_CLOSED: {
    //                     DEBUG_MSG( "[ViewerServer] server closed" );
    //                     assert( m_sock.isOpen() );
    //                     m_sock.write( io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
    //                     throw net::ClientSocket::exception( "[viewer] server closed" );
    //                 }
    //
    //                 case io::StreamBase::ServerMessage::VIEWER_CLOSED: {
    //                     DEBUG_MSG( "[ViewerServer] viewer client closed" );
    //                     assert( m_sock.isOpen() );
    //                     m_sock.write( io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
    //                     throw net::ClientSocket::exception( "[viewer] viewer client closed" );
    //                 }
    //
    //                 case io::StreamBase::ServerMessage::VIEWER_SET_PROPERTY: {
    //                     DEBUG_MSG( "[ViewerServer] viewer client set property" );
    //                     assert( m_sock.isOpen() );
    //                     std::string streamName;
    //                     std::string objectName;
    //                     int property;
    //                     Any value;
    //                     m_sock.read( streamName );
    //                     m_sock.read( objectName );
    //                     m_sock.read( property );
    //                     m_sock.read( value );
    //
    //                     if ( m_viewerHandler.onSetProperty )
    //                         m_viewerHandler.onSetProperty(
    //                             streamName.c_str(), objectName.c_str(), property, value );
    //
    //                 } break;
    //
    //                 default: {
    //                     assert( false );
    //                     DEBUG_MSG( "[ViewerServer] unknown message from server" );
    //                     throw net::ClientSocket::exception(
    //                         "[viewer] unknown message from server " );
    //                 }
    //                 } // switch (serverMessage)
    //
    //             } // while (! m_stopThread)
    //         }
    //         catch ( net::ClientSocket::exception& e ) {
    //             if ( m_serverConnected ) {
    //                 DEBUG_MSG( "[ViewerServer] server disconnected, catch exception " << e.what() );
    //             }
    //             else {
    //                 DEBUG_MSG( "[ViewerServer] server not found at ipv4 "
    //                            << m_sock.getIpv4() << " and port " << m_sock.getPort() );
    //                 if ( m_viewerHandler.onServerNotFound )
    //                     m_viewerHandler.onServerNotFound( m_sock.getIpv4().c_str(),
    //                                                       m_sock.getPort() );
    //             }
    //             int i = 0;
    //             while ( !m_stopThread && i < 10 ) {
    //                 std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //                 ++i;
    //             }
    //         }
    //
    //         if ( m_serverConnected ) {
    //             m_serverConnected = false;
    //             DEBUG_MSG( "[ViewerServer] server disconnected, close all client connections" );
    //
    //             if ( m_viewerHandler.onServerDisconnected )
    //                 m_viewerHandler.onServerDisconnected( m_sock.getIpv4().c_str(),
    //                                                       m_sock.getPort() );
    //
    //             m_streams.clear();
    //             assert( m_streams.empty() );
    //         }
    //
    //         if ( m_sock.isOpen() ) { m_sock.close(); }
    //
    //     } // while (! m_stopThread)
    // } );  // thread
}

ViewerServer::~ViewerServer() {
    DEBUG_MSG( "[ViewerServer] ~ViewerServer()" );
    m_stopThread = true;

    if ( m_sock.isOpen() ) { m_sock.write( io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED ); }

    assert( m_thread.joinable() );
    m_thread.join();

    m_streams.clear();
    DEBUG_MSG( "[ViewerServer] ~ViewerServer() done" );
}

void ViewerServer::setIpv4( const std::string& ipv4 ) {
    ViewerInterface::setIpv4( ipv4 );

    DEBUG_MSG( "[ViewerServer] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_sock.setIpv4( ipv4 );
}

void ViewerServer::setPort( int port ) {
    ViewerInterface::setPort( port );

    DEBUG_MSG( "[ViewerServer] setPort " << port );
    assert( !m_serverConnected );
    m_sock.setPort( port );
}

void ViewerServer::setProperty( const std::string& streamName,
                                const std::string& objectName,
                                int property,
                                const Any& value ) {
    if ( m_sock.isOpen() ) {
        m_sock.write( io::StreamBase::ClientMessage::VIEWER_CLIENT_SET_PROPERTY );
        m_sock.write( streamName );
        m_sock.write( objectName );
        m_sock.write( property );
        m_sock.write( value );
    }
}

} // namespace client
} // namespace hub

#endif
