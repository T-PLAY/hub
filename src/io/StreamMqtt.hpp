/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/08/22

#pragma once

#include <string>

#include "StreamBase.hpp"

namespace hub {
namespace io {

///
/// \brief The StreamMqtt class
///
class StreamMqtt : public StreamBase
{
  public:
    static const std::string s_defaultIpv4;
    static const int s_defaultPort;

    static const std::string s_topicStream;
    static const std::string s_topicViewer;
    static const std::string s_topicEvents;

    static constexpr int s_bufferAcqSize = 100;

  protected:
    StreamMqtt( const std::string& name, const std::string& ipv4, int port );
    StreamMqtt( const char* name, const char* ipv4, int port );

    int m_iAcq = 0;
};

} // namespace io
} // namespace hub
