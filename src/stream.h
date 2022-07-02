
#ifndef STREAM_H
#define STREAM_H

#include <any>
//#include <socket.h>
#include <IOStream.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include <socket.h>
//#include <FileIO.h>
//#include <RamIO.h>

// namespace std {
// namespace any {

namespace any {

static std::string to_string(const std::any& any)
{
    assert(any.has_value());
    const auto& hashCode = any.type().hash_code();

    if (hashCode == typeid(double).hash_code()) {
        const double* val = std::any_cast<double>(&any);
        return std::to_string(*val);

    } else if (hashCode == typeid(std::string).hash_code()) {
        const std::string* val = std::any_cast<std::string>(&any);
        return std::string(*val);

    } else if (hashCode == typeid(const char*).hash_code()) {
        const char* val = *std::any_cast<const char*>(&any);
        return std::string(val);

    } else if (hashCode == typeid(int).hash_code()) {
        const int* val = std::any_cast<int>(&any);
        return std::to_string(*val);


    } else {
        auto name = any.type().name();
        auto raw_name = any.type().name();
        assert(false);
    }
    return "";
}

} // any

// template <typename T>
// T get(const std::any& any)
//{

//}

// friend std::ostream& operator<<(std::ostream& os, const std::any& any)

//}
//}

class InputStream;
//#define DEBUG_STREAM

class Stream {
public:
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
        DOF6, /**< 			Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
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

    static constexpr char const* format2string[static_cast<int>(Format::COUNT)] = {
        "NONE",
        "Z16",
        "DISPARITY16",
        "XYZ32F",
        "YUYV",
        "RGB8",
        "BGR8",
        "RGBA8",
        "BGRA8",
        "Y8",
        "Y16",
        "RAW10",
        "RAW16",
        "RAW8",
        "UYVY",
        "MOTION_RAW",
        "MOTION_XYZ32F",
        "GPIO_RAW",
        "DISPARITY32",
        "DOF6",
        "Y10BPACK",
        "DISTANCE",
        "MJPEG",
        "Y8I",
        "Y12I",
        "INZI",
        "INVI",
        "W10",
        "Z16H",
        "FG",
        "Y411",
        // COUNT
    };
    friend std::ostream& operator<<(std::ostream& os, const Format& format)
    {
        os << format2string[(int)format];
        return os;
    }


    static constexpr int format2nByte[static_cast<int>(Format::COUNT)] = {
        0, // NONE
        2, // Z16             , /**< 16-bit linear depth values. The depth is meters is equal to depth scale * pixel value. */
        2, // DISPARITY16     , /**< 16-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
        4, // XYZ32F          , /**< 32-bit floating point 3D coordinates. */
        4, // YUYV            , /**< 32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV */
        3, // RGB8            , /**< 8-bit red, green and blue channels */
        3, // BGR8            , /**< 8-bit blue, green, and red channels -- suitable for OpenCV */
        4, // RGBA8           , /**< 8-bit red, green and blue channels + constant alpha channel equal to FF */
        4, // BGRA8           , /**< 8-bit blue, green, and red channels + constant alpha channel equal to FF */
        1, // Y8              , /**< 8-bit per-pixel grayscale image */
        2, // Y16             , /**< 16-bit per-pixel grayscale image */
        0, // RAW10           , /**< Four 10 bits per pixel luminance values packed into a 5-byte macropixel */
        2, // RAW16           , /**< 16-bit raw image */
        1, // RAW8            , /**< 8-bit raw image */
        0, // UYVY            , /**< Similar to the standard YUYV pixel format, but packed in a different order */
        0, // MOTION_RAW      , /**< Raw data from the motion sensor */
        0, // MOTION_XYZ32F   , /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
        0, // GPIO_RAW        , /**< Raw data from the external sensors hooked to one of the GPIO's */
        0, // DISPARITY32     , /**< 32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth */
        12 + 16, // DOF6           , /**< Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
        0, // Y10BPACK        , /**< 16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits */
        4, // DISTANCE        , /**< 32-bit float-point depth distance value.  */
        0, // MJPEG           , /**< Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB   */
        2, // Y8I             , /**< 8-bit per pixel interleaved. 8-bit left, 8-bit right.  */
        3, // Y12I            , /**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order. */
        0, // INZI            , /**< multi-planar Depth 16bit + IR 10bit.  */
        1, // INVI            , /**< 8-bit IR stream.  */
        0, // W10             , /**< Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 bytes */
        2, // Z16H            , /**< Variable-length Huffman-compressed 16-bit depth values. */
        2, // FG              , /**< 16-bit per-pixel frame grabber format. */
        2, // Y411            , /**< 12-bit per-pixel. */
        // COUNT             	/**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
    };

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

    class Acquisition {
    public:
//        Acquisition(long long backendTimestamp = 0, long long backendTimeOfArrival = 0, unsigned char* data = nullptr);
        Acquisition(long long backendTimestamp, long long backendTimeOfArrival, const unsigned char* const data, size_t size);
        ~Acquisition();

        Acquisition(const Acquisition& acq) = delete;
//        Acquisition(const Acquisition& acq);
//        Acquisition(Acquisition&& acq) = default;
        Acquisition(Acquisition&& acq);

        Acquisition & operator=(const Acquisition & acq) = delete;
        Acquisition& operator=(Acquisition&& acq) = delete;
        Acquisition clone() const;

        const long long mBackendTimestamp; // microseconds
        const long long mBackendTimeOfArrival; // microseconds
        const unsigned char* const mData;
        const size_t mSize;
//        bool mOwnData = false;

    private:
        bool mIsMoved = false;
    public:
        friend std::ostream& operator<<(std::ostream& os, const Acquisition& acq);
    };

    using MetaData = std::map<std::string, std::any>; // C++17
    static std::string to_string(const MetaData& metaData, bool expand = false)
    {
        std::string str = "";
        if (expand) {
            bool first = true;
            for (const auto& pair : metaData) {
                if (first)
                    first = false;
                else
                    str += "\n";

                const auto& name = pair.first;
                const auto& val = pair.second;
                str += std::string(val.type().name()) + " " + name + " = '" + any::to_string(val) + "'";
            }
        } else {
            str += "[";
            for (const auto& pair : metaData) {
                const auto& name = pair.first;
                const auto& val = pair.second;
                str += std::string(val.type().name()) + " " + name + " = '" + any::to_string(val) + "', ";
            }
            str += "]";
        }
        return str;
    }

