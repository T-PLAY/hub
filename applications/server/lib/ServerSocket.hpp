#pragma once

#include <Net/ClientSocket.hpp>

namespace hub {
namespace net {

class SRC_API ServerSocket : public Socket
{
  public:
    ServerSocket();
    ServerSocket( int port );

    ClientSocket waitNewClient();

  private:
    void initServer();

  private:
    int mPort;
    struct sockaddr_in mAddress;
};



} // namespace net
} // namespace hub
