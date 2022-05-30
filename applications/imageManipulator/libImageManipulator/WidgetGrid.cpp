#include "WidgetGrid.h"

#include <QPainter>
#include <iostream>
#include <cmath>

WidgetGrid::WidgetGrid(QWidget* parent)
    : QWidget { parent }
{
}

void WidgetGrid::paintEvent(QPaintEvent* event)
{
    assert(mCanvasPixelPerUnit != nullptr);
    assert(mImageUnitWidth != nullptr);
    assert(mImageUnitHeight != nullptr);
    assert(mCanvasPixelWidth != nullptr);
    assert(mCanvasPixelHeight != nullptr);
//    std::cout << "[WidgetGrid] paintEvent" << std::endl;

    //        this->setMinimumWidth(*mCanvasPixelWidth);
    //        this->setMaximumWidth(*mCanvasPixelWidth);
    //        this->setMinimumHeight(*mCanvasPixelHeight);
    //        this->setMaximumHeight(*mCanvasPixelHeight);

    QPainter painter(this);
    //    painter.fillRect(0, 0, width(), height(), Qt::green);
    //    painter.drawLine(0, 0, 100, 100);
//    int alpha = 100;
//    painter.setPen(QPen(Qt::red));
//    painter.setPen(QColor(255, 0, 0, 50));
    painter.setPen(Qt::gray);

    const std::vector<double> stepSizes = { 1.0, 2.0, 5.0, 10.0, 20.0, 50.0 };
    double tmp;
    for (const auto& stepSize : stepSizes) {
        tmp = stepSize;
        if (*mCanvasPixelPerUnit * stepSize > 25)
            break;
    }
    const double stepSize = tmp;

        const int hZero = (width() - *mCanvasPixelWidth) / 2.0;
        const int vZero = (height() - *mCanvasPixelHeight) / 2.0;
    {
        //        double mImageUnitWidth =
        const int nHStep = std::ceil(*mImageUnitWidth / stepSize);
        //            const int nHStep = std::ceil((*mCanvasPixelWidth / *mCanvasPixelPerUnit) / stepSize);

        //    painter.setPen(QColor(255, 0, 0, 127));
        //            painter.setPen(QColor(255, 0, 0, alpha));
        for (int iHStep = 0; iHStep <= nHStep; ++iHStep) {
            int x = iHStep * stepSize * *mCanvasPixelPerUnit;
            painter.drawLine(hZero + x, 0, hZero + x, height() - vZero - 1 + 50);
        }
    }

    {
        const int nVStep = std::ceil(*mImageUnitHeight / stepSize);

        //    painter.setPen(QColor(255, 0, 0, 127));
        //            painter.setPen(QColor(255, 0, 0, alpha));
        for (int iVStep = 0; iVStep <= nVStep; ++iVStep) {
            int y = iVStep * stepSize * *mCanvasPixelPerUnit;
            painter.drawLine(0, vZero + y, width() - hZero - 1 + 50, vZero + y);
        }
    }

    painter.end();
}

void WidgetGrid::onPixelPerUnitChanged()
{
    update();
}

void WidgetGrid::setCanvasPixelHeight(const int* newCanvasPixelHeight)
{
    mCanvasPixelHeight = newCanvasPixelHeight;
}

void WidgetGrid::setCanvasPixelWidth(const int* newCanvasPixelWidth)
{
    mCanvasPixelWidth = newCanvasPixelWidth;
}

void WidgetGrid::setImageUnitHeight(const double* newImageUnitHeight)
{
    mImageUnitHeight = newImageUnitHeight;
}

void WidgetGrid::setImageUnitWidth(const double* newImageUnitWidth)
{
    mImageUnitWidth = newImageUnitWidth;
}

void WidgetGrid::setCanvasPixelPerUnit(const double* newCanvasPixelPerUnit)
{
    mCanvasPixelPerUnit = newCanvasPixelPerUnit;
}
