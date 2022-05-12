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

//#include <QEvent>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create app and show viewer window
    m_app = new MinimalApp;
    auto& app = *m_app;

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
    CORE_ASSERT(app.m_viewer->getContext()->isValid(),
        "OpenGL was not initialized");
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

    //    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors,
    //    *ui->mdiArea_sensors, *ui->comboBox_scan, *ui->comboBox_pose);
//    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors);
    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors);
    QObject::connect(m_sensorViews, &SensorViews::streamingStarted, this,
        &MainWindow::on_startStreaming);
    QObject::connect(m_sensorViews, &SensorViews::streamingStopped, this,
        &MainWindow::on_stopStreaming);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::sensorDeleted, this,
    //        &MainWindow::on_delSensor );

    ui->label_scanSource->setText((g_probeScanSensorName + " :").c_str());
    ui->label_poseSource->setText((g_probePoseSensorName + " :").c_str());

    // records view
    QString recordPath = PROJECT_DIR "data/records/";
    assert(std::filesystem::exists(recordPath.toStdString()));
    m_recordFileModel = new QFileSystemModel(this);

    m_recordFileModel->setReadOnly(true);

    // Set filter
    m_recordFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::AllEntries);
    //    m_recordFileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);

    // QFileSystemModel requires root path
    m_recordFileModel->setRootPath(recordPath);

    // Attach the model to the view
    ui->treeView_record->setModel(m_recordFileModel);
    const auto& rootIndex = m_recordFileModel->index(recordPath);
    ui->treeView_record->setRootIndex(rootIndex);
    ui->treeView_record->expand(rootIndex);
    ui->treeView_record->setColumnHidden(1, true);
    ui->treeView_record->setColumnHidden(2, true);
    ui->treeView_record->setColumnHidden(3, true);
    //    ui->treeView->setItemsExpandable(false);
    //    ui->treeView->setColumnHidden(0, true);
}

MainWindow::~MainWindow()
{
    delete m_sensorViews;
    delete m_app;
}

void MainWindow::on_action2D_triggered()
{
    //    ui->page3D->hide();
    //    ui->page2D->show();
    //    ui->page2D->update();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_action3D_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
    //    ui->page2D->hide();
    //    ui->page3D->show();
    //    ui->stackedWidget->update();
}

void MainWindow::on_startStreaming(std::string streamName)
{
    assert(m_sensorViews != nullptr);
    //    const auto & sensorView = m_sensorViews->getSensorView(streamName);
    //    const auto & inputStream = sensorView.getInputStream();
    const auto sensorName = streamName.substr(0, g_probeScanSensorName.size());

    if (sensorName == g_probeScanSensorName) {

        const auto sourceType = streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2);
        assert(sourceType == "record");
        ui->comboBox_scan->addItem(sourceType.c_str());

    } else if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {

        const auto sourceType = streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2);
        ui->comboBox_pose->addItem(sourceType.c_str());
    }
}

void MainWindow::on_stopStreaming(std::string streamName)
{
    std::cout << "[MainWindow] on_stopStreaming " << streamName << std::endl;

    assert(m_sensorViews != nullptr);

    if (streamName.substr(0, g_probeScanSensorName.size()) == g_probeScanSensorName) {
        auto sourceType = streamName.substr(g_probeScanSensorName.size() + 2, streamName.size() - 1 - g_probeScanSensorName.size() - 2);
        int iItem = ui->comboBox_scan->findText(sourceType.c_str());
        assert(iItem >= 0);
        ui->comboBox_scan->removeItem(iItem);

    } else if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {
        auto sourceType = streamName.substr(g_probePoseSensorName.size() + 2, streamName.size() - 1 - g_probePoseSensorName.size() - 2);
        int iItem = ui->comboBox_pose->findText(sourceType.c_str());
        assert(iItem >= 0);
        ui->comboBox_pose->removeItem(iItem);
    }
}

