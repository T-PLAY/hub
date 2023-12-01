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

class SRC_API ViewerServer2 : public hub::client::ViewerInterface<input::InputStreamServer2>
{
  public:
    explicit ViewerServer2( const std::string& name,
                            ViewerHandler&& viewerHandler,
                            const std::string& ipv4 = input::InputStreamServer2::s_defaultIpv4,
                            int port                = input::InputStreamServer2::s_defaultPort );

    ~ViewerServer2();

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

    hub::io::InputOutputSocket m_sock;
    //    std::map<std::string, std::shared_ptr<viewer::Stream>> m_streams;
    //    friend class viewer::Stream;

  private:
};

} // namespace client
} // namespace hub
