#pragma once

#include "core/Macros.hpp"

namespace hub {
namespace net {

class SRC_API ClientSocketInterface
{
  public:
    virtual void setIpv4( const std::string& newIpv4 ) = 0;
    virtual const std::string& getIpv4() const         = 0;

    virtual void setPort( int newPort ) = 0;
    virtual int getPort() const  = 0;

    virtual void connect()           = 0;
    virtual bool isConnected() const = 0;
    virtual void disconnect()        = 0;

    virtual void write( const unsigned char* data, size_t len ) = 0;
    virtual void read( unsigned char* data, size_t len )        = 0;
};

} // namespace net
} // namespace hub
