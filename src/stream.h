
#ifndef STREAM_H
#define STREAM_H

#include <any>
#include <IOStream.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "socket.h"
#include "export.h"


class InputStream;

class Stream {
public:
    using MetaData = std::map<std::string, std::any>; // C++17

    class exception : public std::runtime_error {
    public:
        explicit exception(const char* const message) throw()
            : std::runtime_error(message)
        {
        }
        const char* what() const throw()
        {
            return std::runtime_error::what();
        }
    };

    enum class Format {
        NONE,
        Z16, /**< 			16-bit linear depth values. The depth is meters is equal to depth scale * pixel value. */
        DISPARITY16, /**< 	16-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
        XYZ32F, /**< 		32-bit floating point 3D coordinates. */
        YUYV, /**< 			32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV */
        RGB8, /**< 			8-bit red, green and blue channels */
        BGR8, /**< 			8-bit blue, green, and red channels -- suitable for OpenCV */
        RGBA8, /**< 		8-bit red, green and blue channels + constant alpha channel equal to FF */
        BGRA8, /**< 		8-bit blue, green, and red channels + constant alpha channel equal to FF */
        Y8, /**< 			8-bit per-pixel grayscale image */
        Y16, /**< 			16-bit per-pixel grayscale image */
        RAW10, /**< 		Four 10 bits per pixel luminance values packed into a 5-byte macropixel */
        RAW16, /**< 		16-bit raw image */
        RAW8, /**< 			8-bit raw image */
        UYVY, /**< 			Similar to the standard YUYV pixel format, but packed in a different order */
        MOTION_RAW, /**< 	Raw data from the motion sensor */
        MOTION_XYZ32F, /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
        GPIO_RAW, /**< 		Raw data from the external sensors hooked to one of the GPIO's */
        DISPARITY32, /**<	32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth */
        DOF6, /**< 			Pose data packed as floats array, containing translation vector (x, y, z), rotation quaternion (x, y, z, w) */
        Y10BPACK, /**< 		16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits */
        DISTANCE, /**< 		32-bit float-point depth distance value.  */
        MJPEG, /**< 		Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB   */
        Y8I, /**< 			8-bit per pixel interleaved. 8-bit left, 8-bit right.  */
        Y12I, /**< 			12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order. */
        INZI, /**< 			multi-planar Depth 16bit + IR 10bit.  */
        INVI, /**< 			8-bit IR stream.  */
        W10, /**< 			Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 bytes */
        Z16H, /**< 			Variable-length Huffman-compressed 16-bit depth values. */
        FG, /**< 			16-bit per-pixel frame grabber format. */
        Y411, /**< 			12-bit per-pixel. */
        COUNT /**< 			Number of enumeration values. Not a valid input: intended to be used in for-loops. */
    };

    class SRC_API Acquisition {
    public:
        Acquisition(long long backendTimestamp, long long backendTimeOfArrival, const unsigned char* const data, size_t size);
        ~Acquisition();

        Acquisition(const Acquisition& acq) = delete;
        Acquisition(Acquisition&& acq) noexcept;

        Acquisition& operator=(const Acquisition& acq) = delete;
        Acquisition& operator=(Acquisition&& acq) = delete;
        Acquisition clone() const;

        const long long mBackendTimestamp; // microseconds
        const long long mBackendTimeOfArrival; // microseconds
        const unsigned char* const mData;
        const size_t mSize;

    private:
        bool mIsMoved = false;

    public:
        SRC_API friend std::ostream& operator<<(std::ostream& os, const Acquisition& acq);
    };


protected:
    SRC_API Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream& ioStream);
    SRC_API ~Stream();

    Stream(const Stream& stream) = delete;
    Stream(Stream& stream) = delete;
    Stream(const Stream&& stream) = delete;
    Stream(Stream&& stream) = delete;

    Stream& operator=(const Stream& stream) = delete;
    Stream& operator=(Stream& stream) = delete;
    Stream& operator=(const Stream&& stream) = delete;
    Stream& operator=(Stream&& stream) = delete;

protected:
    SRC_API static size_t computeAcquisitionSize(Format format, const std::vector<int>& dims);

protected:
    std::string mSensorName = "";
    Format mFormat = Format::NONE;
    std::vector<int> mDims;

    IOStream& mIOStream;
    size_t mAcquisitionSize;

public:
    SRC_API const std::string& getSensorName() const;
    SRC_API Format getFormat() const;
    SRC_API const std::vector<int>& getDims() const;

    IOStream& getIOStream() const;
    SRC_API size_t getAcquisitionSize() const;

    SRC_API static std::string dims2string( const std::vector<int>& dims );
    static std::string metaData2string( const MetaData& metaData, bool expand = false );
	SRC_API static std::string any2string( const std::any& any );
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Stream& stream );
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Format& format );
};

//////////////////////////// InputStream ///////////////////////////////////////

class OutputStream;

class InputStream : public Stream {
public:
    SRC_API InputStream(const std::string& sensorName, const std::string& syncSensorName = "");
    template <class IOStreamT>
    InputStream(IOStreamT&& ioStream);
    template <class IOStreamT>
    InputStream(IOStreamT& ioStream) = delete;

    SRC_API ~InputStream();

public:
    SRC_API Acquisition getAcquisition() const;
    SRC_API std::vector<Acquisition> getAllAcquisition();


private:
    MetaData mMetaData;

public:
    SRC_API const MetaData& getMetaData() const;
    friend std::ostream& operator<<( std::ostream& os, const InputStream& inputStream );
};

template <class IOStreamT>
InputStream::InputStream(IOStreamT&& ioStream)
    : Stream("", Format::NONE, {}, *std::move(new IOStreamT(std::move(ioStream))))
{

    mIOStream.read(mSensorName);
    mIOStream.read(mFormat);
    mIOStream.read(mDims);
    mIOStream.read(mMetaData);

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}

//////////////////////////// OutputStream ///////////////////////////////////////

class OutputStream : public Stream {
public:
    SRC_API OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, ClientSocket&& ioStream = ClientSocket(), const MetaData& metaData = {});
    template <class IOStreamT>
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStreamT&& ioStream, const MetaData& metaData = {});
    template <class IOStreamT>
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStreamT& ioStream, const MetaData& metaData = {}) = delete;

    OutputStream(const OutputStream& outputStream) = delete;
    OutputStream(OutputStream&& outputStream) = delete;

public:
    SRC_API void operator<<(const Acquisition& acquisition) const;

private:
};

template <class IOStreamT>
OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStreamT&& ioStream, const MetaData& metaData)
    : Stream(sensorName, format, dims, *std::move(new IOStreamT(std::move(ioStream))))
{
    mIOStream.setupOutput(sensorName);

    mIOStream.write(mSensorName);
    mIOStream.write(mFormat);
    mIOStream.write(mDims);
    mIOStream.write(metaData);
}

#endif // STREAM_H
