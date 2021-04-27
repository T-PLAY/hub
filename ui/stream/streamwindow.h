#ifndef STREAMWINDOW_H
#define STREAMWINDOW_H

#include <QMainWindow>

namespace Ui {
    class StreamWindow;
}

class StreamWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StreamWindow(QWidget *parent = nullptr);
    ~StreamWindow();

private:
    Ui::StreamWindow *ui;
};

#endif