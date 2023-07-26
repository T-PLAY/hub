#pragma once

#include <thread>

#include "Client.hpp"
#include "OutputSensor.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace server {

class StreamerClient;

///
/// \brief The StreamViewerClient class
///
class StreamViewerClient : public Client
{
  private:
    StreamViewerClient( Server* server, int iClient, hub::net::ClientSocket&& sock );
    ~StreamViewerClient();

    std::string headerMsg() const override;

    void update( const hub::Acquisition& acq );

    void end( net::ClientSocket::Message message ) override;

  private:
    std::unique_ptr<hub::OutputSensor> m_outputSensor;

    std::string m_streamName;

    std::thread m_thread;

    bool m_ending = false;

    friend class Server;
};

} // namespace server
} // namespace hub
