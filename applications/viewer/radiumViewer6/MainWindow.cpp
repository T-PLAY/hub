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

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );
    //    ui->dockWidgetContents_server->setMdiArea(ui->mdiArea);
    //    delete ui->dockWidget_server;

    // configure radium
    {
        // Create app and show viewer window
        m_app     = new MinimalApp;
        auto& app = *m_app;

        //    QWidget* viewerWidget = QWidget::createWindowContainer( app.m_viewer.get(), this );
        QWidget* viewerWidget = QWidget::createWindowContainer( app.m_viewer.get() );
        //    viewerWidget->setAutoFillBackground(false);
        //    ui->verticalLayout->addWidget(viewerWidget);
        //    ui->stackedWidget->update();
        const int currentIndex = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex( 1 );
        //    ui->page3D->layout()->addWidget(viewerWidget);
        ui->verticalLayout_3dView->removeWidget( ui->frame );
        delete ui->frame;
        ui->verticalLayout_3dView->addWidget( viewerWidget );
        this->show();

        //    app.m_viewer->show();
        //    app.m_viewer->resize( { 500, 500 } );
        CORE_ASSERT( app.m_viewer->getContext()->isValid(), "OpenGL was not initialized" );
        // process all events so that everithing is initialized
        QApplication::processEvents();

        // Create one system
        MinimalSystem* sys = new MinimalSystem;
        app.m_engine->registerSystem( "Minimal system", sys );

        // Create and initialize entity and component
        Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity( "Cube" );
        m_comp                       = new MinimalComponent( e, *app.m_engine, *app.m_viewer );
        sys->addComponent( e, m_comp );
        m_comp->initialize();

        // prepare the viewer to render the scene (i.e. build RenderTechniques for the
        // active renderer)
        app.m_viewer->prepareDisplay();

        m_comp->updateShader();

        auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
        // Add default manipulator listener
        keyMappingManager->addListener(
            Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping );

        // Start the app.
        app.m_frame_timer->start();
        app.m_viewer->setCameraManipulator( new Ra::Gui::RotateAroundCameraManipulator(
            *( app.m_viewer->getCameraManipulator() ), app.m_viewer.get() ) );

        ui->stackedWidget->setCurrentIndex( currentIndex );
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

    m_formInputStreamViews = new FormInputStreamViews( this );
    ui->dockWidget_top->setWidget( m_formInputStreamViews );
    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::initPose, this,
    //    &MainWindow::onInitPose); QObject::connect(m_formInputStreamViews,
    //    &FormInputStreamViews::initScan, this, &MainWindow::onInitScan);
    QObject::connect( m_formInputStreamViews,
                      &FormInputStreamViews::newAcquisition,
                      this,
                      &MainWindow::onNewAcquisition );
    QObject::connect(
        m_formInputStreamViews, &FormInputStreamViews::init, this, &MainWindow::onInit );
    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::newAcquisitionScan, this,
    //    &MainWindow::onUpdateScan);

    m_formWidgetLoader = new FormWidgetLoader( this );
    ui->dockWidget_bottom->setWidget( m_formWidgetLoader );
    QObject::connect( m_formWidgetLoader,
                      &FormWidgetLoader::recordPathLoaded,
                      this,
                      &MainWindow::onRecordLoaderPathLoaded );
    QObject::connect( m_formWidgetLoader,
                      &FormWidgetLoader::snapshotPathLoaded,
                      this,
                      &MainWindow::onSnapshotLoaderPathLoaded );

#ifdef USE_FORM_SENSOR_VIEWS
    m_formSensorViews = new FormSensorViews( this );
    ui->dockWidget_left->setWidget( m_formSensorViews );
    QObject::connect( m_formSensorViews,
                      &FormSensorViews::streamingStarted,
                      this,
                      &MainWindow::onServerStreamStarted );
    QObject::connect( m_formSensorViews,
                      &FormSensorViews::streamingStopped,
                      this,
                      &MainWindow::onServerStreamStopped );
//        m_formSensorViews->startStreaming();
#else
    ui->dockWidget_left->close();
#endif
    //    ui->dockWidgetContents_left = new FormSensorViews(this);
}

