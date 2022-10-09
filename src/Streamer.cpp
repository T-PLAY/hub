#include "Streamer.hpp"

//#define DEBUG_VIEWER

#include <regex>

#include "IO/Stream.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {

Streamer::Streamer(
    //        std::function<void( Streamer&, const std::string&, int )> onServerConnected,
    //                    std::function<void( Streamer&, const std::string&, int )>
    //                    onServerDisconnected,
    const std::string& ipv4,
    int port ) :
    //    m_onServerConnected( onServerConnected ),
    //    m_onServerDisconnected( onServerDisconnected ),
    m_ipv4( ipv4 ),
    m_port( port ),
    m_ipv4Regex( std::regex( "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" ) ) {

    assert( std::regex_match( m_ipv4, m_ipv4Regex ) );
    assert( 0 <= m_port && m_port <= 65535 );
}

void Streamer::addStream( const std::string& streamName,
                          SensorSpec&& sensorSpec,
                          const std::vector<Acquisition>& initAcqs ) {
    std::cout << "[Streamer] addStream " << streamName << std::endl;
    assert( m_streamName2outputSensor.find( streamName ) == m_streamName2outputSensor.end() );

    //    m_streamName2initAcqs[streamName].emplace_back(initAcqs);
    assert( m_streamName2initAcqs.find( streamName ) == m_streamName2initAcqs.end() );
    std::vector<Acquisition> acqs;
    for ( const auto& acq : initAcqs ) {
        acqs.push_back( acq.clone() );
    }
    m_streamName2initAcqs[streamName] = std::move( acqs );

    //    OutputSensor&& outputSensor = OutputSensor( sensorSpec, io::OutputStream( "streamName" )
    //    );

    //    m_streamName2outputSensor[streamName] =
    //    std::make_unique<OutputSensor>(std::move(outputSensor));
    try {
        auto outputSensor = std::make_unique<OutputSensor>(
            sensorSpec, io::OutputStream( streamName, net::ClientSocket( m_ipv4, m_port ) ) );
        m_streamName2outputSensor[streamName] = std::move( outputSensor );
        for ( const auto& acq : m_streamName2initAcqs.at( streamName ) ) {
            newAcquisition( streamName, acq.clone() );
        }
        if ( !m_serverConnected ) {
            onServerConnected();
            //            m_serverConnected = true;
            //            if ( m_onServerConnected ) m_onServerConnected( *this, m_ipv4, m_port );
        }
    }
    catch ( net::Socket::exception& ex ) {
        std::cout << "[Streamer] unable to connect to server : " << m_ipv4 << " " << m_port
                  << std::endl;
        //        m_serverConnected = false;
        if ( m_serverConnected ) {
            onServerDisconnected();
            //            m_serverConnected = false;
            //            if ( m_onServerDisconnected ) m_onServerDisconnected( *this, m_ipv4,
            //            m_port );
        }
    }

    m_streamName2lastLogout[streamName] = std::chrono::high_resolution_clock::now();
    //        std::make_unique<OutputSensor>( sensorSpec, io::OutputStream( "streamName" ) );

    //    const auto & sensorSpec = m_streamName2sensorSpec.at(streamName);

    assert( m_streamName2sensorSpec.find( streamName ) == m_streamName2sensorSpec.end() );
    m_streamName2sensorSpec[streamName] = std::move( sensorSpec );
}

void Streamer::newAcquisition( const std::string& streamName, Acquisition&& acquisition ) {
//    std::cout << "[Streamer] new acquisition " << streamName << " " << acquisition << std::endl;

    //                     assert( m_streamName2outputSensor.find( streamName ) !=
    //    m_streamName2outputSensor.end() );
    if ( m_streamName2outputSensor.find( streamName ) == m_streamName2outputSensor.end() ) {
        auto& lastLogout = m_streamName2lastLogout.at( streamName );
        auto now         = std::chrono::high_resolution_clock::now();
        auto diff =
            std::chrono::duration_cast<std::chrono::milliseconds>( now - lastLogout ).count();
        if ( diff > 1000 ) {
            try {
                auto& sensorSpec  = m_streamName2sensorSpec.at( streamName );
                auto outputSensor = std::make_unique<OutputSensor>(
                    sensorSpec,
                    io::OutputStream( streamName, net::ClientSocket( m_ipv4, m_port ) ) );
                m_streamName2outputSensor[streamName] = std::move( outputSensor );
                for ( const auto& acq : m_streamName2initAcqs.at( streamName ) ) {
                    newAcquisition( streamName, acq.clone() );
                }
                if ( !m_serverConnected ) {
                    onServerConnected();
                    //                    m_serverConnected = true;
                    //                    if ( m_onServerConnected ) m_onServerConnected( *this,
                    //                    m_ipv4, m_port );
                }
            }
            catch ( net::Socket::exception& ex ) {
                std::cout << "[Streamer] unable to connect to server : " << m_ipv4 << " " << m_port
                          << std::endl;
                m_streamName2lastLogout.at( streamName ) = now;
                if ( m_serverConnected ) {
                    onServerDisconnected();
                    //                    m_serverConnected = false;
                    //                    if ( m_onServerDisconnected ) m_onServerDisconnected(
                    //                    *this, m_ipv4, m_port );
                }
            }
        }
    }

    if ( m_streamName2outputSensor.find( streamName ) != m_streamName2outputSensor.end() ) {
        try {
            auto& outputSensor = *m_streamName2outputSensor.at( streamName );
            outputSensor << acquisition;
            if ( !m_serverConnected ) {
                //                m_serverConnected = true;
                onServerConnected();
                //                if ( m_onServerConnected ) m_onServerConnected( *this, m_ipv4,
                //                m_port );
            }
        }
        catch ( net::Socket::exception& ex ) {
            m_streamName2outputSensor.clear();
            std::cout << "[Streamer] catch outputSensor new acq exception" << std::endl;
            //            m_streamName2outputSensor.erase( streamName );
            auto now = std::chrono::high_resolution_clock::now();
            for ( auto& pair : m_streamName2lastLogout ) {
                pair.second = now;
            }
            if ( m_serverConnected ) {
                onServerDisconnected();
                //                m_serverConnected = false;
                //                if ( m_onServerDisconnected ) m_onServerDisconnected( *this,
                //                m_ipv4, m_port );
            }
//            m_streamName2lastLogout[streamName] = std::chrono::high_resolution_clock::now();
#ifdef OS_LINUX
            ++m_port;
#endif
        }
    }
}

