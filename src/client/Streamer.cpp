#include "Streamer.hpp"

#include <algorithm>
#include <chrono>
#include <regex>
// #include <mutex>

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

        //        assert( m_streamName2initAcqs.find( streamName ) == m_streamName2initAcqs.end() );
        //        std::vector<Acquisition> acqs;
        //        for ( const auto& initAcq : initAcqs ) {
        //            acqs.push_back( initAcq.clone() );
        //        }
        //        //    std::transform( initAcqs.crbegin(),
        //        //                    initAcqs.crend(),
        //        //                    std::back_inserter( acqs ),
        //        //                    []( const Acquisition& acq ) { return acq.clone(); } );
        //        m_streamName2initAcqs[streamName] = std::move( acqs );

        if ( m_streamer.m_serverConnected ) {
            try {

                init();
                assert(m_outputSensor != nullptr);
                //                    if ( !m_serverConnected ) { onServerConnected(); }
            }
            catch ( net::Socket::exception& e ) {
                //                m_outputSensor.release();
                //                m_outputSensor.reset( nullptr );

                //                m_outputSensor = nullptr;

                //                if ( m_streamer.m_serverConnected ) {
                std::cout << "[Streamer][Stream] loose connection from server : "
                          << m_streamer.m_ipv4 << " " << m_streamer.m_port << std::endl;

                delete m_outputSensor;
                m_outputSensor = nullptr;

                //                }
                //                else {
                //                    std::cout << "[Streamer][Stream] unable to connect to server :
                //                    "
                //                              << m_streamer.m_ipv4 << " " << m_streamer.m_port <<
                //                              std::endl;
                //                }
                std::cout << "[Streamer][Stream] catch exception : " << e.what() << std::endl;

                //                if ( m_streamer.m_serverConnected ) {
                m_streamer.onServerDisconnected();
                //            }
                //                else { m_streamer.waitingForServer(); }
                //                    if ( m_serverConnected ) { onServerDisconnected(); }
            }
        }

        //        m_streamName2lastLogout[streamName] = std::chrono::high_resolution_clock::now();

        //        assert( m_streamName2sensorSpec.find( streamName ) ==
        //        m_streamName2sensorSpec.end() ); m_streamName2sensorSpec[streamName] = std::move(
        //        sensorSpec );
    }

    Stream( Stream&& stream ) = delete;
    //        m_streamer(stream.m_streamer),
    //        m_streamName(stream.m_streamName),
    //        m_initAcqs()
    //    {
    //    }

    ~Stream() {
        std::cout << "[Stream] ~Stream()" << std::endl;

        //        m_outputSensor.release();
        //        m_outputSensor.reset( nullptr );
        if ( m_outputSensor != nullptr ) {
            delete m_outputSensor;
            m_outputSensor = nullptr;
        }
        //        assert( m_outputSensor == nullptr );
    }

    void onServerClosed() { std::cout << "[Stream] outputStream closed by server" << std::endl; }

    void init() {

        std::cout << "[Stream] " << m_streamName << " init start " << std::endl;

        assert( m_outputSensor == nullptr );

//        bool sockInited = false;
        try {
            hub::net::ClientSocket sock( m_streamer.m_ipv4, m_streamer.m_port );
//            sockInited = true;
            assert(sock.isConnected());
            assert(sock.isOpen());

//            std::cout << "[Stream] init() sock ready" << std::endl;
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            //    auto onServerClosed = []( ) {
            //                std::cout << "onServerClosed" << std::endl;
            //    };

            //            io::OutputStream outputStream(
            //                m_streamName, std::move( sock ), [this]() { onServerClosed(); } );
            //                m_streamName, std::move( sock ) );

            //                std::make_unique<OutputSensor>( m_sensorSpec, std::move(
            //                outputStream ) );
//            std::make_unique<OutputSensor>( m_sensorSpec, m_streamName, std::move( sock ) );
            assert(m_outputSensor == nullptr);
            m_outputSensor = new OutputSensor( m_sensorSpec, m_streamName, std::move( sock ) );
//            std::cout << "[Stream] init() outputSensor ready" << std::endl;
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            //            auto & output = m_outputSensor->getOutput();
            //            output.setCloseEvent(onServerClosed);

            //                m_sensorSpec, io::OutputStream( m_streamName, std::move( sock ) ) );
            //                              net::ClientSocket( m_streamer.m_ipv4, m_streamer.m_port
            //                              ) ) );

            //                    m_streams[streamName] = std::move( outputSensor );

            m_mtxOutputSensor.lock();
            for ( const auto& acq : m_initAcqs ) {
                //                std::cout << "[Streamer:Stream] " << m_streamName << " init acq "
                //                << std::endl;
                //            newAcquisition( acq );
                *m_outputSensor << acq;
            }
            m_mtxOutputSensor.unlock();
        }
        catch ( std::exception& ex ) {
//            assert( !sockInited );
            m_mtxOutputSensor.unlock();
            std::cout << "[Stream] init() catch exception : " << ex.what() << std::endl;

            throw ex;
        }

        std::cout << "[Stream] " << m_streamName << " init end " << std::endl;
    }

    void newAcquisition( const Acquisition& acquisition ) {

        assert( m_streamer.m_serverConnected );

        //        assert( m_outputSensor != nullptr );
//        m_mtxOutputSensor.lock();
        if ( m_outputSensor != nullptr ) {
            try {
                m_mtxOutputSensor.lock();
                *m_outputSensor << acquisition;
                m_mtxOutputSensor.unlock();
            }
            catch ( std::exception& ex ) {
                delete m_outputSensor;
                m_outputSensor = nullptr;
                m_mtxOutputSensor.unlock();
                //                m_outputSensor.release();
                //                m_outputSensor.reset( nullptr );
                throw ex;
            }
        }

        //        if ( !m_streamer.m_serverConnected ) { m_streamer.onServerConnected(); }

        //        m_mtxOutputSensor.unlock();

        //        if ( m_streams.find( streamName ) == m_streams.end() ) {

        //            const auto& lastLogout = m_streamName2lastLogout.at( streamName );
        //            auto now               = std::chrono::high_resolution_clock::now();
        //            auto diff =
        //                std::chrono::duration_cast<std::chrono::milliseconds>( now - lastLogout
        //                ).count();
        //            //        std::cout << "diff = " << diff << std::endl;
        //            if ( diff > 1000 ) {
        //                try {
        //                    auto& sensorSpec  = m_streamName2sensorSpec.at( streamName );
        //                    auto outputSensor = std::make_unique<OutputSensor>(
        //                        sensorSpec,
        //                        io::OutputStream( streamName, net::ClientSocket( m_ipv4, m_port )
        //                        ) );
        //                    m_streams[streamName] = std::move( outputSensor );
        //                    if ( !m_serverConnected ) { onServerConnected(); }
        //                }
        //                catch ( net::Socket::exception& e ) {
        //                    std::cout << "[Streamer] unable to connect to server : " << m_ipv4 <<
        //                    " "
        //                              << m_port << ", catch exception : " << e.what() <<
        //                              std::endl;
        //                    m_streamName2lastLogout.at( streamName ) = now;
        //                    if ( m_serverConnected ) { onServerDisconnected(); }
        //                }
        //            }
        //        }

        //        if ( m_streams.find( streamName ) != m_streams.end() ) {
        //            try {
        //                //            std::cout << "[Streamer] send acq" << std::endl;
        //                auto& outputSensor = *m_streams.at( streamName );
        //                outputSensor << acquisition;
        //                if ( !m_serverConnected ) { onServerConnected(); }
        //            }
        //            catch ( net::Socket::exception& e ) {
        //                m_streams.clear();
        //                std::cout << "[Streamer] catch outputSensor new acq exception : " <<
        //                e.what()
        //                          << std::endl;
        //                auto now = std::chrono::high_resolution_clock::now();
        //                for ( auto& pair : m_streamName2lastLogout ) {
        //                    pair.second = now;
        //                }
        //                if ( m_serverConnected ) { onServerDisconnected(); }
        // #ifdef OS_LINUX
        //                ++m_port;
        // #endif
        //            }
        //        }
    }

  private:
    Streamer& m_streamer;
    const std::string m_streamName;
    const hub::SensorSpec m_sensorSpec;
    //    std::unique_ptr<hub::OutputSensor> m_outputSensor = nullptr;
    hub::OutputSensor* m_outputSensor = nullptr;

    std::mutex m_mtxOutputSensor;
    //    std::mutex m_mtxOutputSensor;

    std::vector<hub::Acquisition> m_initAcqs;
