#include "WidgetStreamView.h"

#include <QPainter>
#include <iostream>

// static constexpr QImage::Format * streamFormat2QtFormat[(int)hub::SensorSpec::Format::COUNT] = {
// };
//#include <qmatrix4x4.h>
#include <QPainter>
#include <QPixmap>
#include <QTransform>
#include <cassert>
#include <cmath>

#include <constants.h>

WidgetStreamView::WidgetStreamView( QWidget* parent ) : QWidget( parent ) {}

// void WidgetStreamView::setData(unsigned char* img_ptr, std::vector<int> dims,
// hub::SensorSpec::Format format)
//{
// }

void WidgetStreamView::setData( unsigned char* img_ptr,
                                size_t size,
                                std::vector<int> dims,
                                hub::SensorSpec::Format format ) {}

///////////////////////////////////////////////////////////////////////////////
WidgetStreamView2D::WidgetStreamView2D( QWidget* parent ) : WidgetStreamView( parent ) {}

void WidgetStreamView2D::init( int imagePixelWidth,
                               int imagePixelHeight,
                               double imageUnitWidth,
                               double imageUnitHeight ) {
    std::cout << "[WidgetStreamView2D] init" << std::endl;
    mImagePixelWidth  = imagePixelWidth;
    mImagePixelHeight = imagePixelHeight;
    mImageUnitWidth   = ( imageUnitWidth == 0.0 ) ? imagePixelWidth : imageUnitWidth;
    mImageUnitHeight  = ( imageUnitHeight == 0.0 ) ? imagePixelHeight : imageUnitHeight;
    mHPixelPerUnit    = mImagePixelWidth / mImageUnitWidth;
    mVPixelPerUnit    = mImagePixelHeight / mImageUnitHeight;
    mRatio            = mImageUnitHeight / mImageUnitWidth;

    double unitWidth  = mImagePixelWidth / mHPixelPerUnit;
    double unitHeight = mImagePixelHeight / mVPixelPerUnit;
    if ( unitWidth * mRatio > unitHeight ) { unitWidth = unitHeight / mRatio; }
    else { unitHeight = unitWidth * mRatio; }

    mCanvasPixelPerUnit = std::floor( ( mHPixelPerUnit + mVPixelPerUnit ) / 2.0 );

    mCanvasPixelWidth  = unitWidth * mCanvasPixelPerUnit;
    mCanvasPixelHeight = unitHeight * mCanvasPixelPerUnit;

    setMinimumWidth( mCanvasPixelWidth );
    setMinimumHeight( mCanvasPixelHeight );

    setMaximumWidth( mCanvasPixelWidth );
    setMaximumHeight( mCanvasPixelHeight );

    emit inited();
}

void WidgetStreamView2D::clear() {
    if ( mData != nullptr ) {
        delete[] mData;
        mData = nullptr;
    }
    mImagePixelWidth  = 0;
    mImagePixelHeight = 0;

    mImageUnitWidth  = 0;
    mImageUnitHeight = 0;

    mRatio         = 1.0;
    mHPixelPerUnit = 1.0;
    mVPixelPerUnit = 1.0;

    mCanvasPixelPerUnit = 1.0;
    mCanvasPixelWidth   = 0.0;
    mCanvasPixelHeight  = 0.0;

    emit inited();

    update();
}

// WidgetStreamView2D::WidgetStreamView2D(int imagePixelWidth, int imagePixelHeight, double
// imageUnitWidth, double imageUnitHeight, QWidget* parent)
//{

//}

// void WidgetStreamView2D::setData(unsigned char* img_ptr, std::vector<int> dims,
// hub::SensorSpec::Format format)
//{

//}