public:
protected:
    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream& ioStream);
    ~Stream();

    Stream(const Stream& stream) = delete;
    Stream(Stream& stream) = delete;
    Stream(const Stream&& stream) = delete;
    Stream(Stream&& stream) = delete;

    Stream& operator=(const Stream& stream) = delete;
    Stream& operator=(Stream& stream) = delete;
    Stream& operator=(const Stream&& stream) = delete;
    Stream& operator=(Stream&& stream) = delete;

    static size_t computeAcquisitionSize(Format format, const std::vector<int>& dims);

public:
    const std::string& getSensorName() const;
    const std::vector<int>& getDims() const;
    Format getFormat() const;
    size_t getAcquisitionSize() const;

    IOStream &getIOStream() const;

    friend std::ostream& operator<<(std::ostream& os, const Stream& stream)
    {
//        os << stream.mSensorName << stream.mFormat << stream.mDims << stream.mAcquisitionSize;
        os << stream.mSensorName;
        return os;
    }

protected:
    std::string mSensorName = "";
    Format mFormat = Format::NONE;
    std::vector<int> mDims;

    IOStream& mIOStream;
    size_t mAcquisitionSize;
};

class OutputStream;

class InputStream : public Stream {
public:
    InputStream(const std::string & sensorName, const std::string& syncSensorName = "");
    template <class IOStreamT>
    InputStream(IOStreamT&& ioStream);
    template <class IOStreamT>
    InputStream(IOStreamT& ioStream) = delete;
//    InputStream(RamIO & ramIO);

    ~InputStream();

    Acquisition getAcquisition() const;

//    Acquisition& operator>>(Acquisition& acquisition) const;
//    void operator>>(const OutputStream& outputStream) const;

    std::vector<Acquisition> getAllAcquisition();
    const MetaData& getMetaData() const;

    friend std::ostream& operator<<(std::ostream& os, const InputStream& inputStream)
    {
        os << "metadata:" << to_string(inputStream.mMetaData);
        return os;
    }


private:
    MetaData mMetaData;
};

// template <typename T, class = typename std::enable_if<std::is_rvalue_reference<T>::value>::type>
template <class IOStreamT>
InputStream::InputStream(IOStreamT&& ioStream)
    : Stream("", Format::NONE, {}, *std::move(new IOStreamT(std::move(ioStream))))
{

    mIOStream.read(mSensorName);
    mIOStream.read(mFormat);
    mIOStream.read(mDims);
//    std::cout << "[InputStream] before read metaData" << std::endl;
    mIOStream.read(mMetaData);
//    std::cout << "[InputStream] after read metaData" << std::endl;

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}
// template <class T>
// InputStream::InputStream(T ioStream)
//{
// }

// template<class T> constexpr T& no_move(T&& t) { return t; }
// template<class T> T& unmove(T&& t) { return t; }

class OutputStream : public Stream {
public:
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, ClientSocket&& ioStream = ClientSocket(), const MetaData& metaData = {});
    template <class IOStreamT>
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStreamT&& ioStream, const MetaData& metaData = {});
    template <class IOStreamT>
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStreamT& ioStream, const MetaData& metaData = {}) = delete;

    OutputStream(const OutputStream& outputStream) = delete;
    OutputStream(OutputStream&& outputStream) = delete;

    void operator<<(const Acquisition& acquisition) const;
    void operator<<(const InputStream& inputStream) const;

private:
};

// template <class T>
// OutputStream::OutputStream(const std::string& sensorName, Stream::Format format, const std::vector<int>& dims, T ioStream)
//{
// }

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

// OutputStream::OutputStream(const std::string &sensorName, Stream::Format format, const std::vector<int> &dims, ClientSocket &&ioStream)
//{

//}

#endif // STREAM_H