//    std::chrono::high_resolution_clock m_lastLogout;
};

} // end namespace streamer

Streamer::Streamer( const std::string& ipv4, int port ) :
    m_ipv4( ipv4 ),
    m_port( port ),
    m_ipv4Regex( std::regex( "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" ) ) {

    assert( std::regex_match( m_ipv4, m_ipv4Regex ) );
    assert( 0 <= m_port && m_port <= 65535 );

    //    m_thread = std::thread( [this]() {
    //        while (! m_stopThread) {

    //        }

    //        while ( !m_serverConnected ) {
    //            for ( const auto& pair : m_streamName2initAcqs ) {
    //                const auto& streamName = pair.first;
    //                const auto& initAcqs   = pair.second;
    //                if ( !initAcqs.empty() ) {
    //                    assert( initAcqs.size() > 0 );
    //                    newAcquisition( streamName, initAcqs.front().clone() );
    //                }
    //            }
    //            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    //        }
    //    } );
}

void Streamer::addStream( const std::string& streamName,
                          const SensorSpec& sensorSpec,
                          std::vector<Acquisition>&& initAcqs ) {
    std::cout << "[Streamer] addStream " << streamName << " started" << std::endl;
    assert( m_streams.find( streamName ) == m_streams.end() );

    //    streamer::Stream stream( *this, streamName, sensorSpec, std::move( initAcqs ) );
    //    m_streams[streamName] = std::make_unique<streamer::Stream>( std::move( stream ) );
    m_streams[streamName] =
        std::make_unique<streamer::Stream>( *this, streamName, sensorSpec, std::move( initAcqs ) );

    if ( !m_serverConnected ) { waitingForServer(); }

    std::cout << "[Streamer] addStream " << streamName << " ended" << std::endl;
}

