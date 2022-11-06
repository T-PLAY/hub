#include "Viewer.hpp"

#include <regex>

#include "IO/Stream.hpp"
#include "InputSensor.hpp"
#include "Net/ClientSocket.hpp"

#define DEBUG_VIEWER

namespace hub {

Viewer::Viewer(
    std::function<bool( const char* streamName, const SensorSpec&, const char* syncStreamName )>
        onNewStreamer,
    std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
    std::function<void( const char* ipv4, int port )> onServerConnected,
    std::function<void( const char* ipv4, int port )> onServerDisconnected,
    std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition,
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

//                        Stream newStream( *this );

                                                std::string streamName;
                                                m_sock.read(streamName);
//                        m_sock.read( newStream.m_streamName );
                                                SensorSpec sensorSpec;
                                                m_sock.read(sensorSpec);
//                        m_sock.read( newStream.m_sensorSpec );
                                                std::string syncStreamName;
                                                m_sock.read(syncStreamName);
//                        m_sock.read( newStream.m_syncStreamName );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] new streamer '" << streamName
                                                             << "', sync stream '"
                                                             << syncStreamName << "'" );
#endif
                        const auto& streamId =
                            ( syncStreamName == "" )
                                ? ( streamName )
                                : ( streamName + " <- " + syncStreamName );
//                        newStream.m_streamId = streamId;
                        assert( m_streams.find( streamId ) == m_streams.end() );
                        //                        m_streams[streamId] = newStream;
                        //                        m_streams.emplace(streamId, *this);
                        //                        m_streams[streamId] =
                        //                        std::make_unique<Stream>(std::move(newStream));
                        m_streams[streamId] = std::make_unique<Stream>(*this);
//                        = std::make_unique<Stream>( std::move( newStream ) );

                        auto & newStream = *m_streams.at(streamId);
                        newStream.m_streamId = streamId;
                        newStream.m_streamName = streamName;
                        newStream.m_sensorSpec = sensorSpec;
                        newStream.m_syncStreamName = syncStreamName;

                        if ( m_onNewStreamer ) {
                            bool added = m_onNewStreamer( streamId.c_str(),
                                                          newStream.m_sensorSpec,
                                                          newStream.m_syncStreamName.c_str() );

                            if ( m_onNewAcquisition && added ) {
                                newStream.startStream();
                                //                                startStream(
                                //                                newStream.m_streamName,
                                //                                newStream.m_sensorSpec,
                                //                                newStream.m_syncStreamName );
                            }
                        }

