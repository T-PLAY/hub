#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

#include <SensorViews.h>

// Q_DECLARE_METATYPE(Streamer);
// Q_DECLARE_METATYPE(std::string)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:


private:
    Ui::MainWindow* ui;

    SensorViews * m_sensorViews = nullptr;
};

#endif // MAINWINDOW_H
