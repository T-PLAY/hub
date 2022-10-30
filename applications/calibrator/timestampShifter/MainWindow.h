#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <Acquisition.hpp>
#include <vector>

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

    void on_pushButton_minus_clicked();
    void on_pushButton_plus_clicked();

private:
    void updateLatency();

  private:
    Ui::MainWindow* ui;

    hub::OutputSensor* m_outputSensor = nullptr;
//    hub::InputSensor* m_inputSensor   = nullptr;

//    std::thread m_thread;

    std::vector<hub::Acquisition> m_acqs;
    std::vector<hub::Acquisition> m_acqs2;
    long long m_latency = 0;
//    long long m_start   = 0;
//    long long m_end     = 0;
};

#endif // MAINWINDOW_H
