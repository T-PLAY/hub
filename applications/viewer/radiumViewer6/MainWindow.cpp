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

//#include <FormSensorViews.h>

//#include <Core/Asset/FileLoaderInterface.hpp>
//#include <DicomLoader/DicomLoader.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

//#include <Core/Geometry/MeshPrimitives.hpp>
//#include <Engine/Scene/EntityManager.hpp>
//#include <Engine/Scene/GeometryComponent.hpp>
//#include <Engine/Scene/GeometrySystem.hpp>

//#include <ImplicitVolume.h>

//#include <QEvent>
// the default priority for systems created here.
constexpr int defaultSystemPriority = 1000;

//#define CUBE_VOLUME

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    ui->dockWidgetContents_server->setMdiArea(ui->mdiArea);
    //    delete ui->dockWidget_server;

    // configure radium
    //    {
    // Create app and show viewer window
    m_app = new MinimalApp;
    auto& app = *m_app;

    //    QWidget* viewerWidget = QWidget::createWindowContainer( app.m_viewer.get(), this );
    QWidget* viewerWidget = QWidget::createWindowContainer(app.m_viewer.get());
    //    viewerWidget->setAutoFillBackground(false);
    //    ui->verticalLayout->addWidget(viewerWidget);
    //    ui->stackedWidget->update();
    //        const int currentIndex = ui->stackedWidget->currentIndex();
    //        ui->stackedWidget->setCurrentIndex(1);
    //    ui->page3D->layout()->addWidget(viewerWidget);
    //        ui->layout3dView->removeWidget(ui->frame);
    //        delete ui->frame;
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
    //        Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity("Cube");
    //        m_comp = new MinimalComponent(e, *app.m_engine, *app.m_viewer);
    //        sys->addComponent(e, m_comp);
    //        m_comp->initialize();

//    app.m_engine->registerFileLoader(std::shared_ptr<Ra::Core::Asset::FileLoaderInterface>(new Ra::IO::DicomLoader()));
    //    app.m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the
    // active renderer)
    app.m_viewer->prepareDisplay();

    //        m_comp->updateShader();

    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping);

    // Start the app.
    app.m_frame_timer->start();
    app.m_viewer->setCameraManipulator(new Ra::Gui::RotateAroundCameraManipulator(
        *(app.m_viewer->getCameraManipulator()), app.m_viewer.get()));

    //        ui->stackedWidget->setCurrentIndex(currentIndex);
    //    }

    //    m_guiManager.m_dockLeft = ui->dockWidget_left;
    //    m_guiManager.m_dockTop = ui->dockWidget_top;
    //    m_guiManager.m_dockRight = ui->dockWidget_right;
    //    m_guiManager.m_dockBottom = ui->dockWidget_bottom;

    m_guiManager.m_mdiArea = ui->mdiArea;
    m_guiManager.m_engine = app.m_engine.get();
    m_guiManager.m_viewer = app.m_viewer.get();
    m_guiManager.m_system = sys;

    m_guiManager.m_mainWindow = this;
    m_guiManager.m_stackedWidget = ui->stackedWidget;
    m_guiManager.m_layout3DView = ui->layout3dView;

    m_guiManager.init();

    //    m_comp->getRoGrid().setVisible(ui->checkBox_grid->isChecked());
    //    m_comp->traceSetVisible(ui->checkBox_trace->isChecked());

//    app.m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");
    //    ImplicitVolume::cube(*app.m_engine);
//    app.m_viewer->prepareDisplay();

    //    m_sensorViews = new SensorViews(*ui->verticalLayout_sensors, *ui->mdiArea_sensors, *this);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStarted, this, &MainWindow::on_startStreaming);
    //    QObject::connect(
    //        m_sensorViews, &SensorViews::streamingStopped, this, &MainWindow::on_stopStreaming);

    //    ui->label_scanSource->setText((g_probeScanSensorName + " :").c_str());
    //    ui->label_poseSource->setText((g_probePoseSensorName + " :").c_str());

    //    delete ui->dockWidget_loader;
    //    delete ui->dockWidget_server;
}

MainWindow::~MainWindow()
{
    std::cout << "[MainWindow] ~MainWindow() start" << std::endl;
    //    delete m_sensorViews;

    m_guiManager.clear();

    delete m_app;
    delete ui;
    std::cout << "[MainWindow] ~MainWindow() end" << std::endl;
}
