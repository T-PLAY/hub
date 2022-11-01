#include "FrameRuler.h"

#include <iostream>

#include <QPainter>
#include <WidgetStreamView.h>
#include <cmath>
#include <constants.h>

FrameRuler::FrameRuler( QWidget* parent ) : QFrame( parent ) {}

void FrameRuler::paintEvent( QPaintEvent* event ) {
    //    return;
    //    std::cout << "[FrameRuler] paintEvent" << std::endl;
    assert( mCanvasPixelPerUnit != nullptr );
    assert( mScrollBar != nullptr );
    assert( mCanvasPixelSize != nullptr );
    assert( mType != NONE );

    //    if (mType == Type::HORIZONTAL) {
    this->setMinimumWidth( *mCanvasPixelSize );
    //        this->setMaximumWidth(*mCanvasPixelSize);
    //    }
    //    else {
    this->setMinimumHeight( *mCanvasPixelSize );
    //        this->setMaximumHeight(*mCanvasPixelSize);
    //    }

    const int rulerSize = ( mType == Type::HORIZONTAL ) ? width() : height();
    const int xyZero =
        ( rulerSize > *mCanvasPixelSize ) ? ( ( rulerSize - *mCanvasPixelSize ) / 2.0 ) : 0.0;
    //    const int xyZero = (rulerSize - *mCanvasPixelSize) / 2.0;
    const double unitSide = *mCanvasPixelSize / *mCanvasPixelPerUnit;

    //    const std::vector<double> stepSizes = { 1.0, 2.0, 5.0, 10.0, 20.0, 50.0 };
    double tmp;
    for ( const auto& stepSize : g_stepSizes ) {
        tmp = stepSize;
        if ( *mCanvasPixelPerUnit * stepSize > 25 ) break;
    }
    const double stepSize = tmp;

    const int nStep = std::ceil( unitSide / stepSize );

    int alpha = 100;

    //    const int scrollPixelStart = mScrollBar->value();
    //    const double scroolUnitStart = scrollPixelStart / *mCanvasPixelPerUnit;

    QPainter painter;
    painter.begin( this );
    //    painter.setPen(QColor(255, 0, 0, 127));
    painter.setPen( QColor( 0, 0, 0, alpha ) );
    for ( int iStep = 0; iStep <= nStep; ++iStep ) {
        const int stepUnit  = iStep * stepSize;
        const int xy        = xyZero + stepUnit * *mCanvasPixelPerUnit;
        const QString value = QString::number( stepUnit );
        const int dec       = value.size() * 6 / 2;

        if ( mType == Type::HORIZONTAL ) {
            painter.drawLine( xy, 0, xy, rulerSize - 1 );
            painter.drawText( xy + 3, 15, value );
        }
        else {
            painter.drawLine( 0, xy, rulerSize - 1, xy );
            painter.drawText( 9 - dec, xy + 12, value );
        }
    }

    painter.end();
}

void FrameRuler::setCanvasPixelSize( const int* newCanvasPixelSize ) {
    mCanvasPixelSize = newCanvasPixelSize;
}

void FrameRuler::setScrollBar( const QScrollBar* newScrollBar ) {
    mScrollBar = newScrollBar;
}

void FrameRuler::setType( Type newType ) {
    mType = newType;
}

void FrameRuler::setPixelPerUnit( const double* newPixelPerUnit ) {
    mCanvasPixelPerUnit = newPixelPerUnit;
}

void FrameRuler::onPixelPerUnitChanged() {
    update();
}
