#pragma once

#include "Output.hpp"
#include "io/Stream.hpp"

namespace hub {
using namespace io;
namespace output {

class OutputStreamInterface : public Output, public Stream {
  public:

    OutputStreamInterface(const std::string & streamName, const std::string & ipv4, int port);
    OutputStreamInterface(const char * streamName, const char * ipv4, int port);
//    OutputStreamInterface() = default;

  private:
};

} // namespace output
} // namespace hub
