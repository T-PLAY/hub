
#pragma once

// #include <numeric>
// #include <string>
#include "core/Vector.hpp"
#include <sstream>
#include <vector>

#include <algorithm>

#include "Format.hpp"
#include "core/Macros.hpp"

//#include "io/input/Input.hpp"
//#include "io/output/Output.hpp"

namespace hub {
namespace sensor {

// using Resolution = std::pair<Dims, Format>;

class Resolution
{
  public:
    using Dims = std::vector<int>;

    // io
    //    static constexpr Size_t ioGetSize() { return sizeof( Measure ) - sizeof( Data_t* ); }
    //    const Data_t* ioGetData() const { return (Data_t*)&m_format; }
    //    void ioSetData( const Data_t* data, Size_t size ) { memcpy( &m_format, data, size ); }
    //    constexpr std::string ioTypeName() const { return "Measure"; }
    // end io

    Resolution() = default;

    template <typename Dim>
    Resolution( Format format, Dim&& dim ) : m_format( format ) {
        m_dims.emplace_back( std::forward<Dim>( dim ) );
        //        std::reverse(m_dims.begin(), m_dims.end());
    }

    template <typename Dim, typename... Dims>
    Resolution( Format format, Dim&& dim, Dims&&... dims ) : Resolution( format, dims... ) {
        //        m_dims.emplace_back(std::forward<Dim>(dim));
        m_dims.insert( m_dims.begin(), dim );
        //        Resolution(format, dims...);
    }

//    Resolution(Resolution &&) = default;

    //    Resolution() = default;

  public:
    constexpr auto nDim() const { return m_dims.size(); }
    //    constexpr std::string name() const {
    //        std::string str = "<";
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

    constexpr auto getDim( int iDim ) const { return m_dims.at( iDim ); }
    constexpr auto getFormat() const { return m_format; }

    constexpr auto nByte() const {
        auto size = m_format.nByte;
        for ( int i = 0; i < nDim(); ++i ) {
            size *= m_dims.at( i );
        }
        return size;
    }

    constexpr bool operator==( const Resolution& resolution ) const {
        return m_format == resolution.m_format && m_dims == resolution.m_dims;
    }

    //    void setData( Data_t* const data ) {
    //        assert( data != nullptr );
    //        m_data = data;
    //    }
    //    void setData( const Data_t* const data, Size_t len ) {
    //        assert( m_data != nullptr );
    //        assert( nByte() == len );
    //        assert( data != nullptr );
    //        std::memcpy( m_data, data, len );
    //    }

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Resolution& resolution );
    //  private:

    //    static constexpr auto s_maxDim = 3;
    //    Data_t* m_data               = nullptr;
    //    template <typename Output>
    //    friend Output& operator<<( Output& output, const Resolution& resolution );

    //    template <typename Input>
    //    friend Input& operator>>( Input& input, Resolution& resolution );

//  private:
    template <typename Input>
    void read( Input& input ) {
        input.read( m_format );
        input.read( m_dims );
    }

    template <typename Output>
    void write( Output& output ) const {
        output.write( m_format );
        output.write( m_dims );
    }

    //    void isReadable();

  private:
    Format m_format;
    //    std::vector<Size_t> m_dims;
    Dims m_dims;

    //    Size_t m_nDim = 0;
    //    Size_t m_dims[s_maxDim];
};

// template <typename Output>
// Output& operator<<( Output& output, const Resolution& resolution );

// template <typename Input>
// Input& operator>>( Input& input, Resolution& resolution );

/////////////////////////////////////////////////////////////

using Resolutions = std::vector<Resolution>;
std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );
std::string toString( const Resolutions& resolutions );
constexpr Size_t nByte( const Resolutions& resolutions );

constexpr Size_t nByte(const Resolutions &resolutions) {
    Size_t size = 0;
    for (const auto & resolution : resolutions) {
        size += resolution.nByte();
    }
    return size;
}

// class Resolutions {
//   public:

