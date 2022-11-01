#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Acquisition.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
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

    void on_pushButton_minus_2_clicked();
    void on_pushButton_plus_2_clicked();

    void on_spinBox_valueChanged( int arg1 );

    void on_checkBox_interpolate_toggled( bool checked );

  private:
    void updateLatency();

  private:
    Ui::MainWindow* ui;

    hub::OutputSensor* m_outputSensor = nullptr;

    int m_maxDist      = 8'000;
    bool m_interpolate = false;

    std::vector<hub::Acquisition> m_acqs;
    std::vector<hub::Acquisition> m_acqs2;
};

#endif // MAINWINDOW_H