MainWindow::~MainWindow() {
    std::cout << "[MainWindow] ~MainWindow() start" << std::endl;
    //    delete m_sensorViews;

    delete m_formWidgetLoader;
    delete m_formInputStreamViews;

#ifdef USE_FORM_SENSOR_VIEWS
    if ( m_formSensorViews != nullptr ) delete m_formSensorViews;
#endif
    delete m_app;
    delete ui;
    std::cout << "[MainWindow] ~MainWindow() end" << std::endl;
}

void MainWindow::onRecordLoaderPathLoaded() {
    std::cout << "[MainWindow] onRecordLoaderPathLoaded()" << std::endl;
    const auto& outputStreamBuffs = m_formWidgetLoader->getRecordLoader().getOutputStreamBuffs();

    for ( const auto& pair : outputStreamBuffs ) {
        const auto& streamName = pair.first;
        auto& cyclicBuff       = const_cast<CyclicBuff&>( *pair.second );

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream( streamName, RamIO( cyclicBuff ) );
        //    m_formInputStreamViews->connect(recordOutputStreams);
    }
}

void MainWindow::onSnapshotLoaderPathLoaded() {
    std::cout << "[MainWindow] onRecordLoaderPathLoaded()" << std::endl;
    const auto& outputStreamBuffs = m_formWidgetLoader->getSnapshotLoader().getOutputStreamBuffs();

    for ( const auto& pair : outputStreamBuffs ) {
        const auto& streamName = pair.first;
        auto& cyclicBuff       = const_cast<CyclicBuff&>( *pair.second );

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream( streamName, RamIO( cyclicBuff ) );
        //    m_formInputStreamViews->connect(recordOutputStreams);
    }
}

void MainWindow::on_action2D_triggered() {
    ui->stackedWidget->setCurrentIndex( 0 );
}

void MainWindow::on_action3D_triggered() {
    ui->stackedWidget->setCurrentIndex( 1 );
}

#ifdef USE_FORM_SENSOR_VIEWS
void MainWindow::onServerStreamStarted( const std::string& sensorName,
                                        const std::string& syncSensorName ) {
    std::cout << "[MainWindow] onServerStreamStarted()" << std::endl;

    //    ClientSocket * socket = nullptr;
    //    if (sensorName == g_probePoseSensorName) {
    //        socket = new ClientSocket(sensorName, syncSensorName);
    //    }

    //    m_formInputStreamViews->addInputStream(sensorName, std::move(*socket));

    m_formInputStreamViews->addInputStream( sensorName,
                                            ClientSocket( sensorName, syncSensorName ) );
}

void MainWindow::onServerStreamStopped( const std::string& sensorName ) {
    std::cout << "[MainWindow] onServerStreamStopped()" << std::endl;

    m_formInputStreamViews->deleteInputStream( sensorName );
    //        m_formInputStreamViews->onKillInputStream(sensorName);
}

#endif

void MainWindow::onInit( const std::string& sensorName ) {
    if ( sensorName == g_probeScanSensorName ) { m_comp->initScan(); }
    else if ( sensorName == g_probePoseSensorName ) {
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

void MainWindow::onNewAcquisition( const std::string& sensorName, const std::string& sourceType ) {
    //    std::cout << "[MainWindow] onNewAcquisition(" << sensorName << ", " << sourceType << ")"
    //    << std::endl;
    //    const auto& acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    //    auto && acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    auto& acqs = m_formInputStreamViews->getAcquisitions( sensorName, sourceType );

    if ( sensorName == g_probeScanSensorName ) {
//        for ( const auto& acq : acqs ) {

//        m_comp->updateScan(acqs);
//        while (! acqs.empty()) {
            m_comp->updateScan( acqs.front() );
            acqs.pop();
//        }
//        }
    }
    else if ( sensorName == g_probePoseSensorName ) {
//        for ( const auto& acq : acqs ) {
//        m_comp->updatePose( acq );
//        }
//        m_comp->updatePose(acqs);
//        while (! acqs.empty()) {
////            const auto & acq = acqs.front();
////            std::cout << "[MainWindow] update pose : " << acq << std::endl;
            m_comp->updatePose( acqs.front() );
            acqs.pop();
//        }
    }
    else {
        assert( false );
    }
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
