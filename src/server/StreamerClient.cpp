
#include <cstring>
#include <iomanip>

#include "Server.hpp"
#include "StreamerClient.hpp"

namespace hub {
namespace server {

class InputStreamClient : public hub::Input
{
  public:
    explicit InputStreamClient( hub::net::ClientSocket&& clientSocket );

    void read( Acquisition& acq ) override;

    InputStreamClient( InputStreamClient&& inputStream );
    ~InputStreamClient();

  protected:
    void read( unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;
    bool isEnd() const override;

  private:
    hub::net::ClientSocket m_clientSocket;
    bool m_outputStreamClosed = false;

    bool m_moved = false;

    friend class StreamerClient;
};

InputStreamClient::InputStreamClient( net::ClientSocket&& clientSocket ) :
    m_clientSocket( std::move( clientSocket ) ) {

    std::cout << "[InputStreamClient] InputStreamClient()" << std::endl;
}

InputStreamClient::InputStreamClient( InputStreamClient&& inputStream ) :
    m_clientSocket( std::move( inputStream.m_clientSocket ) ),
    m_outputStreamClosed( inputStream.m_outputStreamClosed ) {
    inputStream.m_moved = true;
    std::cout << "[InputStreamClient] InputStreamClient(&&)" << std::endl;
}

InputStreamClient::~InputStreamClient() {
    std::cout << "[InputStreamClient] ~InputStreamClient()" << std::endl;
#ifdef DEBUG
    if ( !m_moved ) { assert( !isOpen() ); }
#endif
}

void InputStreamClient::read( Acquisition& acq )
// Acquisition InputStreamClient::getAcq()
{
    io::StreamInterface::ClientMessage mess;
    m_clientSocket.read( mess );
    if ( mess == io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED ) {
        throw net::Socket::exception( "streamer closed" );
    }
    assert( mess == io::StreamInterface::ClientMessage::STREAMER_CLIENT_NEW_ACQ );
    Input::read( acq );
}

void InputStreamClient::read( unsigned char* data, size_t len ) {
    m_clientSocket.read( data, len );
}

void InputStreamClient::close() {
    std::cout << "[InputStreamClient] close()" << std::endl;
    m_clientSocket.write( io::StreamInterface::ServerMessage::STREAMER_CLOSED );
    m_clientSocket.close();
}

bool InputStreamClient::isOpen() const {
    return m_clientSocket.isOpen();
}

bool InputStreamClient::isEnd() const {
    return m_clientSocket.isEnd();
}

//////////////////////////////////////////////////////////////////////////////

StreamerClient::StreamerClient( Server* server,
                                int iClient,
                                hub::net::ClientSocket&& sock,
                                std::string streamName ) :
    Client( server, iClient ),
    m_streamName( std::move( streamName ) )

{
    std::cout << "[StreamerClient] StreamerClient() m_clientSocket : " << &sock << std::endl;
    std::cout << headerMsg() << "StreamerClient() start" << std::endl;

    // get record acqs before prevent viewer
    // std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    assert( m_server != nullptr );
    m_server->addStreamer( this );

    m_thread = std::thread( [this, sock = std::move( sock )]() mutable {
        try {
            io::StreamInterface::ClientMessage mess;
            do {
                sock.read( mess );
                if ( mess == io::StreamInterface::ClientMessage::STREAMER_CLIENT_CLOSED ) {
                    sock.write( io::StreamInterface::ServerMessage::STREAMER_CLOSED );
                    //                assert(sock.isOpen());
                    sock.close();
                    throw net::Socket::exception( "output stream closed" );
                }
            } while ( mess != io::StreamInterface::ClientMessage::STREAMER_CLIENT_INIT_SENSOR );
            assert( mess == io::StreamInterface::ClientMessage::STREAMER_CLIENT_INIT_SENSOR );

            m_inputSensor =
                std::make_unique<hub::InputSensor>( InputStreamClient( std::move( sock ) ) );
            //                std::make_unique<hub::InputSensor>( InputStreamClient( sock ) );
        }
        catch ( hub::net::Socket::exception& e ) {
            std::cout << headerMsg() << "InputSensor() : catch exception : " << e.what()
                      << std::endl;
            std::thread( [this]() { delete this; } ).detach();
            //            throw e;
            return;
        }

        std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

        const auto& sensorSpec = m_inputSensor->getSpec();

        const size_t acquisitionSize = sensorSpec.getAcquisitionSize();
        std::cout << headerMsg() << "sensor name:'" << sensorSpec.getSensorName() << "'"
                  << std::endl;
        std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
        std::cout << headerMsg() << "resolutions:" << sensorSpec.getResolutions() << std::endl;

        const auto& metaData = sensorSpec.getMetaData();
        for ( const auto& pair : metaData ) {
            std::cout << headerMsg() << "metaData: " << hub::SensorSpec::to_string( pair )
                      << std::endl;
            ////            mesh.printStats();
        }
        if ( metaData.find( "nAcq" ) != metaData.end() ) {
            std::cout << headerMsg() << "type detected : packed stream" << std::endl;
            m_nAcq           = metaData.at( "nAcq" ).getInt();
            m_isPackedStream = true;
        }

        //        // get record acqs before prevent viewer
        //        // std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        //        assert( m_server != nullptr );
        //        m_server->addStreamer( this );

        m_server->newInputSensor( this );

        try {

            while ( m_inputSensor->getInput().isOpen() ) {

                *m_inputSensor >> m_lastAcq;
                //                std::cout << "get acq " << m_lastAcq << std::endl;
                assert( !m_lastAcq.isEmpty() );

                m_server->newAcquisition( this, m_lastAcq );

                if ( m_isPackedStream ) {
                    auto it = m_packedAcqs.insert( m_lastAcq.clone() );
                    assert( m_packedAcqs.size() <= m_nAcq );
                    if ( m_packedAcqs.size() == m_nAcq ) {
                        std::cout << headerMsg() << "updating saved acq "
                                  << std::distance( m_packedAcqs.begin(), it.first ) << "/"
                                  << m_nAcq << std::endl;
                    }
                }

            } // while (true)
            assert( false );
        }
        catch ( hub::net::Socket::exception& ex ) {
            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }

        std::thread( [this]() { delete this; } ).detach();
    } );
}

StreamerClient::~StreamerClient() {
    std::cout << headerMsg() << "delete start" << std::endl;

    assert( m_thread.joinable() );
    m_thread.join();

    if ( m_server != nullptr ) m_server->delStreamer( this );

    std::cout << headerMsg() << "delete ended" << std::endl;
}

std::string StreamerClient::headerMsg() const {
    return Client::headerMsg() + "[Streamer] ";
}

const hub::InputSensor* StreamerClient::getInputSensor() const {
    //    assert(m_inputSensor != nullptr);
    return m_inputSensor.get();
}

Acquisition StreamerClient::getLastAcq() const {

    while ( m_lastAcq.isEmpty() ) {
        std::cout << "last acq empty" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    assert( !m_lastAcq.isEmpty() );
    auto acq = m_lastAcq.clone();

    return acq;
}

void StreamerClient::end( io::StreamInterface::ServerMessage message ) {
    InputStreamClient& input = dynamic_cast<InputStreamClient&>( m_inputSensor->getInput() );
    assert( input.m_clientSocket.isOpen() );
    input.m_clientSocket.write( message );
}

const std::string& StreamerClient::getStreamName() const {
    return m_streamName;
}

bool StreamerClient::isPackedStream() const {
    return m_isPackedStream;
}

const std::set<hub::Acquisition>& StreamerClient::getPackedAcqs() const {
    while ( m_packedAcqs.size() != m_nAcq ) {
        std::cout << headerMsg() << "waiting for all packed acqs" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    assert( m_packedAcqs.size() == m_nAcq );
    return m_packedAcqs;
}

// void StreamerClient::addStreamViewer( StreamViewerClient* streamViewer ) {

// void StreamerClient::addSyncStreamViewer( StreamViewerClient* syncStreamViewer ) {

// void StreamerClient::delStreamViewer( StreamViewerClient* streamViewer ) {

// void StreamerClient::newAcquisition( const std::string& streamerName,

// const std::string& StreamerClient::getStreamName() const {
// }

// const std::map<std::string, std::list<StreamViewerClient*>>&
// StreamerClient::getSyncViewers() const {
// }

// const std::shared_ptr<hub::Acquisition>
// StreamerClient::getLastAcq( const std::string& streamName ) const {
// }

// const std::map<long long, std::shared_ptr<hub::Acquisition>>&
// StreamerClient::getSaveAcqs( const std::string& streamName ) const {
// }

// void StreamerClient::saveNewAcq( const std::string& streamName, hub::Acquisition&& newAcq ) {

// #if ( __cplusplus >= 201703L )
// #else
// #endif

// bool StreamerClient::isRecordStream() const {
// }

// const std::string& StreamerClient::getParent() const {
// }

// const std::list<StreamViewerClient*>& StreamerClient::getStreamViewers() const {
// }

// const std::chrono::time_point<std::chrono::high_resolution_clock>&
// StreamerClient::getLastUpdateAcqDate( const std::string& streamName ) const {
// }

} // namespace server
} // namespace hub

// #if ( __cplusplus >= 201703L )
// #else
// #endif
