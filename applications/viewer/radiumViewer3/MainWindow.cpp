#include "MainWindow.h"
#include "./ui_MainWindow.h"

//#include <QWidget>
//#include <Engine/Rendering/Renderer.hpp>
//#include <Core/Utils/Singleton.hpp>

#include <Engine/RaEngine.hpp>
#include <QApplication>
#include <QTimer>

//#include <QApplication>

//#include <QOpenGLContext>

//#include <Engine/Data/ShaderConfigFactory.hpp>
#include <Engine/Rendering/ForwardRenderer.hpp>
//#include <Engine/RadiumEngine.hpp>
//#include <QOpenGLContext>

#include <Gui/Utils/KeyMappingManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <QLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setCentralWidget(ui->centralwidget);
    //    ui->centralwidget->show();

    // Initialize Engine.
    m_engine = Ra::Engine::RadiumEngine::createInstance();
    m_engine->initialize();

    ///\todo update when a basic viewer is implemented ... (to call setupKeyMappingCallbacks)
    Ra::Gui::KeyMappingManager::createInstance();
    Ra::Gui::KeyMappingManager::getInstance()->addListener(
        Ra::Gui::TrackballCameraManipulator::configureKeyMapping);
    Ra::Gui::KeyMappingManager::getInstance()->addListener(Ra::Gui::Viewer::configureKeyMapping);

    // Initialize taskqueue.
    m_task_queue = new Ra::Core::TaskQueue(std::thread::hardware_concurrency() - 1);

    // Initialize viewer.
    m_viewer = new Ra::Gui::Viewer;
    CORE_ASSERT(m_viewer != nullptr, "GUI was not initialized");

    m_viewerWidget = QWidget::createWindowContainer(m_viewer, ui->splitter);
    //    delete ui->widgetRadium;
    ui->layoutRadium->addWidget(m_viewerWidget);
    //    ui->splitter->replaceWidget(1, m_viewerWidget);
    //    ui->splitter->addWidget(m_viewerWidget);
    //    ui->widgetRadium = QWidget::createWindowContainer(m_viewer, this);
    //    setCentralWidget(m_viewerWidget);
    connect(m_viewer,
        &Ra::Gui::Viewer::requestEngineOpenGLInitialization,
        this,
        &MainWindow::onGLInitialized);
    m_viewer->setupKeyMappingCallbacks();

    m_viewer->show();
    //    CORE_ASSERT(m_viewer->getContext()->isValid(), "OpenGL was not initialized");

    // Create one system
    //    MinimalSystem* sys = new MinimalSystem;
    //    app.m_engine->registerSystem( "Minimal system", sys );

    //! [Creating the cube]
    auto cube = Ra::Core::Geometry::makeSharpBox({ 0.1f, 0.1f, 0.1f });
    //! [Creating the cube]

    //! [Colorize the Cube]
    cube.addAttrib(
        "in_color",
        Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::Green() });
    //! [Colorize the Cube]

    //! [Create the engine entity for the cube]
    auto e = m_engine->getEntityManager()->createEntity("Green cube");
    //! [Create the engine entity for the cube]

    //! [Create a geometry component with the cube]
    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Cube Mesh", e, std::move(cube), nullptr);
    //! [Create a geometry component with the cube]

    m_engine->registerSystem("GeometrySystem", new Ra::Engine::Scene::GeometrySystem, 1000); // BaseApplication.cpp
    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = m_engine->getSystem("GeometrySystem");
    geometrySystem->addComponent(e, c);
    //! [Register the entity/component association to the geometry system ]

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    m_viewer->prepareDisplay();
    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping);

    //    m_viewer->setCameraManipulator(new Ra::Gui::RotateAroundCameraManipulator(
    //        *(m_viewer->getCameraManipulator()), m_viewer));

    // Initialize timer for the spinning cube.
    m_frame_timer = new QTimer(this);
    m_frame_timer->setInterval(1000 / 60.0);

    // Start timer
    m_frame_timer->start();
}

MainWindow::~MainWindow()
{
    // need to clean up everithing before engine is cleaned up.
    delete m_task_queue;
    m_task_queue = nullptr;

    delete m_viewer;
    m_viewer = nullptr;

    m_engine->cleanup();
    delete m_engine;
    m_engine = nullptr;

    delete m_viewerWidget;
    m_viewerWidget = nullptr;

    delete ui;
}

void MainWindow::onGLInitialized()
{
    // initialize here the OpenGL part of the engine used by the application
    m_engine->initializeGL();

    // add the renderer
    std::shared_ptr<Ra::Engine::Rendering::Renderer> e(
        new Ra::Engine::Rendering::ForwardRenderer());
    m_viewer->addRenderer(e);
    connect(m_frame_timer, &QTimer::timeout, this, &MainWindow::frame);
}

void MainWindow::frame()
{
    // We use a fixed time step, but it is also possible
    // to check the time from last frame.
    const Scalar dt = 1.f / Scalar(m_target_fps);

    // Collect and run tasks
    m_engine->getTasks(m_task_queue, dt);
    m_task_queue->startTasks();
    m_task_queue->waitForTasks();
    m_task_queue->flushTaskQueue();

    // Starts the renderer
    m_viewer->startRendering(dt);

    // Finish the frame
    m_viewer->swapBuffers();

    m_engine->endFrameSync();
}
