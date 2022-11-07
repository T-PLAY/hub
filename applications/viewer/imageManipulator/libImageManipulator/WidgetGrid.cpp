#include "WidgetGrid.h"

#include <QPainter>
#include <cmath>
#include <iostream>

#include <WidgetStreamView.h>
//#include <constants.h>

WidgetGrid::WidgetGrid( QWidget* parent ) : QWidget { parent } {}

void WidgetGrid::paintEvent( QPaintEvent* event ) {
    assert( mCanvasPixelPerUnit != nullptr );
    assert( mImageUnitWidth != nullptr );
    assert( mImageUnitHeight != nullptr );
    assert( mCanvasPixelWidth != nullptr );
    assert( mCanvasPixelHeight != nullptr );

    QPainter painter( this );
    painter.setPen( Qt::gray );

    double tmp;
    for ( const auto& stepSize : g_stepSizes ) {
        tmp = stepSize;
        if ( *mCanvasPixelPerUnit * stepSize > 25 ) break;
    }
    const double stepSize = tmp;

    const int hZero = ( width() - *mCanvasPixelWidth ) / 2.0;
    const int vZero = ( height() - *mCanvasPixelHeight ) / 2.0;
    {
        const int nHStep = std::ceil( *mImageUnitWidth / stepSize );

        for ( int iHStep = 0; iHStep <= nHStep; ++iHStep ) {
            int x = iHStep * stepSize * *mCanvasPixelPerUnit;
            painter.drawLine( hZero + x, 0, hZero + x, height() - vZero - 1 + 50 );
        }
    }

    {
        const int nVStep = std::ceil( *mImageUnitHeight / stepSize );

        for ( int iVStep = 0; iVStep <= nVStep; ++iVStep ) {
            int y = iVStep * stepSize * *mCanvasPixelPerUnit;
            painter.drawLine( 0, vZero + y, width() - hZero - 1 + 50, vZero + y );
        }
    }

    painter.end();
}

void WidgetGrid::onPixelPerUnitChanged() {
    update();
}

void WidgetGrid::setCanvasPixelHeight( const int* newCanvasPixelHeight ) {
    mCanvasPixelHeight = newCanvasPixelHeight;
}

void WidgetGrid::setCanvasPixelWidth( const int* newCanvasPixelWidth ) {
    mCanvasPixelWidth = newCanvasPixelWidth;
}

void WidgetGrid::setImageUnitHeight( const double* newImageUnitHeight ) {
    mImageUnitHeight = newImageUnitHeight;
}

void WidgetGrid::setImageUnitWidth( const double* newImageUnitWidth ) {
    mImageUnitWidth = newImageUnitWidth;
}

void WidgetGrid::setCanvasPixelPerUnit( const double* newCanvasPixelPerUnit ) {
    mCanvasPixelPerUnit = newCanvasPixelPerUnit;
}
