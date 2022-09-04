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

WidgetStreamView::WidgetStreamView(QWidget* parent)
    : QWidget(parent)
{
}

//void WidgetStreamView::setData(unsigned char* img_ptr, std::vector<int> dims, hub::SensorSpec::Format format)
//{
//}

void WidgetStreamView::setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format)
{
}

///////////////////////////////////////////////////////////////////////////////
WidgetStreamView2D::WidgetStreamView2D(QWidget* parent)
    : WidgetStreamView(parent)
{
}

void WidgetStreamView2D::init(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth, double imageUnitHeight)
{
    std::cout << "[WidgetStreamView2D] init" << std::endl;
    mImagePixelWidth = imagePixelWidth;
    mImagePixelHeight = imagePixelHeight;
    mImageUnitWidth = (imageUnitWidth == 0.0) ? imagePixelWidth : imageUnitWidth;
    mImageUnitHeight = (imageUnitHeight == 0.0) ? imagePixelHeight : imageUnitHeight;
    mHPixelPerUnit = mImagePixelWidth / mImageUnitWidth;
    mVPixelPerUnit = mImagePixelHeight / mImageUnitHeight;
    //    mRatio = mImageUnitHeight / mImageUnitWidth;
    //    mRatio = mHPixelPerUnit / mVPixelPerUnit;
    //        mRatio = mVPixelPerUnit / mHPixelPerUnit;
    mRatio = mImageUnitHeight / mImageUnitWidth;
    //        mRatio = mImageUnitWidth / mImageUnitHeight;

    //    mCanvasPixelWidth = width;
    //    mCanvasPixelHeight = height;

    //    mCanvasPixelWidth = unitWidth * mHPixelPerUnit;
    //    mCanvasPixelHeight = unitHeight * mVPixelPerUnit;

    //    mImageUnitWidth = mUnitPerImageHPixel * mImagePixelWidth;
    //    mImageUnitHeight = mUnitPerImageVPixel * mImagePixelHeight;

    double unitWidth = mImagePixelWidth / mHPixelPerUnit;
    double unitHeight = mImagePixelHeight / mVPixelPerUnit;
    if (unitWidth * mRatio > unitHeight) {
        unitWidth = unitHeight / mRatio;
        //    mCanvasPixelPerUnit = mHPixelPerUnit;
    } else {
        unitHeight = unitWidth * mRatio;
        //    mCanvasPixelPerUnit = mVPixelPerUnit;
    }

    //    update();
    //    assert(mCanvasPixelPerUnit != nullptr);

    //        mCanvasPixelPerUnit = std::floor(std::min(width() / mImageUnitWidth, height() / mImageUnitHeight));
    //    mCanvasPixelPerUnit = 10.0;
    //        mCanvasPixelPerUnit = std::floor((width() / mImageUnitWidth + height() / mImageUnitHeight) / 2.0);
    //        mCanvasPixelPerUnit = std::floor(std::min(mHPixelPerUnit, mVPixelPerUnit));
    //        mCanvasPixelPerUnit = std::floor(std::max(mHPixelPerUnit, mVPixelPerUnit));
    mCanvasPixelPerUnit = std::floor((mHPixelPerUnit + mVPixelPerUnit) / 2.0);
    //    mCanvasPixelPerUnit = std::floor((mHPixelPerUnit + mVPixelPerUnit) / 2.0);
    //    mCanvasPixelPerUnit = std::floor(mHPixelPerUnit);

    mCanvasPixelWidth = unitWidth * mCanvasPixelPerUnit;
    mCanvasPixelHeight = unitHeight * mCanvasPixelPerUnit;

    setMinimumWidth(mCanvasPixelWidth);
    setMinimumHeight(mCanvasPixelHeight);

    setMaximumWidth(mCanvasPixelWidth);
    setMaximumHeight(mCanvasPixelHeight);

    //    onPixelPerUnitChanged();
}

void WidgetStreamView2D::clear()
{
    if (mData != nullptr) {
        delete [] mData;
        mData = nullptr;
    }
//    mData = nullptr;

    update();
}

