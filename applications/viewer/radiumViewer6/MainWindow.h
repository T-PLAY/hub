#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MinimalApp.hpp>
#include <MinimalComponent.hpp>
#include <QMainWindow>
#include <QFileSystemModel>

//#include <SensorViews.h>
#include <Player.h>
#include <Recorder.h>

//#define USE_FORM_SENSOR_VIEWS
#ifdef USE_FORM_SENSOR_VIEWS
#include <FormSensorViews.h>
#endif

#include <FormWidgetLoader.h>
#include <FormInputStreamViews.h>


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

    void onRecordLoaderPathLoaded();
    void onSnapshotLoaderPathLoaded();

  public:
private slots:
    void on_action2D_triggered();
    void on_action3D_triggered();

//    void on_newSensor(std::string sensorName);
//    void on_delSensor(std::string sensorName);


//    void on_comboBox_scan_currentTextChanged(const QString &sourceType);
//    void on_comboBox_pose_currentTextChanged(const QString &sourceType);

//    void on_newScanAcquisition();
//    void on_newPoseAcquisition();
    void onInitPose();
    void onInitScan();
    void onUpdatePose();
    void onUpdateScan();

//    void on_toolButton_record_toggled(bool checked);
    void on_toolButton_record_clicked();
    void on_toolButton_snapshot_clicked();



private:
    Ui::MainWindow* ui;

    MinimalApp* m_app        = nullptr;
    MinimalComponent* m_comp = nullptr;

//    SensorViews * m_sensorViews = nullptr;

    FormWidgetLoader * m_formWidgetLoader = nullptr;

    FormInputStreamViews * m_formInputStreamViews = nullptr;

#ifdef USE_FORM_SENSOR_VIEWS
    FormSensorViews * m_formSensorViews = nullptr;
#endif


    Recorder m_recorder;
};
#endif // MAINWINDOW_H
