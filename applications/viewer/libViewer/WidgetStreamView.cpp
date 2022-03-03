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
    img = img_ptr;
    mWidth = pWidth;
    mHeight = pHeight;
    update();
}

void WidgetStreamView::paintEvent(QPaintEvent* event)
{
    std::cout << "[WidgetStreamView] paintEvent" << std::endl;
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);
    if (img != nullptr) {
        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_Grayscale8).scaled(this->size());
//        QImage image = QImage((unsigned char*)img, mWidth, mHeight, mWidth, QImage::Format_BGR888).scaled(this->size());
        const QPoint p = QPoint(0, 0);
        painter.drawImage(p, image);
    } else {
        painter.fillRect(0, 0, width(), height(), Qt::red);
    }
    painter.end();
}
