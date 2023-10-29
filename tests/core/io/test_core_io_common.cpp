#include "test_core_io_common.hpp"

//#include <chrono>
//#include <string>
//#include <tuple>
//#include <vector>

//// #include <core/Macros.hpp>
//#include <core/Traits.hpp>

namespace testCoreIoCommon {


std::ostream& operator<<( std::ostream& os, const Data& data ) {
    os << data.a << " " << data.b << " " << data.name;
    return os;
}


} // namespace testCoreIoCommon
