#pragma once

#include <functional>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "Configurations.hpp"
#include "SensorSpec.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {

///
/// \brief The Viewer class (event dispatcher)
/// can be use by client application to be aware of the events comming from server
/// client (user of this class) connection/deconnection to the server,
/// streamer connected/disconnected to the server,
/// new acquisition sended from a connected streamer.
/// Example of use : open input stream in your application (client side)
/// when new streamer is connected to server to have an interactive application.
/// Close the input stream when server or streamer are disconnected.
///
class SRC_API Viewer
{
  public:
    Viewer( std::function<bool( const char* streamName, const SensorSpec&, const char * syncStreamName )> onNewStreamer = {},
            std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer = {},
            std::function<void( const char* ipv4, int port )> onServerConnected    = {},
            std::function<void( const char* ipv4, int port )> onServerDisconnected = {},
            std::function<void( const char* streamName, const hub::Acquisition& )>
                onNewAcquisition    = {},
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port                = net::s_defaultServicePort );

    ~Viewer();

    void setIpv4( const std::string& ipv4 );
    void setPort( int port );

    const std::string & getIpv4() const;
    const int & getPort() const;

    ////////////////////////////////////////////////////////////////////////////////

  protected:
    std::thread m_thread;
    bool m_stopThread = false;

    std::function<bool( const char* streamName, const SensorSpec&, const char * syncStreamName )> m_onNewStreamer;
    std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void( const char* ipv4, int port )> m_onServerConnected;
    std::function<void( const char* ipv4, int port )> m_onServerDisconnected;
    std::function<void( const char* streamName, const hub::Acquisition& )> m_onNewAcquisition;

    net::ClientSocket m_sock;
//    std::string m_ipv4;
//    int m_port;
    bool m_serverConnected = false;
//    std::regex m_ipv4Regex;

    class Stream {
    public:
        Viewer & m_viewer;

//        Stream(Stream & stream) = default;
        Stream(Viewer & viewer) : m_viewer(viewer)
        {
        }

        Stream(Stream && stream) :
            m_viewer(stream.m_viewer),
            m_streamName(std::move(stream.m_streamName)),
            m_sensorSpec(std::move(stream.m_sensorSpec)),
            m_syncStreamName(std::move(stream.m_syncStreamName)),
            m_streamId(std::move(stream.m_streamId)),
            m_thread(stream.m_thread),
            m_stopThread(stream.m_stopThread)
        {
            stream.m_thread = nullptr;
        }

        ~Stream() {
            if (m_thread != nullptr) {
                stopStream();
            }
        }

        std::string m_streamName;
        SensorSpec m_sensorSpec;
        std::string m_syncStreamName;
        std::string m_streamId;

        std::thread * m_thread = nullptr;
        bool m_stopThread = true;

        void startStream();
        void stopStream();
//        friend class Viewer;
    };

    std::map<std::string, std::unique_ptr<Stream>> m_streams;

//    std::map<std::string, SensorSpec> m_streamName2sensorSpec;
//    std::map<std::string, std::thread> m_streamName2thread;
//    std::map<std::string, bool> m_streamName2stopThread;
    friend class Stream;

  private:
};

} // namespace hub
