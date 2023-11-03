#include "NDim.hpp"

// #include <iostream>

namespace hub {
namespace sensor {
namespace resolution {

SRC_API std::ostream& operator<<( std::ostream& os, const NDim& nDim ) {
    //    for (const auto & n : nDim.m_array) {
    //        os << n << " ";
    //    }
    for ( int i = 0; i < nDim.size(); ++i ) {
        const auto& n = nDim.ns[ i ];
        os << n;
        if (i != nDim.size() - 1)
            os << "x";
    }
    return os;
}


} // namespace resolution
} // namespace sensor
} // namespace hub
