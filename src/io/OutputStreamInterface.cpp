
#include "OutputStreamInterface.hpp"

namespace hub {
namespace io {


OutputStreamInterface::OutputStreamInterface(const std::string & streamName, const std::string & ipv4, int port)
    :
    Stream(streamName, ipv4, port)
{
}

OutputStreamInterface::OutputStreamInterface(const char *streamName, const char *ipv4, int port) :
    Stream(streamName, ipv4, port)
{
}

} // namespace io
} // namespace hub
