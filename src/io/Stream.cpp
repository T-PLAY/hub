#include "Stream.hpp"

namespace hub {
namespace io {

std::string Stream::to_string(const Message &message)
{
    return std::to_string((int)message);
}

Stream::Stream( const std::string& name, const std::string& ipv4, int port ) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

Stream::Stream(const char *name, const char *ipv4, int port) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

} // namespace io
} // namespace hub
