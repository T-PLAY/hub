
//#pragma once

#include "Format.hpp"


namespace hub {
namespace sensor {

//std::ostream& operator<<( std::ostream& os, const Format& format ) {
//    os << format::to_string(format);
//    return os;
//}

//namespace format {

std::ostream& operator<<( std::ostream& os, const Format& format ) {
    os << "{" << format.m_name << ", " << format.m_nByte << ", " << format.m_isInterpolable << "}";
    return os;
}

//Output &operator<<(Output &output, const Format &format) {
//    //    output.isOutput();
////    std::cout << "write format" << std::endl;
//    output.write((const Data_t*)&format.m_nByte, format.ioGetSize());
//    //    output.write( reinterpret_cast<const unsigned char*>( &format.m_nByte ), format.ioGetSize() );
//    return output;
//}

//Input &operator>>(Input &input, Format &format) {
//    //    input.read(format.ioGetData(), format.ioGetSize());
//    input.read((Data_t*)&format.m_nByte, format.ioGetSize());
//    //    const Data_t* ioGetData() const { return (Data_t*)this; }
//    return input;
//}

//static constexpr int s_format2nByte[static_cast<int>( Format::COUNT )] = {
//    0,       // NONE
//    2,       // Z16
//    2,       // DISPARITY16
//    3 * 4,   // XYZ32F
//    4,       // YUYV
//    3,       // RGB8
//    3,       // BGR8
//    4,       // RGBA8
//    4,       // BGRA8
//    1,       // Y8
//    2,       // Y16
//    2,       // RAW10
//    2,       // RAW16
//    1,       // RAW8
//    2,       // UYVY
//    1,       // MOTION_RAW
//    4,       // MOTION_XYZ32F
//    1,       // GPIO_RAW
//    4,       // DISPARITY32
//    12 + 16, // DOF6
//    2,       // Y10BPACK
//    4,       // DISTANCE
//    1,       // MJPEG
//    2,       // Y8I
//    3,       // Y12I
//    1,       // INZI
//    1,       // INVI
//    2,       // W10
//    2,       // Z16H
//    2,       // FG
//    2,       // Y411
//    64,      // MAT4
//    80 + 4,  // USER_DATA (non constant), string size ~= 80 and 4 bytes for common use int value
//    80 + 4,  // MESH (non constant), string size ~= 80 and 4 bytes for common use int value
//    12 + 16, // POINT
//    4,       // DENSITY

//};

//inline constexpr int getSize( const Format& format ) noexcept {

//    return s_format2nByte[static_cast<int>( format )];
//}


//static constexpr bool s_format2isInterpolable[static_cast<int>( Format::COUNT )] = {
//    false, // NONE
//    false, // Z16
//    false, // DISPARITY16
//    false, // XYZ32F
//    false, // YUYV
//    false, // RGB8
//    false, // BGR8
//    false, // RGBA8
//    false, // BGRA8
//    false, // Y8
//    false, // Y16
//    false, // RAW10
//    false, // RAW16
//    false, // RAW8
//    false, // UYVY
//    false, // MOTION_RAW
//    false, // MOTION_XYZ32F
//    false, // GPIO_RAW
//    false, // DISPARITY32
//    true,  // DOF6
//    false, // Y10BPACK
//    false, // DISTANCE
//    false, // MJPEG
//    false, // Y8I
//    false, // Y12I
//    false, // INZI
//    false, // INVI
//    false, // W10
//    false, // Z16H
//    false, // FG
//    false, // Y411
//    false, // MAT4
//    false, // USER_DATA
//    false, // MESH
//    true,  // POINT
//    false, // DENSITY
//};

//inline constexpr bool isInterpolable( const Format& format ) noexcept {
//    return s_format2isInterpolable[(int)format];
//}


//static std::string format2stringArray[static_cast<int>( Format::COUNT )] = {
//    "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
//    "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
//    "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
//    "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
//    "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
//    "Y411",        "MAT4",  "USER_DATA",   "MESH",       "POINT",         "DENSITY",
//};

//static inline constexpr std::string& to_string( const Format& format ) {
//    return format2stringArray[(int)format];
//}

//static constexpr bool s_format2hasFixedSize[static_cast<int>( Format::COUNT )] = {
//    true,  // NONE
//    true,  // Z16
//    true,  // DISPARITY16
//    true,  // XYZ32F
//    true,  // YUYV
//    true,  // RGB8
//    true,  // BGR8
//    true,  // RGBA8
//    true,  // BGRA8
//    true,  // Y8
//    true,  // Y16
//    true,  // RAW10
//    true,  // RAW16
//    true,  // RAW8
//    true,  // UYVY
//    true,  // MOTION_RAW
//    true,  // MOTION_XYZ32F
//    true,  // GPIO_RAW
//    true,  // DISPARITY32
//    true,  // DOF6
//    true,  // Y10BPACK
//    true,  // DISTANCE
//    true,  // MJPEG
//    true,  // Y8I
//    true,  // Y12I
//    true,  // INZI
//    true,  // INVI
//    true,  // W10
//    true,  // Z16H
//    true,  // FG
//    true,  // Y411
//    true,  // MAT4
//    false, // USER_DATA
//    false, // MESH
//    true,  // POINT
//    true,  // DENSITY
//};

//inline constexpr bool hasFixedSize( const Format& format ) noexcept {
//    return s_format2hasFixedSize[(int)format];
//}

//static constexpr bool s_formatIsImage[static_cast<int>( Format::COUNT )] = {
//    false,       // NONE
//    true,       // Z16
//    false,       // DISPARITY16
//    false,   // XYZ32F
//    false,       // YUYV
//    true,       // RGB8
//    true,       // BGR8
//    true,       // RGBA8
//    true,       // BGRA8
//    true,       // Y8
//    true,       // Y16
//    false,       // RAW10
//    false,       // RAW16
//    false,       // RAW8
//    false,       // UYVY
//    false,       // MOTION_RAW
//    false,       // MOTION_XYZ32F
//    false,       // GPIO_RAW
//    false,       // DISPARITY32
//    false, // DOF6
//    true,       // Y10BPACK
//    false,       // DISTANCE
//    true,       // MJPEG
//    true,       // Y8I
//    true,       // Y12I
//    false,       // INZI
//    false,       // INVI
//    false,       // W10
//    true,       // Z16H
//    false,       // FG
//    true,       // Y411
//    false,      // MAT4
//    false,  // USER_DATA (non constant), string size ~= 80 and 4 bytes for common use int value
//    false,  // MESH (non constant), string size ~= 80 and 4 bytes for common use int value
//    false, // POINT
//    false,       // DENSITY
//};

//static inline constexpr bool isImage( const Format& format ) noexcept {
//    return s_formatIsImage[static_cast<int>( format )];
//}


//} // namespace format

} // namespace sensor
} // namespace hub
