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
//#include "MinimalComponent.hpp"
//#include "MinimalSystem.hpp"

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
    ui->layoutRadium->addWidget( m_viewerWidget );
    connect( m_viewer,
             &Ra::Gui::Viewer::requestEngineOpenGLInitialization,
             this,
             &MainWindow::onGLInitialized );
    m_viewer->setupKeyMappingCallbacks();

    m_viewer->show();

    m_engine->registerSystem(
        "GeometrySystem", new Ra::Engine::Scene::GeometrySystem, 1000 ); // BaseApplication.cpp
                                                                         // Create one system
    auto* sys = m_engine->getSystem( "GeometrySystem" );
    ;

    // Create and initialize entity and component

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    m_viewer->prepareDisplay();
    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping );


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
            m_poseStream = new InputStream( "Polhemus Patriot (probe)" );
        }
        else { m_poseStream = new InputStream( "Polhemus Patriot (probe)" ); }
    }
    catch ( std::exception& e ) {
        std::cout << "[main] catch exception " << e.what() << std::endl;
        m_poseStream = nullptr;
    }
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

}

void MainWindow::onGLInitialized() {
    // initialize here the OpenGL part of the engine used by the application
    m_engine->initializeGL();

    // add the renderer
    std::shared_ptr<Ra::Engine::Rendering::Renderer> e(
        new Ra::Engine::Rendering::ForwardRenderer() );
    m_viewer->addRenderer( e );
    connect( m_frame_timer, &QTimer::timeout, this, &MainWindow::frame );
}

void MainWindow::frame() {
    // update geometry
    {
        if ( m_scan != nullptr ) {
            // update position and orientation








            // update texture
            if ( m_scanStream != nullptr ) {
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
        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        ro->setLocalTransform( Transform { Translation( Vector3( 0_ra, 0_ra, 0_ra ) ) } );
        ro->setMaterial( lambertianMaterial );

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
        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        ro->setLocalTransform( Transform { Translation( Vector3( 0_ra, 2_ra, 0_ra ) ) } );
        ro->setMaterial( lambertianMaterial );

        m_probe = ro.get();
    }

    // quad texture
    {
        //! [Creating the quad]
#ifdef USE_GOT_PR
        auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
#else
        auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {} );
#endif
        //! [Creating the quad]
        // [Add missing texture coordonates for to the quad]

        // [Add missing texture coordonates for to the quad]

        //! [Creating a texture for the quad]
        unsigned char* data = new unsigned char[192 * 512];
        // fill with some function
        for ( int i = 0; i < 192; ++i ) {
            for ( int j = 0; j < 512; j++ ) {
                if ( std::abs( i - 20 ) < 3 || std::abs( j - 20 ) < 3 ) {
                    data[( i * 512 + j )] = 0;
                }
                else { data[( i * 512 + j )] = ( j / 2 ) % 256; }
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


        auto ro =
            Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                c->m_renderObjects[0] );
        Ra::Engine::Data::ShaderConfiguration shaderConfig( "myShader" );
        shaderConfig.addShaderSource( Ra::Engine::Data::ShaderType_VERTEX, _vertexShaderSource );
        shaderConfig.addShaderSource( Ra::Engine::Data::ShaderType_FRAGMENT,
                                      _fragmentShaderSource );

        ro->getRenderTechnique()->setConfiguration( shaderConfig );

        m_scan = ro.get();
    }
}