////    template <typename... T>
////    Resolutions(T&&... t)
////        : m_resolutions(std::forward<std::vector<Resolution>>(t...)) {
////    }
//    Resolutions(std::initializer_list<Resolution> && resolutions)
//        : m_resolutions(std::forward<std::vector<Resolution>>(resolutions))
//    {
//    }

//    Resolutions() = default;

//  public:
//    constexpr auto nByte() const {
//        auto size = 0;
//        for (const auto & resolution : m_resolutions) {
//            size += resolution.nByte();
//        }
//        return size;
//    }

//    auto & operator()() {
//        return m_resolutions;
//    }

//    const auto & operator()() const {
//        return m_resolutions;
//    }

//    friend std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );

//    std::string toString() const {
//        std::stringstream ss;
//        ss << *this;
//        return ss.str();
//    }
////    auto empty() const {
////        return m_resolutions.empty();
////    }
//    template <typename Output>
//    friend Output& operator<<( Output& output, const Resolutions& resolutions );

//    template <typename Input>
//    friend Input& operator>>( Input& input, Resolutions& resolutions );

//    std::vector<Resolution> m_resolutions;
//};

// template <typename Output>
// Output& operator<<( Output& output, const Resolutions& resolutions ) {
////    output.isOutput();
////    output << resolutions.m_resolutions;
//    for (const auto & resolution : resolutions.m_resolutions) {
//        output << resolution;
//    }
////    output << resolutions.m_resolutions.at(0);
//    return output;
//}

// template <typename Input>
// Input& operator>>( Input& input, Resolutions& resolutions ) {
//     input.isInput();
////    input >> resolutions.m_resolutions;
//    return input;
//}

// std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );

/////
///// \brief The Format enum
///// describes the atomic unit measured by a sensor.
///// This information is used to determine how this data can be viewed/used/processed.
///// This unit has a fixed size in memory.
/////
//// clang-format off
// enum class Format {
//     NONE = 0,		/**< Not a valid input : intended to check the non initialization */
//     Z16, 			/**< 16-bit linear depth values. The depth is meters is equal to depth scale pixel
//     value. */ DISPARITY16, 	/**< 16-bit float-point disparity values. Depth->Disparity
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
//     MJPEG,    		/**< Bitstream encoding for video in which an image of each frame is encoded as
//     JPEG-DIB */ Y8I,      		/**< 8-bit per pixel interleaved. 8-bit left, 8-bit right. */
//     Y12I,     		/**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is
//     stored in a 24-bit word in little-endian order. */ INZI,     		/**< multi-planar Depth
//     16bit + IR 10bit.  */ INVI,     		/**< 8-bit IR stream.  */
//     W10,      		/**< Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 * byte
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

/////
///// \brief
///// Describes the N-dimensional lengths.
/////
// using Dims = std::vector<int>;

/////
///// \brief
///// Corresponds to n-dimensional lengths with a fixed format of the data.
///// Therefore, the total vectorised data has a fixed size in memory (TCP datagram).
/////
// using Resolution = std::pair<Dims, Format>;

/////
///// \brief
///// Package of resolution.
///// A single sensor can provide several types of information at the same time,
///// for the same acquisition time (data with the exact same time stamp).
/////
// using Resolutions = std::vector<Resolution>;

/////
///// \brief operator <<
///// \param os
///// \param format
///// \return
/////
// SRC_API std::ostream& operator<<( std::ostream& os, const Format& format );

/////
///// \brief operator <<
///// \param os
///// \param dims
///// \return
/////
// SRC_API std::ostream& operator<<( std::ostream& os, const Dims& dims );

/////
///// \brief operator <<
///// \param os
///// \param resolution
///// \return
/////
// SRC_API std::ostream& operator<<( std::ostream& os, const Resolution& resolution );

/////
///// \brief operator <<
///// \param os
///// \param resolutions
///// \return
/////
// SRC_API std::ostream& operator<<( std::ostream& os, const Resolutions& resolutions );

// namespace resolution {

// #define HUB_TO_STRING to_string

