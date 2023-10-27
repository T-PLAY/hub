#pragma once

//#include "core/Macros.hpp"
//#include "core/Traits.hpp"

namespace hub {
namespace io {

class iosBase
{
  public:
    virtual void close()                          = 0;
    virtual bool isOpen() const                   = 0;

};

} // namespace io
} // namespace hub
