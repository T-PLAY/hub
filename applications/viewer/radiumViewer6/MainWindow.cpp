#include "MainWindow.h"

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include "./ui_MainWindow.h"
//#include <cassert>
//#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
#include <MinimalSystem.hpp>
#include <QOpenGLContext>
#include <QDoubleSpinBox>

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
    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors);
    QObject::connect(m_sensorViews, &SensorViews::streamingStarted, this,
        &MainWindow::on_startStreaming);
    QObject::connect(m_sensorViews, &SensorViews::streamingStopped, this,
        &MainWindow::on_stopStreaming);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::sensorDeleted, this,
    //        &MainWindow::on_delSensor );
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

void MainWindow::on_startStreaming(std::string sensorName)
{
    assert(m_sensorViews != nullptr);
    //    const auto & sensorView = m_sensorViews->getSensorView(sensorName);
    //    const auto & inputStream = sensorView.getInputStream();

    if (sensorName == "ULA-OP 256") {
        ui->comboBox_scan->addItem(sensorName.c_str());
    } else if (sensorName == "Polhemus Patriot (probe)") {
        ui->comboBox_pose->addItem(sensorName.c_str());
    }
}

void MainWindow::on_stopStreaming(std::string sensorName)
{
    assert(m_sensorViews != nullptr);

    if (sensorName == "ULA-OP 256") {
        int iItem = ui->comboBox_scan->findText(sensorName.c_str());
        assert(iItem >= 0);
        ui->comboBox_scan->removeItem(iItem);
    } else if (sensorName == "Polhemus Patriot (probe)") {
        int iItem = ui->comboBox_pose->findText(sensorName.c_str());
        assert(iItem >= 0);
        ui->comboBox_pose->removeItem(iItem);
    }
}

void MainWindow::on_comboBox_scan_currentTextChanged(
    const QString& sensorName)
{
    const auto& sensorNameStd = sensorName.toStdString();
    std::cout << "[on_comboBox_scan_currentTextChanged] " << sensorNameStd
              << std::endl;
    assert(m_sensorViews != nullptr);

    if (sensorNameStd != "" && sensorNameStd != m_activeStreamScan) {
        QObject::disconnect(m_threadInputStreamScan,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newScanAcquisition);
    }

    if (sensorNameStd == "") {
        assert(m_activeStreamScan != "");
        m_threadInputStreamScan = nullptr;

    } else {
        const auto& sensorView = m_sensorViews->getSensorView(sensorNameStd);
        m_threadInputStreamScan = sensorView.getInputStreamThread();

        QObject::connect(m_threadInputStreamScan,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newScanAcquisition);
    }
    m_activeStreamScan = sensorNameStd;
}

void MainWindow::on_comboBox_pose_currentTextChanged(
    const QString& sensorName)
{
    const auto& sensorNameStd = sensorName.toStdString();
    std::cout << "[on_comboBox_pose_currentTextChanged] " << sensorNameStd
              << std::endl;
    assert(m_sensorViews != nullptr);

    if (sensorNameStd != "" && sensorNameStd != m_activeStreamPose) {
        QObject::disconnect(m_threadInputStreamPose,
            &Thread_InputStream::newAcquisition, this,
            &MainWindow::on_newPoseAcquisition);
    }

    if (sensorNameStd == "") {
        assert(m_activeStreamPose != "");
        m_threadInputStreamPose = nullptr;

    } else {
        const auto& sensorView = m_sensorViews->getSensorView(sensorNameStd);
        m_threadInputStreamPose = sensorView.getInputStreamThread();

        QObject::connect(m_threadInputStreamPose, &Thread_InputStream::newAcquisition,
            this, &MainWindow::on_newPoseAcquisition);
    }

    m_activeStreamPose = sensorNameStd;
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
