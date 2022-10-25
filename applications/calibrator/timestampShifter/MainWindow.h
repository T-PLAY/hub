#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  private slots:

  private:
    void updateLatency();

  private:
    Ui::MainWindow* ui;

    hub::OutputSensor* m_outputSensor = nullptr;
    hub::InputSensor* m_inputSensor   = nullptr;

    std::thread m_thread;

    long long m_latency = 0;
    long long m_start   = 0;
    long long m_end     = 0;
};

#endif // MAINWINDOW_H
