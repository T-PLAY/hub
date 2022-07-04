#include "MainWindow.h"

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include "./ui_MainWindow.h"
//#include <cassert>
//#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
//#include <MinimalSystem.hpp>
#include <QDoubleSpinBox>
#include <QOpenGLContext>

#include <constants.h>
#include <filesystem>

#include <FormSensorViews.h>

#include <Core/Asset/FileLoaderInterface.hpp>
#include <DicomLoader/DicomLoader.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

//#include <QEvent>
// the default priority for systems created here.
constexpr int defaultSystemPriority = 1000;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    ui->dockWidgetContents_server->setMdiArea(ui->mdiArea);
    //    delete ui->dockWidget_server;

    ui->dockWidgetContents_right->setMinimumWidth(500);

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
        ui->layout3dView->removeWidget(ui->frame);
        delete ui->frame;
        ui->layout3dView->addWidget(viewerWidget);
        this->show();

        //    app.m_viewer->show();
        //    app.m_viewer->resize( { 500, 500 } );
        CORE_ASSERT(app.m_viewer->getContext()->isValid(), "OpenGL was not initialized");
        // process all events so that everithing is initialized
        QApplication::processEvents();

        // Create one system
        //        MinimalSystem* sys = new MinimalSystem;
        //        app.m_engine->registerSystem("Minimal system", sys);
        app.m_engine->registerSystem(
            "GeometrySystem", new Ra::Engine::Scene::GeometrySystem, defaultSystemPriority);
        auto* sys = app.m_engine->getSystem("GeometrySystem");

        // Create and initialize entity and component
        Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity("Cube");
        m_comp = new MinimalComponent(e, *app.m_engine, *app.m_viewer);
        sys->addComponent(e, m_comp);
        m_comp->initialize();

        app.m_engine->registerFileLoader(std::shared_ptr<Ra::Core::Asset::FileLoaderInterface>(new Ra::IO::DicomLoader()));
                app.m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");
//        app.m_engine->loadFile("/home/gauthier/document/projet/Clone_css/resourcesCStrike/maps/de_aztec/de_aztec.obj");

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

    m_comp->getRoGrid().setVisible(ui->checkBox_grid->isChecked());
    m_comp->traceSetVisible(ui->checkBox_trace->isChecked());

    auto& streamView = ui->dockWidgetContents_right->getWidgetStreamView();
    streamView.init(512, 192, 35.0, 50.0);

    //    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors, *this);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStarted, this, &MainWindow::on_startStreaming);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStopped, this, &MainWindow::on_stopStreaming);

    //    ui->label_scanSource->setText((g_probeScanSensorName + " :").c_str());
    //    ui->label_poseSource->setText((g_probePoseSensorName + " :").c_str());

    //    delete ui->dockWidget_loader;
    //    delete ui->dockWidget_server;

    m_formInputStreamViews = new FormInputStreamViews(*ui->mdiArea, this);
    ui->dockWidget_top->setWidget(m_formInputStreamViews);
    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::initPose, this,
    //    &MainWindow::onInitPose); QObject::connect(m_formInputStreamViews,
    //    &FormInputStreamViews::initScan, this, &MainWindow::onInitScan);
    QObject::connect(m_formInputStreamViews,
        &FormInputStreamViews::newAcquisition,
        this,
        &MainWindow::onNewAcquisition);
    QObject::connect(
        m_formInputStreamViews, &FormInputStreamViews::init, this, &MainWindow::onInit);
    QObject::connect(m_formInputStreamViews,
        &FormInputStreamViews::selectedSourceChanged,
        this,
        &MainWindow::onSelectedSourceChanged);
    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::newAcquisitionScan, this,
    //    &MainWindow::onUpdateScan);

    m_formWidgetLoader = new FormWidgetLoader(this);
    ui->dockWidget_bottom->setWidget(m_formWidgetLoader);
    QObject::connect(m_formWidgetLoader,
        &FormWidgetLoader::recordPathLoaded,
        this,
        &MainWindow::onRecordLoaderPathLoaded);
    QObject::connect(m_formWidgetLoader,
        &FormWidgetLoader::snapshotPathLoaded,
        this,
        &MainWindow::onSnapshotLoaderPathLoaded);

#ifdef USE_FORM_SENSOR_VIEWS
    m_formSensorViews = new FormSensorViews(this);

    //    if (m_formSensorViews->isServerConnected()) {
    ui->dockWidget_left->setWidget(m_formSensorViews);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::streamingStarted,
        this,
        &MainWindow::onServerStreamStarted);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::streamingStopped,
        this,
        &MainWindow::onServerStreamStopped);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::serverDisconnected,
        this,
        &MainWindow::onServerDisconnected);
    //        m_formSensorViews->startStreaming();
