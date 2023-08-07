#include "Viewer.hpp"

#include <regex>
#include <sstream>

#include "InputSensor.hpp"

#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                       \
    do {                                           \
        if ( m_onLogMessage ) {                    \
            std::stringstream _sstr;               \
            _sstr << _params;                      \
            m_onLogMessage( _sstr.str().c_str() ); \
        }                                          \
        else {                                     \
            std::cout << _params << std::endl;     \
        }                                          \
    } while ( false );

namespace hub {
namespace client {

namespace viewer {

class Stream
{
  public:
    explicit Stream( Viewer& viewer, const std::string& streamName, const SensorSpec& sensorSpec ) :
        m_viewer( viewer ),
        m_streamName( streamName ),
        m_sensorSpec( sensorSpec ),
        m_onLogMessage( m_viewer.m_onLogMessage ) {

        assert( m_viewer.m_onNewStreamer );
        m_streaming = m_viewer.m_onNewStreamer( streamName.c_str(), sensorSpec );
        if ( m_viewer.m_onNewAcquisition && m_streaming ) { startStream(); }
    }

    ~Stream() {
        DEBUG_MSG( "[Stream] ~Stream() streamer '" << m_streamName << "' started" );
        if ( m_viewer.m_onNewAcquisition && m_streaming ) {
            assert( m_thread != nullptr );
            if ( m_viewer.m_onDelStreamer ) {
                m_viewer.m_onDelStreamer( m_streamName.c_str(), m_sensorSpec );
            }
            stopStream();
        }

        DEBUG_MSG( "[Stream] ~Stream() streamer '" << m_streamName << "' ended" );
    }

    void startStream() {
        DEBUG_MSG( "[Viewer][Stream] startStream" );

        assert( m_thread == nullptr );

        m_stopThread = false;
        assert( m_thread == nullptr );
        m_thread = new std::thread( [this]() {
            try {

                m_inputSensor = std::make_unique<InputSensor>( io::InputStream(
                    m_streamName,
                    net::ClientSocket( m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort() ) ) );

                while ( !m_stopThread ) {
                    hub::Acquisition acq;
                    *m_inputSensor >> acq;
                    m_viewer.m_onNewAcquisition( m_streamName.c_str(), acq );
                }
            }
            catch ( net::Socket::exception& e ) {
                DEBUG_MSG( "[Viewer] startStream() streamer '"
                           << "'" << m_streamName << "' disconnected, catch exception "
                           << e.what() );
            }

            if ( m_stopThread ) {
                DEBUG_MSG( "[Viewer] streamer '" << m_streamName << "' thread killed " );
            }
            DEBUG_MSG( "[Viewer] thread end " );
        } );

        m_streaming = true;

        DEBUG_MSG( "[Viewer] startStream() streamer '" << m_streamName << "' inited " );
    }

    void stopStream() {
        DEBUG_MSG( "[Stream] stopStream() streamer '" << m_streamName << "' started" );

        assert( m_thread != nullptr );
        assert( m_stopThread == false );
        m_stopThread = true;

        m_inputSensor->getInput().close();

        assert( m_thread->joinable() );
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;

        m_streaming = false;

        DEBUG_MSG( "[Stream] stopStream() streamer '" << m_streamName << "' ended" );
    }

    Viewer& m_viewer;
    std::string m_streamName;
    SensorSpec m_sensorSpec;

    std::thread* m_thread = nullptr;
    bool m_stopThread     = false;
    bool m_streaming      = false;
    std::function<void( const char* logMessage )> m_onLogMessage;

