
#include "InputStreamInterface.hpp"

namespace hub {
namespace io {


InputStreamInterface::InputStreamInterface(const std::string & streamName, const std::string & ipv4, int port)
    :
    Stream(streamName, ipv4, port)
{
}

InputStreamInterface::InputStreamInterface(const char *streamName, const char *ipv4, int port) :
    Stream(streamName, ipv4, port)
{
}

} // namespace io
} // namespace hub