// static std::string HUB_TO_STRING( const Dims& dims );
// static inline constexpr std::string& HUB_TO_STRING( const Format& format );
// static std::string HUB_TO_STRING( const Resolutions& resolutions );
// static std::string HUB_TO_STRING( const Resolution& resolution );

// static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolutions& resolutions )
// noexcept; static inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolution& resolution )
// noexcept; static inline CONSTEXPR20 size_t computeAcquisitionSize( Format format, const Dims&
// dims ) noexcept; static inline constexpr int format2nByte( const Format& format ) noexcept;
// static inline constexpr bool format2isInterpolable( const Format& format ) noexcept;
// static inline constexpr bool format2hasFixedSize( const Format& format ) noexcept;

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

// inline constexpr int format2nByte( const Format& format ) noexcept {

//    return s_format2nByte[static_cast<int>( format )];
//}

// inline CONSTEXPR20 size_t computeAcquisitionSize( Format format, const Dims& dims ) noexcept {
//     return std::accumulate( dims.cbegin(), dims.cend(), 1, std::multiplies<int> {} ) *
//            s_format2nByte[static_cast<int>( format )];
// }

// inline size_t CONSTEXPR20 computeAcquisitionSize( const Resolutions& resolutions ) noexcept {
//     return std::accumulate(
//         resolutions.cbegin(), resolutions.cend(), 0, []( size_t size, const Resolution&
//         resolution ) {
//             return size + computeAcquisitionSize( resolution );
//         } );
// }

// inline CONSTEXPR20 size_t computeAcquisitionSize( const Resolution& resolution ) noexcept {
//     const auto& dims   = resolution.first;
//     const auto& format = resolution.second;
//     return computeAcquisitionSize( format, dims );
// }

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

// inline constexpr bool format2isInterpolable( const Format& format ) noexcept {
//     return s_format2isInterpolable[(int)format];
// }

////////////////////////////////////////////////////////////////////////////////

// static std::string HUB_TO_STRING( const Dims& dims ) {
//     std::string str = "";
//     for ( unsigned int i = 0; i < dims.size(); ++i ) {
//         str += std::to_string( dims[i] );
//         if ( i != dims.size() - 1 ) { str += " x "; }
//     }
//     return str;
// }

// static std::string format2stringArray[static_cast<int>( Format::COUNT )] = {
//     "NONE",        "Z16",   "DISPARITY16", "XYZ32F",     "YUYV",          "RGB8",
//     "BGR8",        "RGBA8", "BGRA8",       "Y8",         "Y16",           "RAW10",
//     "RAW16",       "RAW8",  "UYVY",        "MOTION_RAW", "MOTION_XYZ32F", "GPIO_RAW",
//     "DISPARITY32", "6DOF",  "Y10BPACK",    "DISTANCE",   "MJPEG",         "Y8I",
//     "Y12I",        "INZI",  "INVI",        "W10",        "Z16H",          "FG",
//     "Y411",        "MAT4",  "USER_DATA",   "MESH",       "POINT",         "DENSITY",
// };

// static inline constexpr std::string& HUB_TO_STRING( const Format& format ) {
//     return format2stringArray[(int)format];
// }

// static std::string HUB_TO_STRING( const Resolution& resolution ) {
//     const auto& dims   = resolution.first;
//     const auto& format = resolution.second;
//     return "{" + HUB_TO_STRING( dims ) + " : " + HUB_TO_STRING( format ) + "}";
// }

// static std::string HUB_TO_STRING( const Resolutions& resolutions ) {
//     // static std::string resolutions2string( const Resolutions& resolutions ) {
//     const int size  = static_cast<int>( resolutions.size() );
//     std::string str = "";
//     str += "(";
//     for ( int i = 0; i < size; ++i ) {
//         const auto& resolution = resolutions.at( i );
//         str += HUB_TO_STRING( resolution );

//        if ( i != size - 1 ) str += ", ";
//    }
//    str += ")";
//    return str;
//}

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

// inline constexpr bool format2hasFixedSize( const Format& format ) noexcept {
//     return s_format2hasFixedSize[(int)format];
// }

//} // namespace resolution

} // namespace sensor
} // namespace hub
