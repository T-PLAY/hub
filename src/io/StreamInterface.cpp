#include "StreamInterface.hpp"

//StreamInterface::StreamInterface()
//{

//}

namespace hub {
namespace io {


std::string StreamInterface::to_string(const Message &message)
{
    return std::to_string((int)message);
}

StreamInterface::StreamInterface( const std::string& name, const std::string& ipv4, int port ) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

StreamInterface::StreamInterface(const char *name, const char *ipv4, int port) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

} // namespace io
} // namespace hub
