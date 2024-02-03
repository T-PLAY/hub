/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/23

#pragma once

#ifdef HUB_USE_SERVER

#    include <string>

#    include "InputOutputSocket.hpp"
#    include "StreamBase.hpp"

namespace hub {
namespace io {

class StreamServer : public StreamBase
{
  public:
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;

  protected:
    StreamServer( const std::string& name, const std::string& ipv4, int port );
    StreamServer( const char* name, const char* ipv4, int port );
};

} // namespace io
} // namespace hub

#endif
