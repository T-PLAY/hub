/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30
	
#pragma once

#include "AskerInterface.hpp"

// #include "net/ClientSocket.hpp"
// #include "impl/server2/io/StreamServer.hpp"
#include "io/StreamServer2.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
// namespace impl2 {
namespace client {

class SRC_API AskerServer2 : public hub::client::AskerInterface
{
  public:
    AskerServer2( const std::string& ipv4 = HUB_SERVICE_IPV4,
                 int port                = HUB_SERVICE_PORT );
    ~AskerServer2();

    //  protected:
    std::list<std::pair<std::string, sensor::SensorSpec>> listStreams() override;
    sensor::Acquisition getAcquisition( const std::string& streamName ) override;

  private:
    //    net::ClientSocket m_sock;
    hub::io::InputOutputSocket m_sock;
};

} // namespace client
// } // namespace impl2
} // namespace hub
