#ifndef FORMIMAGEMANIPULATOR_H
#define FORMIMAGEMANIPULATOR_H

#include <QWidget>

namespace Ui {
class FormImageManipulator;
}

class FormImageManipulator : public QWidget {
    Q_OBJECT

public:
    explicit FormImageManipulator(QWidget* parent = nullptr);
    ~FormImageManipulator();

protected:
//    void paintEvent(QPaintEvent* event) override;

private:
    Ui::FormImageManipulator* ui;

    const double m_realWidth = 50.0; // millimeters
    const double m_realHeight = 35.0; // millimeters
    double m_ratio = 1.0;
};

#endif // FORMIMAGEMANIPULATOR_H