void MainWindow::on_comboBox_scan_currentTextChanged(
    const QString& sourceType)
{
    const auto& streamNameStd = g_probeScanSensorName + " (" + sourceType.toStdString() + ")";
    std::cout << "[on_comboBox_scan_currentTextChanged] " << streamNameStd
              << std::endl;
    assert(m_sensorViews != nullptr);

    if (sourceType != "" && streamNameStd != m_activeStreamScan) {
        QObject::disconnect(m_threadInputStreamScan,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newScanAcquisition);
    }

    if (sourceType == "") {
        assert(m_activeStreamScan != "");
        m_threadInputStreamScan = nullptr;
        m_comp->initScan();

    } else {
        const auto& sensorView = m_sensorViews->getSensorView(streamNameStd);
        m_threadInputStreamScan = sensorView.getInputStreamThread();

        QObject::connect(m_threadInputStreamScan,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newScanAcquisition);
    }
    m_activeStreamScan = streamNameStd;
}

void MainWindow::on_comboBox_pose_currentTextChanged(
    const QString& sourceType)
{
    const auto& streamNameStd = g_probePoseSensorName + " (" + sourceType.toStdString() + ")";
    std::cout << "[on_comboBox_pose_currentTextChanged] " << streamNameStd
              << std::endl;
    assert(m_sensorViews != nullptr);

    if (sourceType != "" && streamNameStd != m_activeStreamPose) {
        QObject::disconnect(m_threadInputStreamPose,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newPoseAcquisition);
    }

    if (sourceType == "") {
        assert(m_activeStreamPose != "");
        m_threadInputStreamPose = nullptr;
        m_comp->initProbe();

    } else {
        const auto& sensorView = m_sensorViews->getSensorView(streamNameStd);
        m_threadInputStreamPose = sensorView.getInputStreamThread();

        QObject::connect(m_threadInputStreamPose, &Thread_InputStream::newAcquisition,
            this, &MainWindow::on_newPoseAcquisition);
    }

    m_activeStreamPose = streamNameStd;
}

void MainWindow::on_newScanAcquisition()
{
    if (m_threadInputStreamScan == nullptr) {
        std::cout << "[on_newScanAcquisition] ##################################### nulptr" << std::endl;
        m_comp->initScan();
        return;
    }

    assert(m_threadInputStreamScan != nullptr);
    //    std::cout << "[on_newScanAcquisition] " << std::endl;

    m_comp->updateScan(m_threadInputStreamScan->mAcq);
}

void MainWindow::on_newPoseAcquisition()
{
    if (m_threadInputStreamPose == nullptr) {
        std::cout << "[on_newPoseAcquisition] ##################################### nulptr" << std::endl;
        m_comp->initProbe();
        return;
    }

    assert(m_threadInputStreamPose != nullptr);
    //    std::cout << "[on_newPoseAcquisition] " << std::endl;

    m_comp->updateProbe(m_threadInputStreamPose->mAcq);
}

void MainWindow::on_toolButton_record_toggled(bool checked)
{
    if (checked) {
        ui->toolButton_record->setText("stopRecording");

    } else {
        ui->toolButton_record->setText("startRecording");
    }
}

void MainWindow::on_toolButton_snapshot_clicked()
{
}

void MainWindow::on_treeView_record_clicked(const QModelIndex& index)
{
    std::string mPath = m_recordFileModel->fileInfo(index).absoluteFilePath().toStdString();

    if (mPath == m_player.getLoadedPath()) {
        std::cout << "stop playing " << mPath << std::endl;
        const auto& selectionModel = ui->treeView_record->selectionModel();
        selectionModel->select(index, QItemSelectionModel::Deselect);
//        m_recordFilePlaying = "";
        m_player.stop();
        m_player.unload();
    } else {
//        m_recordFilePlaying = mPath;
        std::cout << "play new record " << mPath << std::endl;
        m_player.load(mPath);
        m_player.play();
    }
}
