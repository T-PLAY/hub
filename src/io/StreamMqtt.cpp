#include "StreamMqtt.hpp"

namespace hub {
namespace io {

const std::string StreamMqtt::s_defaultIpv4 = "127.0.0.1";
const int StreamMqtt::s_defaultPort         = 1883;

const std::string StreamMqtt::s_topicStream = "streams/";
const std::string StreamMqtt::s_topicViewer = "viewers/";
const std::string StreamMqtt::s_topicEvents = "events";

StreamMqtt::StreamMqtt( const std::string& name, const std::string& ipv4, int port ) :
    StreamBase( name, ipv4, port ) {}

StreamMqtt::StreamMqtt( const char* name, const char* ipv4, int port ) :
    StreamBase( name, ipv4, port ) {}

} // namespace io
} // namespace hub
