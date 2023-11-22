#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <string>
#include <thread>

//#include "sensor/Acquisition.hpp"
//#include "sensor/SensorSpec.hpp"
//#include "net/ClientSocket.hpp"
#include "client/ViewerInterface.hpp"
#include "impl/server2/io/input/InputStreamServer.hpp"

namespace hub {
namespace client {

class SRC_API ViewerServer : public ViewerInterface<input::InputStreamServer>
{
  public:
    explicit ViewerServer(
        const std::string & name,
        std::function<bool( const char* streamName, const sensor::SensorSpec& )> onNewStreamer = {},
        std::function<void( const char* streamName, const sensor::SensorSpec& )> onDelStreamer = {},
        std::function<void( const char* ipv4, int port )> onServerNotFound             = {},
        std::function<void( const char* ipv4, int port )> onServerConnected            = {},
        std::function<void( const char* ipv4, int port )> onServerDisconnected         = {},
        std::function<void( const char* streamName, const sensor::Acquisition& )> onNewAcquisition =
            {},
        std::function<
            void( const char* streamName, const char* objectName, int property, const Any& value )>
            onSetProperty                                          = {},
//        bool autoSync                                              = true,
        std::function<void( const char* logMessage )> onLogMessage = {},
        const std::string& ipv4                                    = input::InputStreamServer::s_defaultIpv4,
        int port                                                   = input::InputStreamServer::s_defaultPort
        );

    ~ViewerServer();

    void setIpv4( const std::string& ipv4 ) override;

    void setPort( int port ) override;

//    const std::string& getIpv4() const;

//    const int& getPort() const;

//    void setAutoSync( bool newAutoSync );

//    bool isConnected() const override;

//    void startStream( const std::string& streamName );

//    void stopStream( const std::string& streamName );

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value ) override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    std::thread m_thread;
    bool m_stopThread = false;

//    std::function<bool( const char* streamName, const SensorSpec& )> m_onNewStreamer;
//    std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
//    std::function<void( const char* ipv4, int port )> m_onServerNotFound;
//    std::function<void( const char* ipv4, int port )> m_onServerConnected;
//    std::function<void( const char* ipv4, int port )> m_onServerDisconnected;
//    std::function<void( const char* streamName, const Acquisition& )> m_onNewAcquisition;
//    std::function<
//        void( const char* streamName, const char* objectName, int property, const Any& value )>
//        m_onSetProperty;

//    net::ClientSocket m_sock;
    io::InputOutputSocket m_sock;

//    bool m_serverConnected = false;
//    std::function<void( const char* logMessage )> m_onLogMessage;

//    std::map<std::string, std::shared_ptr<viewer::Stream>> m_streams;

//    friend class viewer::Stream;

  private:
};

} // namespace client
} // namespace hub
