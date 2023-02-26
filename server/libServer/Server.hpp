#pragma once

#include <deque>
#include <functional>
#include <map>
// #include <mutex>
#include <iomanip>
#include <memory>

#include <InputSensor.hpp>
#include <Net/ServerSocket.hpp>
#include <OutputSensor.hpp>

#include "StreamViewerClient.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"
#include "AskerClient.hpp"

class Server
{
  public:
    Server();
    explicit Server( int port );
    ~Server();

    std::string headerMsg() const;

    void run();
    void asyncRun();
    void stop();

    Client* initClient( hub::net::ClientSocket&& sock, int iClient );
    std::string getStatus();

    void addStreamer( StreamerClient* streamer );
    //    void addStreamViewer( StreamViewerClient* streamViewer );
    void addViewer( ViewerClient* viewer );

    void delStreamer( StreamerClient* streamer );
    //    void delStreamViewer( StreamViewerClient* streamViewer );
    void delViewer( ViewerClient* viewer );

    void newAcquisition( StreamerClient* streamer, const hub::Acquisition& acq );

    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() const;
    const hub::SensorSpec & getSensorSpec(const std::string & streamName);
    const std::shared_ptr<hub::Acquisition> getAcquisition(const std::string & streamName);

    void removeClient(const Client * client);

  private:
    //    bool m_acqPing = true;
    std::thread m_thread;

    std::map<std::string, StreamerClient*> m_streamers;
    std::mutex m_mtxStreamers;

    std::list<ViewerClient*> m_viewers;
    //    std::mutex m_mtxViewers;

    //    std::map<std::string, std::list<StreamViewerClient*>> m_streamViewers;

    hub::net::ServerSocket m_serverSock;
    std::list<const Client*> m_clients;

    int m_maxClients = 1'000'000;

  public:
    void setMaxClients( int maxClients );
    const std::map<std::string, StreamerClient*>& getStreamers() const;
    //    void setAcqPing( bool newAcqPing );
    //    bool getAcqPing() const;
};
