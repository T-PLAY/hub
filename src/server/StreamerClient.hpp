#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

#include <InputSensor.hpp>
#include <net/ClientSocket.hpp>

#include "Client.hpp"

namespace hub {
namespace server {

class Server;
class StreamViewerClient;

///
/// \brief The StreamerClient class
///
class StreamerClient : public Client
{
  public:
    StreamerClient( Server* server,
                    int iClient,
                    hub::net::ClientSocket&& sock,
                    std::string streamName );
    ~StreamerClient();

    std::string headerMsg() const override;

    const hub::InputSensor& getInputSensor() const;

    hub::Acquisition getLastAcq() const;

    void end( net::ClientSocket::Message message ) override;

    const std::string& getStreamName() const;
    bool isPackedStream() const;

    const std::set<hub::Acquisition>& getPackedAcqs() const;

  private:
    std::thread m_thread;

    std::unique_ptr<hub::InputSensor> m_inputSensor;
    std::string m_streamName;

    hub::Acquisition m_lastAcq;

    int m_nAcq            = 0;
    bool m_isPackedStream = false;
    std::set<hub::Acquisition> m_packedAcqs;
};

} // namespace server
} // namespace hub
