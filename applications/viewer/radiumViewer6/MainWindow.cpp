#include "MainWindow.h"

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include "./ui_MainWindow.h"
//#include <cassert>
//#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
#include <MinimalSystem.hpp>
#include <QDoubleSpinBox>
#include <QOpenGLContext>

#include <constants.h>
#include <filesystem>

#include <FormSensorViews.h>

//#include <QEvent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_recorder(PROJECT_DIR "data/")
{
    ui->setupUi(this);
    //    ui->dockWidgetContents_server->setMdiArea(ui->mdiArea);
    //    delete ui->dockWidget_server;

    // configure radium
    {
        // Create app and show viewer window
        m_app = new MinimalApp;
        auto& app = *m_app;

        //    QWidget* viewerWidget = QWidget::createWindowContainer( app.m_viewer.get(), this );
        QWidget* viewerWidget = QWidget::createWindowContainer(app.m_viewer.get());
        //    viewerWidget->setAutoFillBackground(false);
        //    ui->verticalLayout->addWidget(viewerWidget);
        //    ui->stackedWidget->update();
        const int currentIndex = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(1);
        //    ui->page3D->layout()->addWidget(viewerWidget);
        ui->verticalLayout_3dView->removeWidget(ui->frame);
        delete ui->frame;
        ui->verticalLayout_3dView->addWidget(viewerWidget);
        this->show();

        //    app.m_viewer->show();
        //    app.m_viewer->resize( { 500, 500 } );
        CORE_ASSERT(app.m_viewer->getContext()->isValid(), "OpenGL was not initialized");
        // process all events so that everithing is initialized
        QApplication::processEvents();

        // Create one system
        MinimalSystem* sys = new MinimalSystem;
        app.m_engine->registerSystem("Minimal system", sys);

        // Create and initialize entity and component
        Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity("Cube");
        m_comp = new MinimalComponent(e, *app.m_engine, *app.m_viewer);
        sys->addComponent(e, m_comp);
        m_comp->initialize();

        // prepare the viewer to render the scene (i.e. build RenderTechniques for the
        // active renderer)
        app.m_viewer->prepareDisplay();

        m_comp->updateShader();

        auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
        // Add default manipulator listener
        keyMappingManager->addListener(
            Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping);

        // Start the app.
        app.m_frame_timer->start();
        app.m_viewer->setCameraManipulator(new Ra::Gui::RotateAroundCameraManipulator(
            *(app.m_viewer->getCameraManipulator()), app.m_viewer.get()));

        ui->stackedWidget->setCurrentIndex(currentIndex);
    }

    //    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors, *this);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStarted, this, &MainWindow::on_startStreaming);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStopped, this, &MainWindow::on_stopStreaming);

    //    ui->label_scanSource->setText((g_probeScanSensorName + " :").c_str());
    //    ui->label_poseSource->setText((g_probePoseSensorName + " :").c_str());

    //    delete ui->dockWidget_loader;
    //    delete ui->dockWidget_server;

    m_formInputStreamViews = new FormInputStreamViews(this);
    ui->dockWidget_top->setWidget(m_formInputStreamViews);
    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::initPose, this, &MainWindow::onInitPose);
    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::initScan, this, &MainWindow::onInitScan);
    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::updatePose, this, &MainWindow::onUpdatePose);
    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::updateScan, this, &MainWindow::onUpdateScan);

    m_formWidgetLoader = new FormWidgetLoader(this);
    ui->dockWidget_bottom->setWidget(m_formWidgetLoader);
    QObject::connect(m_formWidgetLoader, &FormWidgetLoader::recordPathLoaded, this, &MainWindow::onRecordLoaderPathLoaded);
    QObject::connect(m_formWidgetLoader, &FormWidgetLoader::snapshotPathLoaded, this, &MainWindow::onSnapshotLoaderPathLoaded);

#ifdef USE_FORM_SENSOR_VIEWS
    m_formSensorViews = new FormSensorViews(this);
    ui->dockWidget_left->setWidget(m_formSensorViews);
#else
    ui->dockWidget_left->close();
#endif
    //    ui->dockWidgetContents_left = new FormSensorViews(this);

}

