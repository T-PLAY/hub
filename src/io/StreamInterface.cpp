#include "StreamInterface.hpp"

// StreamInterface::StreamInterface()
//{

//}

namespace hub {
namespace io {

static const std::string type2string[static_cast<int>( StreamInterface::ClientType::COUNT )] = {
    "NONE",
    "STREAMER",
    "STREAM_VIEWER",
    "VIEWER",
    "ASKER",
    "KILLER",
};
std::ostream& operator<<( std::ostream& os, const StreamInterface::ClientType& clientType ) {
    os << type2string[(int)clientType];
    return os;
}

static const std::string
    clientMessage2string[static_cast<int>( StreamInterface::ClientMessage::COUNT )] = {
        "NONE",

        "STREAMER_CLIENT_START",
        "STREAMER_CLIENT_CLOSED",
        "STREAMER_CLIENT_NEW_ACQ",
        "STREAMER_CLIENT_INIT_SENSOR",

        "STREAM_VIEWER_CLIENT_CLOSED",

        "VIEWER_CLIENT_CLOSED",
        "VIEWER_CLIENT_SET_PROPERTY",

        "ASKER_CLIENT_CLOSED",
        "ASKER_CLIENT_GET_LIST_STREAMS",
        "ASKER_CLIENT_GET_ACQ",

        "CLIENT_SERVER_DOWN",
        "STREAMER_CLIENT_NEW_STREAM_VIEWER",
        "STREAMER_CLIENT_INITED",
        "STREAMER_CLIENT_STREAM_VIEWER_INITED",
};
std::ostream& operator<<( std::ostream& os, const StreamInterface::ClientMessage& msg ) {
    os << clientMessage2string[(int)msg];
    return os;
}

static const std::string
    serverMessage2string[static_cast<int>( StreamInterface::ServerMessage::COUNT )] = {
        "NONE",

        "FOUND",
        "NOT_FOUND",
        "OK",

        "SERVER_CLOSED",

        "STREAMER_CLOSED",

        "STREAM_VIEWER_CLOSED",
        "STREAM_VIEWER_NEW_ACQ",

        "VIEWER_CLOSED",
        "VIEWER_NEW_STREAMER",
        "VIEWER_DEL_STREAMER",
        "VIEWER_SET_PROPERTY",

        "ASKER_CLOSED",
        "ASKER_LIST_STREAMS",

        "STREAM_VIEWER_INITED",
};
std::ostream& operator<<( std::ostream& os, const StreamInterface::ServerMessage& msg ) {
    os << serverMessage2string[(int)msg];
    return os;
}

// std::string StreamInterface::to_string(const ClientMessage &message)
//{
//     return std::to_string((int)message);
//}

// std::string StreamInterface::to_string(const ServerMessage &message)
//{
//     return std::to_string((int)message);
//}

StreamInterface::StreamInterface( const std::string& name, const std::string& ipv4, int port ) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

StreamInterface::StreamInterface( const char* name, const char* ipv4, int port ) :
    m_name( name ), m_ipv4( ipv4 ), m_port( port ) {}

} // namespace io
} // namespace hub
