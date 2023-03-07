
#pragma once

#include <string>
#include <vector>

#include "Macros.hpp"

namespace hub {

///
/// \brief The Format enum
/// describes the atomic unit measured by a sensor.
/// This information is used to determine how this data can be viewed/used/processed.
/// This unit has a fixed size in memory.
///
// clang-format off
enum class Format {
    NONE = 0,		/**< Not a valid input : intended to check the non initialization */
    Z16, 			/**< 16-bit linear depth values. The depth is meters is equal to depth scale pixel value. */
    DISPARITY16, 	/**< 16-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
    XYZ32F,      	/**< 32-bit floating point 3D coordinates. */
    YUYV,        	/**< 32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV */
    RGB8,        	/**< 8-bit red, green and blue channels */
    BGR8,        	/**< 8-bit blue, green, and red channels -- suitable for OpenCV */
    RGBA8,       	/**< 8-bit red, green and blue channels + constant alpha channel equal to FF */
    BGRA8,       	/**< 8-bit blue, green, and red channels + constant alpha channel equal to FF */
    Y8,          	/**< 8-bit per-pixel grayscale image */
    Y16,         	/**< 16-bit per-pixel grayscale image */
    //        Y16 = Z16,         	/**< 16-bit per-pixel grayscale image */
    RAW10,       	/**< Four 10 bits per pixel luminance values packed into a 5-byte macropixel */
    RAW16,       	/**< 16-bit raw image */
    RAW8,        	/**< 8-bit raw image */
    UYVY,        	/**< Similar to the standard YUYV pixel format, but packed in a different order */
    MOTION_RAW,  	/**< Raw data from the motion sensor */
    MOTION_XYZ32F, 	/**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
    GPIO_RAW,      	/**< Raw data from the external sensors hooked to one of the GPIO's */
    DISPARITY32,   	/**< 32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth */
    DOF6,     		/**< Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion (w0, w1, w2, w3 || w, x, y, z) */
    Y10BPACK, 		/**< 16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits */
    DISTANCE, 		/**< 32-bit float-point depth distance value.  */
    MJPEG,    		/**< Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB */
    Y8I,      		/**< 8-bit per pixel interleaved. 8-bit left, 8-bit right. */
    Y12I,     		/**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order. */
    INZI,     		/**< multi-planar Depth 16bit + IR 10bit.  */
    INVI,     		/**< 8-bit IR stream.  */
    W10,      		/**< Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 * byte */
    Z16H,     		/**< Variable-length Huffman-compressed 16-bit depth values. */
    FG,       		/**< 16-bit per-pixel frame grabber format. */
    Y411,     		/**< 12-bit per-pixel. */
    MAT4,     		/**< Transform matrix 4x4 of float. */
    USER_DATA,		/**< User data with name and any value */
    COUNT     		/**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
};
// clang-format on

///
/// \brief
/// Describes the N-dimensional lengths.
///
using Dims = std::vector<int>;

///
/// \brief
/// Corresponds to n-dimensional lengths with a fixed format of the data.
/// Therefore, the total vectorised data has a fixed size in memory (TCP datagram).
///
using Resolution = std::pair<Dims, Format>;

///
/// \brief
/// Package of resolution.
/// A single sensor can provide several types of information at the same time,
/// for the same acquisition time (data with the exact same time stamp).
///
using Resolutions = std::vector<Resolution>;

//    static const Resolution s_Atomic_Resolution_Dof6 = Resolution {{1}, Format::DOF6};

#define HUB_TO_STRING to_string

static std::string HUB_TO_STRING( const Dims& dims );
static inline constexpr std::string& HUB_TO_STRING( const Format& format );
static std::string HUB_TO_STRING( const Resolutions& resolutions );
static std::string HUB_TO_STRING( const Resolution& resolution );
// static std::string to_string( const std::string & string );

//    static std::string metaData2string( const MetaData& metaData, bool expand = false );
//    static std::string metaData2string( const std::pair<std::string, std::any>& metaData );
static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolutions& resolutions ) noexcept;
static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolution& resolution ) noexcept;
static inline CONSTEXPR20 size_t computeAcquisitionSize( Format format, const Dims& dims ) noexcept;
static inline constexpr int format2nByte( const Format& format ) noexcept;
static inline constexpr bool format2isInterpolable( const Format& format ) noexcept;
static inline constexpr bool format2hasFixedSize( const Format& format) noexcept;

///
/// \brief operator <<
/// \param os
/// \param format
/// \return
///
SRC_API std::ostream& operator<<( std::ostream& os, const Format& format );
SRC_API std::ostream& operator<<( std::ostream& os, const Dims& dims );
//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );

SRC_API std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
SRC_API std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );

// template <class T = Dims>
// SRC_API std::ostream& operator<<( std::ostream& os, const T& t ) {
//    os << hub::to_stringe(t);
//    return os;
//}

// static std::string(Resolutions & resolutions ) {
//}

} // namespace hub

#include <Resolution.inl>
