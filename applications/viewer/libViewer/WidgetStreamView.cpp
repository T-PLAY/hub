#include "WidgetStreamView.h"

#include <QPainter>
#include <iostream>

WidgetStreamView::WidgetStreamView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void WidgetStreamView::setImage(unsigned char* img_ptr, int pWidth, int pHeight)
{
    //    std::cout << "setImage " << width() << " " << height() << std::endl;
    img = img_ptr;
    mWidth = pWidth;
    mHeight = pHeight;
    update();
    //        mtx.unlock();
}

void WidgetStreamView::paintEvent(QPaintEvent* event)
{
    //    mtx.lock();
    //    std::cout << "paintEvent " << width() << " " << height() << std::endl;
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (img != nullptr) {
        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_Grayscale8).scaled(this->size());
        const QPoint p = QPoint(0, 0);
        painter.drawImage(p, image);
        //        mtx.unlock();
    } else {
        painter.fillRect(0, 0, width(), height(), Qt::red);
    }
    painter.end();
}
