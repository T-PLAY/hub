#pragma once

#include "ViewerInterface.hpp"
#include "io/input/InputStreamMqtt.hpp"

namespace hub {
namespace client {


class SRC_API ViewerMqtt : public ViewerInterface<input::InputStreamMqtt>
{
  public:
    ViewerMqtt(
        const std::string & name,
        std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer = {},
        std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer = {},
        std::function<void( const char* ipv4, int port )> onServerNotFound             = {},
        std::function<void( const char* ipv4, int port )> onServerConnected            = {},
        std::function<void( const char* ipv4, int port )> onServerDisconnected         = {},
        std::function<void( const char* streamName, const hub::sensor::Acquisition& )> onNewAcquisition =
            {},
        std::function<
            void( const char* streamName, const char* id, int property, const Any& value )>
            onSetProperty                                          = {},
//        bool autoSync                                              = true,
        std::function<void( const char* logMessage )> onLogMessage = {},
        const std::string & ipv4 = input::InputStreamMqtt::s_defaultIpv4,
        int port = input::InputStreamMqtt::s_defaultPort
        );



    ~ViewerMqtt();

//    void routine() override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
//    std::string m_hostName;


    std::thread m_thread;
    bool m_stopThread = false;
//    std::unique_ptr<mqtt::async_client> m_client;
    std::unique_ptr<mqtt::client> m_client;
    mqtt::const_message_ptr m_inputMsgPtr;
    mqtt::message_ptr m_outputMsgPtr;

};

} // namespace client
} // namespace hub
