#pragma once

#include <string>

#include "io/StreamInterface.hpp"
// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace impl2 {
namespace io {

class StreamServer : public hub::io::StreamInterface
{
  public:
    //    static const std::string s_exitSignal;
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;
    //    static const std::string s_topicStream;
    //    static const std::string s_topicViewer;
    //    static const std::string s_topicEvents;

    //    static constexpr int s_bufferAcqSize = 100;

    //        static const std::string s_defaultIpv4;
    //        static constexpr int s_defaultPort     = 4042;
    static void stopServer( std::string ipv4 = s_defaultIpv4, int port = s_defaultPort ) {
        auto clientSock = hub::io::InputOutputSocket( hub::net::ClientSocket( ipv4, port ) );
        clientSock.write( hub::io::StreamInterface::ClientType::KILLER );
    }

  protected:
    StreamServer( const std::string& name, const std::string& ipv4, int port );
    StreamServer( const char* name, const char* ipv4, int port );

    //    int m_iAcq = 0;
    //    net::ClientSocket m_clientSocket;
};

} // namespace io
} // namespace impl2
} // namespace hub
