#include "ViewerServer2.hpp"

#include <regex>
#include <sstream>

// #include <core/io/Book.hpp>
#include <core/Serializer.hpp>
#include <core/io/Memory.hpp>

namespace hub {
// namespace impl2 {
namespace client {

ViewerServer2::ViewerServer2( const std::string& name,
                              ViewerHandler&& viewerHandler,
                              const std::string& ipv4,
                              int port ) :

    ViewerInterface( name, std::move( viewerHandler ), ipv4, port ),
    m_sock( net::ClientSocket( ipv4, port, false ) ) {

    m_thread = std::thread( [this]() {
        while ( !m_stopThread ) {
            try {

                DEBUG_MSG( "[Viewer] trying to connect to server : " << m_sock );
                // DEBUG_MSG( "[Viewer] trying to connect to server at ipv4 "
                // << m_sock.getIpv4() << " and port " << m_sock.getPort() );

                assert( !m_sock.isOpen() );
                m_sock.connect();
                m_serverConnected = true;
                assert( m_sock.isOpen() );

                //                m_sock.write( net::ClientSocket::Type::VIEWER );
                m_sock.write( hub::io::StreamBase::ClientType::VIEWER );

                if ( m_viewerHandler.onServerConnected )
                    m_viewerHandler.onServerConnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                printStatus();

                while ( !m_stopThread ) {

                    hub::io::StreamBase::ServerMessage serverMessage;
                    DEBUG_MSG( "[Viewer] waiting for new server message ..." )
                    m_sock.read( serverMessage );
                    DEBUG_MSG( "[Viewer] new server message : " << serverMessage )

                    switch ( serverMessage ) {

                    case hub::io::StreamBase::ServerMessage::VIEWER_NEW_STREAMER: {
                        ++m_nStreamer;

#ifdef DEBUG
                        std::this_thread::sleep_for(
                            std::chrono::milliseconds( 100 ) ); // log messages concurrency
#endif

                        std::string streamName;
                        m_sock.read( streamName );
                        // std::cout << "[Viewer] read stream name: " << streamName << std::endl;

                        hub::io::StreamBase::ServerMessage servMess;
                        m_sock.read( servMess );
                        assert( servMess ==
                                hub::io::StreamBase::ServerMessage::RETAINED_DATA_START );
                        std::vector<Data_t> retainedData;
                        m_sock.read( retainedData );
                        m_sock.read( servMess );
                        assert( servMess == hub::io::StreamBase::ServerMessage::RETAINED_DATA_END );

                        // hub::io::Book book(retainedData);

                        hub::io::Memory memory( retainedData );

                        // hub::Serializer serializer;
                        sensor::SensorSpec sensorSpec;
                        // serializer.read(sensorSpec);
                        memory.read( sensorSpec );
                        // m_sock.read( sensorSpec );
                        // std::cout << "[Viewer] read sensor spec: " << sensorSpec << std::endl;
                        // std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "'" );

                        addStream( streamName, sensorSpec );

                        // prevent all son the father is comming

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        //                        std::this_thread::sleep_for(
                        //                        std::chrono::milliseconds( 100 ) );

                    } break;

                    case hub::io::StreamBase::ServerMessage::VIEWER_DEL_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        // sensor::SensorSpec sensorSpec;
                        // m_sock.read( sensorSpec );
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );

                        //                        if ( m_onNewStreamer ) {
                        //                            assert( m_onDelStreamer );
                        //                            assert( m_streams.find( streamName ) !=
                        //                            m_streams.end() ); m_streams.erase( streamName
                        //                            );
                        //                        }
                        deleteStream( streamName );

                        // prevent all son the father is leaving

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        //                        std::this_thread::sleep_for(
                        //                        std::chrono::milliseconds( 100 ) );

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
                        throw net::ClientSocket::exception(
                            "[viewer] unknown message from server " );
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
                    // DEBUG_MSG( "[Viewer] server not found at ipv4 "
                    // << m_sock.getIpv4() << " and port " << m_sock.getPort() );
                    if ( m_viewerHandler.onServerNotFound )
                        m_viewerHandler.onServerNotFound( m_sock.getIpv4().c_str(),
                                                          m_sock.getPort() );
                }
                // ping the server when this one is not started or visible in the network
                // able the viewer clients to be aware of the starting of server less than 100
                // milliseconds.
                int i = 0;
                while ( !m_stopThread && i < 10 ) {
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++i;
                }
            }

            if ( m_serverConnected ) {
                m_serverConnected = false;
                DEBUG_MSG( "[Viewer] server disconnected, close all client connections" );
                printStatus();

                if ( m_viewerHandler.onServerDisconnected )
                    m_viewerHandler.onServerDisconnected( m_sock.getIpv4().c_str(),
                                                          m_sock.getPort() );

                m_streams.clear();
                assert( m_streams.empty() );
            }

            if ( m_sock.isOpen() ) { m_sock.close(); }

        } // while (! m_stopThread)
    } );  // thread
}

ViewerServer2::~ViewerServer2() {
    DEBUG_MSG( "[Viewer] ~Viewer()" );
    m_stopThread = true;

    if ( m_sock.isOpen() ) {
        m_sock.write( hub::io::StreamBase::ClientMessage::VIEWER_CLIENT_CLOSED );
    }

    assert( m_thread.joinable() );
    m_thread.join();

    m_streams.clear();
    DEBUG_MSG( "[Viewer] ~Viewer() done" );
}

void ViewerServer2::setIpv4( const std::string& ipv4 ) {
    ViewerInterface::setIpv4( ipv4 );

    DEBUG_MSG( "[Viewer] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_sock.setIpv4( ipv4 );
}

void ViewerServer2::setPort( int port ) {
    ViewerInterface::setPort( port );

    DEBUG_MSG( "[Viewer] setPort " << port );
    assert( !m_serverConnected );
    m_sock.setPort( port );
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

} // namespace client
// } // namespace impl2
} // namespace hub
