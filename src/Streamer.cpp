#include "Streamer.hpp"

//#define DEBUG_VIEWER

#include <chrono>
#include <regex>

#include "IO/Stream.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {

Streamer::Streamer(
    const std::string& ipv4,
    int port ) :
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

    assert( m_streamName2initAcqs.find( streamName ) == m_streamName2initAcqs.end() );
    std::vector<Acquisition> acqs;
    for ( const auto& acq : initAcqs ) {
        acqs.push_back( acq.clone() );
    }
    m_streamName2initAcqs[streamName] = std::move( acqs );


    try {
        auto outputSensor = std::make_unique<OutputSensor>(
            sensorSpec, io::OutputStream( streamName, net::ClientSocket( m_ipv4, m_port ) ) );
        m_streamName2outputSensor[streamName] = std::move( outputSensor );
        for ( const auto& acq : m_streamName2initAcqs.at( streamName ) ) {
            newAcquisition( streamName, acq.clone() );
        }
        if ( !m_serverConnected ) {
            onServerConnected();
        }
    }
    catch ( net::Socket::exception& ex ) {
        std::cout << "[Streamer] unable to connect to server : " << m_ipv4 << " " << m_port
                  << std::endl;
        if ( m_serverConnected ) {
            onServerDisconnected();
        }
    }

    m_streamName2lastLogout[streamName] = std::chrono::high_resolution_clock::now();


    assert( m_streamName2sensorSpec.find( streamName ) == m_streamName2sensorSpec.end() );
    m_streamName2sensorSpec[streamName] = std::move( sensorSpec );
}

void Streamer::newAcquisition( const std::string& streamName, Acquisition&& acquisition ) {

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
                if ( !m_serverConnected ) {
                    onServerConnected();
                }
            }
            catch ( net::Socket::exception& ex ) {
                std::cout << "[Streamer] unable to connect to server : " << m_ipv4 << " " << m_port
                          << std::endl;
                m_streamName2lastLogout.at( streamName ) = now;
                if ( m_serverConnected ) {
                    onServerDisconnected();
                }
            }
        }
    }

    if ( m_streamName2outputSensor.find( streamName ) != m_streamName2outputSensor.end() ) {
        try {
            auto& outputSensor = *m_streamName2outputSensor.at( streamName );
            outputSensor << acquisition;
            if ( !m_serverConnected ) {
                onServerConnected();
            }
        }
        catch ( net::Socket::exception& ex ) {
            m_streamName2outputSensor.clear();
            std::cout << "[Streamer] catch outputSensor new acq exception" << std::endl;
            auto now = std::chrono::high_resolution_clock::now();
            for ( auto& pair : m_streamName2lastLogout ) {
                pair.second = now;
            }
            if ( m_serverConnected ) {
                onServerDisconnected();
            }
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
    assert( !m_serverConnected );
    std::cout << "[Streamer] onServerConnected" << std::endl;
    m_serverConnected = true;

    if ( m_thread.joinable() ) { m_thread.join(); }
}

void Streamer::onServerDisconnected() {
    assert( m_serverConnected );
    std::cout << "[Streamer] onServerDisconnected" << std::endl;
    m_serverConnected = false;

    m_thread = std::thread( [this]() {
        while ( !m_serverConnected ) {
            for ( const auto& pair : m_streamName2initAcqs ) {
                const auto& streamName = pair.first;
                const auto& initAcqs   = pair.second;
                if ( !initAcqs.empty() ) {
                    assert( initAcqs.size() > 0 );
                    newAcquisition( streamName, initAcqs.front().clone() );
                }
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
    } );
}

// void Streamer::onServerConnected(const std::string &streamName)
//{

//}






//#ifdef DEBUG_VIEWER
//#endif

//#ifdef DEBUG_VIEWER
//#endif




//#ifdef DEBUG_VIEWER
//#endif



//#ifdef DEBUG_VIEWER
//#endif

//#ifdef DEBUG_VIEWER
//#endif


Streamer::~Streamer() {
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Streamer] ~Streamer()" );
#endif
#ifdef DEBUG_VIEWER
    DEBUG_MSG( "[Streamer] ~Streamer() done" );
#endif
}

// void Streamer::setIpv4( const std::string& ipv4 ) {
// }

// void Streamer::setPort( int port ) {
// }

} // namespace hub