//    } else {
//        delete m_formSensorViews;
//        ui->dockWidget_left->close();
//    }
#else
    ui->dockWidget_left->close();
#endif
    //    ui->dockWidgetContents_left = new FormSensorViews(this);
}

MainWindow::~MainWindow()
{
    std::cout << "[MainWindow] ~MainWindow() start" << std::endl;
    //    delete m_sensorViews;

    delete m_formWidgetLoader;
    delete m_formInputStreamViews;

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
    const auto& recordLoader = m_formWidgetLoader->getRecordLoader();
    const auto& outputStreamBuffs = recordLoader.getOutputStreamBuffs();

    for (const auto& pair : outputStreamBuffs) {
        const auto& streamName = pair.first;
        auto& cyclicBuff = const_cast<CyclicBuff&>(*pair.second);

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream(streamName, RamIO(cyclicBuff));
        //    m_formInputStreamViews->connect(recordOutputStreams);

        if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {
            const auto& poseAcqs = recordLoader.getAcquisitions(g_probePoseSensorName);
            m_comp->initPoseTraces(poseAcqs);
        }
    }
}

void MainWindow::onSnapshotLoaderPathLoaded()
{
    std::cout << "[MainWindow] onRecordLoaderPathLoaded()" << std::endl;
    const auto& outputStreamBuffs = m_formWidgetLoader->getSnapshotLoader().getOutputStreamBuffs();

    for (const auto& pair : outputStreamBuffs) {
        const auto& streamName = pair.first;
        auto& cyclicBuff = const_cast<CyclicBuff&>(*pair.second);

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream(streamName, RamIO(cyclicBuff));
        //    m_formInputStreamViews->connect(recordOutputStreams);
    }
}

void MainWindow::on_action2D_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_action3D_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

#ifdef USE_FORM_SENSOR_VIEWS
void MainWindow::onServerStreamStarted(const std::string& sensorName,
    const std::string& syncSensorName)
{
    std::cout << "[MainWindow] onServerStreamStarted()" << std::endl;

    //    ClientSocket * socket = nullptr;
    //    if (sensorName == g_probePoseSensorName) {
    //        socket = new ClientSocket(sensorName, syncSensorName);
    //    }

    //    m_formInputStreamViews->addInputStream(sensorName, std::move(*socket));

    m_formInputStreamViews->addInputStream(sensorName,
        ClientSocket(sensorName, syncSensorName));
}

void MainWindow::onServerStreamStopped(const std::string& sensorName)
{
    std::cout << "[MainWindow] onServerStreamStopped()" << std::endl;

    m_formInputStreamViews->deleteInputStream(sensorName);
    //        m_formInputStreamViews->onKillInputStream(sensorName);
}

void MainWindow::onServerDisconnected()
{
    delete m_formSensorViews;
    m_formSensorViews = nullptr;
    //    ui->dockWidgetContents_left.c
    ui->dockWidget_left->close();
}

#endif

void MainWindow::onInit(const std::string& sensorName)
{
    std::cout << "[MainWindow] onInit '" << sensorName << "'" << std::endl;
    if (sensorName == g_probeScanSensorName) {
        m_comp->initScan();
        ui->dockWidgetContents_right->init();
    } else if (sensorName == g_probePoseSensorName) {
        m_comp->initPose();
    }
}

// void MainWindow::onInitPose()
//{
//    std::cout << "[MainWindow] onInitPose" << std::endl;
//    m_comp->initPose();
//}

// void MainWindow::onInitScan()
//{
//    std::cout << "[MainWindow] onInitScan" << std::endl;
//    m_comp->initScan();
//}

void MainWindow::onNewAcquisition(const std::string& sensorName, const std::string& sourceType)
{
    //    std::cout << "[MainWindow] onNewAcquisition(" << sensorName << ", " << sourceType << ")"
    //    << std::endl;
    //    const auto& acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    //    auto && acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    auto& acqs = m_formInputStreamViews->getAcquisitions(sensorName, sourceType);

    if (acqs.empty()) {
        std::cout << "[MainWindow] void signal, empty acqs --------------" << std::endl;
        return;
    }
    assert(!acqs.empty());

    if (sensorName == g_probeScanSensorName) {
        //        for ( const auto& acq : acqs ) {

        //        m_comp->updateScan(acqs);
        //        while (! acqs.empty()) {
        const auto& acq = acqs.front();

        m_comp->updateScan(acq);
        ui->dockWidgetContents_right->update(acq);

        //            acqs.pop();
        //        }
        //        }
    } else if (sensorName == g_probePoseSensorName) {
        //        for ( const auto& acq : acqs ) {
        //        m_comp->updatePose( acq );
        //        }
        //        m_comp->updatePose(acqs);
        //        while (! acqs.empty()) {
        ////            const auto & acq = acqs.front();
        ////            std::cout << "[MainWindow] update pose : " << acq << std::endl;
        m_comp->updatePose(acqs.front());
        //            acqs.pop();
        //        }
    }
    acqs.pop();
    //    else {
    // do n
    //        assert( false );
    //    }
}

