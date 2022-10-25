#pragma once

#include "Client.hpp"

#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>

#include <mutex>
#include <memory>
#include <deque>

class Server;
class StreamViewerClient;

class StreamerClient : public Client
{
  public:
    StreamerClient( Server& server, int iClient, hub::net::ClientSocket&& sock );
    ~StreamerClient();

    std::string headerMsg() const override;

    const hub::InputSensor& getInputSensor() const;

    void addSyncStreamViewer( StreamViewerClient* streamViewer );
    void delStreamViewer( StreamViewerClient* streamViewer );

    void newAcquisition( const std::string& streamerName, const hub::Acquisition& acq );

    const std::string& getStreamName() const;

    const std::map<std::string, std::list<StreamViewerClient*>>& getSyncViewers() const;

    const std::vector<std::shared_ptr<hub::Acquisition>> & getLastAcqs(const std::string & streamName) const;

public:
    mutable std::mutex m_mtxLastAcqs;
  private:
    std::mutex m_mtx;
    std::unique_ptr<hub::InputSensor> m_inputSensor;
    std::string m_streamName;

    std::map<std::string, std::list<StreamViewerClient*>> m_syncViewers;
    std::mutex m_mtxSyncViewers;

    std::map<std::string, std::deque<hub::Acquisition>> m_syncAcqs;
    std::mutex m_mtxSyncAcqs;

    std::map<std::string, std::vector<std::shared_ptr<hub::Acquisition>>> m_lastAcqs;

    bool m_isRecordStream = false;

};
