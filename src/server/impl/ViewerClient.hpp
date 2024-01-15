/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27
	
#pragma once

#ifdef HUB_USE_SERVER

#include <mutex>
#include <thread>

#include "Client.hpp"
// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace server {
namespace impl {

class StreamerClient;

///
/// \brief The ViewerClient class
///
class ViewerClient : public Client
{
  private:
    ViewerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

    void notifyNewStreamer( const std::string& streamName, const sensor::SensorSpec& sensorSpec );
    void notifyDelStreamer( const std::string& streamName, const sensor::SensorSpec& sensorSpec );

    void end( io::StreamBase::ServerMessage message ) override;
    void notifyProperty( const std::string& streamName,
                         const std::string& objectName,
                         int property,
                         const Any& value );

  private:
    std::thread m_thread;

    //    net::ClientSocket m_socket;
    io::InputOutputSocket m_socket;

    bool m_viewerClosed = false;

    friend class ServerImpl;
};

} // namespace impl
} // namespace server
} // namespace hub


#endif
