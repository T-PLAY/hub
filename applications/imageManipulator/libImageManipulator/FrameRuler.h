#ifndef FRAMERULER_H
#define FRAMERULER_H

#include <QFrame>

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

public slots:
    void onPixelPerUnitChanged();

    void setType(Type newType);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const double * mCanvasPixelPerUnit = nullptr;
    Type mType = NONE;
};

#endif // FRAMERULER_H
