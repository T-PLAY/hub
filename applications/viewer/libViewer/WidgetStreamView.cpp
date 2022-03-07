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
        QImage image;
//        QPixmap pixmap;
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
