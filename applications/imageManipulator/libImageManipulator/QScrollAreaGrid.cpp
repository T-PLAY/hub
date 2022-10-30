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

    constexpr double speed = 0.1;
//    std::cout << ry << std::endl;
    if (ry > 0) {
//        *mCanvasPixelPerUnit += speed;
        *mCanvasPixelPerUnit *= (1.0 + speed);
    } else if (ry < 0) {
//        *mCanvasPixelPerUnit = std::max(*mCanvasPixelPerUnit - speed, 1.0);
        *mCanvasPixelPerUnit *= (1.0 - speed);
    }

    double vScroll = verticalScrollBar()->value();
#if QT_VERSION <= QT_VERSION_CHECK(5, 12, 0)
    double y = event->position().y();
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    double y = event->position().y();
#else
    double y = event->x();
#endif
    //    double vUnit = (vScroll + y) / mCanvasPixelPerUnit;
    verticalScrollBar()->setValue(vScroll + y);
    m_scrollAreaLeft->verticalScrollBar()->setValue(vScroll + y);

    double hScroll = horizontalScrollBar()->value();
#if QT_VERSION <= QT_VERSION_CHECK(5, 12, 0)
    double x = event->position().x();
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    double x = event->position().x();
#else
    double x = event->x();
#endif
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
