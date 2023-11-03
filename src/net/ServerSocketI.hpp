#pragma once

#include "core/Macros.hpp"
#include "ClientSocketI.hpp"

namespace hub {
namespace net {

class SRC_API ServerSocketI
{
//  public:
//    explicit ServerSocketI(int port);
  public:
    virtual int getPort() const = 0;
    ClientSocketI waitNewClient();
};

} // namespace net
} // namespace hub
