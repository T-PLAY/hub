
#pragma once

// #include <numeric>
// #include <string>
// #include <vector>
#include <iostream>
// #include <stdlib.h>
// #include <cstdlib>
#include <cassert>
#include <cstring>
#include <memory>

//#include "io/input/Input.hpp"
//#include "io/output/Output.hpp"
#include "core/Macros.hpp"

namespace hub {
namespace sensor {

class Format
{
  public:
    // io
//    static constexpr Size_t ioGetSize() { return sizeof( Format ); }
//    const Data_t* ioGetData() const { return (Data_t*)this; }
//    void ioSetData( const Data_t* data, Size_t size ) { memcpy( this, data, size ); }
//    static constexpr std::string ioTypeName() { return "Format"; }
    // end io


    constexpr auto nByte() const { return m_nByte; }
    constexpr std::string name() const { return m_name; }

    constexpr bool operator==( const Format& format ) const {
        return m_nByte == format.m_nByte && !std::memcmp( m_name, format.m_name, s_maxNameLen ) &&
               m_isInterpolable == format.m_isInterpolable;
    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );

//    template <typename Output>
//    friend Output& operator<<( Output& output, const Format& format );

//    template <typename Input>
//    friend Input& operator>>( Input& input, Format& format );

    //  private:
    static constexpr auto s_maxNameLen = 32;
    Size_t m_nByte                   = 0;
    char m_name[s_maxNameLen]          = { 0 };
//    std::array<char, s_maxNameLen> m_name;
    bool m_isInterpolable;
};

//template <typename Output>
//Output& operator<<( Output& output, const Format& format );

//template <typename Input>
//Input& operator>>( Input& input, Format& format );

namespace format {
// clang-format off

// 1D
static constexpr Format MAT4 			{ 4 * 4 * 4, 		"MAT4", 			false }; // Transform matrix 4x4 of float.
static constexpr Format DENSITY 		{ 4, 				"DENSITY", 			false };	// 32-bit density values. For MRI, CT scan and US representations.
static constexpr Format DISTANCE 		{ 4, 				"DISTANCE", 		false };	// 32-bit float-point depth distance value.

// 2D
static constexpr Format Z16				{ 2, 				"Z16",				false };	// 16-bit linear depth values. The depth is meters is equal to depth scale pixel value.
static constexpr Format RGB8			{ 3, 				"RGB8", 			false };	// 8-bit red, green and blue channels.
static constexpr Format BGR8			{ 3, 				"BGR8", 			false };	// 8-bit blue, green, and red channels -- suitable for OpenCV.
static constexpr Format RGBA8			{ 4,			 	"RGBA8",	 		false };	// 8-bit red, green and blue channels + constant alpha channel equal to FF.
static constexpr Format BGRA8			{ 4,			 	"BGRA8",	 		false };	// 8-bit blue, green, and red channels + constant alpha channel equal to FF.
static constexpr Format Y8				{ 1, 				"Y8", 				false };	// 8-bit per-pixel grayscale image.
static constexpr Format Y16				{ 2, 				"Y16",		 		false };	// 16-bit per-pixel grayscale image.
static constexpr Format Y10BPACK		{ 2, 				"Y10BPACK", 		false }; // 16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits.
static constexpr Format Y8I				{ 1, 				"Y8I", 				false };	// 8-bit per pixel interleaved. 8-bit left, 8-bit right.
static constexpr Format Y12I			{ 0, 				"Y12I", 			false };	// 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order.
static constexpr Format YUYV			{ 4, 				"YUYV", 			false };	// 32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV.
static constexpr Format RAW10			{ 0,			 	"RAW10", 			false };	// Four 10 bits per pixel luminance values packed into a 5-byte macropixel.
static constexpr Format RAW16			{ 2,				"RAW16", 			false };	// 16-bit raw image.
static constexpr Format RAW8			{ 1, 				"RAW6", 			false };	// 8-bit raw image.
static constexpr Format UYVY			{ 0, 				"UYVY", 			false };	// Similar to the standard YUYV pixel format, but packed in a different order.
static constexpr Format Z16H			{ 2, 				"Z16H", 			false };	// Variable-length Huffman-compressed 16-bit depth values.
static constexpr Format FG				{ 2, 				"FG", 				false };	// 16-bit per-pixel frame grabber format.
static constexpr Format Y411			{ 0, 				"Y411", 			false };	// 12-bit per-pixel..
static constexpr Format MJPEG			{ 0,				"MJPEG", 			false };	// Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB.
static constexpr Format INVI			{ 1, 				"INVI", 			false };	// 8-bit IR stream. .
static constexpr Format W10				{ 0, 				"W10", 				false };	// Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 * byte.

// 3D
static constexpr Format DISPARITY16		{ 2, 				"DISPARITY16", 		false };	// 16-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
static constexpr Format XYZ32F			{ 3 * 4, 			"XYZ32F", 			true  };	// 32-bit floating point 3D coordinates.
static constexpr Format MOTION_RAW		{ 0, 				"MOTION_RAW", 		false };	// Raw data from the motion sensor.
static constexpr Format MOTION_XYZ32F	{ 3 * 4,			"MOTION_XYZ32F", 	false };	// Motion data packed as 3 32-bit float values, for X, Y, and Z axis.
static constexpr Format GPIO_RAW		{ 0, 				"GPIO_RAW", 		false };	// Raw data from the external sensors hooked to one of the GPIO's.
static constexpr Format DISPARITY32		{ 4, 				"DISPARITY32", 		false };	// 32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth.
static constexpr Format DOF6			{ 3 * 4 + 4 * 4,	"DOF6", 			false };	// Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion (w0, w1, w2, w3 || w, x, y, z).
static constexpr Format INZI			{ 0, 				"INZI", 			false };	// multi-planar Depth 16bit + IR 10bit.

// custom
static constexpr Format USER_DATA		{ 0, 				"USER_DATA", 		false };	// User data with name and any value.
static constexpr Format MESH			{ 0, 				"MESH", 			false };	// Mesh consist of shapes of vertices with indexes.
static constexpr Format POINT			{ 0, 				"POINT",		 	false };	// Point consist of x, y and z position, rgb color and depth .

// clang-format on
} // namespace format

/////
///// \brief The Format enum
///// describes the atomic unit measured by a sensor.
///// This information is used to determine how this data can be viewed/used/processed.
///// This unit has a fixed size in memory.
/////
//// clang-format off
// enum class Format  {
//     NONE = 0,		/**< Not a valid input : intended to check the non initialization */
//     Z16, 			/**< 16-bit linear depth values. The depth is meters is equal to depth scale
//     pixel value. */ DISPARITY16, 	/**< 16-bit float-point disparity values. Depth->Disparity
//     conversion : Disparity = Baseline*FocalLength/Depth. */ XYZ32F,      	/**< 32-bit floating
//     point 3D coordinates. */ YUYV,        	/**< 32-bit y0, u, y1, v data for every two pixels.
//     Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV */
//     RGB8,        	/**< 8-bit red, green and blue channels */
//     BGR8,        	/**< 8-bit blue, green, and red channels -- suitable for OpenCV */
//     RGBA8,       	/**< 8-bit red, green and blue channels + constant alpha channel equal to FF
//     */ BGRA8,       	/**< 8-bit blue, green, and red channels + constant alpha channel equal to
//     FF */ Y8,          	/**< 8-bit per-pixel grayscale image */ Y16,         	/**< 16-bit
//     per-pixel grayscale image */ RAW10,       	/**< Four 10 bits per pixel luminance values
//     packed into a 5-byte macropixel */ RAW16,       	/**< 16-bit raw image */ RAW8, /**< 8-bit
//     raw image */ UYVY,        	/**< Similar to the standard YUYV pixel format, but packed in a
//     different order */ MOTION_RAW,  	/**< Raw data from the motion sensor */ MOTION_XYZ32F, /**<
//     Motion data packed as 3 32-bit float values, for X, Y, and Z axis */ GPIO_RAW,      	/**< Raw
//     data from the external sensors hooked to one of the GPIO's */ DISPARITY32,   	/**< 32-bit
//     float-point disparity values. Depth->Disparity conversion : Disparity =
//     Baseline*FocalLength/Depth */ DOF6,     		/**< Pose data packed as floats array,
//     containing translation vector (x, y, z), rotation quaternion (w0, w1, w2, w3 || w, x, y, z)
//     */
//     Y10BPACK, 		/**< 16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed
//     ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits */
//     DISTANCE, 		/**< 32-bit float-point depth distance value.  */
//     MJPEG,    		/**< Bitstream encoding for video in which an image of each frame is encoded
//     as JPEG-DIB */ Y8I,      		/**< 8-bit per pixel interleaved. 8-bit left, 8-bit right.
//     */ Y12I,     		/**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel
//     is stored in a 24-bit word in little-endian order. */ INZI,     		/**< multi-planar Depth
//     16bit + IR 10bit.  */ INVI,     		/**< 8-bit IR stream.  */
//     W10,      		/**< Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 *
//     byte
//     */ Z16H,     		/**< Variable-length Huffman-compressed 16-bit depth values. */ FG,
//     /**< 16-bit per-pixel frame grabber format. */ Y411,     		/**< 12-bit per-pixel. */
//     MAT4,     		/**< Transform matrix 4x4 of float. */
//     USER_DATA,		/**< User data with name and any value */
//     MESH,			/**< Mesh consist of shapes of vertices with indexes */
//     POINT,			/**< Point consist of x, y and z position, rgb color and depth  */
//     DENSITY,		/**< 32-bit density values. For MRI, CT scan and US representations. */
//     COUNT     		/**< Number of enumeration values. Not a valid input: intended to be used in
//     for-loops. */
// };
//// clang-format on

////constexpr uint64_t FormatImg = ( (uint64_t)Format::RGB8 | (uint64_t)Format::Y8  );

/////
///// \brief operator <<
///// \param os
///// \param format
///// \return
/////
// SRC_API std::ostream& operator<<( std::ostream& os, const Format& format );

// namespace format {

// static inline constexpr std::string& to_string( const Format& format );
// static inline constexpr size_t getSize( const Format& format ) noexcept;
// static inline constexpr bool isInterpolable( const Format& format ) noexcept;
// static inline constexpr bool hasFixedSize( const Format& format ) noexcept;
// static inline constexpr bool isImage(const Format& format) noexcept;

// static constexpr int s_format2nByte[static_cast<int>( Format::COUNT )] = {
//     0,       // NONE
//     2,       // Z16
//     2,       // DISPARITY16
//     3 * 4,   // XYZ32F
//     4,       // YUYV
//     3,       // RGB8
//     3,       // BGR8
//     4,       // RGBA8
//     4,       // BGRA8
//     1,       // Y8
//     2,       // Y16
//     2,       // RAW10
//     2,       // RAW16
//     1,       // RAW8
//     2,       // UYVY
//     1,       // MOTION_RAW
//     4,       // MOTION_XYZ32F
//     1,       // GPIO_RAW
//     4,       // DISPARITY32
//     12 + 16, // DOF6
//     2,       // Y10BPACK
//     4,       // DISTANCE
//     1,       // MJPEG
//     2,       // Y8I
//     3,       // Y12I
//     1,       // INZI
//     1,       // INVI
//     2,       // W10
//     2,       // Z16H
//     2,       // FG
//     2,       // Y411
//     64,      // MAT4
//     80 + 4,  // USER_DATA (non constant), string size ~= 80 and 4 bytes for common use int value
//     80 + 4,  // MESH (non constant), string size ~= 80 and 4 bytes for common use int value
//     12 + 16, // POINT
//     4,       // DENSITY

//};

// inline constexpr size_t getSize( const Format& format ) noexcept {

//    return s_format2nByte[static_cast<int>( format )];
//}

// static constexpr bool s_format2isInterpolable[static_cast<int>( Format::COUNT )] = {
//     false, // NONE
//     false, // Z16
//     false, // DISPARITY16
//     false, // XYZ32F
//     false, // YUYV
//     false, // RGB8
//     false, // BGR8
//     false, // RGBA8
//     false, // BGRA8
//     false, // Y8
//     false, // Y16
//     false, // RAW10
//     false, // RAW16
//     false, // RAW8
//     false, // UYVY
//     false, // MOTION_RAW
//     false, // MOTION_XYZ32F
//     false, // GPIO_RAW
//     false, // DISPARITY32
//     true,  // DOF6
//     false, // Y10BPACK
//     false, // DISTANCE
//     false, // MJPEG
//     false, // Y8I
//     false, // Y12I
//     false, // INZI
//     false, // INVI
//     false, // W10
//     false, // Z16H
//     false, // FG
//     false, // Y411
//     false, // MAT4
//     false, // USER_DATA
//     false, // MESH
//     true,  // POINT
//     false, // DENSITY
// };

// inline constexpr bool isInterpolable( const Format& format ) noexcept {
//     return s_format2isInterpolable[(int)format];
// }

// static std::string format2stringArray[static_cast<int>( Format::COUNT )] = {
//     "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
//     "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
//     "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
//     "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
//     "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
//     "Y411",        "MAT4",  "USER_DATA",   "MESH",       "POINT",         "DENSITY",
// };

// static inline constexpr std::string& to_string( const Format& format ) {
//     return format2stringArray[(int)format];
// }

// static constexpr bool s_format2hasFixedSize[static_cast<int>( Format::COUNT )] = {
//     true,  // NONE
//     true,  // Z16
//     true,  // DISPARITY16
//     true,  // XYZ32F
//     true,  // YUYV
//     true,  // RGB8
//     true,  // BGR8
//     true,  // RGBA8
//     true,  // BGRA8
//     true,  // Y8
//     true,  // Y16
//     true,  // RAW10
//     true,  // RAW16
//     true,  // RAW8
//     true,  // UYVY
//     true,  // MOTION_RAW
//     true,  // MOTION_XYZ32F
//     true,  // GPIO_RAW
//     true,  // DISPARITY32
//     true,  // DOF6
//     true,  // Y10BPACK
//     true,  // DISTANCE
//     true,  // MJPEG
//     true,  // Y8I
//     true,  // Y12I
//     true,  // INZI
//     true,  // INVI
//     true,  // W10
//     true,  // Z16H
//     true,  // FG
//     true,  // Y411
//     true,  // MAT4
//     false, // USER_DATA
//     false, // MESH
//     true,  // POINT
//     true,  // DENSITY
// };

// inline constexpr bool hasFixedSize( const Format& format ) noexcept {
//     return s_format2hasFixedSize[(int)format];
// }

// static constexpr bool s_formatIsImage[static_cast<int>( Format::COUNT )] = {
//     false,       // NONE
//     true,       // Z16
//     false,       // DISPARITY16
//     false,   // XYZ32F
//     false,       // YUYV
//     true,       // RGB8
//     true,       // BGR8
//     true,       // RGBA8
//     true,       // BGRA8
//     true,       // Y8
//     true,       // Y16
//     false,       // RAW10
//     false,       // RAW16
//     false,       // RAW8
//     false,       // UYVY
//     false,       // MOTION_RAW
//     false,       // MOTION_XYZ32F
//     false,       // GPIO_RAW
//     false,       // DISPARITY32
//     false, // DOF6
//     true,       // Y10BPACK
//     false,       // DISTANCE
//     true,       // MJPEG
//     true,       // Y8I
//     true,       // Y12I
//     false,       // INZI
//     false,       // INVI
//     false,       // W10
//     true,       // Z16H
//     false,       // FG
//     true,       // Y411
//     false,      // MAT4
//     false,  // USER_DATA (non constant), string size ~= 80 and 4 bytes for common use int value
//     false,  // MESH (non constant), string size ~= 80 and 4 bytes for common use int value
//     false, // POINT
//     false,       // DENSITY
// };

// static inline constexpr bool isImage( const Format& format ) noexcept {
//     return s_formatIsImage[static_cast<int>( format )];
// }

//} // namespace format
} // namespace sensor
} // namespace hub
