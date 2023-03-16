#pragma once

#include <thread>

#include "Client.hpp"
#include <OutputSensor.hpp>
#include <net/ClientSocket.hpp>

class StreamerClient;

class StreamViewerClient : public Client
{
  public:
    StreamViewerClient( Server* server, int iClient, hub::net::ClientSocket&& sock );
    ~StreamViewerClient();

    std::string headerMsg() const override;

    bool update( const hub::Acquisition& acq );

    const std::string& getSyncStreamName() const;
    const std::string& getStreamName() const;

    bool shoudMergeSyncAcqs() const;

  private:
    StreamerClient* m_streamer = nullptr;
    std::unique_ptr<hub::OutputSensor> m_outputSensor;
    std::mutex m_mtxOutputSensor;

    std::string m_streamName;
    std::string m_syncStreamName;
    bool m_mergeSyncAcqs;

    std::thread m_thread;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdateAcq;

    bool m_isKilled = false;
    bool m_suicide  = false;
};
