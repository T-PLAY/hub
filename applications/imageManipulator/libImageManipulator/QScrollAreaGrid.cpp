#include "QScrollAreaGrid.h"

#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <iostream>

QScrollAreaGrid::QScrollAreaGrid( QWidget* parent ) : QScrollArea( parent ) {}

void QScrollAreaGrid::wheelEvent( QWheelEvent* event ) {
    assert( mCanvasPixelPerUnit != nullptr );
    int ry = event->angleDelta().ry();

    constexpr double speed = 0.1;
    if ( ry > 0 ) { *mCanvasPixelPerUnit *= ( 1.0 + speed ); }
    else if ( ry < 0 ) { *mCanvasPixelPerUnit *= ( 1.0 - speed ); }

    double vScroll = verticalScrollBar()->value();
#if QT_VERSION <= QT_VERSION_CHECK( 5, 12, 0 )
    double y = event->position().y();
#elif QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    double y = event->position().y();
#else
    double y = event->x();
#endif
    verticalScrollBar()->setValue( vScroll + y );
    m_scrollAreaLeft->verticalScrollBar()->setValue( vScroll + y );

    double hScroll = horizontalScrollBar()->value();
#if QT_VERSION <= QT_VERSION_CHECK( 5, 12, 0 )
    double x = event->position().x();
#elif QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
    double x = event->position().x();
#else
    double x = event->x();
#endif
    horizontalScrollBar()->setValue( hScroll + x );
    m_scrollAreaTop->horizontalScrollBar()->setValue( hScroll + x );

    emit pixelPerUnitChanged();

    event->accept();
}

void QScrollAreaGrid::mousePressEvent( QMouseEvent* event ) {
    if ( event->button() == Qt::LeftButton ) {
        setCursor( Qt::DragMoveCursor );
        mousePosX  = event->position().x();
        mousePosY  = event->position().y();
        hSliderPos = this->horizontalScrollBar()->value();
        vSliderPos = this->verticalScrollBar()->value();

        leftMouseDown = true;
    }
}

void QScrollAreaGrid::mouseReleaseEvent( QMouseEvent* event ) {
    if ( event->button() == Qt::LeftButton ) {
        leftMouseDown = false;
        setCursor( Qt::ArrowCursor );
    }
}

void QScrollAreaGrid::mouseMoveEvent( QMouseEvent* event ) {
    if ( leftMouseDown ) {
        const int xPos = hSliderPos + mousePosX - event->position().x();
        horizontalScrollBar()->setValue( xPos );
        m_scrollAreaTop->horizontalScrollBar()->setValue( xPos );

        const int yPos = vSliderPos + mousePosY - event->position().y();
        verticalScrollBar()->setValue( yPos );
        m_scrollAreaLeft->verticalScrollBar()->setValue( yPos );
    }
}

void QScrollAreaGrid::setCanvasPixelPerUnit( double* newCanvasPixelPerUnit ) {
    mCanvasPixelPerUnit = newCanvasPixelPerUnit;
}

void QScrollAreaGrid::setScrollAreaLeft( QScrollArea* newScrollAreaLeft ) {
    m_scrollAreaLeft = newScrollAreaLeft;
}

void QScrollAreaGrid::setScrollAreaTop( QScrollArea* newScrollAreaTop ) {
    m_scrollAreaTop = newScrollAreaTop;
}

// const double& QScrollAreaGrid::getCanvasPixelPerUnit() const
//{
// }
