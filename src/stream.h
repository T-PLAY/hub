
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

//namespace std {
//namespace any {

namespace any {

static std::string to_string(const std::any& any)
{
    //    std::any;
    assert(any.has_value());
    //    std::string hashCode = any.type().name();
    //    const type_info & type = any.type();
    const auto& hashCode = any.type().hash_code();
    //    write(hashCode);

    //    double val = my_any_cast<>(&any);
    //    const auto * val = getValue(any);
    //    double d = std::get<double>(any);
    //    auto d = any::get<double>(any);

    //    if (hashCode == "double") {
    if (hashCode == typeid(double).hash_code()) {
        const double* val = std::any_cast<double>(&any);
        //        write(*val);
        //        return std::string(*val);
        return std::to_string(*val);

    }
    //    else if (hashCode == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
    else if (hashCode == typeid(std::string).hash_code()) {
        const std::string* val = std::any_cast<std::string>(&any);
        //        write(*val);
        return std::string(*val);

    } else if (hashCode == typeid(const char*).hash_code()) {
        const char* val = *std::any_cast<const char*>(&any);
//        write(val);
        return std::string(val);


    } else {
        auto name = any.type().name();
        auto raw_name = any.type().raw_name();
        assert(false);
    }
    return "";
}

} // any

//template <typename T>
//T get(const std::any& any)
//{
//    const std::type_info& typeId = any.type();
//    assert(typeId == typeid(T));
//    return *std::any_cast<T>(&any);

//    //    if (typeId == typeid(int))
//    ////        return static_cast<T>(*std::any_cast<int>(any));
//    //        return std::any_cast<int>(any);
//    //    else if (typeId == typeid(long))
//    //        return static_cast<T>(*std::any_cast<long>(any));
//    //    else if (typeId == typeid(std::string))
//    //        return static_cast<T>(*std::any_cast<std::string>(any));
//}

//friend std::ostream& operator<<(std::ostream& os, const std::any& any)
//    {
//        os << format2string[(int)format];
//        return os;
//    }

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
        //        12 + 16 + 12, // DOF6           , /**< Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
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
        Acquisition(long long backendTimestamp = 0, long long backendTimeOfArrival = 0, unsigned char* data = nullptr);
        Acquisition(const Acquisition& acq) = delete;
        ~Acquisition();

        Acquisition(Acquisition&& acq);
        Acquisition& operator=(Acquisition&& acq);
        Acquisition clone() const;

        long long mBackendTimestamp; // microseconds
        long long mBackendTimeOfArrival; // microseconds
        unsigned char* mData;

        size_t mSize = 0;
        bool mOwnData = false;

    private:
    public:
        friend std::ostream& operator<<(std::ostream& os, const Acquisition& acq);
    };

    //    class MetaData {
    //    public:
    //        template <class T>
    //        void add(std::string name, T val) {
    //            mMetaDatas[name] = val;
    //        }

    //    private:
    //        //        std::map<std::string, unsigned
    ////        std::vector<std::tuple<std::string, std::string, std::any>> mMetaDatas;
    //        std::map<std::string, std::any> mMetaDatas;
    //    };
    using MetaData = std::map<std::string, std::any>; // C++17
    //    using MetaData = std::map<std::string, int>;
    //    friend std::ostream& operator<<(std::ostream& os, const MetaData& metaData)
    static std::string to_string(const MetaData& metaData)
    {
        std::string str = "";
        str += "[";
        for (const auto& pair : metaData) {
            const auto& name = pair.first;
            const auto& val = pair.second;
            //                        std::cout << getServerHeader(iThread) << "[streamer] metaData: " << val.type().name() << " " << name << " = " << std::endl;
            str += std::string(val.type().name()) + " " + name + " = '" + any::to_string(val) + "', ";
        }
        str += "]";
        return str;
    }
    //            std::string hashCode = any.type().name();

    //        if (hashCode == "double") {
    //            const double* val = std::any_cast<double>(&any);
    //            os << *val;

    //        }
    //        else if (hashCode == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
    //            const std::string* val = std::any_cast<std::string>(&any);
    //            os << *val;
    //        }
    //        else {
    //            auto name = any.type().name();
    //            auto raw_name = any.type().raw_name();
    //            assert(false);
    //        }
    //        //os << format2string[(int)format];
    //        return os;
    //    }

public:
protected:
    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream& ioStream);
    //    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4 = ("127.0.0.1"), int port = SERVICE_PORT);
    //    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file);
    ~Stream();

    Stream(const Stream& stream) = delete;
    Stream(Stream& stream) = delete;
    Stream(const Stream&& stream) = delete;
    Stream(Stream&& stream) = delete;

    //    Stream(ClientSocket&& clientSocket);

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

protected:
    std::string mSensorName = "";
    Format mFormat = Format::NONE;
    std::vector<int> mDims;

    //    std::unique_ptr<IOStream> mIOStream;
    IOStream& mIOStream;
    size_t mAcquisitionSize;
};

