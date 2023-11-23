#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "Client.hpp"
// #include "AskerClient.hpp"
#include "StreamViewerClient.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"

#include "io/StreamServer.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
// namespace server {

///
/// \brief The Server class
///
class SRC_API Server
{
  public:
    //    Server();
    ///
    /// \brief Server
    /// \param port
    ///
    explicit Server( int port = io::StreamServer::s_defaultPort );
    ~Server();

    ///
    /// \brief run
    ///
    void run();

    ///
    /// \brief asyncRun
    ///
    void asyncRun();

    ///
    /// \brief detach
    ///
    //    void detach();

    ///
    /// \brief setMaxClients
    /// \param maxClients
    ///
    //    void setMaxClients( int maxClients );

    void stop();
    void printStatus() const;
    void exitMainThread();
    //    void exit();

  private:
    std::string getStatus() const;
    std::string headerMsg() const;

    //    server::Client* initClient( net::ClientSocket&& sock, int iClient );
    server::Client* initClient( io::InputOutputSocket&& sock, int iClient );

    void addStreamer( server::StreamerClient* streamer );
    //    void newInputSensor(server::StreamerClient *streamer );
    //    void addStreamViewer( server::StreamViewerClient* streamViewer );
    void addViewer( server::ViewerClient* viewer );

    void delStreamer( server::StreamerClient* streamer );
    //    void delStreamViewer( server::StreamViewerClient* streamViewer );
    void delViewer( server::ViewerClient* viewer );

    //    void newAcquisition( const server::StreamerClient* streamer, const sensor::Acquisition&
    //    acq );

    //    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() const;
    //    sensor::Acquisition getAcquisition( const std::string& streamName ) const;

    void removeClient( server::Client* client );
    //    const std::map<std::string, server::StreamerClient*>& getStreamers() const;

    //    const SensorSpec& getSensorSpec( const std::string& streamName ) const;
    //    const sensor::InputSensor * getInputSensor( const std::string & streamName) const;

    //    void setProperty( const std::string& streamName,
    //                      const std::string& objectName,
    //                      int property,
    //                      const Any& value );

  private:
    std::thread* m_thread = nullptr;

    std::map<std::string, server::StreamerClient*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    //    std::map<std::string, std::list<server::StreamViewerClient*>> m_streamName2streamViewers;
    //    std::mutex m_mtxSreamName2streamViewers;

    std::list<server::ViewerClient*> m_viewers;
    std::mutex m_mtxViewers;

    net::ServerSocket m_serverSock;
    std::list<server::Client*> m_clients;
    std::mutex m_mtxClients;

    //    int m_iClient    = 0;
    int m_nActiveClient = 0;
    //    int m_maxClients = 1'000'000;

    std::mutex m_mtxPrint;

    //    bool m_detached = false;
    bool m_killed  = false;
    bool m_running = false;

    int m_givingPort;

    //    bool m_exiting = false;

    friend class server::Client;
    friend class server::StreamerClient;
    friend class server::StreamViewerClient;
    friend class server::ViewerClient;
    //    friend class server::AskerClient;
};

//} // namespace server
} // namespace hub
