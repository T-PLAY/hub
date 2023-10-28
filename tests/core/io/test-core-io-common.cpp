#include "test-core-io-common.hpp"

//#include <chrono>
//#include <string>
//#include <tuple>
//#include <vector>

//// #include <core/Macros.hpp>
//#include <core/Traits.hpp>

namespace testCoreIoCommon {


std::ostream& operator<<( std::ostream& os, const Character& character ) {
    os << character.a << " " << character.b << " " << character.name;
    return os;
}


} // namespace testCoreIoCommon
