#include "Viewer.hpp"

#include "Net/ClientSocket.hpp"

//#define DEBUG_VIEWER
#include <regex>

namespace hub {

Viewer::Viewer( std::function<void( const std::string&, const SensorSpec& )> onNewStreamer,
                std::function<void( const std::string&, const SensorSpec& )> onDelStreamer,
                std::function<void( const std::string& ipv4, int port )> onServerConnected,
                std::function<void( const std::string& ipv4, int port )> onServerDisconnected,
                const std::string& ipv4,
                int port ) :

    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_ipv4( ipv4 ),
    m_port( port ),
    m_ipv4Regex( std::regex( "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" ) ) {

    assert( std::regex_match( m_ipv4, m_ipv4Regex ) );
    assert( 0 <= m_port && m_port <= 65535 );

    m_thread = std::thread( [this]() {
        while ( !m_stopThread ) {
            try {
                std::cout << "[Viewer] unable to connect to server : " << m_ipv4 << " " << m_port
                          << std::endl;
                net::ClientSocket sock( m_ipv4, m_port );

                m_serverConnected = true;
                sock.write( net::ClientSocket::Type::VIEWER );

                if ( m_onServerConnected ) m_onServerConnected( m_ipv4, m_port );

                while ( !m_stopThread ) {
                    net::ClientSocket::Message serverMessage;
                    sock.read( serverMessage );

                    switch ( serverMessage ) {

                    case net::ClientSocket::Message::PING: {
                        // server check client connection
                        // nothing to do
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] receive ping " );
#endif
                    } break;

                    case net::ClientSocket::Message::NEW_STREAMER: {
                        std::string streamName;
                        sock.read( streamName );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "'" );
#endif

                        SensorSpec sensorSpec;
                        sock.read( sensorSpec );

                        m_onNewStreamer( streamName, sensorSpec );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
                        sock.read( streamName );
                        SensorSpec sensorSpec;
                        sock.read( sensorSpec );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );
#endif

                        m_onDelStreamer( streamName, sensorSpec );

                    } break;

                    default: {
                        assert( false );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] unknown message from server" );
#endif
                    }
                    } // switch (serverMessage)

                } // while (! m_stopThread)
            }
            catch ( std::exception& e ) {
#ifdef DEBUG_VIEWER
                DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
#endif
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }
            if ( m_serverConnected && m_onServerDisconnected )
                m_onServerDisconnected( m_ipv4, m_port );
            m_serverConnected = false;

        } // while (! m_stopThread)
    } );  // thread
}

Viewer::~Viewer() {
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Viewer] ~Viewer()" );
#endif
    m_stopThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Viewer] ~Viewer() done" );
#endif
}

void Viewer::setIpv4( const std::string& ipv4 ) {
    assert( !m_serverConnected );
    assert( std::regex_match( ipv4, m_ipv4Regex ) );
    m_ipv4 = ipv4;
}

void Viewer::setPort( int port ) {
    assert( !m_serverConnected );
    assert( 0 <= m_port && m_port <= 65535 );
    m_port = port;
}

} // namespace hub
