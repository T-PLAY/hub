
#include "ServerImpl.hpp"
#ifdef HUB_USE_SERVER

#    include <cstring>
#    include <iomanip>

#    include "StreamerClient.hpp"

namespace hub {
namespace server {
namespace impl {

class InputStreamClient : public Input
{
  public:
    explicit InputStreamClient( io::InputOutputSocket&& clientSocket );

    void read( sensor::Acquisition& acq );

    InputStreamClient( InputStreamClient&& inputStream );
    ~InputStreamClient();

  protected:
    void read( Data_t* data, Size_t len ) override;
    void close() override;
    bool isOpen() const override;
    bool isEnd() const override;
    void clear() override;

  private:
    io::InputOutputSocket m_clientSocket;
    bool m_outputStreamClosed = false;

    bool m_moved = false;

    friend class StreamerClient;
};

InputStreamClient::InputStreamClient( io::InputOutputSocket&& clientSocket ) :
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
#    ifdef DEBUG
    if ( !m_moved ) { assert( !isOpen() ); }
#    endif
}

void InputStreamClient::read( sensor::Acquisition& acq ) {
    io::StreamBase::ClientMessage mess;
    m_clientSocket.read( mess );
    if ( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_CLOSED ) {
        throw net::system::SocketSystem::exception( "streamer closed" );
    }
    assert( mess == io::StreamBase::ClientMessage::STREAMER_CLIENT_NEW_ACQ );
    Input::read( acq );
}

void InputStreamClient::read( Data_t* data, Size_t len ) {
    m_clientSocket.read( data, len );
}

void InputStreamClient::close() {
    std::cout << "[InputStreamClient] close()" << std::endl;
    m_clientSocket.write( io::StreamBase::ServerMessage::STREAMER_CLOSED );
    m_clientSocket.close();
}

bool InputStreamClient::isOpen() const {
    return m_clientSocket.isOpen();
}

bool InputStreamClient::isEnd() const {
    return m_clientSocket.isEnd();
}

void InputStreamClient::clear() {}

StreamerClient::StreamerClient( ServerImpl* server,
                                int iClient,
                                io::InputOutputSocket&& sock,
                                std::string streamName ) :
    Client( server, iClient ),
    m_streamName( std::move( streamName ) )

{
    std::cout << "[StreamerClient] StreamerClient() m_clientSocket : " << &sock << std::endl;
    std::cout << headerMsg() << "StreamerClient() start" << std::endl;

    m_inputSensor = std::make_unique<sensor::InputSensor>( InputStreamClient( std::move( sock ) ) );

    std::cout << headerMsg() << "stream name = '" << m_streamName << "'" << std::endl;

    const auto& sensorSpec = m_inputSensor->getSpec();

    const size_t acquisitionSize = sensorSpec.getResolution().size();
    std::cout << headerMsg() << "sensor name:'" << sensorSpec.getSensorName() << "'" << std::endl;
    std::cout << headerMsg() << "acquisitionSize:" << acquisitionSize << std::endl;
    std::cout << headerMsg() << "resolution:" << sensorSpec.getResolution() << std::endl;

    const auto& metaData = sensorSpec.getMetaData();
    for ( const auto& pair : metaData ) {
        std::cout << headerMsg() << "metaData: " << pair << std::endl;
        ////            mesh.printStats();
    }
    if ( metaData.find( "nAcq" ) != metaData.end() ) {
        std::cout << headerMsg() << "type detected : packed stream" << std::endl;
        m_nAcq           = metaData.at( "nAcq" ).get<int>();
        m_isPackedStream = true;
    }

    assert( m_server != nullptr );
    m_server->addStreamer( this );

    m_thread = std::thread( [this, sock = std::move( sock )]() mutable {
        try {

            while ( m_inputSensor->getInput().isOpen() ) {

                *m_inputSensor >> m_lastAcq;

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
        catch ( net::system::SocketSystem::exception& ex ) {
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

const sensor::InputSensor* StreamerClient::getInputSensor() const {
    assert( m_inputSensor != nullptr );
    return m_inputSensor.get();
}

sensor::Acquisition StreamerClient::getLastAcq() const {

    while ( !m_lastAcq.hasValue() ) {
        std::cout << "last acq empty" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    assert( m_lastAcq.hasValue() );
    auto acq = m_lastAcq.clone();

    return acq;
}

void StreamerClient::end( io::StreamBase::ServerMessage message ) {
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

const std::set<sensor::Acquisition>& StreamerClient::getPackedAcqs() const {
    int iTry = 0;
    while ( m_packedAcqs.size() != m_nAcq && iTry < 10 ) {
        std::cout << headerMsg() << "waiting for all packed acqs" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        ++iTry;
    }
    assert( iTry < 10 );
    assert( m_packedAcqs.size() == m_nAcq );
    return m_packedAcqs;
}

} // namespace impl
} // namespace server
} // namespace hub

#endif