void WidgetStreamView2D::setData( unsigned char* img_ptr,
                                  size_t size,
                                  std::vector<int> dims,
                                  hub::SensorSpec::Format format ) {
    assert( dims.size() == 2 );
    if ( mImagePixelWidth != dims.at( 0 ) || mImagePixelHeight != dims.at( 1 ) )
        init( dims.at( 0 ), dims.at( 1 ) );

    if ( img_ptr == nullptr ) {
        if ( mData != nullptr ) { delete[] mData; }
        update();
        return;
    }
    if ( mData == nullptr ) mData = new unsigned char[size];

    memcpy( mData, img_ptr, size );
    assert( dims.size() == 2 );
    assert( mImagePixelWidth == dims.at( 0 ) );
    assert( mImagePixelHeight == dims.at( 1 ) );
    mFormat = format;

    updateImage();
}

void WidgetStreamView2D::onPixelPerUnitChanged() {
    double unitWidth  = mImagePixelWidth / mHPixelPerUnit;
    double unitHeight = mImagePixelHeight / mVPixelPerUnit;
    if ( unitWidth * mRatio > unitHeight ) { unitWidth = unitHeight / mRatio; }
    else { unitHeight = unitWidth * mRatio; }

    mCanvasPixelWidth  = unitWidth * mCanvasPixelPerUnit;
    mCanvasPixelHeight = unitHeight * mCanvasPixelPerUnit;

    this->setMinimumWidth( mCanvasPixelWidth );
    this->setMinimumHeight( mCanvasPixelHeight );
    setMaximumWidth( mCanvasPixelWidth );
    setMaximumHeight( mCanvasPixelHeight );

    update();
}

// void WidgetStreamView2D::canvasUpdated()
//{

//}

