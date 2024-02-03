/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#ifdef HUB_USE_SERVER

#    include <deque>
#    include <functional>
#    include <iomanip>
#    include <map>
#    include <memory>

#    include "AskerClient.hpp"
#    include "Client.hpp"
#    include "StreamViewerClient.hpp"
#    include "StreamerClient.hpp"
#    include "ViewerClient.hpp"

#    include "io/StreamServer.hpp"
#    include "net/ServerSocket.hpp"

namespace hub {
namespace server {

namespace impl {

///
/// \brief The ServerImpl class
///
class SRC_API ServerImpl
{
  public:
    ///
    /// \brief ServerImpl
    /// \param port
    ///
    explicit ServerImpl( int port = io::StreamServer::s_defaultPort );
    ~ServerImpl();

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

    void printStatus() const;

  private:
    std::string getStatus() const;
    std::string headerMsg() const;

    Client* initClient( io::InputOutputSocket&& sock, int iClient );

    void addStreamer( StreamerClient* streamer );
    void addStreamViewer( StreamViewerClient* streamViewer );
    void addViewer( ViewerClient* viewer );

    void delStreamer( StreamerClient* streamer );
    void delStreamViewer( StreamViewerClient* streamViewer );
    void delViewer( ViewerClient* viewer );

    void newAcquisition( const StreamerClient* streamer, const sensor::Acquisition& acq );

    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() const;
    sensor::Acquisition getAcquisition( const std::string& streamName ) const;

    void removeClient( Client* client );
    const std::map<std::string, StreamerClient*>& getStreamers() const;

    const sensor::InputSensor* getInputSensor( const std::string& streamName ) const;

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value );

  private:
    std::thread* m_thread = nullptr;

    std::map<std::string, StreamerClient*> m_streamName2streamer;
    mutable std::mutex m_mtxStreamName2streamer;

    std::map<std::string, std::list<StreamViewerClient*>> m_streamName2streamViewers;
    std::mutex m_mtxSreamName2streamViewers;

    std::list<ViewerClient*> m_viewers;
    std::mutex m_mtxViewers;

    net::ServerSocket m_serverSock;
    std::list<Client*> m_clients;
    std::mutex m_mtxClients;

    int m_nClient       = 0;
    int m_nActiveClient = 0;
    int m_maxClients    = 1'000'000;

    std::mutex m_mtxPrint;

    bool m_detached = false;
    bool m_killed   = false;
    bool m_running  = false;

    friend class Client;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};

} // namespace impl
} // namespace server
} // namespace hub

#endif
