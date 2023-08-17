#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "AskerClient.hpp"
#include "StreamViewerClient.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
//namespace server {

using namespace server;

///
/// \brief The Server class
///
class SRC_API Server
{
  public:
    Server();
    ///
    /// \brief Server
    /// \param port
    ///
    explicit Server( int port );
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
    void detach();

    ///
    /// \brief setMaxClients
    /// \param maxClients
    ///
    void setMaxClients( int maxClients );


  private:
    std::string headerMsg() const;

    Client* initClient( hub::net::ClientSocket&& sock, int iClient );
    std::string getStatus();

    void addStreamer( StreamerClient* streamer );
    void addStreamViewer( StreamViewerClient* streamViewer );
    void addViewer( ViewerClient* viewer );

    void delStreamer( StreamerClient* streamer );
    void delStreamViewer( StreamViewerClient* streamViewer );
    void delViewer( ViewerClient* viewer );

    void newAcquisition( const StreamerClient* streamer, const Acquisition& acq );

    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() const;
    hub::Acquisition getAcquisition( const std::string& streamName ) const;

    void removeClient( Client* client );
    const std::map<std::string, StreamerClient*>& getStreamers() const;

    const hub::SensorSpec& getSensorSpec( const std::string& streamName ) const;

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value );

  private:
    std::thread m_thread;

    std::map<std::string, StreamerClient*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    std::map<std::string, std::list<StreamViewerClient*>> m_streamName2streamViewers;
    std::mutex m_mtxSreamName2streamViewers;

    std::list<ViewerClient*> m_viewers;
    std::mutex m_mtxViewers;

    hub::net::ServerSocket m_serverSock;
    std::list<Client*> m_clients;
    std::mutex m_mtxClients;

    int m_nClient    = 0;
    int m_maxClients = 1'000'000;

    std::mutex m_mtxPrint;

    bool m_detached = false;

    friend class server::Client;
    friend class server::StreamerClient;
    friend class server::StreamViewerClient;
    friend class server::ViewerClient;
    friend class server::AskerClient;
};

//} // namespace server
} // namespace hub
