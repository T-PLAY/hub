#include "Resolution.hpp"

#include <iostream>

namespace hub {
namespace sensor {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
    os << resolution::format2stringArray[(int)format];
    return os;
}

std::ostream& operator<<( std::ostream& os, const Dims& dims ) {

    os << resolution::HUB_TO_STRING( dims );
    return os;
}

std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
    os << resolution::HUB_TO_STRING( resolution );
    return os;
}

std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
    os << resolution::HUB_TO_STRING( resolutions );
    return os;
}

namespace resolution {} // namespace resolution

} // namespace sensor
} // namespace hub
