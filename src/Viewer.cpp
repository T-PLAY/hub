#include "Viewer.hpp"

#include <regex>

#include "IO/Stream.hpp"
#include "InputSensor.hpp"
#include "Net/ClientSocket.hpp"

#define DEBUG_VIEWER

namespace hub {

Viewer::Viewer( std::function<bool( const char*, const SensorSpec& )> onNewStreamer,
                std::function<void( const char*, const SensorSpec& )> onDelStreamer,
                std::function<void( const char*, int )> onServerConnected,
                std::function<void( const char*, int )> onServerDisconnected,
                std::function<void( const char*, const hub::Acquisition& )> onNewAcquisition,
                const std::string& ipv4,
                int port ) :

    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    //    m_ipv4( ipv4 ),
    //    m_port( port ),
    m_sock( ipv4, port, false )
//    m_ipv4Regex( std::regex( "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" ) ) {
{

    m_thread = std::thread( [this]() {
        //        net::ClientSocket sock( m_ipv4, m_port );

        while ( !m_stopThread ) {
            try {

                assert( !m_sock.isOpen() );
                m_sock.connect();
                m_serverConnected = true;
                assert( m_sock.isOpen() );

                m_sock.write( net::ClientSocket::Type::VIEWER );

                if ( m_onServerConnected )
                    m_onServerConnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                while ( !m_stopThread ) {

                    net::ClientSocket::Message serverMessage;
                    m_sock.read( serverMessage );

                    switch ( serverMessage ) {

                    case net::ClientSocket::Message::PING: {
                        // server checking if client is connected (only way to know if client viewer
                        // still alive) nothing to do
                        //#ifdef DEBUG_VIEWER
                        //                        DEBUG_MSG( "[Viewer] receive ping " );
                        //#endif
                    } break;

                    case net::ClientSocket::Message::NEW_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "'" );
#endif

                        assert( m_streamName2sensorSpec.find( streamName ) ==
                                m_streamName2sensorSpec.end() );
                        m_streamName2sensorSpec[streamName] = sensorSpec;

                        if ( m_onNewStreamer ) {
                            bool added = m_onNewStreamer( streamName.c_str(), sensorSpec );

                            if ( m_onNewAcquisition && added ) {
                                startStream( streamName, sensorSpec );
                            }
                        }
                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );
#endif
                        assert( m_streamName2sensorSpec.find( streamName ) !=
                                m_streamName2sensorSpec.end() );
                        m_streamName2sensorSpec.erase( streamName );

                        if ( m_onDelStreamer ) {
                            if ( m_onNewAcquisition ) { stopStream( streamName, sensorSpec ); }

                            m_onDelStreamer( streamName.c_str(), sensorSpec );
                        }
                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    default: {
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] unknown message from server" );
#endif
                        assert( false );
                    }
                    } // switch (serverMessage)

                } // while (! m_stopThread)
            }
            catch ( net::ClientSocket::exception& e ) {
#ifdef DEBUG_VIEWER
                if (m_serverConnected) {
                DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
                }
                else {
                DEBUG_MSG( "[Viewer] unable to connect to server, catch exception " << e.what() );

                }
#endif
                // ping the server when this one is not started or visible in the network
                // able the viewer clients to be aware of the starting of server less than 100 milliseconds.
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }

            if ( m_serverConnected ) {
                m_serverConnected = false;

                if ( m_onServerDisconnected )
                    m_onServerDisconnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                for ( const auto& pair : m_streamName2sensorSpec ) {
                    const auto& streamName = pair.first;
                    const auto& sensorSpec = pair.second;
                    m_onDelStreamer( streamName.c_str(), sensorSpec );

                    if ( m_onNewAcquisition ) { stopStream( streamName, sensorSpec ); }
                }
                m_streamName2sensorSpec.clear();

#ifdef OS_LINUX
//                if ( !m_stopThread ) { m_sock.setPort( m_sock.getPort() + 1 ); }
//                ++m_port;
#endif
            }
            else {
//#ifdef DEBUG_VIEWER
                DEBUG_MSG( "[Viewer] unable to connect to server : " << m_sock.getIpv4() << " "
                                                                     << m_sock.getPort() );
//#endif
            }

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

    for ( auto& pair : m_streamName2thread ) {
        const auto& streamName = pair.first;
        assert( m_streamName2stopThread.find( streamName ) != m_streamName2stopThread.end() );
        m_streamName2stopThread.at( streamName ) = true;
        auto& thread                             = pair.second;
        assert( thread.joinable() );
        thread.join();
    }
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Viewer] ~Viewer() done" );
#endif
}

void Viewer::setIpv4( const std::string& ipv4 ) {
    assert( !m_serverConnected );
    //    assert( std::regex_match( ipv4, m_ipv4Regex ) );
    //    m_ipv4 = ipv4;
    m_sock.setIpv4( ipv4 );
}

void Viewer::setPort( int port ) {
    assert( !m_serverConnected );
    //    assert( 0 <= port && port <= 65535 );
    //    m_port = port;
    m_sock.setPort( port );
}

const std::string &Viewer::getIpv4() const
{
    return m_sock.getIpv4();
}

const int &Viewer::getPort() const
{
    return m_sock.getPort();
}

void Viewer::startStream( const std::string& streamName, const SensorSpec& sensorSpec ) {

    assert( m_streamName2stopThread.find( streamName ) == m_streamName2stopThread.end() );
    m_streamName2stopThread[streamName] = false;
    assert( m_streamName2thread.find( streamName ) == m_streamName2thread.end() );
    m_streamName2thread[streamName] = std::thread( [this, streamName, sensorSpec]() {
        try {
            InputSensor inputSensor = InputSensor( io::InputStream(
                streamName, "", net::ClientSocket( m_sock.getIpv4(), m_sock.getPort() ) ) );

            while ( !m_streamName2stopThread.at( streamName ) ) {
                auto acq = inputSensor.getAcquisition();
                m_onNewAcquisition( streamName.c_str(), acq );
            }
        }
        catch ( net::Socket::exception& e ) {
            DEBUG_MSG( "[Viewer] startStream() streamer '"
                       << streamName << "' disconnected, catch exception " << e.what() );
        }
    } );
}

void Viewer::stopStream( const std::string& streamName, const SensorSpec& ) {
    if ( m_streamName2stopThread.find( streamName ) != m_streamName2stopThread.end() ) {
        m_streamName2stopThread.at( streamName ) = true;

        assert( m_streamName2thread.find( streamName ) != m_streamName2thread.end() );
        auto& thread = m_streamName2thread.at( streamName );
        assert( thread.joinable() );
        thread.join();
        m_streamName2stopThread.erase( streamName );
        m_streamName2thread.erase( streamName );
    }
}

} // namespace hub
