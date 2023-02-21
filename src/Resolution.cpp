#include "Resolution.hpp"

#include <iostream>

namespace hub {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
//    os << format2stringArray[(int)format] << " (" << s_format2nByte[(int)format] << "o)";
    os << format2stringArray[(int)format];
    return os;
}

std::ostream& operator<<( std::ostream& os, const Dims& dims ) {

//    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        os << std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { os << " x "; }
    }
    return os;
//    return str;
}

std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
//    os << resolution.first;
        const auto& dims   = resolution.first;
        const auto& format = resolution.second;
//        if ( size > 1 ) str += "{";
//        os << dims2string( dims ) + " : " + format2string( format );
//        os << dims2string( dims ) + " : " + format2string( format );
//        os << dims2string( dims ) << " : " << format;
        os << "{" << dims << " : " << format << "}";
//        if ( size > 1 ) str += "}";
//        if ( i != size - 1 ) str += ", ";
    return os;
}


std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
//    os << resolution.first;
//    os << resolutions2string(resolutions);
    const int size  = static_cast<int>( resolutions.size() );
//    std::string str = "";
    if ( size > 1 ) os << "[";
    for ( int i = 0; i < size; ++i ) {
        const auto & resolution = resolutions.at(i);
//        const auto& dims   = resolutions[i].first;
//        const auto& format = resolutions[i].second;
//        if ( size > 1 ) os << "{";
//        str += dims2string( dims ) + " : " + format2string( format );
//        str += resolution2string(resolution);
        os << resolution;
//        if ( size > 1 ) os << "}";
        if ( i != size - 1 ) os << ", ";
    }
    if ( size > 1 ) os << "]";
//    os << str;
//    return str;
    return os;
}

} // namespace hub