void Streamer::newAcquisition( const std::string& streamName, const Acquisition& acquisition ) {

    //    std::cout << "[Streamer] newAcquisition" << std::endl;
    //    assert( m_serverConnected );
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
    std::cout << "[Streamer] onServerConnected" << std::endl;
    m_serverConnected = true;

    assert( m_thread.joinable() );
    //        m_thread.join();
    //    if ( m_thread.joinable() ) { m_thread.join(); }
}

void Streamer::onServerDisconnected() {
    assert( m_serverConnected );
    std::cout << "[Streamer] onServerDisconnected" << std::endl;
    m_serverConnected = false;

    waitingForServer();
}

void Streamer::waitingForServer() {
    std::cout << "[Streamer] waiting for server started " << std::endl;

    if ( m_thread.joinable() ) m_thread.join();
    assert( !m_thread.joinable() );
    //    assert( !m_serverConnected );
    if ( !m_serverConnected ) {

        m_thread = std::thread( [this]() {
            while ( !m_exitThread && !m_serverConnected ) {
                try {
                    assert( !m_streams.empty() );
                    for ( const auto& pair : m_streams ) {
                        auto& stream = pair.second;
                        //                for ( auto& stream : m_streams ) {
                        stream->init();
                    }
                    onServerConnected();
                }
                catch ( std::exception& ex ) {
                    std::cout << "[Streamer] unable to init streams, server is not running "
                              << std::endl;
                    std::cout << "[Streamer] catch exception : " << ex.what() << std::endl;
                    //                    const auto& streamName = pair.first;
                    //                    const auto& initAcqs   = pair.second;
                    //                    if ( !initAcqs.empty() ) {
                    //                        assert( initAcqs.size() > 0 );
                    //                        newAcquisition( streamName, initAcqs.front().clone()
                    //                        );
                    //                    }
                    //                }
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
                }
            }
            std::cout << "[Streamer] waiting for server ended " << std::endl;
        } );
    }
}

Streamer::~Streamer() {
    DEBUG_MSG( "[Streamer] ~Streamer() started" );
    m_exitThread = true;
    assert( m_thread.joinable() );
    m_thread.join();
    DEBUG_MSG( "[Streamer] ~Streamer() ended" );
}

} // namespace client
} // namespace hub
