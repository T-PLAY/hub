#ifndef QSCROLLAREAGRID_H
#define QSCROLLAREAGRID_H

#include <QScrollArea>
#include <QWidget>
#include <QWheelEvent>

class QScrollAreaGrid : public QScrollArea
{
    Q_OBJECT
public:
    QScrollAreaGrid(QWidget * parent = nullptr);

    const double & getCanvasPixelPerUnit() const;

signals:
    void pixelPerUnitChanged();

protected:
    void wheelEvent(QWheelEvent * event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;



private:
    double mCanvasPixelPerUnit = 10.0;

    int mousePosX;
    int mousePosY;
    int hSliderPos;
    int vSliderPos;
    bool leftMouseDown = false;
};

#endif // QSCROLLAREAGRID_H
