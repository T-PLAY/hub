#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include <QMainWindow>

namespace Ui {
    class SelectWindow;
}

class SelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectWindow(QWidget *parent = nullptr);
    ~SelectWindow();

private:
    Ui::SelectWindow *ui;
};

#endif