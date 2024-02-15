#include "ViewerClient2.hpp"

#include "ServerImpl2.hpp"
#include "StreamerClient2.hpp"
#include "core/Any.hpp"

namespace hub {
namespace server {

ViewerClient2::ViewerClient2( ServerImpl2* server, int iClient, net::ClientSocket&& sock ) :
    Client2( server, iClient ), m_socket( std::move( sock ) ) {

    assert( m_server != nullptr );
    m_server->addViewer( this );

    std::cout << headerMsg() << "new viewer at " << m_socket << std::endl;

    m_thread = std::thread( [this]() {
        try {

            assert( !m_viewerClosed );
            while ( !m_viewerClosed ) {
                hub::io::StreamBase::ClientMessage message;
                m_socket.read( message );
                if ( message == hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_SET_PROPERTY ) {
                    std::string streamName;
                    std::string objectName;
                    int property;
                    Any value;
                    m_socket.read( streamName );
                    m_socket.read( objectName );
                    m_socket.read( property );
                    m_socket.read( value );

                    m_socket.read( message );
                }
                else if ( message ==
                          hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_STREAM_ADDED ) {
                    // Todo fix
                    //assert( !m_clientStreamAdded );
                    m_clientStreamAdded = true;
                }
                else if ( message == hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED ) {
                    m_viewerClosed = true;
                }
                else {
                    assert( false );
                }
            }

            if ( m_socket.isOpen() )
                m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_CLOSED );
        }
        catch ( std::exception& ex ) {
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }
        std::thread( [this]() { delete this; } ).detach();
    } );

    printStatusMessage( "new viewer" );
}

ViewerClient2::~ViewerClient2() {

    assert( m_thread.joinable() );
    m_thread.join();

    assert( m_server != nullptr );
    m_server->delViewer( this );

    if ( m_socket.isOpen() ) {
        if ( !m_viewerClosed ) {
            m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_CLOSED );
        }
        int iTry = 0;
        while ( !m_viewerClosed && iTry < 10 ) {
            std::cout << "[ViewerClient2] close() waiting for server/viewer closing" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        assert( iTry < 10 );

        m_socket.close();
    }
}

std::string ViewerClient2::headerMsg() const {
    return Client2::headerMsg() + "[Viewer] ";
}

void ViewerClient2::notifyNewStreamer( const StreamerClient2* streamer ) {

    m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER );
    m_socket.write( streamer->m_streamName );

    m_socket.write( streamer->m_streamIpv4 );
    m_socket.write( streamer->m_streamPort );

    m_socket.write( streamer->m_header );

    m_clientStreamAdded = false;
    int iTry            = 0;
    while ( iTry < 10 && !m_clientStreamAdded ) {
        std::cout << "[ViewerClient] waiting for streamer added ..." << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        ++iTry;
    }
}

void ViewerClient2::notifyDelStreamer( const std::string& streamName ) {
    if ( m_viewerClosed ) return;

    try {
        m_socket.write( hub::io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER );
        m_socket.write( streamName );
    }
    catch ( std::exception& e ) {
        std::cout << headerMsg()
                  << "in : viewer is dead, this append when "
                     "viewer/streamer process was stopped in same time : "
                  << e.what() << std::endl;
        std::thread( [this]() { delete this; } ).detach();
    }
}

void ViewerClient2::end( hub::io::StreamBase::ServerMessage message ) {
    if ( m_socket.isOpen() ) {
        try {
            assert( m_socket.isOpen() );
            m_socket.write( message );
        }
        catch ( std::exception& ex ) {
            std::cout << headerMsg() << "catch exception " << ex.what() << std::endl;
        }
    }
}

void ViewerClient2::notifyInited() {
    assert( m_socket.isOpen() );
}

} // namespace server
} // namespace hub
