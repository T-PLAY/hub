#include "QScrollAreaGrid.h"

#include <iostream>
#include <QScrollBar>

QScrollAreaGrid::QScrollAreaGrid(QWidget* parent)
    : QScrollArea(parent)
{
}

void QScrollAreaGrid::wheelEvent(QWheelEvent *event)
{
    int ry = event->angleDelta().ry();

    std::cout << ry << std::endl;
    if (ry > 0) {
        mCanvasPixelPerUnit += 1.0;
    }
    else if (ry < 0) {
        mCanvasPixelPerUnit = std::max(mCanvasPixelPerUnit - 1.0, 1.0);
    }

//    update();
    emit pixelPerUnitChanged();

    event->accept();
}

void QScrollAreaGrid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
//        setCursor(Qt::SplitHCursor);
        setCursor(Qt::DragMoveCursor);
        mousePosX = event->x();
        mousePosY = event->y();
        hSliderPos = this->horizontalScrollBar()->value();
        vSliderPos = this->verticalScrollBar()->value();

        leftMouseDown = true;
    }
}

void QScrollAreaGrid::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        leftMouseDown = false;
        setCursor(Qt::ArrowCursor);
    }
}

void QScrollAreaGrid::mouseMoveEvent(QMouseEvent* event)
{
    if (leftMouseDown) {
        horizontalScrollBar()->setValue((hSliderPos + mousePosX - event->x()));
        verticalScrollBar()->setValue((vSliderPos + mousePosY - event->y()));
    }
//    event->accept();
}



const double & QScrollAreaGrid::getCanvasPixelPerUnit() const
{
    return mCanvasPixelPerUnit;
}