class OutputStream;

class InputStream : public Stream {
public:
    //    InputStream(const std::string& sensorName, const std::string& syncSensorName = "", IOStream && ioStream = ClientSocket("127.0.0.1", SERVICE_PORT));
    //    InputStream(ClientSocket&& ioStream);
    //    InputStream(FileIO&& ioStream);
    //    InputStream(const std::string& sensorName, const std::string& syncSensorName = "");
    InputStream(const char* sensorName, const std::string& syncSensorName = "");
    template <class IOStream>
    InputStream(IOStream&& ioStream);
    template <class IOStream>
    InputStream(IOStream& ioStream) = delete;
    //    template <class T>
    //    InputStream(T ioStream) = delete;
    //    InputStream(const std::string& sensorName, const std::string& syncSensorName = "", const std::string& ipv4 = "127.0.0.1", int port = SERVICE_PORT);
    //    InputStream(std::fstream& file);
    //    InputStream(ClientSocket&& sock, const std::string& sensorName);
    ~InputStream();

    Acquisition& operator>>(Acquisition& acquisition) const;
    void operator>>(const OutputStream& outputStream) const;

    std::vector<Acquisition> getAllAcquisition();
    const MetaData& getMetaData() const;

private:
    MetaData mMetaData;
};

//template <typename T, class = typename std::enable_if<std::is_rvalue_reference<T>::value>::type>
template <class IOStream>
InputStream::InputStream(IOStream&& ioStream)
    //    : Stream("", Format::NONE, {}, std::move(ioStream))
    //    : Stream("", Format::NONE, {}, std::forward<IOStream&>(ioStream))
    //    : Stream("", Format::NONE, {}, std::move(ioStream))
    //    : Stream("", Format::NONE, {}, unmove(ioStream))
    : Stream("", Format::NONE, {}, *std::move(new IOStream(std::move(ioStream))))
{
    //    ioStream.setupInput();

    mIOStream.read(mSensorName);
    mIOStream.read(mFormat);
    mIOStream.read(mDims);
    mIOStream.read(mMetaData);

    mAcquisitionSize = computeAcquisitionSize(mFormat, mDims);
}
//template <class T>
//InputStream::InputStream(T ioStream)
//    : Stream("", Format::NONE, {}, ioStream))
//{
//}

// template<class T> constexpr T& no_move(T&& t) { return t; }
//template<class T> T& unmove(T&& t) { return t; }

class OutputStream : public Stream {
public:
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, ClientSocket&& ioStream = ClientSocket(), const MetaData& metaData = {});
    //    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, FileIO&& ioStream);
    template <class IOStream>
    //    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream&& ioStream = ClientSocket("127.0.0.1", 4043));
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream&& ioStream, const MetaData& metaData = {});
    template <class IOStream>
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream& ioStream, const MetaData& metaData = {}) = delete;
    //    template <class T>
    //    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, T ioStream);

    //    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, const MetaData& metaData = {}, const std::string& ipv4 = "127.0.0.1", int port = SERVICE_PORT);
    //    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file);
    OutputStream(const OutputStream& outputStream) = delete;
    OutputStream(OutputStream&& outputStream) = delete;
    //    OutputStream(ClientSocket&& sock, const InputStream& inputStream);
    //    OutputStream(IOStream&& ioStream, const InputStream& inputStream);
    //    OutputStream(IOStream&& ioStream);

    void operator<<(const Acquisition& acquisition) const;
    void operator<<(const InputStream& inputStream) const;

private:
};

//template <class T>
//OutputStream::OutputStream(const std::string& sensorName, Stream::Format format, const std::vector<int>& dims, T ioStream)
//{
//    assert(false);
//}

template <class IOStream>
OutputStream::OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, IOStream&& ioStream, const MetaData& metaData)
    //        : Stream(sensorName, format, dims, dynamic_cast<IOStream&>(std::move(ioStream)))
    //        : Stream(sensorName, format, dims, (unmove(ioStream)))
    : Stream(sensorName, format, dims, *std::move(new IOStream(std::move(ioStream))))
//    : Stream(sensorName, format, dims, std::move(T(std::move(ioStream))))
//    : Stream(sensorName, format, dims, std::move(ioStream))
//    : Stream(sensorName, format, dims, std::move(ioStream))
//    : Stream(sensorName, format, dims, std::forward<IOStream&>(ioStream))
{
    //    mIOStream.setupMode(IOStream::Mode::OUTPUT);
    //    mIOStream.init(sensorName);
    mIOStream.setupOutput(sensorName);

    mIOStream.write(mSensorName);
    mIOStream.write(mFormat);
    mIOStream.write(mDims);
    mIOStream.write(metaData);
}

//OutputStream::OutputStream(const std::string &sensorName, Stream::Format format, const std::vector<int> &dims, ClientSocket &&ioStream)
//    : OutputStream(sensorName, format, dims, std::move(ioStream))
//{

//}

#endif // STREAM_H
