#include <Engine/Data/ShaderConfigFactory.hpp>
#include <Engine/Rendering/ForwardRenderer.hpp>
#include <MinimalApp.hpp>

#include <Gui/Utils/KeyMappingManager.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>

//#include <Core/Asset/FileLoaderInterface.hpp>
//#include <DicomLoader/DicomLoader.hpp>
//#include <Engine/Scene/GeometrySystem.hpp>
//#include <Engine/Scene/SkeletonBasedAnimationSystem.hpp>>

// the default priority for systems created here.
// constexpr int defaultSystemPriority = 1000;

#include <Engine/Data/TextureManager.hpp>

MinimalApp::MinimalApp() :
    m_task_queue( nullptr ), m_viewer( nullptr ), m_frame_timer( nullptr ), m_target_fps( 40 ) {
    // Set application and organization names in order to ensure uniform
    // QSettings configurations.
    // \see http://doc.qt.io/qt-5/qsettings.html#QSettings-4
    QCoreApplication::setOrganizationName( "STORM-IRIT" );
    QCoreApplication::setApplicationName( "RadiumViewer" );

    // Initialize Engine.
    m_engine = Ra::Engine::RadiumEngine::createInstance();
    m_engine->initialize();

    // Register the TimeSystem managing time dependant systems

    ///\todo update when a basic viewer is implemented ... (to call setupKeyMappingCallbacks)
    Ra::Gui::KeyMappingManager::createInstance();
    Ra::Gui::KeyMappingManager::getInstance()->addListener(
        Ra::Gui::TrackballCameraManipulator::configureKeyMapping );
    Ra::Gui::KeyMappingManager::getInstance()->addListener( Ra::Gui::Viewer::configureKeyMapping );

    // Initialize taskqueue.
    m_task_queue.reset( new Ra::Core::TaskQueue( std::thread::hardware_concurrency() - 1 ) );
    // Initialize viewer.
    m_viewer.reset( new Ra::Gui::Viewer );

    CORE_ASSERT( m_viewer != nullptr, "GUI was not initialized" );
    connect( m_viewer.get(),
             &Ra::Gui::Viewer::requestEngineOpenGLInitialization,
             this,
             &MinimalApp::onGLInitialized );
    m_viewer->setupKeyMappingCallbacks();

    // Initialize timer for the spinning cube.
    m_frame_timer = new QTimer( this );
    m_frame_timer->setInterval( 1000 / m_target_fps );
}

MinimalApp::~MinimalApp() {
    std::cout << "[MinimalApp] ~MinimalApp() start" << std::endl;
    m_frame_timer->stop();

    // need to clean up everithing before engine is cleaned up.
    m_task_queue.reset( nullptr );
    m_viewer.reset( nullptr );
    m_engine->cleanup();
    std::cout << "[MinimalApp] ~MinimalApp() end" << std::endl;
}

void MinimalApp::onGLInitialized() {
    // initialize here the OpenGL part of the engine used by the application
    m_engine->initializeGL();
    // add the renderer
    std::shared_ptr<Ra::Engine::Rendering::Renderer> e(
        new Ra::Engine::Rendering::ForwardRenderer() );
    m_viewer->addRenderer( e );
    connect( m_frame_timer, &QTimer::timeout, this, &MinimalApp::frame );
}

void MinimalApp::frame() {

    // We use a fixed time step, but it is also possible
    // to check the time from last frame.
    const Scalar dt = 1.f / Scalar( m_target_fps );

    // Collect and run tasks
    m_engine->getTasks( m_task_queue.get(), dt );
    m_task_queue->startTasks();
    m_task_queue->waitForTasks();
    m_task_queue->flushTaskQueue();

#ifdef USE_PR_UPDATE_TEXTURE
    m_viewer->makeCurrent();
    m_engine->runGpuTasks();
    m_viewer->doneCurrent();
#endif


    // Starts the renderer
    m_viewer->startRendering( dt );

    //#ifdef NO_PR_976
#ifndef USE_PR_UPDATE_TEXTURE
    auto* textureManager = m_engine->getTextureManager();
    textureManager->updatePendingTextures();
#endif
    //#endif

    // Finish the frame
    m_viewer->swapBuffers();

    m_engine->endFrameSync();
}
