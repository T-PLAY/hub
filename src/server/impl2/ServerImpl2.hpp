#pragma once

#include <deque>
#include <functional>
#include <iomanip>
#include <map>
#include <memory>

#include "Client2.hpp"
// #include "AskerClient.hpp"
#include "StreamViewerClient2.hpp"
#include "StreamerClient2.hpp"
#include "ViewerClient2.hpp"

// #include "io/StreamServer.hpp"
// #include "io/Stream.hpp"
#include "io/StreamServer2.hpp"
#include "net/ServerSocket.hpp"

namespace hub {
namespace server {

// static void stopServer( std::string ipv4, int port );
namespace impl2 {

///
/// \brief The ServerImpl2 class
///
class SRC_API ServerImpl2
{
  public:
    //    ServerImpl2();
    ///
    /// \brief ServerImpl2
    /// \param port
    ///
    // explicit ServerImpl2( int port = io::Stream::s_defaultPort );
    explicit ServerImpl2( int port = io::StreamServer2::s_defaultPort );
    // explicit ServerImpl2( int port = HUB_SERVICE_PORT );
    ~ServerImpl2();

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
    bool running() const;
    //    void exit();

  private:
    void exitMainThread();

    std::string getStatus() const;
    std::string headerMsg() const;

    //    server::Client2* initClient( net::ClientSocket&& sock, int iClient );
    Client2* initClient( hub::io::InputOutputSocket&& sock, int iClient );

    void addStreamer( StreamerClient2* streamer );
    void newStreamViewer( StreamerClient2* streamer );
    //    void newInputSensor(server::StreamerClient2 *streamer );
    //    void addStreamViewer( server::StreamViewerClient* streamViewer );
    void addViewer( ViewerClient2* viewer );

    void delStreamer( StreamerClient2* streamer );
    //    void delStreamViewer( server::StreamViewerClient* streamViewer );
    void delViewer( ViewerClient2* viewer );

    //    void newAcquisition( const server::StreamerClient2* streamer, const sensor::Acquisition&
    //    acq );

    //    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() const;
    //    sensor::Acquisition getAcquisition( const std::string& streamName ) const;

    void removeClient( Client2* client );
    //    const std::map<std::string, server::StreamerClient2*>& getStreamers() const;

    //    const SensorSpec& getSensorSpec( const std::string& streamName ) const;
    //    const sensor::InputSensor * getInputSensor( const std::string & streamName) const;

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

    //    int m_iClient    = 0;
    int m_nActiveClient = 0;
    //    int m_maxClients = 1'000'000;

    std::mutex m_mtxPrint;

    //    bool m_detached = false;
    bool m_killed  = false;
    bool m_running = false;

    int m_givingPort;

    //    bool m_exiting = false;

    friend class Client2;
    friend class StreamerClient2;
    friend class StreamViewerClient2;
    friend class ViewerClient2;
    //    friend class server::AskerClient;
};

} // namespace impl2
} // namespace server
} // namespace hub
