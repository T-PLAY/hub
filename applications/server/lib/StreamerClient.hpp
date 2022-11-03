#pragma once

#include "Client.hpp"

#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>

#include <deque>
#include <memory>
#include <mutex>
#include <set>

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

    const std::shared_ptr<hub::Acquisition> getLastAcq( const std::string& streamName ) const;
    const std::map<long long, std::shared_ptr<hub::Acquisition>>&
    getSaveAcqs( const std::string& streamName ) const;

    void saveNewAcq( const std::string& streamName, hub::Acquisition&& newAcq );

  public:
    mutable std::mutex m_mtxLastAcq;
    mutable std::mutex m_mtxSaveAcqs;

  private:
    std::mutex m_mtx;
    std::unique_ptr<hub::InputSensor> m_inputSensor;
    std::string m_streamName;

    std::map<std::string, std::list<StreamViewerClient*>> m_syncViewers;
    std::mutex m_mtxSyncViewers;

    std::map<std::string, std::list<hub::Acquisition>> m_syncAcqs;
    std::map<std::string, bool> m_isSyncthing;
    std::mutex m_mtxSyncAcqs;

    std::map<std::string, std::shared_ptr<hub::Acquisition>> m_lastAcq;
    std::map<std::string, std::map<long long, std::shared_ptr<hub::Acquisition>>>
        m_streamName2saveAcqs;

    static std::mutex s_mtxCout;
//    bool m_isRecordStream = false;
};
