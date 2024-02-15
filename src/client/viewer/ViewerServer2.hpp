/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <string>
#include <thread>

#include "ViewerInterface.hpp"
#include "io/input/InputStreamServer2.hpp"

namespace hub {
namespace client {

class ViewerServer2 : public hub::client::ViewerInterface<input::InputStreamServer2>
{
  public:
    SRC_API explicit ViewerServer2( const std::string& name,
                                    ViewerHandler&& viewerHandler,
                                    const std::string& serverIpv4 = HUB_SERVICE_IPV4,
                                    int serverPort                = HUB_SERVICE_PORT,
                                    bool autoConnect        = true );

    SRC_API ~ViewerServer2();

    void setIpv4( const std::string& serverIpv4 ) override;

    void setPort( int serverPort ) override;

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value ) override;

    void setAutoConnect( bool autoConnect ) override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    void threadRoutine();

    std::thread m_thread;
    bool m_threadRunning = false;
    bool m_stopThread    = false;

    hub::io::InputOutputSocket m_sock;

  private:
};

} // namespace client
} // namespace hub
