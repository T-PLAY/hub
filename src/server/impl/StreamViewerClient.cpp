
#include "ServerImpl.hpp"

#ifdef HUB_USE_SERVER

#    include "StreamViewerClient.hpp"
#    include <iomanip>

namespace hub {
namespace server {
namespace impl {

class OutputStreamClient : public Output
{
  public:
    explicit OutputStreamClient( io::InputOutputSocket&& clientSocket ) :
        m_clientSocket( std::move( clientSocket ) ) {}

    OutputStreamClient( OutputStreamClient&& outputStream );
    ~OutputStreamClient();

  protected:
    void write( const sensor::Acquisition& acq );

    void write( const Data_t* data, Size_t len ) override;
    void close() override;
    bool isOpen() const override;
    void setRetain( bool retained ) override;

  private:
    io::InputOutputSocket m_clientSocket;
    bool m_moved = false;

    friend class StreamViewerClient;
};

OutputStreamClient::OutputStreamClient( OutputStreamClient&& outputStream ) :
    m_clientSocket( std::move( outputStream.m_clientSocket ) ) {
    outputStream.m_moved = true;
}

OutputStreamClient::~OutputStreamClient() {
#    ifdef DEBUG
    if ( !m_moved ) { assert( !isOpen() ); }
#    endif
}

void OutputStreamClient::write( const sensor::Acquisition& acq ) {
    Output::write( io::StreamBase::ServerMessage::STREAM_VIEWER_NEW_ACQ );
    Output::write( acq );
}

void OutputStreamClient::write( const Data_t* data, Size_t len ) {
    m_clientSocket.write( data, len );
}

void OutputStreamClient::close() {
    assert( isOpen() );
    m_clientSocket.write( io::StreamBase::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
    m_clientSocket.close();
}

bool OutputStreamClient::isOpen() const {
    return m_clientSocket.isOpen();
}

void OutputStreamClient::setRetain( bool retained ) {}

///////////////////////////////////////////////// StreamViewerClient /////////////////////

StreamViewerClient::StreamViewerClient( ServerImpl* server,
                                        int iClient,
                                        io::InputOutputSocket&& sock,
                                        std::string streamName ) :
    Client( server, iClient ), m_streamName( std::move( streamName ) ) {

    const auto& inputSensor = m_server->getInputSensor( m_streamName );
    const auto& sensorSpec  = inputSensor->getSpec();
    m_outputSensor          = std::make_unique<sensor::OutputSensor>(
        sensorSpec, OutputStreamClient( std::move( sock ) ) );

    m_server->addStreamViewer( this );

    m_thread = new std::thread( [this]() {
        try {
            OutputStreamClient& outputStream =
                dynamic_cast<OutputStreamClient&>( m_outputSensor->getOutput() );

            io::StreamBase::ClientMessage message;
            outputStream.m_clientSocket.read( message );
            assert( message == io::StreamBase::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );

            std::cout << headerMsg() << "input stream closed by client " << std::endl;
        }
        catch ( net::system::SocketSystem::exception& ex ) {

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
    assert( m_thread->joinable() );
    m_thread->join();

    m_server->delStreamViewer( this );
    std::cout << headerMsg() << "delete ended" << std::endl;
}

std::string StreamViewerClient::headerMsg() const {
    return Client::headerMsg() + "[StreamViewer] ";
}

void StreamViewerClient::update( const sensor::Acquisition& acq ) {
    assert( m_outputSensor != nullptr );
    *m_outputSensor << acq;
}

void StreamViewerClient::end( io::StreamBase::ServerMessage message ) {

    m_ending = true;

    assert( m_outputSensor != nullptr );
    auto& output = m_outputSensor->getOutput();
    if ( output.isOpen() ) output.write( message );
}

} // namespace impl
} // namespace server
} // namespace hub

#endif
