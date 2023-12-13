#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "Client2.hpp"
// #include "StreamViewerClient2.hpp"
#include "StreamerClient2.hpp"
#include "ViewerClient2.hpp"

#include "io/StreamServer2.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
namespace server {

static std::mutex m_mtxPrint;

///
/// \brief The ServerImpl2 class
///
class SRC_API ServerImpl2
{
  public:
    ///
    /// \brief ServerImpl2
    /// \param port
    ///
    explicit ServerImpl2( int port = HUB_SERVICE_PORT );

    ~ServerImpl2();

    ///
    /// \brief run
    ///
    void run();

    ///
    /// \brief asyncRun
    ///
    void asyncRun();

    void stop();
    void printStatus() const;
    bool running() const;

    int nStreamer() const;
    int nClient() const;

  private:
    void exitMainThread();

    std::string getStatus() const;
    std::string headerMsg() const;

    Client2* initClient( hub::io::InputOutputSocket&& sock, int iClient );

    void addStreamer( StreamerClient2* streamer );
    void addStreamViewer( StreamerClient2* streamer );
    void delStreamViewer( StreamerClient2* streamer );

    //    void addStreamViewer( server::StreamViewerClient* streamViewer );
    void addViewer( ViewerClient2* viewer );

    void delStreamer( StreamerClient2* streamer );
    //    void delStreamViewer( server::StreamViewerClient* streamViewer );
    void delViewer( ViewerClient2* viewer );

    //    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() const;
    //    sensor::Acquisition getAcquisition( const std::string& streamName ) const;

    void removeClient( Client2* client );
    //    const std::map<std::string, server::StreamerClient2*>& getStreamers() const;

    //    void setProperty( const std::string& streamName,
    //                      const std::string& objectName,
    //                      int property,
    //                      const Any& value );

  private:
    std::thread* m_thread = nullptr;

    std::map<std::string, StreamerClient2*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    //    std::map<std::string, std::list<server::StreamViewerClient*>> m_streamName2streamViewers;
    //    std::mutex m_mtxSreamName2streamViewers;

    std::list<ViewerClient2*> m_viewers;
    std::mutex m_mtxViewers;

    net::ServerSocket m_serverSock;
    std::list<Client2*> m_clients;
    std::mutex m_mtxClients;

    int m_iClient       = 0;
    int m_nActiveClient = 0;
    //    int m_maxClients = 1'000'000;

    bool m_killed  = false;
    bool m_running = false;

    int m_givingPort;

    friend class Client2;
    friend class StreamerClient2;
    friend class StreamViewerClient2;
    friend class ViewerClient2;
    //    friend class server::AskerClient;
};

} // namespace server
} // namespace hub
