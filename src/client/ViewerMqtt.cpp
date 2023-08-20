#include "ViewerMqtt.hpp"

//#include <regex>
//#include <sstream>

//#include "InputSensor.hpp"
#include <io/input/InputStream.hpp>
#include <client/StreamViewer.hpp>

namespace hub {
namespace client {
namespace viewer {}

ViewerMqtt::ViewerMqtt(
    const std::string& ipv4,
    int port,
    std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
    std::function<void( const char*, const SensorSpec& )> onDelStreamer,
    std::function<void( const char*, int )> onServerNotFound,
    std::function<void( const char*, int )> onServerConnected,
    std::function<void( const char*, int )> onServerDisconnected,
    std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char*, const char*, int, const Any& )> onSetProperty,
    std::function<void( const char* )> onLogMessage ) :
    ViewerInterface(ipv4, port, onNewStreamer, onDelStreamer, onServerNotFound, onServerConnected, onServerDisconnected, onNewAcquisition, onSetProperty, onLogMessage)
{}

ViewerMqtt::~ViewerMqtt()
{
    DEBUG_MSG( "[ViewerMqtt] ~ViewerMqtt()" );

    assert(! m_stopThread);
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();

}

void ViewerMqtt::routine()
{
        while ( !m_stopThread ) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;

            try {

                DEBUG_MSG( "[ViewerServer] trying to connect to server at ipv4 "
                           << m_ipv4 << " and port " << m_port );

//                assert( !m_sock.isOpen() );
//                m_sock.connect();
                m_serverConnected = true;
//                assert( m_sock.isOpen() );

//                m_sock.write( net::ClientSocket::Type::VIEWER );

                if ( m_onServerConnected )
                    m_onServerConnected( m_ipv4.c_str(), m_port );

                while ( !m_stopThread ) {

                    net::ClientSocket::Message serverMessage;
//                    m_sock.read( serverMessage );

                    switch ( serverMessage ) {

                    case net::ClientSocket::Message::NEW_STREAMER: {

                        std::string streamName;
//                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
//                        m_sock.read( sensorSpec );
                        std::cout << "[ViewerServer] new streamer '" << streamName << "'" << std::endl;

                        DEBUG_MSG( "[ViewerServer] new streamer '" << streamName << "'" );

                        assert( m_streams.find( streamName ) == m_streams.end() );

                        if ( m_onNewStreamer ) {

//                            StreamViewer streamViewer<InputStreamT>( m_ipv4, m_port, streamName, sensorSpec, m_onNewStreamer, m_onDelStreamer, m_onNewAcquisition, m_onLogMessage );
//                            auto streamViewer = StreamViewer<input::InputStreamMqtt>( m_ipv4, m_port, streamName, sensorSpec, m_onNewStreamer, m_onDelStreamer, m_onNewAcquisition, m_onLogMessage );


                            m_streams[streamName] =
                                std::make_unique<StreamViewer<input::InputStreamMqtt>>( m_ipv4, m_port, streamName, sensorSpec, m_onNewStreamer, m_onDelStreamer, m_onNewAcquisition, m_onLogMessage );
                        }

                        // prevent all son the father is comming

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
//                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
//                        m_sock.read( sensorSpec );
                        DEBUG_MSG( "[ViewerServer] del streamer '" << streamName << "'" );

                        if ( m_onNewStreamer ) {
                            assert( m_onDelStreamer );
                            assert( m_streams.find( streamName ) != m_streams.end() );
                            m_streams.erase( streamName );
                        }

                        // prevent all son the father is leaving

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::SERVER_CLOSED: {
                        DEBUG_MSG( "[ViewerServer] server closed" );
//                        assert( m_sock.isOpen() );
//                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        throw net::ClientSocket::exception( "[viewer] server closed" );
                    }

                    case net::ClientSocket::Message::VIEWER_CLIENT_CLOSED: {
                        DEBUG_MSG( "[ViewerServer] viewer client closed" );
//                        assert( m_sock.isOpen() );
//                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        throw net::ClientSocket::exception( "[viewer] viewer client closed" );
                    }

                    case net::ClientSocket::Message::SET_PROPERTY: {
                        DEBUG_MSG( "[ViewerServer] viewer client set property" );
//                        assert( m_sock.isOpen() );
                        std::string streamName;
                        std::string objectName;
                        int property;
                        Any value;
//                        m_sock.read( streamName );
//                        m_sock.read( objectName );
//                        m_sock.read( property );
//                        m_sock.read( value );

                        if ( m_onSetProperty )
                            m_onSetProperty(
                                streamName.c_str(), objectName.c_str(), property, value );

                    } break;

                    default: {
                        assert( false );
                        DEBUG_MSG( "[ViewerServer] unknown message from server" );
                        throw net::ClientSocket::exception(
                            "[viewer] unknown message from server " );
                    }
                    } // switch (serverMessage)

                } // while (! m_stopThread)
            }
            catch ( net::ClientSocket::exception& e ) {
                if ( m_serverConnected ) {
                    DEBUG_MSG( "[ViewerServer] server disconnected, catch exception " << e.what() );
                }
                else {
                    DEBUG_MSG( "[ViewerServer] server not found at ipv4 "
                               << m_ipv4 << " and port " << m_port );
                    if ( m_onServerNotFound )
                        m_onServerNotFound( m_ipv4.c_str(), m_port );
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
                DEBUG_MSG( "[ViewerServer] server disconnected, close all client connections" );

                if ( m_onServerDisconnected )
                    m_onServerDisconnected( m_ipv4.c_str(), m_port );

                m_streams.clear();
                assert( m_streams.empty() );

                // #ifdef OS_LINUX
                // #endif
            }

        } // while (! m_stopThread)

}

// end namespace viewer

// ViewerMqtt::ViewerMqtt(
//     std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
//     std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
//     std::function<void( const char* ipv4, int port )> onServerNotFound,
//     std::function<void( const char* ipv4, int port )> onServerConnected,
//     std::function<void( const char* ipv4, int port )> onServerDisconnected,
//     std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
//     std::function<
//         void( const char* streamName, const char* objectName, int property, const Any& value )>
//         onSetProperty,
//     const std::string& ipv4,
//     int port,
//     bool autoSync,
//     std::function<void( const char* logMessage )> onLogMessage ) :

//{
//}

} // namespace client
} // namespace hub