// WidgetStreamView2D::WidgetStreamView2D(int imagePixelWidth, int imagePixelHeight, double imageUnitWidth, double imageUnitHeight, QWidget* parent)
//     : WidgetStreamView(parent)
//     , mImagePixelWidth(imagePixelWidth)
//     , mImagePixelHeight(imagePixelHeight)
//     , mImageUnitWidth(imageUnitWidth)
//     , mImageUnitHeight(imageUnitHeight)
//     , mRatio(mImageUnitHeight / mImageUnitWidth)
//     , mHPixelPerUnit(mImagePixelWidth / mImageUnitWidth)
//     , mVPixelPerUnit(mImagePixelHeight / mImageUnitHeight)
//{
//     int width = mImagePixelWidth;
//     int height = mImagePixelHeight;
//     if (width * mRatio > height) {
//         width = height / mRatio;
//     } else {
//         height = width * mRatio;
//     }
//     this->setMinimumWidth(width);
//     this->setMinimumHeight(height);

//    //    mCanvasPixelWidth = width;
//    //    mCanvasPixelHeight = height;

//    //    mImageUnitWidth = mUnitPerImageHPixel * mImagePixelWidth;
//    //    mImageUnitHeight = mUnitPerImageVPixel * mImagePixelHeight;
//}

//void WidgetStreamView2D::setData(unsigned char* img_ptr, std::vector<int> dims, hub::SensorSpec::Format format)
//{
//    assert(dims.size() == 2);
//    if (mImagePixelWidth != dims.at(0) || mImagePixelHeight != dims.at(1))
//        init(dims.at(0), dims.at(1));

//    mData = img_ptr;
//    assert(dims.size() == 2);
//    assert(mImagePixelWidth == dims.at(0));
//    assert(mImagePixelHeight == dims.at(1));
//    mFormat = format;

//    updateImage();
//}

void WidgetStreamView2D::setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format)
{
    assert(dims.size() == 2);
    if (mImagePixelWidth != dims.at(0) || mImagePixelHeight != dims.at(1))
        init(dims.at(0), dims.at(1));

    if (img_ptr == nullptr) {
        if (mData != nullptr) {
            delete [] mData;
        }
        update();
        return;
    }
    //    mData = img_ptr;
//    assert(mData == nullptr);
    if (mData == nullptr)
        mData = new unsigned char[size];

    memcpy(mData, img_ptr, size);
    assert(dims.size() == 2);
    assert(mImagePixelWidth == dims.at(0));
    assert(mImagePixelHeight == dims.at(1));
    mFormat = format;

    updateImage();
}

void WidgetStreamView2D::onPixelPerUnitChanged()
{
//    std::cout << "[WidgetStreamView2D] onPixelPerUnitChanged" << std::endl;
    double unitWidth = mImagePixelWidth / mHPixelPerUnit;
    double unitHeight = mImagePixelHeight / mVPixelPerUnit;
    if (unitWidth * mRatio > unitHeight) {
        unitWidth = unitHeight / mRatio;
    } else {
        unitHeight = unitWidth * mRatio;
    }

    //    update();
    //    assert(mCanvasPixelPerUnit != nullptr);

    mCanvasPixelWidth = unitWidth * mCanvasPixelPerUnit;
    mCanvasPixelHeight = unitHeight * mCanvasPixelPerUnit;

    //        mCanvasPixelWidth = mImagePixelWidth / mHPixelPerUnit;
    //        mCanvasPixelHeight = mImageUnitHeight * mVPixelPerUnit;

    this->setMinimumWidth(mCanvasPixelWidth);
    this->setMinimumHeight(mCanvasPixelHeight);
    setMaximumWidth(mCanvasPixelWidth);
    setMaximumHeight(mCanvasPixelHeight);

    update();
}

// void WidgetStreamView2D::canvasUpdated()
//{

//}