bool Streamer::isConnected() const {
    return m_serverConnected;
}

void Streamer::onServerConnected() {
    assert(! m_serverConnected);
    std::cout << "[Streamer] onServerConnected" << std::endl;
    m_serverConnected = true;

    if ( m_thread.joinable() ) { m_thread.join(); }
}

void Streamer::onServerDisconnected() {
    assert(m_serverConnected);
    std::cout << "[Streamer] onServerDisconnected" << std::endl;
    m_serverConnected = false;

    m_thread = std::thread( [this]() {
        while ( !m_serverConnected ) {
            for ( const auto& pair : m_streamName2initAcqs ) {
                const auto& streamName = pair.first;
                const auto& initAcqs   = pair.second;
                newAcquisition( streamName, initAcqs.front().clone() );
                //                if (m_serverConnected)
                //                    break;
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
    } );
}

// void Streamer::onServerConnected(const std::string &streamName)
//{

//}

//    m_thread = std::thread( [this]() {
//        while ( !m_stopThread ) {
//            try {
////                std::cout << "[Streamer] try to connect to server : " << m_ipv4 << " " << m_port
////                          << std::endl;
//                net::ClientSocket sock( m_ipv4, m_port );

//                m_serverConnected = true;
//                sock.write( net::ClientSocket::Type::VIEWER );

//                if ( m_onServerConnected ) m_onServerConnected( m_ipv4, m_port );

//                while ( !m_stopThread ) {
//                    net::ClientSocket::Message serverMessage;
//                    sock.read( serverMessage );

//                    switch ( serverMessage ) {

//                    case net::ClientSocket::Message::PING: {
//                        // server check client connection
//                        // nothing to do
//#ifdef DEBUG_VIEWER
//                        DEBUG_MSG( "[Streamer] receive ping " );
//#endif
//                    } break;

//                    case net::ClientSocket::Message::NEW_STREAMER: {
//                        std::string streamName;
//                        sock.read( streamName );
//#ifdef DEBUG_VIEWER
//                        DEBUG_MSG( "[Streamer] new streamer '" << streamName << "'" );
//#endif

//                        SensorSpec sensorSpec;
//                        sock.read( sensorSpec );

//                        m_onNewStreamer( streamName, sensorSpec );

//                    } break;

//                    case net::ClientSocket::Message::DEL_STREAMER: {
//                        std::string streamName;
//                        sock.read( streamName );
//                        SensorSpec sensorSpec;
//                        sock.read( sensorSpec );
//#ifdef DEBUG_VIEWER
//                        DEBUG_MSG( "[Streamer] del streamer '" << streamName << "'" );
//#endif

//                        m_onDelStreamer( streamName, sensorSpec );

//                    } break;

//                    default: {
//                        assert( false );
//#ifdef DEBUG_VIEWER
//                        DEBUG_MSG( "[Streamer] unknown message from server" );
//#endif
//                    }
//                    } // switch (serverMessage)

//                } // while (! m_stopThread)
//            }
//            catch ( std::exception& e ) {
//#ifdef DEBUG_VIEWER
//                DEBUG_MSG( "[Streamer] server disconnected, catch exception " << e.what() );
//#endif
//                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            }
//            if ( m_serverConnected && m_onServerDisconnected )
//                m_onServerDisconnected( m_ipv4, m_port );
//            m_serverConnected = false;

//        } // while (! m_stopThread)
//    } );  // thread

Streamer::~Streamer() {
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Streamer] ~Streamer()" );
#endif
//    m_stopThread = true;
//    assert( m_thread.joinable() );
//    m_thread.join();
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Streamer] ~Streamer() done" );
#endif
}

// void Streamer::setIpv4( const std::string& ipv4 ) {
//     assert( !m_serverConnected );
//     assert( std::regex_match( ipv4, m_ipv4Regex ) );
//     m_ipv4 = ipv4;
// }

// void Streamer::setPort( int port ) {
//     assert( !m_serverConnected );
//     assert( 0 <= m_port && m_port <= 65535 );
//     m_port = port;
// }

} // namespace hub
