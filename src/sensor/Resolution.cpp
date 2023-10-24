#include "Resolution.hpp"

// #include <iostream>

namespace hub {
namespace sensor {

std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
    os << resolution.format.name << ":" << resolution.dims;
    return os;
}

///////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
    //    os << resolutions;
    os << "<";
    for ( int i = 0; i < resolutions.size(); ++i ) {
        //    for (const auto & resolution : resolutions) {
        const auto& resolution = resolutions.at( i );
        //        os << resolution.getFormat().name() << ":" << resolution.;
        os << resolution;
        if ( i != resolutions.size() - 1 ) os << ", ";
    }
    os << ">";
    return os;
}

std::string toString( const Resolutions& resolutions ) {
    std::stringstream ss;
    ss << resolutions;
    return ss.str();
}

} // namespace sensor
} // namespace hub
