#include "Streamer.hpp"

#include <algorithm>
#include <chrono>
#include <regex>

#include "OutputSensor.hpp"

namespace hub {
namespace client {
namespace streamer {

class Stream
{
  public:
    Stream( Streamer& streamer,
            const std::string& streamName,
            const SensorSpec& sensorSpec,
            std::vector<Acquisition>&& initAcqs ) :
        m_streamer( streamer ),
        m_streamName( streamName ),
        m_sensorSpec( sensorSpec ),
        m_initAcqs( std::move( initAcqs ) ) {

        if ( m_streamer.m_serverConnected ) {
            try {

                init();
                assert( m_outputSensor != nullptr );
            }
            catch ( net::Socket::exception& e ) {

                std::cout << "[Streamer][Stream] loose connection from server : "
                          << m_streamer.m_ipv4 << " " << m_streamer.m_port << std::endl;

                delete m_outputSensor;
                m_outputSensor = nullptr;

                std::cout << "[Streamer][Stream] catch exception : " << e.what() << std::endl;

                m_streamer.onServerDisconnected();
            }
        }
    }

    Stream( Stream&& stream ) = delete;

    ~Stream() {
        std::cout << "[Stream] ~Stream()" << std::endl;

        if ( m_outputSensor != nullptr ) {
            delete m_outputSensor;
            m_outputSensor = nullptr;
        }
    }

//    void onServerClosed() { std::cout << "[Stream] outputStream closed by server" << std::endl; }

    void init() {

        assert( m_outputSensor == nullptr );
        m_mtxOutputSensor.lock();

        try {
            hub::net::ClientSocket sock( m_streamer.m_ipv4, m_streamer.m_port );
            assert( sock.isConnected() );
            assert( sock.isOpen() );

            assert( m_outputSensor == nullptr );
            m_outputSensor = new OutputSensor( m_sensorSpec, m_streamName, std::move( sock ) );

            for ( const auto& acq : m_initAcqs ) {
                *m_outputSensor << acq;
            }
            m_mtxOutputSensor.unlock();
        }
        catch ( std::exception& ex ) {
            m_mtxOutputSensor.unlock();
            std::cout << "[Stream] init() catch exception : " << ex.what() << std::endl;

            throw ex;
        }

        std::cout << "[Stream] " << m_streamName << " inited " << std::endl;
    }

    void newAcquisition( const Acquisition& acquisition ) {

        assert( m_streamer.m_serverConnected );

        if ( m_outputSensor != nullptr ) {
            try {
                m_mtxOutputSensor.lock();
                *m_outputSensor << acquisition;
                m_mtxOutputSensor.unlock();
            }
            catch ( std::exception& ) {
                delete m_outputSensor;
                m_outputSensor = nullptr;
                m_mtxOutputSensor.unlock();
                throw;
            }
        }
    }

  private:
    Streamer& m_streamer;
    const std::string m_streamName;
    const hub::SensorSpec m_sensorSpec;
    hub::OutputSensor* m_outputSensor = nullptr;

    std::mutex m_mtxOutputSensor;

    std::vector<hub::Acquisition> m_initAcqs;
};

} // end namespace streamer

Streamer::Streamer( const std::string& ipv4, int port ) :
    m_ipv4( ipv4 ),
    m_port( port ),
    m_ipv4Regex( std::regex( "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" ) ) {

    assert( std::regex_match( m_ipv4, m_ipv4Regex ) );
    assert( 0 <= m_port && m_port <= 65535 );
}

void Streamer::addStream( const std::string& streamName,
                          const SensorSpec& sensorSpec,
                          std::vector<Acquisition>&& initAcqs ) {
    std::cout << "[Streamer] addStream '" << streamName << "'" << std::endl;
    assert( m_streams.find( streamName ) == m_streams.end() );

    m_streams[streamName] =
        std::make_unique<streamer::Stream>( *this, streamName, sensorSpec, std::move( initAcqs ) );

    if ( !m_serverConnected ) { waitingForServer(); }
}

void Streamer::newAcquisition( const std::string& streamName, const Acquisition& acquisition ) {

    if ( m_serverConnected ) {

        assert( m_streams.find( streamName ) != m_streams.end() );

        auto& stream = m_streams.at( streamName );
        try {
            stream->newAcquisition( acquisition );
        }
        catch ( std::exception& ex ) {
            std::cout << "[Streamer] loosing server connection " << std::endl;
            std::cout << "[Streamer] catch exception : " << ex.what() << std::endl;
            onServerDisconnected();
        }
    }
}

bool Streamer::isConnected() const {
    return m_serverConnected;
}

void Streamer::onServerConnected() {
    assert( !m_serverConnected );
    std::cout << "[Streamer] server connected" << std::endl;
    m_serverConnected = true;

    assert( m_thread.joinable() );
}

void Streamer::onServerDisconnected() {
    assert( m_serverConnected );
    std::cout << "[Streamer] onServerDisconnected" << std::endl;
    m_serverConnected = false;

    waitingForServer();
}

void Streamer::waitingForServer() {
    std::cout << "[Streamer] waiting for server ..." << std::endl;

    if ( m_thread.joinable() ) m_thread.join();
    assert( !m_thread.joinable() );
    if ( !m_serverConnected ) {

        m_thread = std::thread( [this]() {
            while ( !m_exitThread && !m_serverConnected ) {
                try {
                    assert( !m_streams.empty() );
                    for ( const auto& pair : m_streams ) {
                        auto& stream = pair.second;
                        stream->init();
                    }
                    onServerConnected();
                }
                catch ( std::exception& ex ) {
                    std::cout << "[Streamer] unable to init streams, server is not running "
                              << std::endl;
                    std::cout << "[Streamer] catch exception : " << ex.what() << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                }
            }
        } );
    }
}

Streamer::~Streamer() {
    DEBUG_MSG( "[Streamer] ~Streamer() started" );
    m_exitThread = true;
    if ( m_thread.joinable() ) m_thread.join();
    DEBUG_MSG( "[Streamer] ~Streamer() ended" );
}

} // namespace client
} // namespace hub
