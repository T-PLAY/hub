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
    m_sock( net::ClientSocket( ipv4, port, false ) ) {}

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