MainWindow::~MainWindow()
{
    std::cout << "[MainWindow] ~MainWindow() start" << std::endl;
    //    delete m_sensorViews;

    delete m_formInputStreamViews;
    delete m_formWidgetLoader;

#ifdef USE_FORM_SENSOR_VIEWS
    if (m_formSensorViews != nullptr)
        delete m_formSensorViews;
#endif
    delete m_app;
    delete ui;
    std::cout << "[MainWindow] ~MainWindow() end" << std::endl;
}

void MainWindow::onRecordLoaderPathLoaded()
{
    std::cout << "[MainWindow] onRecordLoaderPathLoaded()" << std::endl;
    const auto& recordOutputStreams = m_formWidgetLoader->getRecordLoader().getOutputStreamBuffs();

    m_formInputStreamViews->connect(recordOutputStreams);
}

void MainWindow::onSnapshotLoaderPathLoaded()
{
}

void MainWindow::on_action2D_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_action3D_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_startStreaming(std::string streamName)
{
    //    assert(m_sensorViews != nullptr);

    //    const auto & sensorView = m_sensorViews->getSensorView(streamName);
    //    const auto & inputStream = sensorView.getInputStream();

}

void MainWindow::on_stopStreaming(std::string streamName)
{
    std::cout << "[MainWindow] on_stopStreaming " << streamName << std::endl;

    //    assert(m_sensorViews != nullptr);

    //    if (streamName.substr(0, g_probeScanSensorName.size()) == g_probeScanSensorName) {
    //        auto sourceType = (g_probeScanSensorName.size() == streamName.size())
    //            ? ("physical")
    //            : (streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2));
    //        int iItem = ui->comboBox_scan->findText(sourceType.c_str());
    //        assert(iItem >= 0);
    //        ui->comboBox_scan->removeItem(iItem);

    //    } else if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {
    //        auto sourceType = (g_probePoseSensorName.size() == streamName.size())
    //            ? ("physical")
    //            : (streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2));
    //        int iItem = ui->comboBox_pose->findText(sourceType.c_str());
    //        assert(iItem >= 0);
    //        ui->comboBox_pose->removeItem(iItem);
    //    }
}

void MainWindow::onInitPose()
{
    m_comp->initPose();

}

void MainWindow::onInitScan()
{
    m_comp->initScan();

}

void MainWindow::onUpdatePose()
{
    std::cout << "[MainWindow] onUpdatePose()" << std::endl;
    m_comp->updatePose(m_formInputStreamViews->getPoseAcquisition());

}

void MainWindow::onUpdateScan()
{
    std::cout << "[MainWindow] onUpdateScan()" << std::endl;
    m_comp->updateScan(m_formInputStreamViews->getScanAcquisition());
}

//void MainWindow::on_comboBox_scan_currentTextChanged(const QString& sourceType)
//{
//}

//void MainWindow::on_comboBox_pose_currentTextChanged(const QString& sourceType)
//{
//}

//void MainWindow::onNewScanAcquisition()
//{
//}

//void MainWindow::onNewPoseAcquisition()
//{
//}

void MainWindow::on_toolButton_record_clicked()
{
    //    if (m_recorder.isRecording()) {

    //        m_recorder.stop();
    //        ui->toolButton_record->setText("startRecording");

    //    } else {
    //        InputStreamParameters inputStreamParameters;
    //        if (m_activeStreamScan != "") {
    //            inputStreamParameters.push_back({ m_activeStreamScan, "" });
    //        }
    //        if (m_activeStreamPose != "") {
    //            inputStreamParameters.push_back({ m_activeStreamPose, "" });
    //        }
    //        if (inputStreamParameters.empty()) {
    //            return;
    //        }
    //        m_recorder.record(inputStreamParameters);
    //        ui->toolButton_record->setText("stopRecording");
    //    }
}

void MainWindow::on_toolButton_snapshot_clicked()
{
    Frame frame;

    //    if (m_activeStreamScan != "") {
    //        Snapshot scanSnapshot(m_threadInputStreamScan->mInputStream, m_threadInputStreamScan->mAcq);
    //        frame.push_back(scanSnapshot);
    //    }
    //    if (m_activeStreamPose != "") {
    //        Snapshot poseSnapshot(m_threadInputStreamPose->mInputStream, m_threadInputStreamPose->mAcq);
    //        frame.push_back(poseSnapshot);
    //    }
    //    if (frame.empty()) {
    //        return;
    //    }
    //    m_recorder.save(frame);
}
