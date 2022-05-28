#include "FrameRuler.h"

#include <iostream>

#include <QPainter>

FrameRuler::FrameRuler(QWidget* parent)
    : QFrame(parent)
{
}

void FrameRuler::paintEvent(QPaintEvent* event)
{
//    std::cout << "[FrameRuler] paintEvent" << std::endl;
    assert(mCanvasPixelPerUnit != nullptr);
    assert(mType != NONE);

    const int sideSize = (mType == Type::HORIZONTAL) ? width() : height();
    const double unitSide = sideSize / *mCanvasPixelPerUnit;

    const std::vector<double> stepSizes = { 1.0, 2.0, 5.0, 10.0, 15.0, 20.0, 50.0 };
    double tmp;
    for (const auto & stepSize : stepSizes) {
        tmp = stepSize;
        if (*mCanvasPixelPerUnit * stepSize > 25)
            break;
    }
    const double stepSize = tmp;

    const int nStep = std::ceil(unitSide / stepSize);

    int alpha = 100;

    QPainter painter;
    painter.begin(this);
    //    painter.setPen(QColor(255, 0, 0, 127));
    painter.setPen(QColor(0, 0, 0, alpha));
    for (int iStep = 1; iStep < nStep; ++iStep) {
        const int stepUnit = iStep * stepSize;
        const int xy = stepUnit * *mCanvasPixelPerUnit;
        const QString value = QString::number(stepUnit);
        const int dec = value.size() * 6 / 2;

        if (mType == Type::HORIZONTAL) {
            painter.drawLine(xy, 0, xy, sideSize - 1);
            painter.drawText(xy + 3, 15, value);

        } else {
            painter.drawLine(0, xy, sideSize - 1, xy);
            painter.drawText(9 - dec, xy + 12, value);
        }
    }

    painter.end();
}

void FrameRuler::setType(Type newType)
{
    mType = newType;
}

void FrameRuler::setPixelPerUnit(const double* newPixelPerUnit)
{
    mCanvasPixelPerUnit = newPixelPerUnit;
}

void FrameRuler::onPixelPerUnitChanged()
{
    update();
}
