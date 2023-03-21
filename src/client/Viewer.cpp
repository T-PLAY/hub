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

Viewer::Viewer(
    std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
    std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
    std::function<void( const char* ipv4, int port )> onServerConnected,
    std::function<void( const char* ipv4, int port )> onServerDisconnected,
    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
    const std::string& ipv4,
    int port,
    bool autoSync,
    std::function<void( const char* logMessage )> onLogMessage ) :

    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    m_sock( ipv4, port, false ),
    m_autoSync( autoSync ),
    m_onLogMessage( onLogMessage ) {

    m_thread = std::thread( [this]() {
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
                    } break;

                    case net::ClientSocket::Message::NEW_STREAMER: {

                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

                        std::string syncStreamName = "";

                        auto streamId     = streamName;
                        auto sensorSpecId = sensorSpec;

                        std::string parentName = "";
                        if ( m_autoSync ) {
                            const auto& metaData = sensorSpec.getMetaData();
                            if ( metaData.find( "parent" ) != metaData.end() ) {
                                // todo any
                                parentName = metaData.at( "parent" ).getConstCharPtr();

                                if ( m_streams.find( parentName ) != m_streams.end() ) {
                                    streamId += " -> " + parentName;
                                    sensorSpecId += m_streams.at( parentName )->m_sensorSpec;
                                    syncStreamName = parentName;
                                }
                            }
                        }

                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "', sync stream '"
                                                             << syncStreamName << "'" );

                        assert( m_streams.find( streamId ) == m_streams.end() );

                        if ( m_onNewStreamer ) {
                            const bool added = m_onNewStreamer( streamId.c_str(), sensorSpecId );

                            m_streams[streamId] = std::make_unique<Stream>( *this );

                            auto& newStream            = *m_streams.at( streamId );
                            newStream.m_streamId       = streamId;
                            newStream.m_streamName     = streamName;
                            newStream.m_sensorSpec     = sensorSpec;
                            newStream.m_sensorSpecId   = sensorSpecId;
                            newStream.m_syncStreamName = syncStreamName;
                            newStream.m_added          = added;
                            newStream.m_parentName     = parentName;

                            if ( m_onNewAcquisition && added ) { newStream.startStream(); }
                        }

                        // prevent all son the father is comming
                        std::list<std::shared_ptr<Stream>> streamsToAdd;
                        if ( m_autoSync ) {
                            auto it = m_streams.begin();
                            while ( it != m_streams.end() ) {

                                auto& sonStream = *it->second;

                                if ( sonStream.m_parentName == streamId ) {
                                    if ( m_onNewAcquisition && sonStream.m_added ) {
                                        sonStream.stopStream();
                                    }
                                    m_onDelStreamer( sonStream.m_streamId.c_str(),
                                                     sonStream.m_sensorSpec );

                                    assert( m_streams.find( sonStream.m_parentName ) !=
                                            m_streams.end() );
                                    sonStream.m_streamId =
                                        sonStream.m_streamName + " -> " + sonStream.m_parentName;
                                    sonStream.m_sensorSpecId =
                                        sonStream.m_sensorSpec +
                                        m_streams.at( sonStream.m_parentName )->m_sensorSpec;
                                    assert( sonStream.m_syncStreamName == "" );
                                    sonStream.m_syncStreamName = sonStream.m_parentName;

                                    if ( m_onNewStreamer ) {
                                        const bool added =
                                            m_onNewStreamer( sonStream.m_streamId.c_str(),
                                                             sonStream.m_sensorSpecId );

                                        if ( m_onNewAcquisition && added ) {
                                            sonStream.startStream();
                                        }
                                    }

                                    streamsToAdd.push_back( it->second );
                                    it = m_streams.erase( it );
                                    continue;
                                }

                                ++it;
                            }
                        }
                        for ( const auto& streamToAdd : streamsToAdd ) {
                            assert( m_streams.find( streamToAdd->m_streamId ) == m_streams.end() );
                            m_streams[streamToAdd->m_streamId] = streamToAdd;
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
//                        std::cout << "[Viewer] del streamer '" << streamName << "'" << std::endl;
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );

                        std::string syncStreamName = "";

                        auto streamId = streamName;

                        if ( m_autoSync ) {
                            const auto& metaData = sensorSpec.getMetaData();
                            if ( metaData.find( "parent" ) != metaData.end() ) {
                                // todo any
                                const std::string parentName =
                                    metaData.at( "parent" ).getConstCharPtr();

                                if ( m_streams.find( parentName ) != m_streams.end() ) {
                                    streamId += " -> " + parentName;
                                }
                            }
                        }


                        // prevent all son the father is leaving
                        std::list<std::shared_ptr<Stream>> streamsToAdd;
                        if ( m_autoSync ) {
                            auto it = m_streams.begin();
                            while ( it != m_streams.end() ) {

                                auto& sonStream = *it->second;

                                if ( sonStream.m_parentName == streamId ) {
                                    if ( m_onNewAcquisition && sonStream.m_added ) {
                                        sonStream.stopStream();
                                    }
                                    m_onDelStreamer( sonStream.m_streamId.c_str(),
                                                     sonStream.m_sensorSpec );

                                    assert( m_streams.find( sonStream.m_parentName ) !=
                                            m_streams.end() );
                                    sonStream.m_streamId     = sonStream.m_streamName;
                                    sonStream.m_sensorSpecId = sonStream.m_sensorSpec;
                                    assert( sonStream.m_syncStreamName != "" );
                                    sonStream.m_syncStreamName = "";

                                    if ( m_onNewStreamer ) {
                                        const bool added =
                                            m_onNewStreamer( sonStream.m_streamId.c_str(),
                                                             sonStream.m_sensorSpecId );

                                        if ( m_onNewAcquisition && added ) {
                                            sonStream.startStream();
                                        }
                                    }

                                    streamsToAdd.push_back( it->second );
                                    it = m_streams.erase( it );
                                    continue;
                                }

                                ++it;
                            }
                        }
                        for ( const auto& streamToAdd : streamsToAdd ) {
                            assert( m_streams.find( streamToAdd->m_streamId ) == m_streams.end() );
                            m_streams[streamToAdd->m_streamId] = streamToAdd;
                        }

                        if ( m_onDelStreamer ) { delStreamer( streamId ); }

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    default: {
                        DEBUG_MSG( "[Viewer] unknown message from server" );
                        assert( false );
                    }
                    } // switch (serverMessage)

                } // while (! m_stopThread)
            }
            catch ( net::ClientSocket::exception& e ) {
                if ( m_serverConnected ) {
                    DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
                }
                // ping the server when this one is not started or visible in the network
                // able the viewer clients to be aware of the starting of server less than 100
                // milliseconds.
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }

            if ( m_serverConnected ) {
                m_serverConnected = false;
                DEBUG_MSG( "[Viewer] server disconnected, close all client connections" );

                if ( m_onServerDisconnected )
                    m_onServerDisconnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                auto it = m_streams.begin();
                while ( it != m_streams.end() ) {
                    const auto& streamId = it->first;
                    ++it;

                    delStreamer( streamId );
                }
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
    m_autoSync = newAutoSync;
}

