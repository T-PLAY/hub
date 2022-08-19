#pragma once

#include <any>
#include <map>
#include <string>
#include <vector>

//#include "IO/Interface.hpp"
#include "Macros.hpp"

namespace hub {

class SensorSpec
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
        DOF6,     		/**< Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion (x, y, z, w) */
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
        COUNT     		/**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
    };
    // clang-format on

    SRC_API SensorSpec( const std::string& pSensorName = "",
                        Format pFormat = Format::NONE,
                        const Dims& pDims = {},
                        const MetaData& pMetaData = {} ) :
        sensorName( pSensorName ),
        format( pFormat ),
        dims( pDims ),
        metaData( pMetaData ),
        acquisitonSize( computeAcquisitionSize( pFormat, pDims ) ) {};

    //        acquisitonSize( computeAcquisitionSize( format, dims ) ) {};

    // SRC_API void write( io::Interface& iostream ) const {
    //     iostream.write( sensorName );
    //     iostream.write( format );
    //     iostream.write( dims );
    //     iostream.write( metaData );
    // };

    // SRC_API void read( const io::Interface& iostream ) {
    //     iostream.read( sensorName );
    //     iostream.read( format );
    //     iostream.read( dims );
    //     iostream.read( metaData );

    //    acquisitonSize = computeAcquisitionSize( format, dims );
    //};

  private:
    SRC_API static size_t computeAcquisitionSize( Format format, const Dims& dims );

  public:
    std::string sensorName;
    Format format;
    Dims dims;
    MetaData metaData;

    size_t acquisitonSize;

  public:
    // friend class Stream;
    //    SRC_API const std::string& getSensorName() const;
    //    SRC_API Format getFormat() const;
    //    SRC_API const Dims& getDims() const;
    //    SRC_API const MetaData& getMetaData() const;

    //    SRC_API size_t getAcquisitionSize() const;

    SRC_API static std::string dims2string( const Dims& dims );
    SRC_API static std::string format2string( const Format& format );
    SRC_API static std::string metaData2string( const MetaData& metaData, bool expand = false );
    SRC_API static std::string any2string( const std::any& any );
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );
};

} // namespace hub
