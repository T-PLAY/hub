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

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    // configure radium
    // Create app and show viewer window
    m_app     = new MinimalApp;
    auto& app = *m_app;

    QWidget* viewerWidget = QWidget::createWindowContainer( app.m_viewer.get() );
    ui->layout3dView->addWidget( viewerWidget );
    this->show();

    CORE_ASSERT( app.m_viewer->getContext()->isValid(), "OpenGL was not initialized" );
    // process all events so that everithing is initialized
    QApplication::processEvents();

    // Create one system
    app.m_engine->registerSystem(
        "GeometrySystem", new Ra::Engine::Scene::GeometrySystem, defaultSystemPriority );
    auto* sys = app.m_engine->getSystem( "GeometrySystem" );

    // Create and initialize entity and component


    // prepare the viewer to render the scene (i.e. build RenderTechniques for the
    // active renderer)
    app.m_viewer->prepareDisplay();


    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping );

    // Start the app.
    app.m_frame_timer->start();
    app.m_viewer->setCameraManipulator( new Ra::Gui::RotateAroundCameraManipulator(
        *( app.m_viewer->getCameraManipulator() ), app.m_viewer.get() ) );



    m_guiManager.m_mdiArea = ui->mdiArea;
    m_guiManager.m_engine = app.m_engine;
    m_guiManager.m_viewer = app.m_viewer.get();
    m_guiManager.m_system = sys;

    m_guiManager.m_mainWindow    = this;
    m_guiManager.m_stackedWidget = ui->stackedWidget;
    m_guiManager.m_layout3DView  = ui->layout3dView;

    m_guiManager.init();

    connect( ui->actionreloadShaders,
             &QAction::triggered,
             app.m_viewer.get(),
             &Ra::Gui::Viewer::reloadShaders );





}

MainWindow::~MainWindow() {
    std::cout << "[MainWindow] ~MainWindow() start" << std::endl;

    m_guiManager.clear();

    delete m_app;
    delete ui;
    std::cout << "[MainWindow] ~MainWindow() end" << std::endl;
}
