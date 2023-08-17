#pragma once

#include "Output.hpp"
#include "Stream.hpp"

namespace hub {
namespace io {

class OutputStreamInterface : public Output, public Stream {
  public:

    OutputStreamInterface(const std::string & streamName, const std::string & ipv4, int port);
    OutputStreamInterface(const char * streamName, const char * ipv4, int port);
//    OutputStreamInterface() = default;

  private:
};

} // namespace io
} // namespace hub