    std::unique_ptr<InputSensor> m_inputSensor;

}; // end class Stream

} // end namespace viewer

Viewer::Viewer(
    std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
    std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
    std::function<void( const char* ipv4, int port )> onServerNotFound,
    std::function<void( const char* ipv4, int port )> onServerConnected,
    std::function<void( const char* ipv4, int port )> onServerDisconnected,
    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        onSetProperty,
    const std::string& ipv4,
    int port,
    bool autoSync,
    std::function<void( const char* logMessage )> onLogMessage ) :

    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerNotFound( onServerNotFound ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onSetProperty( onSetProperty ),
    m_sock( ipv4, port, false ),
    m_onLogMessage( onLogMessage ) {

    m_thread = std::thread( [this]() {
        while ( !m_stopThread ) {
            try {

                DEBUG_MSG( "[Viewer] trying to connect to server at ipv4 "
                           << m_sock.getIpv4() << " and port " << m_sock.getPort() );

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

                    case net::ClientSocket::Message::NEW_STREAMER: {

                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "'" );

                        assert( m_streams.find( streamName ) == m_streams.end() );

                        if ( m_onNewStreamer ) {

                            m_streams[streamName] =
                                std::make_unique<viewer::Stream>( *this, streamName, sensorSpec );
                        }

                        // prevent all son the father is comming

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );

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
                        DEBUG_MSG( "[Viewer] server closed" );
                        assert( m_sock.isOpen() );
                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        throw net::ClientSocket::exception( "[viewer] server closed" );
                    }

                    case net::ClientSocket::Message::VIEWER_CLIENT_CLOSED: {
                        DEBUG_MSG( "[Viewer] viewer client closed" );
                        assert( m_sock.isOpen() );
                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        throw net::ClientSocket::exception( "[viewer] viewer client closed" );
                    }

                    case net::ClientSocket::Message::SET_PROPERTY: {
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

                        if ( m_onSetProperty )
                            m_onSetProperty(
                                streamName.c_str(), objectName.c_str(), property, value );

                    } break;

                    default: {
                        assert( false );
                        DEBUG_MSG( "[Viewer] unknown message from server" );
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
                    DEBUG_MSG( "[Viewer] server not found at ipv4 "
                               << m_sock.getIpv4() << " and port " << m_sock.getPort() );
                    if ( m_onServerNotFound )
                        m_onServerNotFound( m_sock.getIpv4().c_str(), m_sock.getPort() );
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

                if ( m_onServerDisconnected )
                    m_onServerDisconnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                m_streams.clear();
                assert( m_streams.empty() );

                // #ifdef OS_LINUX
                // #endif
            }

        } // while (! m_stopThread)
    } );  // thread
}

Viewer::~Viewer() {
    DEBUG_MSG( "[Viewer] ~Viewer()" );
    m_stopThread = true;

    if ( m_sock.isOpen() ) { m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED ); }


    assert( m_thread.joinable() );
    m_thread.join();

    m_streams.clear();
    DEBUG_MSG( "[Viewer] ~Viewer() done" );
}

void Viewer::setIpv4( const std::string& ipv4 ) {
    DEBUG_MSG( "[Viewer] setIpv4 " << ipv4 );
    assert( !m_serverConnected );
    m_sock.setIpv4( ipv4 );
}

void Viewer::setPort( int port ) {
    DEBUG_MSG( "[Viewer] setPort " << port );
    assert( !m_serverConnected );
    m_sock.setPort( port );
}

const std::string& Viewer::getIpv4() const {
    return m_sock.getIpv4();
}

const int& Viewer::getPort() const {
    return m_sock.getPort();
}

void Viewer::setAutoSync( bool newAutoSync ) {
    DEBUG_MSG( "[Viewer] setAutoSync " << newAutoSync );
}

bool Viewer::isConnected() const {
    return m_serverConnected;
}

void Viewer::startStream( const std::string& streamName ) {
    assert( m_onNewStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.startStream();
}

void Viewer::stopStream( const std::string& streamName ) {
    DEBUG_MSG( "[Viewer] stopStream by user " << streamName );

    assert( m_onDelStreamer );
    assert( m_streams.find( streamName ) != m_streams.end() );
    auto& stream = *m_streams.at( streamName );
    stream.stopStream();
}

void Viewer::setProperty( const std::string& streamName,
                          const std::string& objectName,
                          int property,
                          const Any& value ) {
    if ( m_sock.isOpen() ) {
        m_sock.write( net::ClientSocket::Message::SET_PROPERTY );
        m_sock.write( streamName );
        m_sock.write( objectName );
        m_sock.write( property );
        m_sock.write( value );
    }
}

} // namespace client
} // namespace hub
