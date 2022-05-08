#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MinimalApp.hpp>
#include <MinimalComponent.hpp>
#include <QMainWindow>

#include <SensorViews.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow( QWidget* parent = nullptr );
    ~MainWindow();

  public slots:

  public:
private slots:
    void on_action2D_triggered();
    void on_action3D_triggered();

//    void on_newSensor(std::string sensorName);
//    void on_delSensor(std::string sensorName);

    void on_startStreaming(std::string sensorName);
    void on_stopStreaming(std::string sensorName);

    void on_comboBox_scan_currentTextChanged(const QString &sensorName);
    void on_comboBox_pose_currentTextChanged(const QString &sensorName);

    void on_newScanAcquisition();
    void on_newPoseAcquisition();

private:
    Ui::MainWindow* ui;

    MinimalApp* m_app        = nullptr;
    MinimalComponent* m_comp = nullptr;

    SensorViews * m_sensorViews = nullptr;

    const Thread_InputStream * m_threadInputStreamScan = nullptr;
    std::string m_activeStreamScan = "";
    const Thread_InputStream * m_threadInputStreamPose = nullptr;
    std::string m_activeStreamPose = "";

};
#endif // MAINWINDOW_H
