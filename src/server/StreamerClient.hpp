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
//  public:
//    protected:
private:
StreamerClient( Server* server, int iClient, hub::net::ClientSocket&& sock, std::string streamName );
    ~StreamerClient();

    std::string headerMsg() const override;

    const hub::InputSensor& getInputSensor() const;

//    void addStreamViewer( StreamViewerClient* streamViewer );
//    void delStreamViewer( StreamViewerClient* streamViewer );
//    void newAcquisition( const std::string& streamerName, const hub::Acquisition& acq );

//    const std::shared_ptr<hub::Acquisition> getLastAcq( const std::string& streamName ) const;
//    const std::map<long long, std::shared_ptr<hub::Acquisition>>&
//    getSaveAcqs( const std::string& streamName ) const;

//    const std::chrono::time_point<std::chrono::high_resolution_clock>&
//    getLastUpdateAcqDate( const std::string& streamName ) const;

//  private:
//    void saveNewAcq( const std::string& streamName, hub::Acquisition&& newAcq );

//    const std::list<StreamViewerClient*>& getStreamViewers() const; // use by server
//    const std::map<std::string, std::list<StreamViewerClient*>>&
//    getSyncViewers() const; // use by server (printStatus)
//    const std::string& getStreamName() const;
//    const std::string& getParent() const;
//    bool isRecordStream() const;

    void end(net::ClientSocket::Message message) override;

  private:
    std::thread m_thread;

    std::unique_ptr<hub::InputSensor> m_inputSensor;
    std::string m_streamName;
//    std::string m_parent = "";

//    std::list<StreamViewerClient*> m_streamViewers;
//    std::mutex m_mtxStreamViewers;

//    std::map<std::string, std::list<StreamViewerClient*>> m_syncViewers;
//    std::mutex m_mtxSyncViewers;

//    std::map<std::string, std::list<hub::Acquisition>> m_syncAcqs;
//    std::map<std::string, bool> m_isSyncthing;
//    std::map<std::string, bool> m_isLooping;
//    std::mutex m_mtxSyncAcqs;

    hub::Acquisition m_lastAcq;

//    std::map<std::string, std::shared_ptr<hub::Acquisition>> m_lastAcq;

//    std::map<std::string, std::map<long long, std::shared_ptr<hub::Acquisition>>>
//        m_streamName2saveAcqs;

    bool m_isPackedStream = false;
    int m_nAcq = 0;
    std::set<hub::Acquisition> m_packedAcqs;

//  public:
//  private:
//  protected:

    friend class Server;
//    friend class ViewerClient;
//    friend class AskerClient;
//    friend class StreamViewerClient;
};


} // server
} // hub
