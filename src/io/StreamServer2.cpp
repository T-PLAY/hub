#include "StreamServer2.hpp"

namespace hub {
namespace io {

StreamServer2::StreamServer2( const std::string& name, const std::string& ipv4, int port ) :
    StreamBase( name, ipv4, port ) {}

} // namespace io
} // namespace hub
