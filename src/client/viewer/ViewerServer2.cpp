#include "ViewerServer2.hpp"

#include <regex>
#include <sstream>

#include <core/Serializer.hpp>
#include <core/io/Memory.hpp>

#include <core/Macros.hpp>

namespace hub {
namespace client {

ViewerServer2::ViewerServer2( const std::string& name,
                              ViewerHandler&& viewerHandler,
                              const std::string& serverIpv4,
                              int serverPort,
                              bool autoConnect ) :

    ViewerInterface( name, std::move( viewerHandler ), serverIpv4, serverPort, autoConnect ),
    m_sock( net::ClientSocket( serverIpv4, serverPort, false ) ) {

    if ( m_autoConnect ) {
        m_thread = std::thread( [this]() { threadRoutine(); } );
    }
}

ViewerServer2::~ViewerServer2() {
    m_stopThread = true;

    if ( m_sock.isOpen() ) {
        m_sock.write( hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
    }

    if ( m_thread.joinable() ) {
        assert( m_thread.joinable() );
        m_thread.join();
    }

    m_streams.clear();
}

void ViewerServer2::setIpv4( const std::string& serverIpv4 ) {
    ViewerInterface::setIpv4( serverIpv4 );

    DEBUG_MSG( "[Viewer] setIpv4 " << serverIpv4 );
    assert( !m_serverConnected );
    m_sock.setIpv4( serverIpv4 );
}

void ViewerServer2::setPort( int serverPort ) {
    ViewerInterface::setPort( serverPort );

    DEBUG_MSG( "[Viewer] setPort " << serverPort );
    assert( !m_serverConnected );
    m_sock.setPort( serverPort );
}

void ViewerServer2::setProperty( const std::string& streamName,
                                 const std::string& objectName,
                                 int property,
                                 const Any& value ) {
    if ( m_sock.isOpen() ) {
        m_sock.write( hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_SET_PROPERTY );
        m_sock.write( streamName );
        m_sock.write( objectName );
        m_sock.write( property );
        m_sock.write( value );
    }
}

void ViewerServer2::setAutoConnect( bool autoConnect ) {
    ViewerInterface::setAutoConnect( autoConnect );

    if ( m_autoConnect ) {
        m_thread = std::thread( [this]() { threadRoutine(); } );
    }
    else {
        m_stopThread = true;
        if ( m_thread.joinable() ) {
            assert( m_thread.joinable() );
            m_thread.join();
        }
    }
}

void ViewerServer2::threadRoutine() {
    assert( !m_threadRunning );
    assert( !m_stopThread );
    m_threadRunning = true;
    while ( !m_stopThread ) {
        try {

            DEBUG_MSG( "[Viewer] trying to connect to server : " << m_sock );
            // DEBUG_MSG( "[Viewer] trying to connect to server at serverIpv4 "

            assert( !m_sock.isOpen() );
            m_sock.connect();
            m_serverConnected = true;
            assert( m_sock.isOpen() );

            m_sock.write( hub::io::StreamBase::ClientType::VIEWER );

            if ( m_viewerHandler.onServerConnected )
                m_viewerHandler.onServerConnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

            printStatus();

            while ( !m_stopThread ) {

                hub::io::StreamBase::ServerMessage serverMessage;
                // DEBUG_MSG( "[Viewer] waiting for new server message ..." )
                m_sock.read( serverMessage );
                // DEBUG_MSG( "[Viewer] new server message : " << serverMessage )

                switch ( serverMessage ) {

                case hub::io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER: {

                    std::string streamName;
                    m_sock.read( streamName );

                    std::string streamIpv4;
                    m_sock.read( streamIpv4 );
                    if (streamIpv4 == "127.0.0.1") {
                        streamIpv4 = m_serverIpv4;
                    }

                    int streamPort;
                    m_sock.read( streamPort );

                    io::Header header;
                    m_sock.read( header );

                    DEBUG_MSG( "[Viewer] new streamer '" << streamName << "', header size : " << PRETTY_BYTES(header.getSize())  );

                    addStream( streamName, streamIpv4, streamPort, std::move( header ) );

                    m_sock.write( io::StreamBase::ClientMessage::VIEWER_CLIENT_STREAM_ADDED );

                } break;

                case hub::io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER: {
                    std::string streamName;
                    m_sock.read( streamName );
                    DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );

                    delStream( streamName );

                } break;

                case hub::io::StreamBase::ServerMessage::SERVER_CLOSED: {
                    DEBUG_MSG( "[Viewer] server closed" );
                    assert( m_sock.isOpen() );
                    m_sock.write( hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
                    throw net::ClientSocket::exception( "[Viewer] server closed" );
                }

                case hub::io::StreamBase::ServerMessage::VIEWER_CLOSED: {
                    DEBUG_MSG( "[Viewer] viewer client closed" );
                    assert( m_sock.isOpen() );
                    m_sock.write( hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
                    throw net::ClientSocket::exception( "[Viewer] viewer client closed" );
                }

                case hub::io::StreamBase::ServerMessage::VIEWER_SET_PROPERTY: {
                    DEBUG_MSG( "[Viewer] viewer client set property" );
                    assert( m_sock.isOpen() );
                    std::string streamName;
                    std::string objectName;
                    int property;
                    Any value;
                    m_sock.read( streamName );
                    m_sock.read( objectName );
                    m_sock.read( property );
                    m_sock.read( value );

                    if ( m_viewerHandler.onSetProperty )
                        m_viewerHandler.onSetProperty(
                            streamName.c_str(), objectName.c_str(), property, value );

                } break;

                default: {
                    DEBUG_MSG( "[Viewer] unknown message from server" );
                    assert( false );
                    throw net::ClientSocket::exception( "[viewer] unknown message from server " );
                }
                } // switch (serverMessage)

            } // while (! m_stopThread)
        }
        catch ( net::ClientSocket::exception& e ) {
            if ( m_serverConnected ) {
                DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
            }
            else {
                DEBUG_MSG( "[Viewer] server not found : " << m_sock );
                // DEBUG_MSG( "[Viewer] server not found at serverIpv4 "
                if ( m_viewerHandler.onServerNotFound )
                    m_viewerHandler.onServerNotFound( m_sock.getIpv4().c_str(), m_sock.getPort() );
            }
            int i = 0;
            while ( !m_stopThread && i < 5 ) {
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++i;
            }
        }

        if ( m_serverConnected ) {
            m_serverConnected = false;
            DEBUG_MSG( "[Viewer] server disconnected, close all client connections" );
            printStatus();

            if ( m_viewerHandler.onServerDisconnected )
                m_viewerHandler.onServerDisconnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

            m_streams.clear();
            assert( m_streams.empty() );
        }

        if ( m_sock.isOpen() ) { m_sock.close(); }

    } // while (! m_stopThread)
    assert( m_stopThread );
    m_threadRunning = false;
}

} // namespace client
} // namespace hub
