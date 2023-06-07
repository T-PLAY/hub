#include "Viewer.hpp"

#include <regex>
#include <sstream>

#include "InputSensor.hpp"

#ifdef DEBUG_MSG
#    undef DEBUG_MSG
#endif
#define DEBUG_MSG( _params )                        \
    do {                                            \
        if ( m_onLogMessage ) {                     \
            std::stringstream _sstr;                \
            _sstr << _params;                       \
            m_onLogMessage( _sstr.str().c_str() );  \
        }                                           \
        else { std::cout << _params << std::endl; } \
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

        //    m_onDelStreamer( streamName.c_str(), delStream.m_sensorSpecId );
        //            DEBUG_MSG( "[Stream] ~Stream() streamer '" << m_streamId << "' started" );
        //        if ( m_thread != nullptr ) { stopStream(); }
        DEBUG_MSG( "[Stream] ~Stream() streamer '" << m_streamName << "' ended" );
    }

    void startStream() {
        DEBUG_MSG( "[Viewer][Stream] startStream" );

        assert( m_thread == nullptr );

        m_stopThread = false;
        assert( m_thread == nullptr );
        m_thread = new std::thread( [this]() {
            try {
                //            const io::Input & inputStream = (m_syncStreamName == "")
                //            ?(io::InputStream(m_streamName,
                //            net::ClientSocket(m_viewer.m_sock.getIpv4(),
                //            m_viewer.m_sock.getPort())))
                //                                                               :(io::InputSyncStream(m_streamName,
                //                                                               m_syncStreamName,
                //                                                               net::ClientSocket(m_viewer.m_sock.getIpv4(),
                //                                                               m_viewer.m_sock.getPort())));

                //                if ( m_syncStreamName == "" ) {
                m_inputSensor = std::make_unique<InputSensor>( io::InputStream(
                    m_streamName,
                    net::ClientSocket( m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort() ) ) );
                //                }
                //                else {
                //                    m_inputSensor = std::make_unique<InputSensor>(
                //                        io::InputSyncStream( m_streamName,
                //                                             m_syncStreamName,
                //                                             m_viewer.m_sock.getIpv4(),
                //                                             m_viewer.m_sock.getPort() ) );
                //                }

                while ( !m_stopThread ) {
                    hub::Acquisition acq;
                    //                    auto acq = m_inputSensor->getAcq();
                    *m_inputSensor >> acq;
                    //                std::cout << "[Viewer][Stream] getAcq : " << acq << std::endl;
                    //                    m_viewer.m_onNewAcquisition( m_streamId.c_str(), acq );
                    m_viewer.m_onNewAcquisition( m_streamName.c_str(), acq );
                }
            }
            catch ( net::Socket::exception& e ) {
                DEBUG_MSG( "[Viewer] startStream() streamer '"
                           //                           << m_streamId << "'" << m_streamName << "'"
                           //                           << m_syncStreamName
                           << "'" << m_streamName << "' disconnected, catch exception "
                           << e.what() );
                //                stopStream();
                //                m_inputSensor->getInput().close();
            }

            if ( m_stopThread ) {
                //                DEBUG_MSG( "[Viewer] streamer '" << m_streamId << "' thread killed
                //                " );
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

    //    Stream( Stream&& stream ) = default;
    //        Stream( Stream&& stream ) :
    //            m_viewer( stream.m_viewer ),

    ////            m_streamName( std::move( stream.m_streamName ) ),
    ////            m_sensorSpec( std::move( stream.m_sensorSpec ) ),
    ////            m_syncStreamName( std::move( stream.m_syncStreamName ) ),
    ////            m_parentName( std::move( stream.m_parentName ) ),

    ////            m_streamId( std::move( stream.m_streamId ) ),
    ////            m_sensorSpecId( std::move( stream.m_sensorSpecId ) ),

    //            m_thread( stream.m_thread ),
    //            m_stopThread( stream.m_stopThread ),
    //            m_streaming( stream.m_streaming ),
    //            m_onLogMessage( m_viewer.m_onLogMessage ) {
    //            stream.m_thread = nullptr;
    //        }

    //      private:
    Viewer& m_viewer;
    std::string m_streamName;
    SensorSpec m_sensorSpec;
    //        std::string m_syncStreamName;
    //        std::string m_parentName;

    //        std::string m_streamId;
    //        SensorSpec m_sensorSpecId;

    std::thread* m_thread = nullptr;
    bool m_stopThread     = false;
    bool m_streaming      = false;
    std::function<void( const char* logMessage )> m_onLogMessage;

    std::unique_ptr<InputSensor> m_inputSensor;

    //    void startStream();
    //    void stopStream();
}; // end class Stream

} // end namespace viewer

Viewer::Viewer(
    std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
    std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
    std::function<void( const char* ipv4, int port )> onServerConnected,
    std::function<void( const char* ipv4, int port )> onServerDisconnected,
    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
    std::function<void( const char* streamName, const char* objectName, int property, const Any& value )> onSetProperty,
    const std::string& ipv4,
    int port,
    bool autoSync,
    std::function<void( const char* logMessage )> onLogMessage ) :

    m_onNewStreamer( onNewStreamer ),
    m_onDelStreamer( onDelStreamer ),
    m_onServerConnected( onServerConnected ),
    m_onServerDisconnected( onServerDisconnected ),
    m_onNewAcquisition( onNewAcquisition ),
    m_onSetProperty(onSetProperty),
    m_sock( ipv4, port, false ),
    //    m_autoSync( autoSync ),
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
                    //                        m_mtxSockRead.lock();
                    m_sock.read( serverMessage );

                    switch ( serverMessage ) {

                        //                    case net::ClientSocket::Message::PING: {
                        //                    } break;

                    case net::ClientSocket::Message::NEW_STREAMER: {

                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        std::cout << "[Viewer] new streamer '" << streamName << "'" << std::endl;

                        //                        std::string syncStreamName = "";

                        //                        auto streamName     = streamName;
                        //                        auto sensorSpecId = sensorSpec;

                        //                        std::string parentName = "";
                        //                        if ( m_autoSync ) {
                        //                            const auto& metaData =
                        //                            sensorSpec.getMetaData(); if ( metaData.find(
                        //                            "parent" ) != metaData.end() ) {
                        //                                // todo any
                        //                                parentName = metaData.at( "parent"
                        //                                ).getConstCharPtr();

                        //                                if ( m_streams.find( parentName ) !=
                        //                                m_streams.end() ) {
                        //                                    streamName += " -> " + parentName;
                        //                                    sensorSpecId += m_streams.at(
                        //                                    parentName )->m_sensorSpec;
                        //                                    syncStreamName = parentName;
                        //                                }
                        //                            }
                        //                        }

                        DEBUG_MSG( "[Viewer] new streamer '" << streamName << "'" );

                        //                        DEBUG_MSG( "[Viewer] new streamer '" << streamName
                        //                        << "', sync stream '"
                        //                                                             <<
                        //                                                             syncStreamName
                        //                                                             << "'" );

                        assert( m_streams.find( streamName ) == m_streams.end() );

                        if ( m_onNewStreamer ) {

                            //                            const bool added = m_onNewStreamer(
                            //                                                        streamName.c_str(),
                            //                                                        sensorSpecId
                            //                                                        );
                            //                            const bool added = m_onNewStreamer(
                            //                            streamName.c_str(), sensorSpec );

                            //                            assert( m_streams.find( streamName ) ==
                            //                            m_streams.end() ); Stream newStream(*this,
                            //                            streamName, sensorSpec);
                            m_streams[streamName] =
                                std::make_unique<viewer::Stream>( *this, streamName, sensorSpec );

                            //                            auto& newStream = *m_streams.at(
                            //                            streamName ); newStream.m_streamId       =
                            //                            streamName; newStream.m_streamName =
                            //                            streamName; newStream.m_sensorSpec =
                            //                            sensorSpec; newStream.m_sensorSpecId   =
                            //                            sensorSpecId; newStream.m_syncStreamName =
                            //                            syncStreamName;
                            //                            newStream.m_streaming = added;
                            //                            newStream.m_parentName     = parentName;
                        }

                        // prevent all son the father is comming
                        //                        std::list<std::shared_ptr<Stream>> streamsToAdd;
                        //                        if ( m_autoSync ) {
                        //                            auto it = m_streams.begin();
                        //                            while ( it != m_streams.end() ) {

                        //                                auto& sonStream = *it->second;

                        //                                if ( sonStream.m_parentName == streamName
                        //                                ) {
                        //                                    if ( m_onNewAcquisition &&
                        //                                    sonStream.m_streaming ) {
                        //                                        sonStream.stopStream();
                        //                                    }
                        //                                    m_onDelStreamer(
                        //                                    sonStream.m_streamId.c_str(),
                        //                                                     sonStream.m_sensorSpec
                        //                                                     );

                        //                                    assert( m_streams.find(
                        //                                    sonStream.m_parentName ) !=
                        //                                            m_streams.end() );
                        //                                    sonStream.m_streamId =
                        //                                        sonStream.m_streamName + " -> " +
                        //                                        sonStream.m_parentName;
                        //                                    sonStream.m_sensorSpecId =
                        //                                        sonStream.m_sensorSpec +
                        //                                        m_streams.at(
                        //                                        sonStream.m_parentName
                        //                                        )->m_sensorSpec;
                        //                                    assert( sonStream.m_syncStreamName ==
                        //                                    "" ); sonStream.m_syncStreamName =
                        //                                    sonStream.m_parentName;

                        //                                    if ( m_onNewStreamer ) {
                        //                                        const bool added =
                        //                                            m_onNewStreamer(
                        //                                            sonStream.m_streamId.c_str(),
                        //                                                             sonStream.m_sensorSpecId
                        //                                                             );

                        //                                        if ( m_onNewAcquisition && added )
                        //                                        {
                        //                                            sonStream.startStream();
                        //                                        }
                        //                                    }

                        //                                    streamsToAdd.push_back( it->second );
                        //                                    it = m_streams.erase( it );
                        //                                    continue;
                        //                                }

                        //                                ++it;
                        //                            }
                        //                        }
                        //                        for ( const auto& streamToAdd : streamsToAdd ) {
                        //                            assert( m_streams.find(
                        //                            streamToAdd->m_streamId ) == m_streams.end()
                        //                            ); m_streams[streamToAdd->m_streamId] =
                        //                            streamToAdd;
                        //                        }

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        //                        std::cout << "[Viewer] del streamer '" <<
                        //                        streamName << "'" << std::endl;
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );

                        //                        std::string syncStreamName = "";

                        if ( m_onNewStreamer ) {
                            assert( m_onDelStreamer );
                            assert( m_streams.find( streamName ) != m_streams.end() );
                            //    auto& delStream = *m_streams.at( streamName );
                            m_streams.erase( streamName );
                        }
                        //    delStream.

                        //    if ( m_onNewAcquisition && delStream.m_streaming ) {
                        //    delStream.stopStream(); }

                        //    m_onDelStreamer( streamName.c_str(), delStream.m_sensorSpecId );

                        //                        auto streamName = streamName;

                        //                        if ( m_autoSync ) {
                        //                            const auto& metaData =
                        //                            sensorSpec.getMetaData(); if ( metaData.find(
                        //                            "parent" ) != metaData.end() ) {
                        //                                // todo any
                        //                                const std::string parentName =
                        //                                    metaData.at( "parent"
                        //                                    ).getConstCharPtr();

                        //                                if ( m_streams.find( parentName ) !=
                        //                                m_streams.end() ) {
                        //                                    streamName += " -> " + parentName;
                        //                                }
                        //                            }
                        //                        }

                        // prevent all son the father is leaving
                        //                        std::list<std::shared_ptr<Stream>> streamsToAdd;
                        //                        if ( m_autoSync ) {
                        //                            auto it = m_streams.begin();
                        //                            while ( it != m_streams.end() ) {

                        //                                auto& sonStream = *it->second;

                        //                                if ( sonStream.m_parentName == streamName
                        //                                ) {
                        //                                    if ( m_onNewAcquisition &&
                        //                                    sonStream.m_streaming ) {
                        //                                        sonStream.stopStream();
                        //                                    }
                        //                                    m_onDelStreamer(
                        //                                    sonStream.m_streamId.c_str(),
                        //                                                     sonStream.m_sensorSpec
                        //                                                     );

                        //                                    assert( m_streams.find(
                        //                                    sonStream.m_parentName ) !=
                        //                                            m_streams.end() );
                        //                                    sonStream.m_streamId     =
                        //                                    sonStream.m_streamName;
                        //                                    sonStream.m_sensorSpecId =
                        //                                    sonStream.m_sensorSpec; assert(
                        //                                    sonStream.m_syncStreamName != "" );
                        //                                    sonStream.m_syncStreamName = "";

                        //                                    if ( m_onNewStreamer ) {
                        //                                        const bool added =
                        //                                            m_onNewStreamer(
                        //                                            sonStream.m_streamId.c_str(),
                        //                                                             sonStream.m_sensorSpecId
                        //                                                             );

                        //                                        if ( m_onNewAcquisition && added )
                        //                                        {
                        //                                            sonStream.startStream();
                        //                                        }
                        //                                    }

                        //                                    streamsToAdd.push_back( it->second );
                        //                                    it = m_streams.erase( it );
                        //                                    continue;
                        //                                }

                        //                                ++it;
                        //                            }
                        //                        }
                        //                        for ( const auto& streamToAdd : streamsToAdd ) {
                        //                            assert( m_streams.find(
                        //                            streamToAdd->m_streamId ) == m_streams.end()
                        //                            ); m_streams[streamToAdd->m_streamId] =
                        //                            streamToAdd;
                        //                        }

                        //                        if ( m_onDelStreamer ) { stopStream( streamName );
                        //                        }

                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                        //                    case net::ClientSocket::Message::VIEWER_CLIENT_CLOSED:
                        //                    {
                    case net::ClientSocket::Message::SERVER_CLOSED: {
                        DEBUG_MSG( "[Viewer] server closed" );
                        assert( m_sock.isOpen() );
                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        //                        m_sock.close();
                        throw net::ClientSocket::exception( "[viewer] server closed" );
                    }

                    case net::ClientSocket::Message::VIEWER_CLIENT_CLOSED: {
                        DEBUG_MSG( "[Viewer] viewer client closed" );
                        assert( m_sock.isOpen() );
                        m_sock.write( net::ClientSocket::Message::VIEWER_CLOSED );
                        //                        m_sock.close();
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

//                        std::cout << "[ViewerClient] setProperty streamName: '" << streamName
//                                  << "', objectName: '" << objectName << "', property: " << property
//                                  << ", value: " << value << std::endl;

                        if ( m_onSetProperty )
                            m_onSetProperty( streamName.c_str(), objectName.c_str(), property, value );

                        //                        m_sock.write(net::ClientSocket::Message::VIEWER_CLOSED);
                        //                        m_sock.close();
                        //                        throw net::ClientSocket::exception( "[viewer]
                        //                        viewer client closed" );
                    } break;

                    default: {
                        assert( false );
                        DEBUG_MSG( "[Viewer] unknown message from server" );
                        throw net::ClientSocket::exception(
                            "[viewer] unknown message from server " );
                        //                            auto message = serverMessage;
                        //                            assert(false);
                    }
                    } // switch (serverMessage)

                    //                        m_mtxSockRead.unlock();

                } // while (! m_stopThread)
            }
            catch ( net::ClientSocket::exception& e ) {
                //                        m_mtxSockRead.unlock();
                if ( m_serverConnected ) {
                    DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
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

                //                auto it = m_streams.begin();
                //                while ( it != m_streams.end() ) {
                //                    const auto& streamName = it->first;
                //                    ++it;

                //                    stopStream( streamName );

                //                }
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

    //            net::ClientSocket::Message message;
    ////        m_mtxSockRead.lock();
    ////        if (m_sock.isOpen()) {
    //            m_sock.read(message);
    //            assert(message == net::ClientSocket::Message::VIEWER_CLIENT_CLOSED);
    //            if (m_sock.isOpen())
    //                m_sock.close();
    //        }
    //        m_mtxSockRead.unlock();

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
    //    m_autoSync = newAutoSync;
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
    //    m_streams.erase(streamName);

    //    if ( m_onNewAcquisition && delStream.m_streaming ) { delStream.stopStream(); }

    //    m_onDelStreamer( streamName.c_str(), delStream.m_sensorSpecId );
    //    m_streams.erase( streamName );
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