void MainWindow::onSelectedSourceChanged(const std::string& sensorName, const std::string& sourceType)
{
    std::cout << "[MainWindow] onSelectedSourceChanged(" << sensorName << ", " << sourceType << ")" << std::endl;

    const auto& inputStream = m_formInputStreamViews->getInputStream(sensorName, sourceType);

    std::cout << inputStream << std::endl;

    const auto& metadata = inputStream.getMetaData();
    double scanWidth = 5.0;
    if (metadata.find("scanWidth") != metadata.end()) {
        scanWidth = std::any_cast<double>(metadata.at("scanWidth"));
    }

    double scanDepth = 3.5;
    if (metadata.find("scanDepth") != metadata.end()) {
        scanDepth = std::any_cast<double>(metadata.at("scanDepth"));
    }

    double x = 3.1;
    if (metadata.find("x") != metadata.end()) {
        x = std::any_cast<double>(metadata.at("x"));
    }
    double y = 0.0;
    if (metadata.find("y") != metadata.end()) {
        y = std::any_cast<double>(metadata.at("y"));
    }
    double z = 16.0;
    if (metadata.find("z") != metadata.end()) {
        z = std::any_cast<double>(metadata.at("z"));
    }

    m_comp->setupScanner(scanWidth, scanDepth, x, y, z);

    m_app->m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");
    //    m_app->m_viewer->prepareDisplay();
}

// void MainWindow::onUpdatePose()
//{
//     //    std::cout << "[MainWindow] onUpdatePose()" << std::endl;
////    m_comp->updatePose(m_formInputStreamViews->getPoseAcquisition());
//}

// void MainWindow::onUpdateScan()
//{
//     //    std::cout << "[MainWindow] onUpdateScan()" << std::endl;
////    m_comp->updateScan(m_formInputStreamViews->getScanAcquisition());
//}

// void MainWindow::on_comboBox_scan_currentTextChanged(const QString& sourceType)
//{
// }

// void MainWindow::on_comboBox_pose_currentTextChanged(const QString& sourceType)
//{
// }

// void MainWindow::onNewScanAcquisition()
//{
// }

// void MainWindow::onNewPoseAcquisition()
//{
// }

void MainWindow::on_checkBox_grid_toggled(bool checked)
{
    m_comp->getRoGrid().setVisible(checked);
}

void MainWindow::on_checkBox_trace_toggled(bool checked)
{
    m_comp->traceSetVisible(checked);
}

void MainWindow::on_toolButton_fitScene_clicked()
{
    m_app->m_viewer->fitCamera();
}

#include <Core/Utils/Color.hpp>
void MainWindow::on_toolButton_fitTrace_clicked()
{
    auto& viewer = *m_app->m_viewer;
    //    auto aabb = Ra::Engine::RadiumEngine::getInstance()->computeSceneAabb();
    //    auto aabb = m_app->m_engine->computeSceneAabb();
    const auto& traces = m_comp->getRoTraces();
    //    auto aabb = traces[0].compu
    auto aabb = traces[0]->computeAabb();
    for (int i = 1; i < g_nTraces; ++i) {
        aabb.extend(traces[i]->computeAabb());
    }

    if (aabb.isEmpty()) {
        viewer.getCameraManipulator()->resetCamera();
    } else {
        viewer.fitCameraToScene(aabb);
    }

    //    RA_DISPLAY_AABB( aabb, Ra::Core::Utils::Color::Blue() );
}

void MainWindow::loadFile(QString path)
{
    std::string filename = path.toLocal8Bit().data();
    //    LOG( logINFO ) << "Loading file " << filename << "...";
    bool res = m_app->m_engine->loadFile(filename);

    if (!res) {
        std::cout << "Aborting file loading !";
        //        return false;
    }

    m_app->m_engine->releaseFile();

    //    m_mainWindow->prepareDisplay();
    //    m_app->m_viewer->update(0);

    //    emit loadComplete();
    //    return true;
}
