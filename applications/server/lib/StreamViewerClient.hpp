#pragma once

#include "Client.hpp"
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

class StreamViewerClient : public Client
{
  public:
    StreamViewerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~StreamViewerClient();

    std::string headerMsg() const override;

    void update( const hub::Acquisition& acq );

    const std::string& getSyncStreamName() const;
    const std::string& getStreamName() const;
    void killThread();

  private:
    std::unique_ptr<hub::OutputSensor> m_outputSensor;
    std::mutex m_mtxOutputSensor;
    std::string m_streamName;

    std::string m_syncStreamName;

    std::thread* m_thread = nullptr;
    bool m_isKilled       = false;
    bool m_updateFailed   = false;

};
