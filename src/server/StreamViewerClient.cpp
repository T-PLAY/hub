
#include <iomanip>

#include "Server.hpp"
#include "StreamViewerClient.hpp"

namespace hub {
namespace server {

class OutputStreamClient : public hub::Output
{
  public:
    explicit OutputStreamClient( hub::net::ClientSocket&& clientSocket ) :
        m_clientSocket( std::move( clientSocket ) ) {}

    OutputStreamClient( OutputStreamClient&& outputStream );
    ~OutputStreamClient();

  protected:
    void write( const hub::Acquisition& acq ) override;

    void write( const unsigned char* data, size_t len ) override;
    void close() override;
    bool isOpen() const override;

  private:
    hub::net::ClientSocket m_clientSocket;
    bool m_moved = false;

    friend class StreamViewerClient;
};

OutputStreamClient::OutputStreamClient( OutputStreamClient&& outputStream ) :
    m_clientSocket( std::move( outputStream.m_clientSocket ) ) {
    outputStream.m_moved = true;
}

OutputStreamClient::~OutputStreamClient() {
#ifdef DEBUG
    if ( !m_moved ) { assert( !isOpen() ); }
#endif
}

void OutputStreamClient::write( const hub::Acquisition& acq ) {
    hub::Output::write( hub::io::StreamInterface::ServerMessage::STREAM_VIEWER_NEW_ACQ );
    hub::Output::write( acq );
}

void OutputStreamClient::write( const unsigned char* data, size_t len ) {
    m_clientSocket.write( data, len );
}

void OutputStreamClient::close() {
    assert( isOpen() );
    m_clientSocket.write( hub::io::StreamInterface::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
    m_clientSocket.close();
}

bool OutputStreamClient::isOpen() const {
    return m_clientSocket.isOpen();
}

///////////////////////////////////////////////// StreamViewerClient /////////////////////

StreamViewerClient::StreamViewerClient( Server* server,
                                        int iClient,
                                        hub::net::ClientSocket&& sock,
                                        std::string streamName ) :
    Client( server, iClient ), m_streamName( std::move( streamName ) ) {

    m_server->addStreamViewer( this );

    m_thread = new std::thread( [this, sock = std::move( sock )]() mutable {
        try {
            const hub::InputSensor* inputSensor;
            while ( !m_ending &&
                    ( inputSensor = m_server->getInputSensor( m_streamName ) ) == nullptr ) {
                std::cout << headerMsg() << "waiting for inited inputSensor" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            }
            if ( m_ending ) {
                sock.write( hub::io::StreamInterface::ServerMessage::STREAM_VIEWER_CLOSED );
                sock.close();
                throw net::Socket::exception( "input stream closed" );
                //                std::thread( [this]() { delete this; } ).detach();
                //                return;
            }
            assert( inputSensor != nullptr );
            //            const auto & inputSensor = m_server->getInputSensor(m_streamName);
            //            const auto& sensorSpec = m_server->getSensorSpec( m_streamName );
            const auto& sensorSpec = inputSensor->getSpec();
            m_outputSensor         = std::make_unique<hub::OutputSensor>(
                sensorSpec, OutputStreamClient( std::move( sock ) ) );

            OutputStreamClient& outputStream =
                dynamic_cast<OutputStreamClient&>( m_outputSensor->getOutput() );

            hub::io::StreamInterface::ClientMessage message;
            // wait for client connection closed
            outputStream.m_clientSocket.read( message );
            assert( message == io::StreamInterface::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );

            std::cout << headerMsg() << "input stream closed by client " << std::endl;
        }
        catch ( hub::net::Socket::exception& ex ) {

            std::cout << headerMsg() << "catch exception : " << ex.what() << std::endl;
        }
        catch ( std::exception& ) {
            assert( false );
        }

        std::thread( [this]() { delete this; } ).detach();
    } );
}

StreamViewerClient::~StreamViewerClient() {
    std::cout << headerMsg() << "delete start" << std::endl;

    assert( m_thread != nullptr );
    //    if ( m_thread != nullptr ) {
    assert( m_thread->joinable() );
    m_thread->join();

    m_server->delStreamViewer( this );
    //    }
    std::cout << headerMsg() << "delete ended" << std::endl;
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewer] ";
}

void StreamViewerClient::update( const Acquisition& acq ) {
    *m_outputSensor << acq;
}

void StreamViewerClient::end( io::StreamInterface::ServerMessage message ) {

    m_ending = true;

    //    if ( m_outputSensor != nullptr ) {
    if ( m_outputSensor != nullptr ) {
        assert( m_outputSensor != nullptr );
        auto& output = m_outputSensor->getOutput();
        //    assert( output.isOpen() );
        if ( output.isOpen() ) output.write( message );
    }
    //    }
}

} // namespace server
} // namespace hub
