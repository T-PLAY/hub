#ifndef WIDGETGRID_H
#define WIDGETGRID_H

#include <QWidget>

class WidgetGrid : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetGrid(QWidget *parent = nullptr);

    void setCanvasPixelPerUnit(const double *newCanvasPixelPerUnit);

    void setImageUnitWidth(const double *newImageUnitWidth);

    void setImageUnitHeight(const double *newImageUnitHeight);

    void setCanvasPixelWidth(const int *newCanvasPixelWidth);

    void setCanvasPixelHeight(const int *newCanvasPixelHeight);

protected:
    void paintEvent(QPaintEvent * event) override;

public slots:
    void onPixelPerUnitChanged();

signals:

private:
    const double * mCanvasPixelPerUnit = nullptr;
    const double * mImageUnitWidth = nullptr;
    const double * mImageUnitHeight = nullptr;
    const int * mCanvasPixelWidth = nullptr;
    const int * mCanvasPixelHeight = nullptr;

};

#endif // WIDGETGRID_H
