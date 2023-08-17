#pragma once

#include <string>

namespace hub {
namespace io {

class Stream
{
  public:
    Stream( const std::string& name, const std::string& ipv4, int port );
    Stream( const char * name, const char * ipv4, int port );

  protected:
    const std::string m_name;
    const std::string m_ipv4;
    const int m_port;
};

} // namespace io
} // namespace hub
