#ifndef FORMIMAGEMANIPULATOR_H
#define FORMIMAGEMANIPULATOR_H

#include <QWidget>

namespace Ui {
class FormImageManipulator;
}

class FormImageManipulator : public QWidget
{
    Q_OBJECT

public:
    explicit FormImageManipulator(QWidget *parent = nullptr);
    ~FormImageManipulator();

private:
    Ui::FormImageManipulator *ui;
};

#endif // FORMIMAGEMANIPULATOR_H