bool Viewer::isConnected() const {
    return m_serverConnected;
}

void Viewer::delStreamer( const std::string& streamId ) {
    DEBUG_MSG( "[Viewer] delStreamer " << streamId );

    assert( m_onDelStreamer );
    assert( m_streams.find( streamId ) != m_streams.end() );
    auto& delStream = *m_streams.at( streamId );

    if ( m_onNewAcquisition && delStream.m_added ) { delStream.stopStream(); }

    m_onDelStreamer( streamId.c_str(), delStream.m_sensorSpecId );
    m_streams.erase( streamId );
}

void Viewer::Stream::startStream() {
    DEBUG_MSG( "[Viewer][Stream] startStream" );

    m_stopThread = false;
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {
//            const io::Input & inputStream = (m_syncStreamName == "") ?(io::InputStream(m_streamName, net::ClientSocket(m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort())))
//                                                               :(io::InputSyncStream(m_streamName, m_syncStreamName, net::ClientSocket(m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort())));

            if (m_syncStreamName == "") {
                m_inputSensor = std::make_unique<InputSensor>( io::InputStream(
                m_streamName,
                net::ClientSocket( m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort() ) ) );

            }
            else {
                m_inputSensor = std::make_unique<InputSensor>( io::InputSyncStream(
                m_streamName,
                m_syncStreamName,
                m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort()) );
            }

            while ( !m_stopThread ) {
                auto acq = m_inputSensor->getAcq();
                m_viewer.m_onNewAcquisition( m_streamId.c_str(), acq );
            }
        }
        catch ( net::Socket::exception& e ) {
            DEBUG_MSG( "[Viewer] startStream() streamer '"
                       << m_streamId << "'" << m_streamName << "'" << m_syncStreamName
                       << "' disconnected, catch exception " << e.what() );
        }

        if ( m_stopThread ) {
            DEBUG_MSG( "[Viewer] streamer '" << m_streamId << "' thread killed " );
        }
        DEBUG_MSG( "[Viewer] thread end " );
    } );

    DEBUG_MSG( "[Viewer] startStream() streamer '" << m_streamId << "' inited " );
}

void Viewer::Stream::stopStream() {
    DEBUG_MSG( "[Stream] stopStream() streamer '" << m_streamId << "' started" );

    assert( m_thread != nullptr );
    assert( m_stopThread == false );
    m_stopThread = true;

    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

    DEBUG_MSG( "[Stream] stopStream() streamer '" << m_streamId << "' ended" );
}

} // namespace client
} // namespace hub
