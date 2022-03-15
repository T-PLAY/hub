#include "WidgetStreamView.h"

#include <QPainter>
#include <iostream>

// static constexpr QImage::Format * streamFormat2QtFormat[(int)Stream::Format::COUNT] = {
// };

WidgetStreamView::WidgetStreamView(QWidget* parent)
    : QWidget(parent)
{
    //    setAttribute(Qt::WA_NoSystemBackground, true);
}

void WidgetStreamView::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{
//    std::cout << "WidgetStreamView::setData" << std::endl;
    //    assert(false);
}

///////////////////////////////////////////////////////////////////////////////

WidgetStreamView2D::WidgetStreamView2D(QWidget* parent)
    : WidgetStreamView(parent)
{
}

void WidgetStreamView2D::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{
//    std::cout << "WidgetStreamView2D::setData" << std::endl;
    mData = img_ptr;
    assert(dims.size() == 2);
    mWidth = dims.at(0);
    mHeight = dims.at(1);
    mFormat = format;
    update();
}

void WidgetStreamView2D::paintEvent(QPaintEvent* event)
{
//    std::cout << "[WidgetStreamView] paintEvent : width:" << mWidth << ", height:" << mHeight << std::endl;
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (mData != nullptr) {
        QImage image;
        //        QPixmap pixmap;
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
        //        image = image.scaled(QSize(641, 481));
        //        painter.drawImage(p, image.scaled(this->size()));
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
    //    mLabel = QLabel(this);
    mLabel = new QLabel(this);
    //    mLabel->setText("hello");
    //    mTable = new QTableWidget(2, 7, this);
    //    setStyleSheet("background-color:green");
//    mLabel->setStyleSheet("background-color: blue");

//    unsigned char data[28] = {0};
//    for (int i =0; i <28 / 4; ++i) {
//        ((float*)data)[i] = 1000.0f;
//    }
//    float* translation = (float*)data;
//    float* quaternion = (float*)&data[12];
//    std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
//    mLabel->setText(str.c_str());

//    std::cout << mLabel->size().width() << " " << mLabel->height() << std::endl;

//    mLabel->setMinimumWidth(width());
//    mLabel->setMinimumHeight(height());
//    setMinimumWidth(width());
//    setMinimumHeight(height());
    mLabel->setMinimumSize(350, 30);
}

WidgetStreamView1D::~WidgetStreamView1D()
{
    delete mLabel;
    //    delete mTable;
}

void WidgetStreamView1D::setData(unsigned char* img_ptr, std::vector<int> dims, Stream::Format format)
{
//    std::cout << "WidgetStreamView1D::setData" << std::endl;

    float* translation = (float*)img_ptr;
    float* quaternion = (float*)&img_ptr[12];
    std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
//    std::cout << "str : " << str << std::endl;
    mLabel->setText(str.c_str());
//    mLabel->update();
}
