#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <FormImageManipulator.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FormImageManipulator * m_imageManipulator = nullptr;

};

#endif // MAINWINDOW_H
