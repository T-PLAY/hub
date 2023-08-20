#pragma once

#include "ViewerInterface.hpp"

namespace hub {
namespace client {


class SRC_API ViewerMqtt : public ViewerInterface<input::InputStreamMqtt>
{
  public:
    ViewerMqtt(
        const std::string & ipv4,
        int port,
        std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer = {},
        std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer = {},
        std::function<void( const char* ipv4, int port )> onServerNotFound             = {},
        std::function<void( const char* ipv4, int port )> onServerConnected            = {},
        std::function<void( const char* ipv4, int port )> onServerDisconnected         = {},
        std::function<void( const char* streamName, const hub::Acquisition& )> onNewAcquisition =
            {},
        std::function<
            void( const char* streamName, const char* id, int property, const Any& value )>
            onSetProperty                                          = {},
//        bool autoSync                                              = true,
        std::function<void( const char* logMessage )> onLogMessage = {} );


    ~ViewerMqtt();

    void routine() override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:

  private:
};

} // namespace client
} // namespace hub
