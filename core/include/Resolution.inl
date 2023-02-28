#pragma once
#include "Resolution.hpp"

// #include <algorithm>
// #include <any>
// #include <map>
// #include <string>
// #include <vector>
// #include <stdexcept>
#include <iostream>
#include <numeric>
#include <sstream>

namespace hub {

// template <int NDim>
// static constexpr int s_format2nByte[static_cast<int>( SensorSpec<NDim>::Format::COUNT )] = {
static constexpr int s_format2nByte[static_cast<int>( Format::COUNT )] = {
    0,       // NONE
    2,       // Z16
    2,       // DISPARITY16
    4,       // XYZ32F
    4,       // YUYV
    3,       // RGB8
    3,       // BGR8
    4,       // RGBA8
    4,       // BGRA8
    1,       // Y8
    2,       // Y16
    0,       // RAW10
    2,       // RAW16
    1,       // RAW8
    0,       // UYVY
    0,       // MOTION_RAW
    0,       // MOTION_XYZ32F
    0,       // GPIO_RAW
    0,       // DISPARITY32
    12 + 16, // DOF6
    0,       // Y10BPACK
    4,       // DISTANCE
    0,       // MJPEG
    2,       // Y8I
    3,       // Y12I
    0,       // INZI
    1,       // INVI
    0,       // W10
    2,       // Z16H
    2,       // FG
    2,       // Y411
    64,      // MAT4
};

inline constexpr int format2nByte( const Format& format ) noexcept {
    return s_format2nByte[static_cast<int>( format )];
}

inline CONSTEXPR20 size_t computeAcquisitionSize( Format format, const Dims& dims ) noexcept {
    return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
           s_format2nByte[static_cast<int>( format )];
}

inline size_t CONSTEXPR20 computeAcquisitionSize( const Resolutions& resolutions ) noexcept {
    //    size_t size = 0;
    //    for ( const auto& resolution : resolutions ) {
    //        size += computeAcquisitionSize( resolution );
    //    }
    //    return size;
    return std::accumulate(
        resolutions.cbegin(), resolutions.cend(), 0, []( size_t size, const Resolution& res ) {
            return size + computeAcquisitionSize( res );
        } );
}

inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolution& resolution ) noexcept {
    const auto& dims   = resolution.first;
    const auto& format = resolution.second;
    return computeAcquisitionSize( format, dims );
}

static constexpr bool s_format2isInterpolable[static_cast<int>( Format::COUNT )] = {
    false, // NONE
    false, // Z16
    false, // DISPARITY16
    false, // XYZ32F
    false, // YUYV
    false, // RGB8
    false, // BGR8
    false, // RGBA8
    false, // BGRA8
    false, // Y8
    false, // Y16
    false, // RAW10
    false, // RAW16
    false, // RAW8
    false, // UYVY
    false, // MOTION_RAW
    false, // MOTION_XYZ32F
    false, // GPIO_RAW
    false, // DISPARITY32
    true,  // DOF6
    false, // Y10BPACK
    false, // DISTANCE
    false, // MJPEG
    false, // Y8I
    false, // Y12I
    false, // INZI
    false, // INVI
    false, // W10
    false, // Z16H
    false, // FG
    false, // Y411
    false, // MAT4
};

inline constexpr bool format2isInterpolable( const Format& format ) noexcept {
    return s_format2isInterpolable[(int)format];
}

//////////////////////////////////////////////////////////////////////////////

static std::string HUB_TO_STRING( const Dims& dims ) {
    std::string str = "";
    for ( unsigned int i = 0; i < dims.size(); ++i ) {
        str += std::to_string( dims[i] );
        if ( i != dims.size() - 1 ) { str += " x "; }
    }
    return str;
}

static std::string format2stringArray[static_cast<int>( Format::COUNT )] = {
    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
    "Y411",        "MAT4",
};

static inline constexpr std::string& HUB_TO_STRING( const Format& format ) {
    return format2stringArray[(int)format];
}

static std::string HUB_TO_STRING( const Resolution& resolution ) {
    const auto& dims   = resolution.first;
    const auto& format = resolution.second;
    return "{" + HUB_TO_STRING( dims ) + " : " + HUB_TO_STRING( format ) + "}";
}

static std::string HUB_TO_STRING( const Resolutions& resolutions ) {
    //    std::stringstream sstream;
    //    sstream << resolutions;
    //    return sstream.str();
    //}
    // static std::string resolutions2string( const Resolutions& resolutions ) {
    const int size  = static_cast<int>( resolutions.size() );
    std::string str = "";
    if ( size > 1 ) str += "[";
    for ( int i = 0; i < size; ++i ) {
        const auto& resolution = resolutions.at( i );
        str += HUB_TO_STRING( resolution );

        //        const auto& dims   = resolutions[i].first;
        //        const auto& format = resolutions[i].second;
        //        if ( size > 1 ) str += "{";
        ////        str += dims2string( dims ) + " : " + format2string( format );
        //        str += to_string( dims ) + " : " + to_string( format );
        ////        str += resolution2string(resolution);
        //        if ( size > 1 ) str += "}";

        if ( i != size - 1 ) str += ", ";
    }
    if ( size > 1 ) str += "]";
    return str;
}

// template <class T>
// std::ostream& operator<<( std::ostream& os, const T& t ) {
////    os << format2stringArray[(int)format] << " (" << s_format2nByte[(int)format] << "o)";
////    os << format2stringArray[(int)format];
//    os << to_string(t);
//    return os;
//}

} // namespace hub
