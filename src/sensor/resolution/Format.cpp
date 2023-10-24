#include "Format.hpp"

namespace hub {
namespace sensor {
namespace resolution {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
    //    os << "{" << format.name << ", " << format.nByte << ", " << format.interpolable << "}";
    //    os << "{" << format.name << ", " << format.nByte << ", " << format.interpolable << "}";
    os << format.name;
    return os;
}

} // namespace resolution
} // namespace sensor
} // namespace hub
