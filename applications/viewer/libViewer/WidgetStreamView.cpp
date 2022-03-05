#include "WidgetStreamView.h"

#include <QPainter>
#include <iostream>

// static constexpr QImage::Format * streamFormat2QtFormat[(int)Stream::Format::COUNT] = {
//     nullptr, // NONE
//     QImage::Format::Format_Grayscale16, // Z16             , /**< 16-bit linear depth values. The depth is meters is equal to depth scale * pixel value. */
//     2, // DISPARITY16     , /**< 16-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth. */
//     4, // XYZ32F          , /**< 32-bit floating point 3D coordinates. */
//     4, // YUYV            , /**< 32-bit y0, u, y1, v data for every two pixels. Similar to YUV422 but packed in a different order - https://en.wikipedia.org/wiki/YUV */
//     3, // RGB8            , /**< 8-bit red, green and blue channels */
//     3, // BGR8            , /**< 8-bit blue, green, and red channels -- suitable for OpenCV */
//     4, // RGBA8           , /**< 8-bit red, green and blue channels + constant alpha channel equal to FF */
//     4, // BGRA8           , /**< 8-bit blue, green, and red channels + constant alpha channel equal to FF */
//     1, // Y8              , /**< 8-bit per-pixel grayscale image */
//     2, // Y16             , /**< 16-bit per-pixel grayscale image */
//     0, // RAW10           , /**< Four 10 bits per pixel luminance values packed into a 5-byte macropixel */
//     2, // RAW16           , /**< 16-bit raw image */
//     1, // RAW8            , /**< 8-bit raw image */
//     0, // UYVY            , /**< Similar to the standard YUYV pixel format, but packed in a different order */
//     0, // MOTION_RAW      , /**< Raw data from the motion sensor */
//     0, // MOTION_XYZ32F   , /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
//     0, // GPIO_RAW        , /**< Raw data from the external sensors hooked to one of the GPIO's */
//     0, // DISPARITY32     , /**< 32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth */
//     0, // DOF6            , /**< Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
//     0, // Y10BPACK        , /**< 16-bit per-pixel grayscale image unpacked from 10 bits per pixel packed ([8:8:8:8:2222]) grey-scale image. The data is unpacked to LSB and padded with 6 zero bits */
//     4, // DISTANCE        , /**< 32-bit float-point depth distance value.  */
//     0, // MJPEG           , /**< Bitstream encoding for video in which an image of each frame is encoded as JPEG-DIB   */
//     2, // Y8I             , /**< 8-bit per pixel interleaved. 8-bit left, 8-bit right.  */
//     3, // Y12I            , /**< 12-bit per pixel interleaved. 12-bit left, 12-bit right. Each pixel is stored in a 24-bit word in little-endian order. */
//     0, // INZI            , /**< multi-planar Depth 16bit + IR 10bit.  */
//     1, // INVI            , /**< 8-bit IR stream.  */
//     0, // W10             , /**< Grey-scale image as a bit-packed array. 4 pixel data stream taking 5 bytes */
//     2, // Z16H            , /**< Variable-length Huffman-compressed 16-bit depth values. */
//     2, // FG              , /**< 16-bit per-pixel frame grabber format. */
//     2, // Y411            , /**< 12-bit per-pixel. */
//     // COUNT             /**< Number of enumeration values. Not a valid input: intended to be used in for-loops. */
// };

WidgetStreamView::WidgetStreamView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void WidgetStreamView::setImage(unsigned char* img_ptr, int pWidth, int pHeight, Stream::Format format)
{
    img = img_ptr;
    mWidth = pWidth;
    mHeight = pHeight;
    mFormat = format;
    update();
}

void WidgetStreamView::paintEvent(QPaintEvent* event)
{
    std::cout << "[WidgetStreamView] paintEvent : width:" << mWidth << ", height:" << mHeight << std::endl;
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (img != nullptr) {
        std::cout << sizeof(img) << std::endl;
        QImage image;
        switch (mFormat) {
        case Stream::Format::Y8:
            image = QImage((unsigned char*)img, mWidth, mHeight, QImage::Format_Grayscale8);
            break;

        case Stream::Format::Y16:
        case Stream::Format::Z16:
            image = QImage((unsigned char*)img, mWidth, mHeight, QImage::Format_Grayscale16);
            break;

        case Stream::Format::RGB8:
            image = QImage((unsigned char*)img, mWidth, mHeight, QImage::Format_RGB888);
            break;

        case Stream::Format::BGR8:
            image = QImage((unsigned char*)img, mWidth, mHeight, QImage::Format_BGR888);
            break;

        default:
            std::cout << "[paintEvent] unknown stream format" << std::endl;
            assert(false);
        }
        assert(!image.isNull());
//        image = image.scaled(this->size());
        //        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_Grayscale8).scaled(this->size());
        //        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_BGR888).scaled(this->size());
        const QPoint p = QPoint(0, 0);
        painter.drawImage(p, image.scaled(this->size()));
    } else {
        //        painter.fillRect(0, 0, width(), height(), Qt::red);
        painter.fillRect(0, 0, width(), height(), Qt::gray);
    }
    painter.end();
}
