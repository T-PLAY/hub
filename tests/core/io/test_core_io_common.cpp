#include "test_core_io_common.hpp"

//#include <chrono>
//#include <string>
//#include <tuple>
//#include <vector>

//// #include <core/Macros.hpp>
//#include <core/Traits.hpp>
#include <core/Vector.hpp>

namespace testCoreIoCommon {

//using namespace hub;


std::ostream& operator<<( std::ostream& os, const UserData& data ) {
    os << data.a << " " << data.b << " " << data.name << " ";
    ::operator<<(os, data.vints);
//    os << data.a << " " << data.b << " " << data.name << " " << data.vints;

    return os;
}


} // namespace testCoreIoCommon
