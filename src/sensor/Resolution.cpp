#include "Resolution.hpp"

// #include <iostream>

namespace hub {
namespace sensor {

// #include "core/Vector.hpp"

// template <typename T>
std::ostream& operator<<( std::ostream& os, const Resolution::Dims& dims ) {
    //    os << "[";
    //    constexpr auto nMaxDataToShow = 512;
    //    const auto iMax               = std::min( (int)vector.size(), nMaxDataToShow );
    //    for ( auto i = 0; i < iMax; ++i ) {
    //        os << std::to_string( vector.at( i ) );
    //        if ( i != iMax - 1 ) os << " ";
    //    }
    for ( Size_t i = 0; i < dims.size(); ++i ) {
        os << std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) os << "x";
        //            ++i;
    }
    //    if ( vector.size() > nMaxDataToShow ) { os << " ... " << std::to_string( vector[iMax - 1]
    //    ); } os << "](" << vector.size() << ")";
    return os;
}

std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
    //    constexpr std::string name() const {
    //        std::string str = "<";
//    os << "<" << resolution.m_format.name() << ":" << resolution.m_dims << ">";
    os << resolution.m_format.name() << ":" << resolution.m_dims;
    //        str += m_format.name();
    //        str += ":";
    //        for ( Size_t i = 0; i < m_nDim; ++i ) {
    //            str += std::to_string( m_dims[i] );
    //            if ( i != nDim() - 1 ) str += "x";
    //            //            ++i;
    //        }
    //        str += ">";
    //        return str;
    //    }

    //    os << resolution.m_format << " " << resolution.m_dims << std::endl;
    return os;
}

///////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
//    os << resolutions;
    os << "<";
    for (int i = 0; i < resolutions.size(); ++i) {
//    for (const auto & resolution : resolutions) {
        const auto & resolution = resolutions.at(i);
//        os << resolution.getFormat().name() << ":" << resolution.;
        os << resolution;
        if (i != resolutions.size() - 1)
            os << ", ";
    }
    os << ">";
    return os;
}

std::string toString(const Resolutions &resolutions) {
    std::stringstream ss;
    ss << resolutions;
    return ss.str();
}

Size_t nByte(const Resolutions &resolutions) {
    Size_t size = 0;
    for (const auto & resolution : resolutions) {
        size += resolution.nByte();
    }
    return size;
}

//Output &operator<<(Output &output, const Resolution &resolution) {
//    output << resolution.m_format;
//    return output;
//}

//Input &operator>>(Input &input, Resolution &resolution) {
//    input >> resolution.m_format;
//    return input;
//}


// std::ostream& operator<<( std::ostream& os, const Format& format ) {
//     os << resolution::format2stringArray[(int)format];
//     return os;
// }

// std::ostream& operator<<( std::ostream& os, const Dims& dims ) {

//    os << resolution::HUB_TO_STRING( dims );
//    return os;
//}

// std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
//     os << resolution::HUB_TO_STRING( resolution );
//     return os;
// }

// std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
//     os << resolution::HUB_TO_STRING( resolutions );
//     return os;
// }

// namespace resolution {} // namespace resolution

} // namespace sensor
} // namespace hub
