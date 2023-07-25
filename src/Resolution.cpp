#include "Resolution.hpp"

#include <iostream>

namespace hub {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
    os << res::format2stringArray[(int)format];
    return os;
}

std::ostream& operator<<( std::ostream& os, const Dims& dims ) {

    os << res::HUB_TO_STRING( dims );
    return os;
}

std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
    os << res::HUB_TO_STRING( resolution );
    return os;
}

std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
    os << res::HUB_TO_STRING( resolutions );
    return os;
}

namespace res {} // namespace res

} // namespace hub
