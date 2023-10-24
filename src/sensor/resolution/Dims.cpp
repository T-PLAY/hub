#include "Dims.hpp"

// #include <iostream>

namespace hub {
namespace sensor {
namespace resolution {

SRC_API std::ostream& operator<<( std::ostream& os, const Dims& dims ) {
    //    for (const auto & n : dims.m_array) {
    //        os << n << " ";
    //    }
    for ( int i = 0; i < dims.size(); ++i ) {
        const auto& n = dims.ns[ i ];
        os << n;
        if (i != dims.size() - 1)
            os << "x";
    }
    return os;
}


} // namespace resolution
} // namespace sensor
} // namespace hub
