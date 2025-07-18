/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <mutex>
#include <string>

#include "io/StreamBase.hpp"

namespace hub {
namespace server {

class ServerImpl;

///
/// \brief The Client class
/// represents client from server side.
/// All member are private because useless for user.
/// User should only use server class
///
class SRC_API Client
{
  private:
    Client( ServerImpl* server, int iClient );
    virtual ~Client();

    std::string clientMsg() const;
    virtual std::string headerMsg() const = 0;
    void printStatusMessage( const std::string& message ) const;

    void setServer( ServerImpl* newServer );

    virtual void end( hub::io::StreamBase::ServerMessage message ) = 0;
    virtual void notifyInited()                                    = 0;

  private:
    ServerImpl* m_server = nullptr;
    int m_iClient;

    friend class ServerImpl;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};

} // namespace server
} // namespace hub
