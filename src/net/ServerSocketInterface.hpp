#pragma once

#include "core/Macros.hpp"
#include "ClientSocketInterface.hpp"

namespace hub {
namespace net {

class SRC_API ServerSocketInterface
{
//  public:
//    explicit ServerSocketInterface(int port);
  public:
    virtual int getPort() const = 0;
    ClientSocketInterface waitNewClient();
};

} // namespace net
} // namespace hub
