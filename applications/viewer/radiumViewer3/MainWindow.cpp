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

#include <Engine/Data/DrawPrimitives.hpp>
#include <Engine/Data/TextureManager.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <Engine/Data/LambertianMaterial.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <Engine/Data/RawShaderMaterial.hpp>
#include <QLayout>

#include <stream.h>

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

const std::string vertexShaderFile =
    PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const std::string fragmentShaderFile =
    PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

// Vertex shader source code
const std::string _vertexShaderSource {
    "#include \"TransformStructs.glsl\"\n"
    "layout (location = 0) in vec3 in_position;\n"
    "layout (location = 4) in vec3 in_texcoord;\n"
    "layout (location = 0) out vec3 out_pos;\n"
    "layout (location = 2) out vec3 out_texcoord;\n"
    "uniform Transform transform;\n"
    "void main(void)\n"
    "{\n"
    "    mat4 mvp    = transform.proj * transform.view * transform.model;\n"
    "    out_pos     = in_position;\n"
    "    gl_Position = mvp*vec4(in_position.xyz, 1.0);\n"
    "    out_texcoord = in_texcoord;\n"
    "}\n" };

//  Fragment shader source code
const std::string _fragmentShaderSource {
    "#include \"BlinnPhong.glsl\"\n"
    "#include \"VertexAttribInterface.frag.glsl\"\n"
    "layout (location = 0) out vec4 out_color;\n"
    "uniform vec4 aColorUniform;\n"
    "uniform float aScalarUniform;\n"
    "void main(void)\n"
    "{\n"
    "   vec3 color = material.kd.rgb;\n"
    "   if (material.tex.hasKd == 1) { color =  texture(material.tex.kd, in_texcoord.xy).rgb; }\n"
    "   out_color =  vec4(color, 1.0);\n"
    "   // out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
    "}\n" };



MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );
    //    setCentralWidget(ui->centralwidget);
    //    ui->centralwidget->show();

    // Initialize Engine.
    m_engine = Ra::Engine::RadiumEngine::createInstance();
    m_engine->initialize();

    ///\todo update when a basic viewer is implemented ... (to call setupKeyMappingCallbacks)
    Ra::Gui::KeyMappingManager::createInstance();
    Ra::Gui::KeyMappingManager::getInstance()->addListener(
        Ra::Gui::TrackballCameraManipulator::configureKeyMapping );
    Ra::Gui::KeyMappingManager::getInstance()->addListener( Ra::Gui::Viewer::configureKeyMapping );

    // Initialize taskqueue.
    m_task_queue = new Ra::Core::TaskQueue( std::thread::hardware_concurrency() - 1 );

    // Initialize viewer.
    m_viewer = new Ra::Gui::Viewer;
    CORE_ASSERT( m_viewer != nullptr, "GUI was not initialized" );

    m_viewerWidget = QWidget::createWindowContainer( m_viewer, ui->splitter );
    //    delete ui->widgetRadium;
    ui->layoutRadium->addWidget( m_viewerWidget );
    //    ui->splitter->replaceWidget(1, m_viewerWidget);
    //    ui->splitter->addWidget(m_viewerWidget);
    //    ui->widgetRadium = QWidget::createWindowContainer(m_viewer, this);
    //    setCentralWidget(m_viewerWidget);
    connect( m_viewer,
             &Ra::Gui::Viewer::requestEngineOpenGLInitialization,
             this,
             &MainWindow::onGLInitialized );
    m_viewer->setupKeyMappingCallbacks();

    m_viewer->show();
    //    CORE_ASSERT(m_viewer->getContext()->isValid(), "OpenGL was not initialized");

    m_engine->registerSystem(
        "GeometrySystem", new Ra::Engine::Scene::GeometrySystem, 1000 ); // BaseApplication.cpp
    // Create one system
    //    MinimalSystem* sys = new MinimalSystem;
    //    app.m_engine->registerSystem( "Minimal system", sys );

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    m_viewer->prepareDisplay();
    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping );

    //    m_viewer->setCameraManipulator(new Ra::Gui::RotateAroundCameraManipulator(
    //        *(m_viewer->getCameraManipulator()), m_viewer));
    //    initScene();

    // Initialize timer for the spinning cube.
    m_frame_timer = new QTimer( this );
    m_frame_timer->setInterval( 1000 / 60.0 );

    // Start timer
    m_frame_timer->start();

