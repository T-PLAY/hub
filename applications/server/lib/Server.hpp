#pragma once

#include <deque>
#include <functional>
#include <map>
#include <mutex>
//#include <semaphore.h>
//#include <semaphore>

#include <Acquisition.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include "ServerSocket.hpp"
#include <memory>
//#include "Client.hpp"
#include "StreamViewerClient.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"

constexpr int g_margin  = 30;
constexpr int g_margin2 = 40;

class Server
{
  public:
    Server();
    Server( int port );
    ~Server();

    std::string headerMsg() const;

    void run();
    void asyncRun();
    void stop();

    Client* initClient( hub::net::ClientSocket&& sock, int iClient );
    std::string getStatus();

    void addStreamer( StreamerClient* streamer );
    void addStreamViewer( StreamViewerClient* streamViewer );
    void addViewer( ViewerClient* viewer );

    void delStreamer( StreamerClient* streamer );
    void delStreamViewer( StreamViewerClient* streamViewer );
    void delViewer( ViewerClient* viewer );

    void newAcquisition( StreamerClient* streamer, hub::Acquisition acq );
    const std::vector<std::shared_ptr<hub::Acquisition>>&
    getLastAcqs( const std::string& streamName );

  public:
    bool m_acqPing = true;

  private:
    std::map<std::string, StreamerClient*> m_streamers;
    std::mutex m_mtxStreamers;

    std::list<ViewerClient*> m_viewers;
    std::map<std::string, std::list<StreamViewerClient*>> m_streamViewers;

  private:
    hub::net::ServerSocket mServerSock;
    std::list<Client*> m_clients;

    std::thread m_thread;
    int m_maxClients = 1000;

  public:
    void setMaxClients( int maxThreads );
    const std::map<std::string, StreamerClient*>& getStreamers() const;
    void setAcqPing( bool newAcqPing );
};


/////////////////////////// old ///////////////////////////////

// class StreamViewerClient : public io::InputInterface, public net::ClientSocket
//{
//   public:
//     StreamViewerClient( net::ClientSocket&& clientSocket ); // server side
// };

// struct Streamer {
//     hub::InputSensor mInputSensor;
//     std::string mSensorName;

//    std::list<hub::OutputSensor> mOutputSensors;
//    std::map<std::string, std::list<hub::OutputSensor>> mSensor2syncViewers;

//    Streamer* mSyncMaster = nullptr;
//    std::map<std::string, std::deque<hub::Acquisition>> mSensor2acqs;
//};

// struct Viewer {
//     const hub::net::ClientSocket* const mSock = nullptr;

//    void notifyNewStreamer( const Streamer& streamer ) const;
//};
