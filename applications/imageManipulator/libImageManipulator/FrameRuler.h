#ifndef FRAMERULER_H
#define FRAMERULER_H

#include <QFrame>
#include <QScrollBar>

class FrameRuler : public QFrame {
    Q_OBJECT
public:
    enum Type {
        NONE,
        HORIZONTAL,
        VERTICAL,
    };
    FrameRuler(QWidget* parent = nullptr);

    void setPixelPerUnit(const double *newPixelPerUnit);

    void setScrollBar(const QScrollBar *newScrollBar);

    void setCanvasPixelSize(const int *newCanvasPixelSize);

public slots:
    void onPixelPerUnitChanged();

    void setType(Type newType);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const double * mCanvasPixelPerUnit = nullptr;
    const int * mCanvasPixelSize = nullptr;
    Type mType = NONE;

    const QScrollBar * mScrollBar = nullptr;
};

#endif // FRAMERULER_H
