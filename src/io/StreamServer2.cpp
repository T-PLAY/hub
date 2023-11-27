#include "StreamServer2.hpp"

namespace hub {
namespace io {

// const std::string StreamServer2::s_exitSignal = "exitServer";
const std::string StreamServer2::s_defaultIpv4 = "127.0.0.1";

#ifdef HUB_SERVER_PORT
const int StreamServer2::s_defaultPort = HUB_SERVER_PORT;
#else
const int StreamServer2::s_defaultPort = 5043;
#endif
// const int StreamServer2::s_defaultPort = HUB_SERVICE_PORT;

// const std::string StreamServer2::s_topicStream = "streams/";
// const std::string StreamServer2::s_topicViewer = "viewers/";
// const std::string StreamServer2::s_topicEvents = "events";

StreamServer2::StreamServer2( const std::string& name, const std::string& ipv4, int port ) :
    StreamBase( name, ipv4, port )
//    m_clientSocket(ipv4, port) {}
{}

StreamServer2::StreamServer2( const char* name, const char* ipv4, int port ) :
    StreamBase( name, ipv4, port )
//    m_clientSocket(ipv4, port)
{}

} // namespace io
} // namespace hub
