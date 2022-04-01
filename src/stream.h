#ifndef FRAME_H
#define FRAME_H

#include <socket.h>
#include <string>

//#define DEBUG_STREAM

constexpr int SERVICE_PORT = 4042;

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

    static constexpr char const * format2string[static_cast<int>(Format::COUNT)] = {
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
        12 + 16 + 12, // DOF6           , /**< Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
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

public:
protected:
    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4 = ("127.0.0.1"), int port = SERVICE_PORT);
    Stream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file);
    ~Stream();

    Stream(const Stream& stream) = delete;
    Stream(Stream& stream) = delete;
    Stream(const Stream&& stream) = delete;
    Stream(Stream&& stream) = delete;

    Stream(ClientSocket&& clientSocket);

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
    std::string mSensorName;
    Format mFormat;
    std::vector<int> mDims;

    std::unique_ptr<IOStream> mIOStream;
    size_t mAcquisitionSize;
};

class OutputStream;

class InputStream : public Stream {
public:
    InputStream(const std::string& sensorName, const std::string& syncSensorName = "", const std::string& ipv4 = "127.0.0.1", int port = SERVICE_PORT);
    InputStream(std::fstream& file);
    InputStream(ClientSocket&& sock, const std::string & sensorName);
    ~InputStream();

    Acquisition& operator>>(Acquisition& acquisition) const;
    void operator>>(const OutputStream& outputStream) const;

    std::vector<Acquisition> getAllAcquisition();

private:
};

class OutputStream : public Stream {
public:
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, const std::string& ipv4 = "127.0.0.1", int port = SERVICE_PORT);
    OutputStream(const std::string& sensorName, Format format, const std::vector<int>& dims, std::fstream& file);
    OutputStream(const OutputStream& outputStream) = delete;
    OutputStream(ClientSocket&& sock, const InputStream& inputStream);

    void operator<<(const Acquisition& acquisition) const;
    void operator<<(const InputStream& inputStream) const;

private:
};

#endif // FRAME_H
