#pragma once

#include "Input.hpp"
#include "Stream.hpp"

namespace hub {
namespace io {

class InputStreamInterface : public Input, public Stream {
  public:

    InputStreamInterface(const std::string & streamName, const std::string & ipv4, int port);
    InputStreamInterface(const char * streamName, const char * ipv4, int port);
//    InputStreamInterface() = default;

  private:
};

} // namespace io
} // namespace hub
