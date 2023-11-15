#include "Resolution.hpp"

// #include <iostream>
//#include "core/Traits.hpp"
//#include "core/Vector.hpp"

namespace hub {
namespace sensor {

//template <typename T>
//// requires (! Printable<T>)
////typename std::enable_if_t<!Printable<T> || std::is_same_v<T, unsigned char>, std::ostream&>
//std::ostream & operator<<( std::ostream& os, const std::vector<T>& vector ) {
//    os << "[";
//    //    auto nMax
//    constexpr auto nMaxDataToShow = 40;
//    const auto iMax               = std::min( (int)vector.size(), nMaxDataToShow );
//    for ( auto i = 0; i < iMax; ++i ) {
//        //    for ( int i = 0; i < vector.size(); ++i ) {
//        os << std::to_string( vector.at( i ) );
//        //        if ( i != vector.size() - 1 ) os << " ";
//        if ( i != iMax - 1 ) os << " ";
//    }
//    if ( vector.size() > nMaxDataToShow ) { os << " ... " << std::to_string( vector[iMax - 1] ); }
//    os << "](" << vector.size() << ")";
//    return os;
//}

//std::ostream& operator<<( std::ostream& os, const Resolution& resolution ) {
////    os << "<" << resolution.format << ":" << resolution.nDim << ">";
////    os << "[Resolution] ";
//    os << "<" << resolution.format << ":";
////    ::operator<<(os, resolution.nDim);
//    const auto & nDim = resolution.nDim;
//    for ( int i = 0; i < nDim.size(); ++i ) {
//        const auto& n = nDim.at( i );
//        os << n;
//        if (i != nDim.size() - 1)
//            os << "x";
//    }
////    os << resolution.nDim << ">";
////    hub::operator<<(os, resolution.nDim);
//    os << ">";
//    return os;
//}

///////////////////////////////////////////////////////////////////////

//std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions ) {
//    //    os << resolutions;
//    os << "<";
//    for ( int i = 0; i < resolutions.size(); ++i ) {
//        //    for (const auto & resolution : resolutions) {
//        const auto& resolution = resolutions.at( i );
//        //        os << resolution.getFormat().name() << ":" << resolution.;
//        os << resolution;
//        if ( i != resolutions.size() - 1 ) os << ", ";
//    }
//    os << ">";
//    return os;
//}

//std::string toString( const Resolutions& resolutions ) {
//    std::stringstream ss;
//    ss << resolutions;
//    return ss.str();
//}

} // namespace sensor
} // namespace hub
