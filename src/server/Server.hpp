#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <net/ServerSocket.hpp>

#include "AskerClient.hpp"
#include "StreamViewerClient.hpp"
#include "StreamerClient.hpp"
#include "ViewerClient.hpp"

namespace hub {
namespace server {

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
    /// \brief stop
    /// \warning need async running
    ///
    void stop();

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
    void addViewer( ViewerClient* viewer );

    void delStreamer( StreamerClient* streamer );
    void delViewer( ViewerClient* viewer );

    void newAcquisition( StreamerClient* streamer, const hub::Acquisition& acq );

    std::list<std::pair<std::string, hub::SensorSpec>> listStreams() const;
    const std::shared_ptr<hub::Acquisition> getAcquisition( const std::string& streamName );

    void removeClient( Client* client );
    const std::map<std::string, StreamerClient*>& getStreamers() const;

  private:
    std::thread m_thread;

    std::map<std::string, StreamerClient*> m_streamers;
    std::mutex m_mtxStreamers;

    std::list<ViewerClient*> m_viewers;

    hub::net::ServerSocket m_serverSock;
    std::list<Client*> m_clients;

    int m_maxClients = 1'000'000;

//  private:
    friend class Client;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};


} // server
} // hub