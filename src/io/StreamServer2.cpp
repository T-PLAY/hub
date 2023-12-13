#include "StreamServer2.hpp"

namespace hub {
namespace io {

// const std::string StreamServer2::s_exitSignal = "exitServer";

// const std::string StreamServer2::s_defaultIpv4 = "127.0.0.1";

// #ifdef HUB_SERVICE_PORT
// const int StreamServer2::s_defaultPort = HUB_SERVICE_PORT;
// #else
// const int StreamServer2::s_defaultPort = 1993; // author's birthday, not snmp-tcp port
// #endif

// const int StreamServer2::s_defaultPort = HUB_SERVICE_PORT;

// const std::string StreamServer2::s_topicStream = "streams/";
// const std::string StreamServer2::s_topicViewer = "viewers/";
// const std::string StreamServer2::s_topicEvents = "events";

StreamServer2::StreamServer2( const std::string& name,
                              const std::string& ipv4,
                              int port,
                              const io::Header& header ) :
    StreamBase( name, ipv4, port, header )
//    m_clientSocket(ipv4, port) {}
{}

StreamServer2::StreamServer2( const char* name, const char* ipv4, int port, const io::Header& header ) :
    StreamBase( name, ipv4, port, header )
//    m_clientSocket(ipv4, port)
{}

} // namespace io
} // namespace hub
