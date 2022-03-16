#include "WidgetStreamView.h"

#include <QPainter>
#include <iostream>

// static constexpr QImage::Format * streamFormat2QtFormat[(int)Stream::Format::COUNT] = {
// };

WidgetStreamView::WidgetStreamView(QWidget* parent)
    : QWidget(parent)
{
}

void WidgetStreamView::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{
}

///////////////////////////////////////////////////////////////////////////////

WidgetStreamView2D::WidgetStreamView2D(QWidget* parent)
    : WidgetStreamView(parent)
{
}

void WidgetStreamView2D::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{
    mData = img_ptr;
    assert(dims.size() == 2);
    mWidth = dims.at(0);
    mHeight = dims.at(1);
    mFormat = format;
    update();
}

void WidgetStreamView2D::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (mData != nullptr) {
        QImage image;
        switch (mFormat) {
        case Stream::Format::Y8:
            image = QImage((unsigned char*)mData, mWidth, mHeight, QImage::Format_Grayscale8);
            break;

        case Stream::Format::Y16:
        case Stream::Format::Z16:
            image = QImage((unsigned char*)mData, mWidth, mHeight, QImage::Format_Grayscale16);
            break;

        case Stream::Format::RGB8:
            image = QImage((unsigned char*)mData, mWidth, mHeight, QImage::Format_RGB888);
            break;

        case Stream::Format::BGR8:
            image = QImage((unsigned char*)mData, mWidth, mHeight, QImage::Format_BGR888);
            break;

        default:
            std::cout << "[paintEvent] unknown stream format" << std::endl;
            assert(false);
        }
        assert(!image.isNull());
        const QPoint p = QPoint(0, 0);

        image = image.scaled(this->size());
        painter.drawImage(p, image);

    } else {
        painter.fillRect(0, 0, width(), height(), Qt::gray);
    }
    painter.end();
}

///////////////////////////////////////////////////////////////////////////////

WidgetStreamView1D::WidgetStreamView1D(QWidget* parent)
    : WidgetStreamView(parent)
{
    mLabel = new QLabel(this);

    mLabel->setMinimumSize(350, 30);
}

WidgetStreamView1D::~WidgetStreamView1D()
{
    delete mLabel;
}

void WidgetStreamView1D::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{

    float* translation = (float*)img_ptr;
    float* quaternion = (float*)&img_ptr[12];
    std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
    mLabel->setText(str.c_str());
}
