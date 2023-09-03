
#include <iomanip>

#include "Server.hpp"
#include "StreamViewerClient.hpp"

namespace hub {
namespace server {

class OutputStreamClient : public hub::io::Output
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
    hub::io::Output::write( hub::net::ClientSocket::Message::NEW_ACQ );
    hub::io::Output::write( acq );
}

void OutputStreamClient::write( const unsigned char* data, size_t len ) {
    m_clientSocket.write( data, len );
}

void OutputStreamClient::close() {
    assert( isOpen() );
    m_clientSocket.write( hub::net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED );
    m_clientSocket.close();
}

bool OutputStreamClient::isOpen() const {
    return m_clientSocket.isOpen();
}

///////////////////////////////////////////////// StreamViewerClient /////////////////////

StreamViewerClient::StreamViewerClient(Server* server,
                                        int iClient,
                                        hub::net::ClientSocket&& sock , std::string streamName) :
    Client( server, iClient ),
    m_streamName(std::move(streamName))
{
    const auto& sensorSpec = m_server->getSensorSpec( m_streamName );
    m_outputSensor =
        std::make_unique<hub::OutputSensor>( sensorSpec, OutputStreamClient( std::move( sock ) ) );

    m_server->addStreamViewer( this );

    m_thread = new std::thread( [this]() {
        try {

            OutputStreamClient& outputStream =
                dynamic_cast<OutputStreamClient&>( m_outputSensor->getOutput() );

            hub::net::ClientSocket::Message message;
            // wait for client connection closed
            outputStream.m_clientSocket.read( message );
            assert( message == net::ClientSocket::Message::INPUT_STREAM_CLOSED );

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

    if ( m_thread != nullptr ) {
        assert( m_thread->joinable() );
        m_thread->join();

        m_server->delStreamViewer( this );
    }
    std::cout << headerMsg() << "delete ended" << std::endl;
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewer] ";
}

void StreamViewerClient::update( const Acquisition& acq ) {
    *m_outputSensor << acq;
}

void StreamViewerClient::end( net::ClientSocket::Message message ) {

    //    if ( m_outputSensor != nullptr ) {
    assert( m_outputSensor != nullptr );
    auto& output = m_outputSensor->getOutput();
    //    assert( output.isOpen() );
    if ( output.isOpen() ) output.write( message );
    //    }
}

} // namespace server
} // namespace hub
