#include "StreamServer.hpp"

namespace hub {
namespace io {

const std::string StreamServer::s_defaultIpv4 = "127.0.0.1";

#ifdef HUB_SERVER_PORT
const int StreamServer::s_defaultPort         = HUB_SERVER_PORT;
#else
const int StreamServer::s_defaultPort         = 4043;
#endif

// const int StreamServer::s_defaultPort         = HUB_SERVICE_PORT;

//const std::string StreamServer::s_topicStream = "streams/";
//const std::string StreamServer::s_topicViewer = "viewers/";
//const std::string StreamServer::s_topicEvents = "events";

StreamServer::StreamServer( const std::string& name, const std::string& ipv4, int port ) :
    StreamBase( name, ipv4, port )
//    m_clientSocket(ipv4, port) {}
{}

StreamServer::StreamServer( const char* name, const char* ipv4, int port ) :
    StreamBase( name, ipv4, port )
//    m_clientSocket(ipv4, port)
{}

} // namespace io
} // namespace hub