void WidgetStreamView2D::updateImage()
{
//    std::cout << "[WidgetStreamView2D] updateImage" << std::endl;

    //        QImage image;
    if (mData != nullptr) {
        switch (mFormat) {
        case hub::SensorSpec::Format::Y8:
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_Grayscale8);
            break;

        case hub::SensorSpec::Format::Y16:
        case hub::SensorSpec::Format::Z16:
#if QT_VERSION <= QT_VERSION_CHECK(5, 12, 0)
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_Grayscale16);
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_Grayscale16);
#else
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_Grayscale16);
#endif
            break;

        case hub::SensorSpec::Format::RGB8:
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_RGB888);
            break;

        case hub::SensorSpec::Format::RGBA8:
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_RGBA8888);
            break;

        case hub::SensorSpec::Format::BGR8:
#if QT_VERSION <= QT_VERSION_CHECK(5, 12, 0)
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_BGR888);
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_BGR888);
#else
        m_image = new QImage((unsigned char*)mData, mImagePixelWidth, mImagePixelHeight, QImage::Format_RGB888);
#endif
        break;

        default:
            std::cout << "[paintEvent] unknown stream format" << std::endl;
            assert(false);
        }
        assert(!m_image->isNull());
    }
    //    QMatrix rm;
    //    if (mRotateDeg != 0.0) {
    //        QTransform tr;
    //        tr.rotate(mRotateDeg);
    //        *m_image = m_image->transformed(tr);
    //    }
    update();
}

void WidgetStreamView2D::paintEvent(QPaintEvent* event)
{
//    std::cout << "[WidgetStreamView2D] paintEvent" << std::endl;

    //    assert(mCanvasPixelPerUnit != nullptr);
    Q_UNUSED(event);
    QPainter painter;

    painter.begin(this);
    //    painter.fillRect(0, 0, width(), height(), Qt::lightGray);
    //    painter.fillRect(0, 0, mCanvasPixelWidth, mCanvasPixelHeight, Qt::red);

    //    const int alpha = 100;

    //    painter.drawPixmap(rect(), *m_grid);
    if (mData != nullptr) {
        const QPoint p = QPoint(0, 0);

        //        QImage image = m_image->scaled(this->size());
        QImage image = m_image->scaled(QSize(mCanvasPixelWidth, mCanvasPixelHeight));
        painter.drawImage(p, image);
        //        painter.drawImage(p, *m_grid);

    } else {
        painter.fillRect(0, 0, width(), height(), Qt::gray);
//        return;
    }

    if (mShowGrid) {
        int alpha = 100;
//        const std::vector<double> stepSizes = { 1.0, 2.0, 5.0, 10.0, 20.0, 50.0 };
        double tmp;
        for (const auto& stepSize : g_stepSizes) {
            tmp = stepSize;
            if (mCanvasPixelPerUnit * stepSize > 25)
                break;
        }
        const double stepSize = tmp;

        {
            //        double mImageUnitWidth =
            const int nHStep = std::ceil(mImageUnitWidth / stepSize);

            //    painter.setPen(QColor(255, 0, 0, 127));
            painter.setPen(QColor(255, 0, 0, alpha));
            for (int iHStep = 1; iHStep < nHStep; ++iHStep) {
                int x = iHStep * stepSize * mCanvasPixelPerUnit;
                painter.drawLine(x, 0, x, mCanvasPixelHeight - 1);
            }
        }

        {
            const int nVStep = std::floor(mImageUnitHeight / stepSize);

            //    painter.setPen(QColor(255, 0, 0, 127));
            painter.setPen(QColor(255, 0, 0, alpha));
            for (int iVStep = 1; iVStep < nVStep; ++iVStep) {
                int y = iVStep * stepSize * mCanvasPixelPerUnit;
                painter.drawLine(0, y, mCanvasPixelWidth - 1, y);
            }
        }
    }

    painter.end();

    //    painter.begin(this);
    //////    painter.drawImage(*m_grid);
    //    painter.drawImage(QPoint(0, 0), *m_grid);
    //    painter.end();

    //    QWidget::paintEvent(event);
}