#ifndef ONLY_POSE
    try {
        m_scanStream = new InputStream( "ULA-OP 256", "" );
    }
    catch ( std::exception& e ) {
        std::cout << "[main] catch exception " << e.what() << std::endl;
        m_scanStream = nullptr;
    }
#endif

    try {
        if ( m_scanStream != nullptr ) {
            //            m_poseStream = new InputStream("Polhemus Patriot (probe)", "ULA-OP 256");
            m_poseStream = new InputStream( "Polhemus Patriot (probe)" );
        }
        else {
            m_poseStream = new InputStream( "Polhemus Patriot (probe)" );
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[main] catch exception " << e.what() << std::endl;
        m_poseStream = nullptr;
    }
    //            initScene();
    //    m_threadClient = new Thread_Client(this, *m_viewer, *m_engine);
    //    m_threadClient->start();
}

MainWindow::~MainWindow() {
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

void MainWindow::onGLInitialized() {
    // initialize here the OpenGL part of the engine used by the application
    m_engine->initializeGL();

    // add the renderer
    std::shared_ptr<Ra::Engine::Rendering::Renderer> e(
        new Ra::Engine::Rendering::ForwardRenderer() );
    m_viewer->addRenderer( e );
    initScene();
    connect( m_frame_timer, &QTimer::timeout, this, &MainWindow::frame );
}

void MainWindow::frame() {
    // update geometry
    {
        //        assert(m_scan != nullptr);
        if ( m_scan != nullptr ) {
            //    g_scan->setLocalTransform(Transform { Translation(Vector3(2_ra, 2_ra, 2_ra)) });
            // update position and orientation
            //            if (m_poseStream != nullptr) {
            //                Stream::Acquisition posAcq;
            //                *m_poseStream >> posAcq;
            //                float* translation = (float*)posAcq.mData;
            //                float* quaternion = (float*)&posAcq.mData[12];

            //                // change to Radium base reference
            //                Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
            //                TRadium.rotate(Eigen::AngleAxis(1.0f * Ra::Core::Math::Pi,
            //                Ra::Core::Vector3(0.0, 0.0, 1.0)));
            //                TRadium.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi,
            //                Ra::Core::Vector3(1.0, 0.0, 0.0)));

            //                // orientation
            //                Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
            //                Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2],
            //                quaternion[3]); TOrientation.rotate(quat);

            //                // World transform
            //                Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();

            //                Ra::Core::Vector3 vecPos(-translation[0], -translation[1],
            //                -translation[2]);

            //                vecPos /= 5.0;
            //                TWorld.translate(vecPos);

            //                //        g_scan->setLocalTransform(TRadium * TWorld * TOrientation *
            //                TLocal);
            //                //        g_probe->setLocalTransform(TRadium * TWorld * TOrientation *
            //                TLocal); m_probe->setLocalTransform(TRadium * TWorld * TOrientation);
            //                //        for (int i = 0; i < 3; ++i) {
            //                //            g_probe_axis[i]->setLocalTransform(TRadium * TWorld *
            //                TOrientation);
            //                //        }

            //                // Local transform scan
            //                Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
            //                TLocal.translate(Ra::Core::Vector3(1.0, 0.0, 2.0));
            //                Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
            //                TLocal.scale(vecScale);
            //                m_scan->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
            //            }

            // update texture
            if ( m_scanStream != nullptr ) {
                //                        unsigned char data[192 * 512] = { 0 };
                //                        for (int i = 0; i < 192; ++i) {
                //                            for (int j = 0; j < 512; j++) {
                //                                data[i * 512 + j] = (m_acquisition + j) % 256;
                //                            }
                //                        }
                Stream::Acquisition scanAcq;
                *m_scanStream >> scanAcq;

                unsigned char* data = scanAcq.mData;

                Ra::Engine::Data::TextureParameters textureParameters;
                textureParameters.name = "myTexture";
                m_viewer->makeCurrent();
                auto texture = m_engine->getTextureManager()->getOrLoadTexture( textureParameters );
                auto& params = texture->getParameters();
                memcpy( params.texels, data, 192 * 512 );
                texture->initializeGL( false );
                m_viewer->doneCurrent();
            }
        }
    }

    // We use a fixed time step, but it is also possible
    // to check the time from last frame.
    const Scalar dt = 1.f / Scalar( m_target_fps );

    // Collect and run tasks
    m_engine->getTasks( m_task_queue, dt );
    m_task_queue->startTasks();
    m_task_queue->waitForTasks();
    m_task_queue->flushTaskQueue();

    // Starts the renderer
    m_viewer->startRendering( dt );

    // Finish the frame
    m_viewer->swapBuffers();

    m_engine->endFrameSync();
}

void MainWindow::initScene() {
    auto geometrySystem = m_engine->getSystem( "GeometrySystem" );

    Ra::Engine::Data::RawShaderMaterial::registerMaterial();

    //// setup ////
    Scalar colorBoost = 1_ra; /// since simple primitive are ambient only, boost their color
    Scalar cellSize   = 1_ra;
    int nCellX        = 10;
    int nCellY        = 10;
    Vector3 cellCorner { -nCellX * cellSize / 2_ra, 0_ra, -nCellY * cellSize / 2_ra };
    Vector3 toCellCenter { cellSize / 2_ra, cellSize / 2_ra, cellSize / 2_ra };
    Scalar offset { 0.05_ra };
    Vector3 offsetVec { offset, offset, offset };
    std::random_device rd;    // Will be used to obtain a seed for the random number engine
    std::mt19937 gen( rd() ); // Standard mersenne_twister_engine seeded with rd()
    gen.seed( 13371337 );
    std::uniform_real_distribution<Scalar> dis015( 0_ra, cellSize - 2_ra * offset );
    std::uniform_real_distribution<Scalar> dis01( 0_ra, 1_ra );
    std::uniform_real_distribution<Scalar> dis11( -1_ra, 1_ra );
    std::uniform_int_distribution<uint> disInt( 0, 128 );
    uint circleGridSize = 8;
    uint numberOfSphere = 32;

    //    //// GRID ////
    //    {

    //        auto gridPrimitive = DrawPrimitives::Grid(Vector3::Zero(),
    //            Vector3::UnitX(),
    //            Vector3::UnitZ(),
    //            Color::Grey(0.6f),
    //            cellSize,
    //            nCellX);

    //        //        auto gridRo = RenderObject::createRenderObject(
    //        //            "test_grid", RenderObjectType::Geometry, gridPrimitive, {});
    //                gridRo->setMaterial(Ra::Core::make_shared<PlainMaterial>("Grid material"));

    //        //        gridRo->setPickable(false);
    //        //        addRenderObject(gridRo);
    //    }

    //    auto plainMaterial = make_shared<PlainMaterial>("Plain Material");
    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian Material" );
    lambertianMaterial->m_perVertexColor = true;

    // ref cube
    {
        //! [Creating the cube]
        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        auto cube     = Ra::Core::Geometry::makeSharpBox( cubeSize * 0.7 / 2 );
        //! [Creating the cube]

        //! [Colorize the Cube]
        cube.addAttrib(
            "in_color",
            Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::Grey() } );
        //! [Colorize the Cube]

        //! [Create the engine entity for the cube]
        auto e = m_engine->getEntityManager()->createEntity( "Green cube" );
        //! [Create the engine entity for the cube]

        //! [Create a geometry component with the cube]
        auto c = new Ra::Engine::Scene::TriangleMeshComponent(
            "Cube Mesh", e, std::move( cube ), nullptr );
        //! [Create a geometry component with the cube]

        //! [Register the entity/component association to the geometry system ]
        geometrySystem->addComponent( e, c );
        //! [Register the entity/component association to the geometry system ]
        //    ! [add the custom material to the material system]
        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        ro->setLocalTransform( Transform { Translation( Vector3( 0_ra, 0_ra, 0_ra ) ) } );
        ro->setMaterial( lambertianMaterial );

        //        m_threadClient->m_probe = ro.get();
    }

    // probe cube
    {
        //! [Creating the cube]
        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        auto cube     = Ra::Core::Geometry::makeSharpBox( cubeSize * 0.2 / 2 );
        //! [Creating the cube]

        //! [Colorize the Cube]
        cube.addAttrib(
            "in_color",
            Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::Grey() } );
        //! [Colorize the Cube]

        //! [Create the engine entity for the cube]
        auto e = m_engine->getEntityManager()->createEntity( "Probe cube" );
        //! [Create the engine entity for the cube]

        //! [Create a geometry component with the cube]
        auto c = new Ra::Engine::Scene::TriangleMeshComponent(
            "Cube Mesh", e, std::move( cube ), nullptr );
        //! [Create a geometry component with the cube]

        //! [Register the entity/component association to the geometry system ]
        geometrySystem->addComponent( e, c );
        //! [Register the entity/component association to the geometry system ]
        //    ! [add the custom material to the material system]
        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        ro->setLocalTransform( Transform { Translation( Vector3( 0_ra, 2_ra, 0_ra ) ) } );
        ro->setMaterial( lambertianMaterial );

        //        m_threadClient->m_probe = ro.get();
        m_probe = ro.get();
    }

    //    std::shared_ptr<Ra::Engine::Rendering::RenderObject> roQuad;
    // quad texture
    //    if (false)
    {
        //! [Creating the quad]
        auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
        //! [Creating the quad]
        // [Add missing texture coordonates for to the quad]
        //        Ra::Core::Vector3Array tex_coords;
        //        tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
        //        tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
        //        tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
        //        tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
        //        quad.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD),
        //        tex_coords);

        // [Add missing texture coordonates for to the quad]

        //! [Creating a texture for the quad]
        //        unsigned char data[192 * 512] = { 0 };
        unsigned char* data = new unsigned char[192 * 512];
        // fill with some function
        for ( int i = 0; i < 192; ++i ) {
            for ( int j = 0; j < 512; j++ ) {
                if ( std::abs( i - 20 ) < 3 || std::abs( j - 20 ) < 3 ) {
                    data[( i * 512 + j )] = 0;
                }
                else {

                    data[( i * 512 + j )] = ( j / 2 ) % 256;
                }
            }
        }
        auto& textureParameters =
            m_engine->getTextureManager()->addTexture( "myTexture", 512, 192, data );
        textureParameters.format         = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the quad]

        //! [create the quad material]
        Ra::Core::Asset::BlinnPhongMaterialData matData( "myMaterialData" );
        // uncomment this to remove glossy highlight
        matData.m_specular      = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular   = true;
        matData.m_hasTexDiffuse = true;
        matData.m_texDiffuse    = "myTexture";

        //! [Create Parameter provider for the shader]
        //! [create the quad material]

        //! [Create the engine entity for the quad]
        auto e = m_engine->getEntityManager()->createEntity( "Textured quad" );
        //! [Create the engine entity for the quad]

        //! [Create a geometry component with the quad]
        auto c = new Ra::Engine::Scene::TriangleMeshComponent(
            "Quad Mesh", e, std::move( quad ), &matData );
        //! [Create a geometry component with the quad]

        //! [Register the entity/component association to the geometry system ]
        geometrySystem->addComponent( e, c );
        //! [Register the entity/component association to the geometry system ]

        //        //! [Tell the window that something is to be displayed]
        //                app.m_mainWindow->prepareDisplay();
        //        //! [Tell the window that something is to be displayed]

        //        //![get the renderobject for further edition]
        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        //        auto ro =
        //        m_engine->getInstance()->getRenderObjectManager()->getRenderObject(c->m_renderObjects[0]);
        Ra::Engine::Data::ShaderConfiguration shaderConfig( "myShader" );
        shaderConfig.addShaderSource( Ra::Engine::Data::ShaderType_VERTEX, _vertexShaderSource );
        shaderConfig.addShaderSource( Ra::Engine::Data::ShaderType_FRAGMENT,
                                      _fragmentShaderSource );

        //    Ra::Engine::Data::ShaderConfiguration shaderConfig(
        //        "myShader", vertexShaderFile, fragmentShaderFile );
        ro->getRenderTechnique()->setConfiguration( shaderConfig );

        //        m_threadClient->m_scan = ro.get();
        m_scan = ro.get();
        //        roQuad = ro;
    }
}
