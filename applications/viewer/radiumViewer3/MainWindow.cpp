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

#include <Core/Asset/BlinnPhongMaterialData.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Data/TextureManager.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <Engine/Data/RawShaderMaterial.hpp>
#include <QLayout>

const std::string vertexShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const std::string fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

Thread_Client::Thread_Client(QObject* parent, Ra::Gui::Viewer& viewer, Ra::Engine::RadiumEngine& engine)
    : QThread(parent)
    , m_viewer(&viewer)
    , m_engine(&engine)
{
    std::cout << "[Thread_Client] Thread_Client()" << std::endl;
}

void Thread_Client::run()
{
    std::cout << "[Thread_Client] Thread_Client::run()" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    while (!this->isInterruptionRequested()) {

        unsigned char data[192 * 512] = { 0 };
        for (int i = 0; i < 192; ++i) {
            for (int j = 0; j < 512; j++) {
                data[i * 512 + j] = (m_acquisition + j) % 256;
            }
        }

        Ra::Engine::Data::TextureParameters textureParameters;
        textureParameters.name = "myTexture";
        auto texture = m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
        auto& params = texture->getParameters();
        memcpy(params.texels, data, 192 * 512);
        m_viewer->makeCurrent();
        texture->initializeGL(false);
        m_viewer->doneCurrent();

        //        try {
        //            ClientSocket sock;
        //            sock.write(ClientSocket::Type::VIEWER);

        //            while (!this->isInterruptionRequested()) {

        //                Socket::Message serverMessage;
        //                sock.read(serverMessage);

        //                switch (serverMessage) {

        //                case Socket::Message::PING: {
        //                    // server check client connection
        //                    // nothing to do
        //                } break;

        //                case Socket::Message::NEW_STREAMER: {
        //                    std::cout << "[Thread_Client] [viewer] new streamer" << std::endl;
        //                    std::string streamerSensorName;
        //                    sock.read(streamerSensorName);
        //                    std::string format;
        //                    sock.read(format);
        //                    std::string dims;
        //                    sock.read(dims);
        //                    std::string size;
        //                    sock.read(size);

        //                    Stream::MetaData metaData;
        //                    sock.read(metaData);

        //                    std::cout << "[Thread_Client] [viewer] new streamer " << streamerSensorName << ", format:" << format << ", dims:" << dims << ", acquisitionSize:" << size << std::endl;
        //                    std::cout << "[Thread_Client] [viewer] emit addSensorSignal '" << streamerSensorName << "'" << std::endl;
        //                    std::cout << "[Thread_Client] [viewer] metadata : " << Stream::to_string(metaData, true);
        //                    emit addSensorSignal(streamerSensorName, format, dims, size, Stream::to_string(metaData, true));

        //                } break;

        //                case Socket::Message::DEL_STREAMER: {
        //                    std::string streamerSensorName;
        //                    sock.read(streamerSensorName);
        //                    std::cout << "[Thread_Client] [viewer] del streamer '" << streamerSensorName << "'" << std::endl;
        //                    emit delSensorSignal(streamerSensorName);
        //                } break;

        //                default:
        //                    std::cout << "[Thread_Client] unknown message from server" << std::endl;
        //                }
        //            }

        //        } catch (std::exception& e) {
        //            std::cout << "[Thread_Client] [viewer] catch exception : " << e.what() << std::endl;
        //        }
    } // while (!this->isInterruptionRequested())
}

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

    m_engine->registerSystem("GeometrySystem", new Ra::Engine::Scene::GeometrySystem, 1000); // BaseApplication.cpp
    // Create one system
    //    MinimalSystem* sys = new MinimalSystem;
    //    app.m_engine->registerSystem( "Minimal system", sys );

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


    //    initScene();
    mThreadClient = new Thread_Client(this, *m_viewer, *m_engine);
    mThreadClient->start();
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

//    mThreadClient.requestInterruption();
//    mThreadClient.wait();
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

    initScene();
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

void MainWindow::initScene()
{
    auto geometrySystem = m_engine->getSystem("GeometrySystem");

    Ra::Engine::Data::RawShaderMaterial::registerMaterial();

    // cube
    {
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

        //! [Register the entity/component association to the geometry system ]
        geometrySystem->addComponent(e, c);
        //! [Register the entity/component association to the geometry system ]
        //    ! [add the custom material to the material system]
    }

    std::shared_ptr<Ra::Engine::Rendering::RenderObject> roQuad;
    // quad texture
    {
        //! [Creating the quad]
        auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });
        //! [Creating the quad]
        // [Add missing texture coordonates for to the quad]
        Ra::Core::Vector3Array tex_coords;
        tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
        tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
        tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
        tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
        quad.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD), tex_coords);

        // [Add missing texture coordonates for to the quad]

        //! [Creating a texture for the quad]
        unsigned char data[192 * 512];
        // fill with some function
        for (int i = 0; i < 192; ++i) {
            for (int j = 0; j < 512; j++) {
                if (std::abs(i - 20) < 3 || std::abs(j - 20) < 3) {
                    data[(i * 512 + j)] = 0;
                } else {

                    data[(i * 512 + j)] = (j / 2) % 256;
                }
            }
        }
        auto& textureParameters = m_engine->getTextureManager()->addTexture("myTexture", 512, 192, data);
        textureParameters.format = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the quad]

        //! [create the quad material]
        Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
        // uncomment this to remove glossy highlight
        matData.m_specular = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular = true;
        matData.m_hasTexDiffuse = true;
        matData.m_texDiffuse = "myTexture";

        //! [Create Parameter provider for the shader]
        //! [create the quad material]

        //! [Create the engine entity for the quad]
        auto e = m_engine->getEntityManager()->createEntity("Textured quad");
        //! [Create the engine entity for the quad]

        //! [Create a geometry component with the quad]
        auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);
        //! [Create a geometry component with the quad]

        //! [Register the entity/component association to the geometry system ]
        geometrySystem->addComponent(e, c);
        //! [Register the entity/component association to the geometry system ]

        //        //! [Tell the window that something is to be displayed]
        //        app.m_mainWindow->prepareDisplay();
        //        //! [Tell the window that something is to be displayed]

        //        //![get the renderobject for further edition]
        auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(c->m_renderObjects[0]);
        //        auto ro = m_engine->getInstance()->getRenderObjectManager()->getRenderObject(c->m_renderObjects[0]);

        auto& renderTechnique = *ro->getRenderTechnique();
        Ra::Engine::Data::ShaderConfiguration shaderConfig("myShader", vertexShaderFile, fragmentShaderFile);
        renderTechnique.setConfiguration(shaderConfig);

        //        roQuad = ro;
    }
}
