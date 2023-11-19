#pragma once

#include <string>

#include "io/StreamInterface.hpp"

namespace hub {
namespace io {


class StreamMqtt : public StreamInterface
{
  public:
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;

    static const std::string s_topicStream;
    static const std::string s_topicViewer;
    static const std::string s_topicEvents;

    static constexpr int s_bufferAcqSize = 100;

    //    static const std::string s_defaultIpv4;
    //    static constexpr int s_defaultPort     = 1883;

  protected:
    StreamMqtt( const std::string& name, const std::string& ipv4, int port );
    StreamMqtt( const char* name, const char* ipv4, int port );

    int m_iAcq = 0;
};

} // namespace io
} // namespace hub
