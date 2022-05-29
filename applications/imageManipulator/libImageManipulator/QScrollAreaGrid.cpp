#include "QScrollAreaGrid.h"

#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <iostream>

QScrollAreaGrid::QScrollAreaGrid(QWidget* parent)
    : QScrollArea(parent)
{
}

void QScrollAreaGrid::wheelEvent(QWheelEvent* event)
{
    assert(mCanvasPixelPerUnit != nullptr);
    int ry = event->angleDelta().ry();

    std::cout << ry << std::endl;
    if (ry > 0) {
        *mCanvasPixelPerUnit += 1.0;
    } else if (ry < 0) {
        *mCanvasPixelPerUnit = std::max(*mCanvasPixelPerUnit - 1.0, 1.0);
    }

    double vScroll = verticalScrollBar()->value();
    double y = event->position().y();
    //    double vUnit = (vScroll + y) / mCanvasPixelPerUnit;
    verticalScrollBar()->setValue(vScroll + y);
    m_scrollAreaLeft->verticalScrollBar()->setValue(vScroll + y);

    double hScroll = horizontalScrollBar()->value();
    double x = event->position().x();
    horizontalScrollBar()->setValue(hScroll + x);
    m_scrollAreaTop->horizontalScrollBar()->setValue(hScroll + x);

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
        const int xPos = hSliderPos + mousePosX - event->x();
        horizontalScrollBar()->setValue(xPos);
        m_scrollAreaTop->horizontalScrollBar()->setValue(xPos);

        const int yPos = vSliderPos + mousePosY - event->y();
        verticalScrollBar()->setValue(yPos);
        m_scrollAreaLeft->verticalScrollBar()->setValue(yPos);
    }
    //    event->accept();
}

void QScrollAreaGrid::setCanvasPixelPerUnit(double* newCanvasPixelPerUnit)
{
    mCanvasPixelPerUnit = newCanvasPixelPerUnit;
}

void QScrollAreaGrid::setScrollAreaLeft(QScrollArea* newScrollAreaLeft)
{
    m_scrollAreaLeft = newScrollAreaLeft;
}

void QScrollAreaGrid::setScrollAreaTop(QScrollArea* newScrollAreaTop)
{
    m_scrollAreaTop = newScrollAreaTop;
}

// const double& QScrollAreaGrid::getCanvasPixelPerUnit() const
//{
//     return mCanvasPixelPerUnit;
// }
