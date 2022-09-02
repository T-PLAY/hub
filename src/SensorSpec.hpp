#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>

#include "Macros.hpp"

namespace hub {

///
/// \brief The SensorSpec class
/// defines the internal properties of a sensor as the format of the acquired data which
/// allows to define how to visualize the data. Also the dimension of the data as well as the
/// metadata.
///
class SRC_API SensorSpec
{
  public:
    using MetaData = std::map<std::string, std::any>; // any -> C++17
    using Dims     = std::vector<int>;

    // clang-format off
    enum class Format {
        NONE,
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
        MAT4,     		/**< Transform matrix 4x4 float. */
        COUNT     		/**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
    };
    // clang-format on

    using Resolution  = std::pair<Dims, Format>;
    using Resolutions = std::vector<Resolution>;

    SensorSpec( const std::string& sensorName = "",
                        //                        Format format                 = Format::NONE,
                        //                        const Dims& dims              = {},
                        const Resolutions& resolutions = {},
                        const MetaData& metaData       = {} ) :
        m_sensorName( sensorName ),
        //        m_format( format ),
        //        m_dims( dims ),
        m_resolutions( resolutions ),
        m_metaData( metaData ),
        m_acquisitionSize( computeAcquisitionSize( resolutions ) ) {};

  public:
     static size_t computeAcquisitionSize( const Resolutions& resolutions );
     static size_t computeAcquisitionSize( const Resolution& resolution );

  private:
     static size_t computeAcquisitionSize( Format format, const Dims& dims );

  private:
  public:
    std::string m_sensorName;
    //    Format m_format;
    //    Dims m_dims;
    Resolutions m_resolutions;
    MetaData m_metaData;

    size_t m_acquisitionSize;

  public:
     static std::string dims2string( const Dims& dims );
     static std::string format2string( const Format& format );
     static std::string resolutions2string( const Resolutions& resolutions );
     static std::string metaData2string( const MetaData& metaData, bool expand = false );
     static std::string metaData2string( const std::pair<std::string, std::any>& metaData );
     SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );
};

} // namespace hub