                        //                        m_streams[streamId] = newStream;
                        // wait for client init sensorSpec with main thread context (async)
                        // for unity side (update context different of static event function)
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                    } break;

                    case net::ClientSocket::Message::DEL_STREAMER: {
                        std::string streamName;
                        m_sock.read( streamName );
                        SensorSpec sensorSpec;
                        m_sock.read( sensorSpec );
                        std::string syncStreamName;
                        m_sock.read( syncStreamName );
#ifdef DEBUG_VIEWER
                        DEBUG_MSG( "[Viewer] del streamer '" << streamName << "'" );
#endif
                        //                        const auto& streamId = streamName +
                        //                        syncStreamName;
                        const auto& streamId = ( syncStreamName == "" )
                                                   ? ( streamName )
                                                   : ( streamName + " <- " + syncStreamName );

                        assert( m_streams.find( streamId ) != m_streams.end() );

                        auto& delStream = *m_streams.at( streamId );

                        if ( m_onDelStreamer ) {
                            if ( m_onNewAcquisition ) { delStream.stopStream(); }

                            m_onDelStreamer( streamId.c_str(), sensorSpec );
                        }

                        m_streams.erase( streamId );
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
                if ( m_serverConnected ) {
                    DEBUG_MSG( "[Viewer] server disconnected, catch exception " << e.what() );
                }
                else {
                    DEBUG_MSG( "[Viewer] unable to connect to server, catch exception "
                               << e.what() );
                }
#endif
                // ping the server when this one is not started or visible in the network
                // able the viewer clients to be aware of the starting of server less than 100
                // milliseconds.
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }

            if ( m_serverConnected ) {
                m_serverConnected = false;

                if ( m_onServerDisconnected )
                    m_onServerDisconnected( m_sock.getIpv4().c_str(), m_sock.getPort() );

                for ( const auto& [streamId, stream] : m_streams ) {
                    //                    const auto& streamId = pair.first;
                    const auto& sensorSpec = stream->m_sensorSpec;
                    //                    if (m_onDelStreamer) m_onDelStreamer( streamId.c_str(),
                    //                    sensorSpec );

                    //                    if ( m_onNewAcquisition ) { stream->stopStream(); }
                    if ( m_onDelStreamer ) {
                        if ( m_onNewAcquisition ) { stream->stopStream(); }

                        m_onDelStreamer( streamId.c_str(), sensorSpec );
                    }
                }
                m_streams.clear();
                //                m_streamName2sensorSpec.clear();

                //#ifdef OS_LINUX
                //                if ( !m_stopThread ) { m_sock.setPort( m_sock.getPort() + 1 ); }
                //                ++m_port;
                //#endif
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

    m_streams.clear();
//    for ( auto& [streamName, stream] : m_streams ) {
//        const auto& streamId = pair.first;
//        stream.stopStream();
//        assert( m_streamName2stopThread.find( streamId ) != m_streamName2stopThread.end() );
//        m_streamName2stopThread.at( streamId ) = true;
//        auto& thread                             = pair.second;
//        assert( thread.joinable() );
//        thread.join();
//    }
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

const std::string& Viewer::getIpv4() const {
    return m_sock.getIpv4();
}

const int& Viewer::getPort() const {
    return m_sock.getPort();
}

void Viewer::Stream::startStream() {
    //    const auto& streamId = streamName + syncStreamName;

    m_stopThread = false;
    //    assert( m_streamName2stopThread.find( streamId ) == m_streamName2stopThread.end() );
    //    m_streamName2stopThread[streamId] = false;
    //    assert( m_streamName2thread.find( streamId ) == m_streamName2thread.end() );
    assert( m_thread == nullptr );
    m_thread = new std::thread( [this]() {
        try {
            InputSensor inputSensor = InputSensor( io::InputStream(
                m_streamName,
                m_syncStreamName,
                net::ClientSocket( m_viewer.m_sock.getIpv4(), m_viewer.m_sock.getPort() ) ) );

            while ( !m_stopThread ) {
                auto acq = inputSensor.getAcquisition();
                m_viewer.m_onNewAcquisition( m_streamId.c_str(), acq );
            }
        }
        catch ( net::Socket::exception& e ) {
            DEBUG_MSG( "[Viewer] startStream() streamer '"
                       << m_streamId << "'" << m_streamName << "'" << m_syncStreamName << "' disconnected, catch exception " << e.what() );
        }
    } );


            DEBUG_MSG( "[Viewer] startStream() streamer '"
                       << m_streamId << "' inited "  );
}

void Viewer::Stream::stopStream() {

    assert( m_stopThread == false );
    //    if ( m_streamName2stopThread.find( streamId ) != m_streamName2stopThread.end() ) {
    //        m_streamName2stopThread.at( streamId ) = true;
    m_stopThread = true;

    //        assert( m_streamName2thread.find( streamId ) != m_streamName2thread.end() );
    //        auto& thread = m_streamName2thread.at( streamId );
    assert( m_thread->joinable() );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;

    DEBUG_MSG( "[Viewer] stopStream() streamer '" << m_streamId << "' ended" );
    //        m_streamName2stopThread.erase( streamId );
    //        m_streamName2thread.erase( streamId );
    //    }
}

} // namespace hub