void WidgetStreamView2D::updateImage() {

    if ( mData != nullptr ) {
        switch ( mFormat ) {
        case hub::SensorSpec::Format::Y8:
            m_image = new QImage( (unsigned char*)mData,
                                  mImagePixelWidth,
                                  mImagePixelHeight,
                                  QImage::Format_Grayscale8 );
            break;

        case hub::SensorSpec::Format::Y16:
        case hub::SensorSpec::Format::Z16:
#if QT_VERSION <= QT_VERSION_CHECK( 5, 12, 0 )
            m_image = new QImage( (unsigned char*)mData,
                                  mImagePixelWidth,
                                  mImagePixelHeight,
                                  QImage::Format_Grayscale16 );
#elif QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
            m_image = new QImage( (unsigned char*)mData,
                                  mImagePixelWidth,
                                  mImagePixelHeight,
                                  QImage::Format_Grayscale16 );
#else
            m_image = new QImage( (unsigned char*)mData,
                                  mImagePixelWidth,
                                  mImagePixelHeight,
                                  QImage::Format_Grayscale16 );
#endif
            break;

        case hub::SensorSpec::Format::RGB8:
            m_image = new QImage(
                (unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_RGB888 );
            break;

        case hub::SensorSpec::Format::RGBA8:
            m_image = new QImage( (unsigned char*)mData,
                                  mImagePixelWidth,
                                  mImagePixelHeight,
                                  QImage::Format_RGBA8888 );
            break;

        case hub::SensorSpec::Format::BGR8:
#if QT_VERSION <= QT_VERSION_CHECK( 5, 12, 0 )
            m_image = new QImage(
                (unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_BGR888 );
#elif QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
            m_image = new QImage(
                (unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_BGR888 );
#else
            m_image = new QImage(
                (unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_RGB888 );
#endif
            break;

        default:
            std::cout << "[paintEvent] unknown stream format" << std::endl;
            assert( false );
        }
        assert( !m_image->isNull() );
    }
    update();
}

void WidgetStreamView2D::paintEvent( QPaintEvent* event ) {

    Q_UNUSED( event );
    QPainter painter;

    painter.begin( this );

    if ( mData != nullptr ) {
        const QPoint p = QPoint( 0, 0 );

        QImage image = m_image->scaled( QSize( mCanvasPixelWidth, mCanvasPixelHeight ) );
        painter.drawImage( p, image );
    }
    else { painter.fillRect( 0, 0, width(), height(), Qt::gray ); }

    if ( mShowGrid ) {
        int alpha = 100;
        double tmp;
        for ( const auto& stepSize : g_stepSizes ) {
            tmp = stepSize;
            if ( mCanvasPixelPerUnit * stepSize > 25 ) break;
        }
        const double stepSize = tmp;

        {
            const int nHStep = std::ceil( mImageUnitWidth / stepSize );

            painter.setPen( QColor( 255, 0, 0, alpha ) );
            for ( int iHStep = 1; iHStep < nHStep; ++iHStep ) {
                int x = iHStep * stepSize * mCanvasPixelPerUnit;
                painter.drawLine( x, 0, x, mCanvasPixelHeight - 1 );
            }
        }

        {
            const int nVStep = std::floor( mImageUnitHeight / stepSize );

            painter.setPen( QColor( 255, 0, 0, alpha ) );
            for ( int iVStep = 1; iVStep < nVStep; ++iVStep ) {
                int y = iVStep * stepSize * mCanvasPixelPerUnit;
                painter.drawLine( 0, y, mCanvasPixelWidth - 1, y );
            }
        }
    }

    painter.end();
}

// void WidgetStreamView2D::resizeEvent(QResizeEvent* event)
//{
///" + QString::number(this->size().height()) + " pixels");

//}

const double& WidgetStreamView2D::getImageUnitHeight() const {
    return mImageUnitHeight;
}

const double& WidgetStreamView2D::getImageUnitWidth() const {
    return mImageUnitWidth;
}

void WidgetStreamView2D::setShowGrid( bool newShowGrid ) {
    mShowGrid = newShowGrid;
}

// void WidgetStreamView2D::setRatio(double newRatio)
//{
// }

// void WidgetStreamView2D::setRealSize(double realWidth, double realHeight)
//{

//}

double& WidgetStreamView2D::getCanvasPixelPerUnit() {
    return mCanvasPixelPerUnit;
}

const int& WidgetStreamView2D::getCanvasPixelHeight() const {
    return mCanvasPixelHeight;
}

const int& WidgetStreamView2D::getCanvasPixelWidth() const {
    return mCanvasPixelWidth;
}

// void WidgetStreamView2D::setCanvasPixelPerUnit(const double& newCanvasPixelPerUnit)
//{
// }

void WidgetStreamView2D::setRotateDeg( double newRotateDeg ) {
    mRotateDeg = newRotateDeg;
}

// const double& WidgetStreamView2D::getPixelPerUnit() const
//{
// }

///////////////////////////////////////////////////////////////////////////////

WidgetStreamView1D::WidgetStreamView1D( QWidget* parent ) : WidgetStreamView( parent ) {
    mLabel = new QLabel( this );

    mLabel->setMinimumSize( 350, 35 );
}

WidgetStreamView1D::~WidgetStreamView1D() {
    delete mLabel;
}

// void WidgetStreamView1D::setData(unsigned char* img_ptr, std::vector<int> dims,
// hub::SensorSpec::Format format)
//{

//}

void WidgetStreamView1D::setData( unsigned char* img_ptr,
                                  size_t size,
                                  std::vector<int> dims,
                                  hub::SensorSpec::Format format ) {

    float* translation = (float*)img_ptr;
    float* quaternion  = (float*)&img_ptr[12];
    std::string str =
        std::string( "x:" ) + std::to_string( translation[0] ) +
        ", y:" + std::to_string( translation[1] ) + ", z:" + std::to_string( translation[2] ) +
        "\naz:" + std::to_string( quaternion[0] ) + ", el:" + std::to_string( quaternion[1] ) +
        ", ro:" + std::to_string( quaternion[2] ) + ", q4:" + std::to_string( quaternion[3] );
    mLabel->setText( str.c_str() );
}
