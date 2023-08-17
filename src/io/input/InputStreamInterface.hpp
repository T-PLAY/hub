#pragma once

#include "Input.hpp"
#include "io/Stream.hpp"

namespace hub {
using namespace io;
namespace input {

class InputStreamInterface : public Input, public Stream {
  public:

    InputStreamInterface(const std::string & streamName, const std::string & ipv4, int port);
    InputStreamInterface(const char * streamName, const char * ipv4, int port);
//    InputStreamInterface() = default;

  private:
};

} // namespace input
} // namespace hub
