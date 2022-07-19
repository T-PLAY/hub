#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#pragma once

//#include <QObject>
#include <QApplication>

#include <FormSensorViews.h>
#include <FormInputStreamViews.h>
#include <FormWidgetLoader.h>
#include <MinimalComponent.hpp>
//#include <Imagema

#include <FormImageManipulator.h>
#include <QStackedWidget>
#include <Form3DToolBox.h>

#include <SceneManager.h>


class GuiManager : public QObject
{
    Q_OBJECT

public:
    explicit GuiManager(QObject *parent = nullptr);
//    GuiManager();

    ~GuiManager();

    void init();
//    void incIter();

signals:

public slots:

    void onRecordLoaderPathLoaded();
    void onSnapshotLoaderPathLoaded();

public:
private slots:
    void on_action2D_triggered();
    void on_action3D_triggered();

    //    void on_newSensor(std::string sensorName);
    //    void on_delSensor(std::string sensorName);

    void onServerStreamStarted(const std::string& sensorName, const std::string & syncSensorName);
    void onServerStreamStopped(const std::string& sensorName);
    void onServerDisconnected();

    //    void on_comboBox_scan_currentTextChanged(const QString &sourceType);
    //    void on_comboBox_pose_currentTextChanged(const QString &sourceType);

    //    void on_newScanAcquisition();
    //    void on_newPoseAcquisition();
//    void onInitPose();
//    void onInitScan();
    void onInit(const std::string& sensorName);
    //    void onNewAcquisition()
//    void onUpdatePose();
//    void onUpdateScan();

    void onNewAcquisition(const std::string & sensorName, const std::string & sourceType);

    //    void on_toolButton_record_toggled(bool checked);
    //    void on_toolButton_record_clicked();
    //    void on_toolButton_snapshot_clicked();

    void onSelectedSourceChanged(const std::string & sensorName, const std::string & sourceType);


    void on_checkBox_grid_toggled(bool checked);
    void on_checkBox_trace_toggled(bool checked);
    void on_toolButton_fitScene_clicked();

    void on_toolButton_fitTrace_clicked();

    void loadFile( QString path );


public:

    QMdiArea * m_mdiArea = nullptr;

    Ra::Engine::RadiumEngine * m_engine = nullptr;
    Ra::Gui::Viewer * m_viewer = nullptr;
    Ra::Engine::Scene::System * m_system = nullptr;

    QMainWindow * m_mainWindow = nullptr;
    QStackedWidget * m_stackedWidget = nullptr;
    QVBoxLayout * m_layout3DView = nullptr;

private:
    QDockWidget * m_dockLeft = nullptr;
    QDockWidget * m_dockTop = nullptr;
    QDockWidget * m_dockRight = nullptr;
    QDockWidget * m_dockBottom = nullptr;

    QToolBar * m_toolBarTopLeft = nullptr;
    QToolBar * m_toolBarTopRight = nullptr;
    QAction * m_action3D = nullptr;
    QAction * m_action2D = nullptr;
    QAction * m_actionSettings = nullptr;
    QAction * m_actionAddSources = nullptr;

    //    SensorViews * m_sensorViews = nullptr;
//    MinimalComponent* m_comp = nullptr;
    SceneManager m_sceneManager;

    FormWidgetLoader* m_formWidgetLoader = nullptr;
//    FormInputStreamViews* m_formInputStreamViews = nullptr;
    FormSensorViews* m_formSensorViews = nullptr;

//    MainWindowStreamView * m_streamView = nullptr;
//    FormImageManipulator * m_imageManipulator = nullptr;
    FormImageManipulator m_imageManipulator;
    Form3DToolBox * m_3DToolBox = nullptr;

    QTableView * m_sensorsView = nullptr;

    bool m_initialized = false;

};

#endif // GUIMANAGER_H
