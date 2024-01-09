/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/23
	
#pragma once

#include <string>

#include "StreamBase.hpp"
//#include "net/ClientSocket.hpp"
#include "InputOutputSocket.hpp"

namespace hub {
namespace io {


class StreamServer : public StreamBase
{
  public:
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;

//    static const std::string s_topicStream;
//    static const std::string s_topicViewer;
//    static const std::string s_topicEvents;

//    static constexpr int s_bufferAcqSize = 100;

//        static const std::string s_defaultIpv4;
//        static constexpr int s_defaultPort     = 4042;

  protected:
    StreamServer( const std::string& name, const std::string& ipv4, int port );
    StreamServer( const char* name, const char* ipv4, int port );

//    int m_iAcq = 0;
//    net::ClientSocket m_clientSocket;
};

} // namespace io
} // namespace hub
