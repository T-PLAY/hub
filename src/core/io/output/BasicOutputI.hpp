#pragma once

#include <cassert>

#include "core/Macros.hpp"
//#include "core/Traits.hpp"
#include "core/io/iosBase.hpp"

namespace hub {
namespace io {
namespace output {

class BasicOutputI : public iosBase
{
  public:
//    virtual void write( const Data_t* data, Size_t len ) = 0;


//    void write() {};

//    template <class T>
//    void write(const T & t) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//        assert(false);
//    }

};

} // namespace output
} // namespace io
} // namespace hub
