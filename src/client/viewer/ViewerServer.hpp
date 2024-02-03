/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#ifdef HUB_USE_SERVER

#    include <functional>
#    include <mutex>
#    include <regex>
#    include <string>
#    include <thread>

#    include "ViewerInterface.hpp"
#    include "io/input/InputStreamServer.hpp"

namespace hub {
namespace client {

class SRC_API ViewerServer : public ViewerInterface<input::InputStreamServer>
{
  public:
    explicit ViewerServer( const std::string& name,
                           ViewerHandler&& viewerHandler,
                           const std::string& ipv4 = input::InputStreamServer::s_defaultIpv4,
                           int port                = input::InputStreamServer::s_defaultPort );

    ~ViewerServer();

    void setIpv4( const std::string& ipv4 ) override;

    void setPort( int port ) override;

    void setProperty( const std::string& streamName,
                      const std::string& objectName,
                      int property,
                      const Any& value ) override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

  private:
    std::thread m_thread;
    bool m_stopThread = false;

    io::InputOutputSocket m_sock;

  private:
};

} // namespace client
} // namespace hub

#endif