// void WidgetStreamView2D::resizeEvent(QResizeEvent* event)
//{
//     std::cout << "[WidgetStreamView2D] resizeEvent" << std::endl;
////    this->setWindowTitle(this->windowTitle() + " " + QString::number(this->size().width()) + " x " + QString::number(this->size().height()) + " pixels");
//    return;

//    //    if (width() == mCanvasPixelWidth && height() == mCanvasPixelHeight)
//    //        return;
//    double unitWidth = width() / mHPixelPerUnit;
//    double unitHeight = height() / mVPixelPerUnit;
//    if (unitWidth * mRatio > unitHeight) {
//        unitWidth = unitHeight / mRatio;
//        //    mCanvasPixelPerUnit = mVPixelPerUnit;
//    } else {
//        unitHeight = unitWidth * mRatio;
//        //    mCanvasPixelPerUnit = mHPixelPerUnit;
//    }

//    //    mCanvasPixelPerUnit = std::min()
//    //    update();
//    //    assert(mCanvasPixelPerUnit != nullptr);

//    //    mCanvasPixelPerUnit = mHPixelPerUnit;

//    mCanvasPixelWidth = unitWidth * mCanvasPixelPerUnit;
//    mCanvasPixelHeight = unitHeight * mCanvasPixelPerUnit;
//    //    this->setMaximumWidth(mCan)
//    //    this.setWid
//    //    this->resize(mCanvasPixelWidth, mCanvasPixelHeight);
//    return;
//    //        int width = this->width();
//    //        int height = this->height();
//    //        if (width * mRatio > height) {
//    //            width = height / mRatio;
//    //        } else {
//    //            height = width * mRatio;
//    //        }

//    //    double unitWidth = width() / mHPixelPerUnit;
//    //    double unitHeight = height() / mVPixelPerUnit;
//    //    if (unitWidth * mRatio > unitHeight) {
//    //        unitWidth = unitHeight / mRatio;
//    //    } else {
//    //        unitHeight = unitWidth * mRatio;
//    //    }

//    ////        mCanvasPixelWidth = width;
//    ////        mCanvasPixelHeight = height;
//    //    mCanvasPixelWidth = unitWidth * mHPixelPerUnit;
//    //    mCanvasPixelHeight = unitHeight * mVPixelPerUnit;

//    //    return;

//    //    double unitWidth = mImagePixelWidth / mHPixelPerUnit;
//    //    double unitHeight = mImagePixelHeight / mVPixelPerUnit;
//    //    if (unitWidth * mRatio > unitHeight) {
//    //        unitWidth = unitHeight / mRatio;
//    //    } else {
//    //        unitHeight = unitWidth * mRatio;
//    //    }

//    //    //    mCanvasPixelWidth = width;
//    //    //    mCanvasPixelHeight = height;

//    //    mCanvasPixelWidth = unitWidth * mHPixelPerUnit;
//    //    mCanvasPixelHeight = unitHeight * mVPixelPerUnit;

//    //    //    mHPixelPerUnit = mImagePixelWidth / mImageUnitWidth;
//    //    //    mCanvasPixelPerUnit =
//    //    const double pixelWidthPerUnit = mCanvasPixelWidth / mImageUnitWidth;
//    //    const double pixelHeightPerUnit = mCanvasPixelHeight / mImageUnitHeight;

//    //    mCanvasPixelWidth = width;
//    //    mCanvasPixelHeight = height;

//    //    mHPixelPerUnit = mImagePixelWidth / mImageUnitWidth;
//    //    mVPixelPerUnit = mImagePixelHeight / mImageUnitHeight;

//    //    const double pixelWidthPerUnit = mCanvasPixelWidth / (mCanvasPixelWidth / mHPixelPerUnit);
//    //    const double pixelHeightPerUnit = mCanvasPixelHeight / (mCanvasPixelHeight / mVPixelPerUnit);

