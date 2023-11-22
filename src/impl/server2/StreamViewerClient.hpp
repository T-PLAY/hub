#pragma once

#include <thread>

#include "Client.hpp"
//#include "sensor/OutputSensor.hpp"
//#include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class StreamerClient;


///
/// \brief The StreamViewerClient class
///
class StreamViewerClient : public Client
{
  private:
//    StreamViewerClient( Server* server, int iClient, net::ClientSocket&& sock, std::string  streamName );
    StreamViewerClient( Server* server, int iClient, io::InputOutputSocket&& sock, std::string  streamName );
    ~StreamViewerClient();

    std::string headerMsg() const override;

//    void update( const sensor::Acquisition& acq );

    void end(io::StreamInterface::ServerMessage message ) override;

  private:
//    std::unique_ptr<sensor::OutputSensor> m_outputSensor;

    std::string m_streamName;

    std::thread * m_thread = nullptr;

//    bool m_ending = false;

    friend class hub::Server;
};

} // namespace server
} // namespace hub
