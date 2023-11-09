#pragma once

// #include <numeric>
// #include <string>
// #include <vector>
#include <iostream>
// #include <stdlib.h>
// #include <cstdlib>
#include <cstring>
#include <memory>

// #include "io/input/Input.hpp"
// #include "io/output/Output.hpp"
#include "core/Input.hpp"
#include "core/Macros.hpp"

namespace hub {
namespace sensor {
// namespace resolution {

class Format
{
  public:
    constexpr bool operator==( const Format& format ) const {
        return nByte == format.nByte && interpolable == format.interpolable && name == format.name;
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );

  public:
    Size_t nByte;
    std::string name;
    bool interpolable;
};

namespace format {

/////////////////////////////////////// 1D ////////////////////////////////////////

// Transform matrix 4x4 of float.
struct Mat4 {
    float data[16];
    //    static constexpr auto nByte() {return  16;};
    static constexpr auto name() { return "MAT4"; };
    static constexpr auto interpolable() { return false; };
};
static_assert( sizeof( Mat4 ) == 64 );

// 32-bit density values. For MRI, CT scan and US representations.
struct Density {
    uint32_t value;
};
static_assert( sizeof( Density ) == 4 );

// 32-bit float-point depth distance value.
struct Distance {
    float value;
};
static_assert( sizeof( Distance ) == 4 );

/////////////////////////////////////// 2D ////////////////////////////////////////

// 8-bit red, green and blue channels.
struct RGB8 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    //    static constexpr std::string_view name() { return "RGB8"; };
    //    static constexpr auto nByte() {return 3;};
    static constexpr auto name() { return "RGB8"; };
    static constexpr auto interpolable() { return false; };
};
static_assert( sizeof( RGB8 ) == 3 );

// 16-bit linear depth values. The depth is meters is equal to depth scale pixel value.
struct Z16 {
    uint16_t depth; // meters
};
static_assert( sizeof( Z16 ) == 2 );

/////////////////////////////////////// 3D ////////////////////////////////////////

// Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion
// (w0, w1, w2, w3 || w, x, y, z).
struct DOF6 {
    // translation
    float x;
    float y;
    float z;
    // quaternion
    float w0; // w
    float w1; // x
    float w2; // y
    float w3; // z
};
static_assert( sizeof( DOF6 ) == 28 );

// Point consist of x, y and z position, rgb color and depth .
// struct Point {
//    float x;
//    float y;
//    float z;

//    unsigned char r;
//    unsigned char g;
//    unsigned char b;
//};

// 32-bit floating point 3D coordinates.
struct XYZ32F {
    float x;
    float y;
    float z;
};
static_assert( sizeof( XYZ32F ) == 12 );

} // namespace format


// namespace format {
//// clang-format off

////using RGB8E = Format<3, "ouou", true>;

////static constexpr Format TEST{ 4 * 4 * 4, std::string_view("MAT4"), false};
////static constexpr Format TEST{ 4 * 4 * 4, "TEST", false};

//// 1D
////static constexpr Format MAT4 			{ 4 * 4 * 4, 		"MAT4", 			false }; // Transform matrix 4x4 of
///float.
////static constexpr Format DENSITY 		{ 4, 				"DENSITY", 			false };	// 32-bit density values. For
///MRI, CT scan and US representations. /static constexpr Format DISTANCE 		{ 4,
///"DISTANCE", 		false };	// 32-bit float-point depth distance value.

////// 2D
////static constexpr Format Z16				{ 2, 				"Z16",				false };	// 16-bit linear depth values. The
///depth is meters is equal to depth scale pixel value.
// static constexpr Format RGB8			{ 3, 				"RGB8", 			false };	// 8-bit red, green and blue
// channels.
// static constexpr Format BGR8			{ 3, 				"BGR8", 			false };	// 8-bit blue, green, and red
// channels -- suitable for OpenCV.
////static constexpr Format RGBA8			{ 4,			 	"RGBA8",	 		false };	// 8-bit red, green and blue
///channels + constant alpha channel equal to FF. /static constexpr Format BGRA8			{ 4,
///"BGRA8",	 		false };	// 8-bit blue, green, and red channels + constant alpha channel equal to
///FF.
// static constexpr Format Y8				{ 1, 				"Y8", 				false };	// 8-bit per-pixel grayscale
// image.
////static constexpr Format Y16				{ 2, 				"Y16",		 		false };	// 16-bit per-pixel grayscale
///image.
////static constexpr Format Y10BPACK		{ 2, 				"Y10BPACK", 		false }; // 16-bit per-pixel grayscale
///image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is
///unpacked to LSB and padded with 6 zero bits. /static constexpr Format Y8I				{ 1,
///"Y8I", 				false };	// 8-bit per pixel interleaved. 8-bit left, 8-bit right. /static
///constexpr Format Y12I			{ 0, 				"Y12I", 			false };	// 12-bit per pixel interleaved. 12-bit
///left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order. /static
///constexpr Format YUYV			{ 4, 				"YUYV", 			false };	// 32-bit y0, u, y1, v data for every two
///pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV.
////static constexpr Format RAW10			{ 0,			 	"RAW10", 			false };	// Four 10 bits per pixel
///luminance values packed into a 5-byte macropixel. /static constexpr Format RAW16			{ 2,
///"RAW16", 			false };	// 16-bit raw image. /static constexpr Format RAW8			{ 1,
///"RAW6", 			false };	// 8-bit raw image. /static constexpr Format UYVY			{ 0,
///"UYVY", 			false };	// Similar to the standard YUYV pixel format, but packed in a different
///order.
////static constexpr Format Z16H			{ 2, 				"Z16H", 			false };	// Variable-length
///Huffman-compressed 16-bit depth values.
////static constexpr Format FG				{ 2, 				"FG", 				false };	// 16-bit per-pixel frame grabber
///format.
////static constexpr Format Y411			{ 0, 				"Y411", 			false };	// 12-bit
///per-pixel..
////static constexpr Format MJPEG			{ 0,				"MJPEG", 			false };	// Bitstream encoding for video in
///which an image of each frame is encoded as JPEG-DIB. /static constexpr Format INVI			{ 1,
///"INVI", 			false };	// 8-bit IR stream. . /static constexpr Format W10				{ 0,
///"W10", 				false };	// Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 *
///byte.

////// 3D
////static constexpr Format DISPARITY16		{ 2, 				"DISPARITY16", 		false };	// 16-bit float-point
///disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
////static constexpr Format XYZ32F			{ 3 * 4, 			"XYZ32F", 			true  };	// 32-bit floating point 3D
///coordinates. /static constexpr Format MOTION_RAW		{ 0, 				"MOTION_RAW",
///false };	// Raw data from the motion sensor. /static constexpr Format MOTION_XYZ32F	{ 3 * 4,
///"MOTION_XYZ32F", 	false };	// Motion data packed as 3 32-bit float values, for X, Y, and Z
///axis.
////static constexpr Format GPIO_RAW		{ 0, 				"GPIO_RAW", 		false };	// Raw data from the external
///sensors hooked to one of the GPIO's. /static constexpr Format DISPARITY32		{ 4,
///"DISPARITY32", 		false };	// 32-bit float-point disparity values. Depth->Disparity conversion :
///Disparity = Baseline*FocalLength/Depth.
// static constexpr Format DOF6			{ 3 * 4 + 4 * 4,	"DOF6", 			false };	// Pose data packed as floats
// array, containing translation vector (x, y, z), rotation quaternion (w0, w1, w2, w3 || w, x, y,
// z).
////static constexpr Format INZI			{ 0, 				"INZI", 			false };	// multi-planar Depth 16bit + IR
///10bit.
////static constexpr Format MESH			{ 0, 				"MESH", 			false };	// Mesh consist of shapes of
///vertices with indexes.
////static constexpr Format POINT			{ 0, 				"POINT",		 	false };	// Point consist of x, y and z
///position, rgb color and depth .

////// custom
////static constexpr Format USER_DATA		{ 0, 				"USER_DATA", 		false };	// User data with name and
///any value.

//// clang-format on
//} // namespace format

//} // namespace resolution
} // namespace sensor
} // namespace hub
