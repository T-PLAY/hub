#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

class OutputStream : public OutputInterface, public net::ClientSocket
{
  public:
    OutputStream( const std::string& streamName,
                  net::ClientSocket&& clientSocket = net::ClientSocket() );
};

class InputStream : public InputInterface, public net::ClientSocket
{
  public:
    InputStream( const std::string& streamName,
                 const std::string& syncStreamName = "",
                 net::ClientSocket&& clientSocket  = net::ClientSocket() );
};

} // namespace io
} // namespace hub
