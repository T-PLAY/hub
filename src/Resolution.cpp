#include "Resolution.hpp"

#include <iostream>

namespace hub {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
    os << format2stringArray[(int)format] << " (byte:" << s_format2nByte[(int)format] << ")";
    return os;
}

} // namespace hub