//    //        const double pixelWidthPerUnit = mImageUnitWidth / (mCanvasPixelWidth * mHPixelPerUnit);
//    //        const double pixelHeightPerUnit = mImageUnitHeight / (mCanvasPixelHeight * mVPixelPerUnit);
//    //        assert(std::abs(pixelWidthPerUnit - pixelHeightPerUnit) < 0.1);

//    //        mCanvasPixelPerUnit = std::min(pixelWidthPerUnit, pixelHeightPerUnit);

//    //    m_grid = new QPixmap(width, height);
//    //    m_grid = new QImage(mCanvasPixelWidth, mCanvasPixelHeight, QImage::Format_BGR888);
//    //    m_grid = new QImage(mCanvasPixelWidth, mCanvasPixelHeight, QImage::Format_Grayscale8);
//    //    QPainter painter(m_grid);
//    //    painter.begin(this);

//    //    double pixelWidthPerUnit = width / mImageUnitWidth;
//    //    double stepSize = 5.0; // millimeters
//    //    double nHStep = std::floor(mImageUnitWidth / stepSize);

//    //    //    painter.setPen(QColor(255, 0, 0, 127));
//    //    painter.setPen(QColor(255, 0, 0, 255));
//    //    for (int iHStep = 1; iHStep < nHStep; ++iHStep) {
//    //        int x = iHStep * stepSize * pixelWidthPerUnit;
//    //        painter.drawLine(x, 0, x, height - 1);
//    //    }

//    //    painter.end();

//    //    *m_image = m_image->scaled(QSize(mCanvasPixelWidth, mCanvasPixelHeight));
//}

const double& WidgetStreamView2D::getImageUnitHeight() const
{
    return mImageUnitHeight;
}

const double& WidgetStreamView2D::getImageUnitWidth() const
{
    return mImageUnitWidth;
}

void WidgetStreamView2D::setShowGrid(bool newShowGrid)
{
    mShowGrid = newShowGrid;
}

// void WidgetStreamView2D::setRatio(double newRatio)
//{
//     mRatio = newRatio;
// }

// void WidgetStreamView2D::setRealSize(double realWidth, double realHeight)
//{
//     mImageUnitWidth = realWidth;
//     mImageUnitHeight = realHeight;

//    //        m_ratio = realWidth / realHeight;
//    mRatio = realHeight / realWidth;
//}

double& WidgetStreamView2D::getCanvasPixelPerUnit()
{
    return mCanvasPixelPerUnit;
}

const int& WidgetStreamView2D::getCanvasPixelHeight() const
{
    return mCanvasPixelHeight;
}

const int& WidgetStreamView2D::getCanvasPixelWidth() const
{
    return mCanvasPixelWidth;
}

// void WidgetStreamView2D::setCanvasPixelPerUnit(const double& newCanvasPixelPerUnit)
//{
//     mCanvasPixelPerUnit = &newCanvasPixelPerUnit;
// }

void WidgetStreamView2D::setRotateDeg(double newRotateDeg)
{
    mRotateDeg = newRotateDeg;
}

// const double& WidgetStreamView2D::getPixelPerUnit() const
//{
//     return mCanvasPixelPerUnit;
// }

///////////////////////////////////////////////////////////////////////////////

WidgetStreamView1D::WidgetStreamView1D(QWidget* parent)
    : WidgetStreamView(parent)
{
    mLabel = new QLabel(this);

    mLabel->setMinimumSize(350, 35);
}

WidgetStreamView1D::~WidgetStreamView1D()
{
    delete mLabel;
}

//void WidgetStreamView1D::setData(unsigned char* img_ptr, std::vector<int> dims, hub::SensorSpec::Format format)
//{

//    float* translation = (float*)img_ptr;
//    float* quaternion = (float*)&img_ptr[12];
//    std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
//    mLabel->setText(str.c_str());
//}


void WidgetStreamView1D::setData(unsigned char* img_ptr, size_t size, std::vector<int> dims, hub::SensorSpec::Format format)
{

    float* translation = (float*)img_ptr;
    float* quaternion = (float*)&img_ptr[12];
    std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
    mLabel->setText(str.c_str());
}
