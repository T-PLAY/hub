
#include "MinimalComponent.hpp"

#include <Core/Asset/BlinnPhongMaterialData.hpp>
//#include <Core/Asset/FileData.hpp>
//#include <Core/Containers/MakeShared.hpp>
//#include <Core/Geometry/MeshPrimitives.hpp>
//#include <Core/Geometry/TopologicalMesh.hpp>
//#include <Core/Resources/Resources.hpp>
//#include <Core/Tasks/Task.hpp>
//#include <Core/Tasks/TaskQueue.hpp>
//#include <Core/Utils/Timer.hpp>

//#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/LambertianMaterial.hpp>
//#include <Engine/Data/Mesh.hpp>
#include <Engine/Data/PlainMaterial.hpp>
//#include <Engine/FrameInfo.hpp>
//#include <Engine/Rendering/RenderObject.hpp>
//#include <Engine/Rendering/RenderObjectManager.hpp>
//#include <Engine/Scene/GeometryComponent.hpp>

#include <Engine/Data/TextureManager.hpp>
//#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

//#include <Gui/BaseApplication.hpp>
//#include <Gui/Viewer/Viewer.hpp>
//#include <QOpenGLContext>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#else
#    include <Core/Geometry/MeshPrimitives.hpp>
#endif

#include <random>
//#include <stream.h>

const char* vertexShaderFile   = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const char* fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

constexpr int g_nProbes = 10;

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

// MinimalComponent::Probe::Probe()
//{
//}

struct Probe {
    Ra::Engine::Rendering::RenderObject* m_refCube    = nullptr;
    Ra::Engine::Rendering::RenderObject* m_scan       = nullptr;
    Ra::Engine::Rendering::RenderObject* m_refAxis[3] = { nullptr, nullptr, nullptr };
    Ra::Engine::Rendering::RenderObject* m_scanLine   = nullptr;
    Ra::Engine::Data::Texture* m_textureScan          = nullptr;
    std::string m_textureName;
};

static std::vector<Probe> g_probes;
static std::shared_ptr<BlinnPhongMaterial> g_blinnPhongMaterial;
static std::shared_ptr<PlainMaterial> g_plainMaterial;
static std::shared_ptr<LambertianMaterial> g_lambertianMaterial;
static std::vector<std::shared_ptr<Engine::Data::Mesh>> m_meshAxis;

static int g_iProbePose               = 0;
static long long g_timestampProbePose = 0;
static int g_iProbeScan               = 0;
static long long g_timestampProbeScan = 0;

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This file contains a minimal radium/qt application which shows the geometrical primitives
 * supported by Radium
 */

MinimalComponent::MinimalComponent( Ra::Engine::Scene::Entity* entity,
                                    Ra::Engine::RadiumEngine& e,
                                    Gui::Viewer& viewer ) :
    Ra::Engine::Scene::Component( "Minimal Component", entity ),
    m_engine( e ),
    m_viewer( viewer ) {}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.

void MinimalComponent::initialize() {
    g_blinnPhongMaterial                   = make_shared<BlinnPhongMaterial>( "Shaded Material" );
    g_blinnPhongMaterial->m_perVertexColor = true;
    g_blinnPhongMaterial->m_ks             = Color::White();
    g_blinnPhongMaterial->m_ns             = 100_ra;

    g_plainMaterial                   = make_shared<PlainMaterial>( "Plain Material" );
    g_plainMaterial->m_perVertexColor = true;

    g_lambertianMaterial = make_shared<LambertianMaterial>( "Lambertian Material" );
    g_lambertianMaterial->m_perVertexColor = true;

    //// setup ////
    Scalar cellSize = 10_ra;
    int nCellX      = 10;

    //// GRID ////
    {

        auto gridPrimitive = DrawPrimitives::Grid( Vector3::Zero(),
                                                   Vector3::UnitX(),
                                                   Vector3::UnitZ(),
                                                   Color::Grey( 0.6f ),
                                                   cellSize,
                                                   nCellX );

        auto gridRo = RenderObject::createRenderObject(
            "test_grid", this, RenderObjectType::Geometry, gridPrimitive, {} );
        gridRo->setMaterial( Ra::Core::make_shared<PlainMaterial>( "Grid material" ) );
        gridRo->setPickable( false );
        addRenderObject( gridRo );
    }

    //    std::vector<std::shared_ptr<Engine::Data::Mesh>> meshAxis;

    // origin gizmo
    {
        constexpr Scalar arrowScale = 1_ra;
        constexpr Scalar axisWidth  = .05_ra;
        constexpr Scalar arrowFrac  = 0_ra;

        std::vector<Color> gizmoColors = {
            Color::Red(), Color::Green(), Color::Blue(), Color::Cyan(), Color::Cyan() };

        for ( uint i = 0; i < 5; ++i ) {
            Core::Vector3 cylinderEnd = Core::Vector3::Zero();
            cylinderEnd[i % 3]        = ( 1_ra - arrowFrac );
            Core::Vector3 arrowEnd    = Core::Vector3::Zero();
            arrowEnd[i % 3]           = 1_ra;
            Core::Geometry::TriangleMesh cylinder =
                Core::Geometry::makeCylinder( Core::Vector3::Zero(),
                                              arrowScale * cylinderEnd,
                                              arrowScale * axisWidth / 2_ra,
                                              32,
                                              gizmoColors[i] );

            std::shared_ptr<Engine::Data::Mesh> mesh(
                new Engine::Data::Mesh( "Translate Gizmo Arrow" ) );
            mesh->loadGeometry( std::move( cylinder ) );

            m_meshAxis.push_back( std::move( mesh ) );
        }

        // origin axis
        for ( uint i = 0; i < 3; ++i ) {
            auto gizmo = RenderObject::createRenderObject( "originAxis" + std::to_string( i ),
                                                           this,
                                                           RenderObjectType::Geometry,
                                                           m_meshAxis[i] );

            gizmo->setMaterial( g_plainMaterial );
            Ra::Core::Transform TLocal = Transform::Identity();
            TLocal.scale( 10.0 );
            gizmo->setLocalTransform( TLocal );
            addRenderObject( gizmo );
        }
    }

    // origin ref cube
    {
        std::shared_ptr<Mesh> cube1( new Mesh( "Cube" ) );
        auto box =
            Core::Geometry::makeSharpBox( Vector3 { 1_ra, 1_ra, 1_ra } * 5.0 / 2.0, Color::Grey() );
        cube1->loadGeometry( std::move( box ) );

        auto renderObject1 = RenderObject::createRenderObject(
            "refCube", this, RenderObjectType::Geometry, cube1, {} );
        renderObject1->setMaterial( g_lambertianMaterial );
        addRenderObject( renderObject1 );
    }

    //    addProbe();
    g_probes.reserve( g_nProbes );
    for ( int i = 0; i < g_nProbes; ++i ) {
        addProbe();
    }
}

constexpr double g_probeRefSize = 1.0;

void MinimalComponent::addProbe() {
    Probe probe;

    const int iProbe = g_probes.size();

    // probe ref cube
    {
        std::shared_ptr<Mesh> cubeMesh( new Mesh( "Cube" ) );

        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        cubeMesh->loadGeometry( Geometry::makeSharpBox( cubeSize * g_probeRefSize / 2.0 ) );
        cubeMesh->getCoreGeometry().addAttrib(
            "in_color", Vector4Array { cubeMesh->getNumVertices(), Color::Grey() } );
        //        cubeMesh->setAttribNameCorrespondance("colour", "in_color");

        probe.m_refCube = RenderObject::createRenderObject(
            "probeCube", this, RenderObjectType::Geometry, cubeMesh, {} );
        probe.m_refCube->setLocalTransform(
            Transform { Translation( Vector3( 0_ra, 20_ra, -iProbe * 5 ) ) } );
        probe.m_refCube->setMaterial( g_lambertianMaterial );
        addRenderObject( probe.m_refCube );
    }

    // probe ref axis
    {
        Ra::Core::Transform TLocal = Transform::Identity();
        TLocal.translate( Vector3( 0_ra, 20.0, -iProbe * 5 ) );
        TLocal.scale( 5.0 );
        for ( uint i = 0; i < 3; ++i ) {
            probe.m_refAxis[i] =
                RenderObject::createRenderObject( "probeAxis" + std::to_string( i ),
                                                  this,
                                                  RenderObjectType::Geometry,
                                                  m_meshAxis[i] );
            probe.m_refAxis[i]->setMaterial( g_plainMaterial );
            addRenderObject( probe.m_refAxis[i] );

            probe.m_refAxis[i]->setLocalTransform( TLocal );
            //            probe.m_refAxis[i]->setLocalTransform(
            //                Transform { Translation(Vector3(0_ra, 20_ra, 0_ra)) });
        }

        probe.m_scanLine = RenderObject::createRenderObject(
            "scanLine", this, RenderObjectType::Geometry, m_meshAxis[4] );
        probe.m_scanLine->setMaterial( g_plainMaterial );
        addRenderObject( probe.m_scanLine );

        //            probe.m_scanLine->setLocalTransform(Transform::Identity());
        //            probe.m_scanLine->setLocalTransform(
        //                Transform { Translation(Vector3(0_ra, 20_ra, 0_ra)) });
    }

    // scan plane
    {
#ifdef USE_GOT_PR
        auto quadTriangle = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
#else
        auto quadTriangle = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {} );
        Ra::Core::Vector3Array tex_coords;
        tex_coords.push_back( { 0_ra, 0_ra, 0_ra } );
        tex_coords.push_back( { 1_ra, 0_ra, 0_ra } );
        tex_coords.push_back( { 0_ra, 1_ra, 0_ra } );
        tex_coords.push_back( { 1_ra, 1_ra, 0_ra } );
        quadTriangle.addAttrib(
            Ra::Engine::Data::Mesh::getAttribName( Ra::Engine::Data::Mesh::VERTEX_TEXCOORD ),
            tex_coords );
#endif

        //! [Creating a texture for the slice]
        //        unsigned char data[192 * 512];
        unsigned char* data = new unsigned char[192 * 512];
        // fill with some function
        for ( int i = 0; i < 192; ++i ) {
            for ( int j = 0; j < 512; j++ ) {
                if ( std::abs( i - 20 - iProbe * 5 ) < 3 || std::abs( j - 20 - iProbe * 5 ) < 3 ) {
                    data[( i * 512 + j )] = 0;
                }
                else {

                    data[( i * 512 + j )] = ( j / 2 ) % 256;
                }
            }
        }
        probe.m_textureName = std::string( "myTexture" ) + std::to_string( iProbe );
        auto& textureParameters =
            m_engine.getTextureManager()->addTexture( probe.m_textureName.c_str(), 512, 192, data );
        textureParameters.format         = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the slice]

        std::shared_ptr<Engine::Data::Mesh> meshQuad( new Engine::Data::Mesh( "Scan plane" ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );

        //        RenderTechnique renderTechnique = RenderTechnique::createDefaultRenderTechnique();
        //        Ra::Engine::Data::ShaderConfiguration shaderConfig(
        //            "myShader", vertexShaderFile, fragmentShaderFile );
        //        renderTechnique.setConfiguration( shaderConfig );

        probe.m_scan = RenderObject::createRenderObject(
            "echoPlane", this, RenderObjectType::Geometry, meshQuad );

        //        auto myMat = make_shared<BlinnPhongMaterial>("Shaded Material");
        Ra::Core::Asset::BlinnPhongMaterialData matData( "myMaterialData" );
        // uncomment this to remove glossy highlight
        matData.m_specular      = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular   = true;
        matData.m_hasTexDiffuse = true;
        //        matData.m_texDiffuse    = "myTexture";
        matData.m_texDiffuse = probe.m_textureName.c_str();
        //        probe.m_scan->setMaterial(matData);

        std::shared_ptr<Ra::Engine::Data::Material> roMaterial;
        auto converter =
            Ra::Engine::Data::EngineMaterialConverters::getMaterialConverter( matData.getType() );
        auto mat = converter.second( &matData );
        roMaterial.reset( mat );

        //        roMaterial->needUpdate();
        probe.m_scan->setMaterial( roMaterial );

        auto TLocal = Transform::Identity();
        TLocal.translate( Vector3( 0_ra, 0_ra, -iProbe * 5 ) );
        TLocal.scale( 10.0 );
        probe.m_scan->setLocalTransform( TLocal );
        //        probe.m_scan->getRenderTechnique()->setConfiguration( shaderConfig );

        addRenderObject( probe.m_scan );
    }

    g_probes.push_back( std::move( probe ) );

    //    updateProbe(Vector3(0_ra, 2_ra, 0_ra));
}

void MinimalComponent::updateShader() {
    for ( auto& probe : g_probes ) {

        //        auto& probe = g_probes.at( iProbe );
        auto& scan = *probe.m_scan;

        auto renderTechnique = scan.getRenderTechnique();
        Ra::Engine::Data::ShaderConfiguration shaderConfig(
            "myShader", vertexShaderFile, fragmentShaderFile );
        renderTechnique->setConfiguration( shaderConfig );

        Ra::Engine::Data::TextureParameters textureParameters;
        textureParameters.name = probe.m_textureName;
        m_viewer.makeCurrent();
        probe.m_textureScan = m_engine.getTextureManager()->getOrLoadTexture( textureParameters );
        m_viewer.doneCurrent();
    }

    for ( int i = 0; i < g_nProbes; ++i ) {
        initPose( i );
        initScan( i );
    }
}

void MinimalComponent::updatePose( const Stream::Acquisition& acq ) {
    // void MinimalComponent::updatePose( Acquisitions& acqs ) {
    // void MinimalComponent::updatePose( Stream::Acquisition && acq) {
    //    iProbe = 1;
    //    assert( iProbe < g_probes.size() );
    //    auto& probe = g_probes.at( iProbe );
    //    std::cout << "[MinimalComponent] upadePose : " << acq << std::endl;

    if ( acq.mBackendTimestamp == g_timestampProbePose ) { ++g_iProbePose; }
    else {
        for ( int i = 1; i <= g_iProbePose; ++i ) {
            initPose( i );
        }
        g_iProbePose         = 0;
        g_timestampProbePose = acq.mBackendTimestamp;
    }

    //    int iProbe = 0;

    //    const int nAcqs = acqs.size();
    //    std::cout << "[MinimalComponent] upadePose : acqs size = " << acqs.size() << std::endl;
    //    int iProbe = 0;
    //    for ( int iProbe = 0; iProbe < nAcqs; ++iProbe ) {
    //    while ( !acqs.empty() ) {
    //        const auto& acq = acqs.front();

//    std::cout << "[MinimalComponent] upadePose : " << acq << std::endl;

    auto& probe = g_probes.at( g_iProbePose );
    //    auto& probe = g_probes.at( iProbe );

    float* translation = (float*)acq.mData;
    float* quaternion  = (float*)&acq.mData[12];

    //        Ra::Core::Vector3 pos(-translation[0], -translation[1],
    //            -translation[2]);
    Ra::Core::Vector3 pos( translation[0], translation[1], translation[2] );
    //    pos = -pos;

    Ra::Core::Quaternion orientation( quaternion[0], quaternion[1], quaternion[2], quaternion[3] );

    // change to Radium base reference
    Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
    TRadium.rotate(
        Eigen::AngleAxis( 1.0f * Ra::Core::Math::Pi, Ra::Core::Vector3( 0.0, 0.0, 1.0 ) ) );
    TRadium.rotate(
        Eigen::AngleAxis( -0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3( 1.0, 0.0, 0.0 ) ) );

    // orientation
    Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
    TOrientation.rotate( orientation );

    // World transform
    Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();
    //    pos /= 10.0; // convert centimetre to metre
    TWorld.translate( pos );
    //    TWorld.translate(Ra::Core::Vector3(0.0, 0.2, -0.1));

    {
        Ra::Core::Transform TLocal = Transform::Identity();
        //                TLocal.scale(10.0);
        TLocal.scale( 5.0 );

        //    TLocal.translate(Ra::Core::Vector3(1.0, 0.0, 1.0));
        //        Ra::Core::Vector3 vecScale = Ra::Core::Vector3::Identity();
        //        vecScale *= 0.7;
        //        TLocal.scale(vecScale);
        //        TLocal.scale(Ra::Core::Vector3(1.0, 1.0, 10.0));

        //        g_scan->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        //        g_probe->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        probe.m_refCube->setLocalTransform( TRadium * TWorld * TOrientation );
//        if ( g_iProbePose > 0 ) probe.m_refCube->setVisible( true );

        for ( int i = 0; i < 3; ++i ) {
            probe.m_refAxis[i]->setLocalTransform( TRadium * TWorld * TOrientation * TLocal );
//            if ( g_iProbePose > 0 ) probe.m_refAxis[i]->setVisible( true );
        }
    }

    {
        // Local transform scan
        Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();

        //        Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
        const double scanWidth = 5.0; // centimeters
        const double scanDepth = 3.5; // centimeters

        TLocal.translate( Ra::Core::Vector3( scanDepth / 2.0 + 3.1, 0.0, 16.0 ) );
        //        TLocal.translate(Ra::Core::Vector3(0.05, 0.0, 0.5));

        const double userScale = 1.0;
        Ra::Core::Vector3 vecScale( userScale * scanDepth / 2.0, userScale * scanWidth / 2.0, 1.0 );
        TLocal.scale( vecScale );

        //        TLocal.translate(Ra::Core::Vector3(scanDepth / 2.0 + tmp2, 0.0, tmp));
        //        TLocal.translate(Ra::Core::Vector3(0.1, 0.0, 0.2));
        //        m_scan->setLocalTransform(TRadium * TWorld * TLocal * TOrientation);
        probe.m_scan->setLocalTransform( TRadium * TWorld * TOrientation * TLocal );
        if ( g_iProbePose > 0 ) probe.m_scan->setVisible( true );

        Ra::Core::Transform TLocal2 = Ra::Core::Transform::Identity();
        TLocal2.translate( Ra::Core::Vector3( 3.1, -scanWidth / 2.0, 16.0 ) );
        Ra::Core::Vector3 vecScale2( 3.0, userScale * scanWidth, 3.0 );
        TLocal2.scale( vecScale2 );
        probe.m_scanLine->setLocalTransform( TRadium * TWorld * TOrientation * TLocal2 );
        if ( g_iProbePose > 0 ) probe.m_scanLine->setVisible( true );
    }

    //        acqs.pop();
    //        ++iProbe;
    //    }
}

void MinimalComponent::initPose( int iProbe ) {
    if ( iProbe == 0 ) {
        for ( int i = 1; i <= g_iProbePose; ++i ) {
            initPose( i );
        }
        g_iProbePose         = 0;
        g_timestampProbePose = 0;
    }
    assert( iProbe < g_probes.size() );
    auto& probe = g_probes.at( iProbe );

    {
        auto TLocal = Transform::Identity();
        TLocal.translate( Vector3( 0_ra, 0_ra, -iProbe * 5 ) );
        TLocal.scale( 10.0 );
        probe.m_scan->setLocalTransform( TLocal );
        if ( iProbe > 0 ) probe.m_scan->setVisible( false );
    }

    {
        auto TLocal = Transform::Identity();
        TLocal.translate( Vector3( 0_ra, 20.0, -iProbe * 5 ) );
        //        TLocal.translate( Vector3( 0.0, 20.0, 0.0 ) );
        probe.m_refCube->setLocalTransform( TLocal );
        if ( iProbe > 0 ) probe.m_refCube->setVisible( false );
        TLocal.scale( 5.0 );
        for ( int i = 0; i < 3; ++i ) {
            probe.m_refAxis[i]->setLocalTransform( TLocal );
            if ( iProbe > 0 ) probe.m_refAxis[i]->setVisible( false );
        }
    }

    probe.m_scanLine->setLocalTransform( Transform::Identity() );
    if ( iProbe > 0 ) probe.m_scanLine->setVisible( false );
}

// void MinimalComponent::updateScan( Acquisitions& acqs ) {
void MinimalComponent::updateScan( const Stream::Acquisition& acq ) {
    //    assert( iProbe < g_probes.size() );
    if ( acq.mBackendTimestamp == g_timestampProbeScan ) { ++g_iProbeScan; }
    else {
        for ( int i = 1; i <= g_iProbeScan; ++i ) {
            initScan( i );
        }
        g_iProbeScan         = 0;
        g_timestampProbeScan = acq.mBackendTimestamp;
    }
    //     int iProbe = 0;

    //    const int nAcqs = acqs.size();

    //    for ( int iProbe = 0; iProbe < nAcqs; ++iProbe ) {
    //        const auto& acq = acqs.front();

    auto& probe = g_probes.at( g_iProbeScan );
    //        auto& probe = g_probes.at( iProbe );

    assert( probe.m_textureScan != nullptr );

    const unsigned char* data = acq.mData;

    m_viewer.makeCurrent();
    auto& params = probe.m_textureScan->getParameters();
    memcpy( params.texels, data, 192 * 512 );
    probe.m_textureScan->initializeGL( false );
    m_viewer.doneCurrent();

    //        acqs.pop();
    //    }
}

void MinimalComponent::initScan( int iProbe ) {
    if ( iProbe == 0 ) {
        for ( int i = 1; i <= g_iProbeScan; ++i ) {
            initScan( i );
        }
        g_iProbeScan         = 0;
        g_timestampProbeScan = 0;
    }
    assert( iProbe < g_probes.size() );
    auto& probe = g_probes.at( iProbe );

    unsigned char* data = new unsigned char[192 * 512];
    // fill with some function
    for ( int i = 0; i < 192; ++i ) {
        for ( int j = 0; j < 512; j++ ) {
            if ( std::abs( i - 20 - iProbe * 5 ) < 3 || std::abs( j - 20 - iProbe * 5 ) < 3 ) {
                data[( i * 512 + j )] = 0;
            }
            //            if ( std::abs( i - 20 ) < 3 || std::abs( j - 20 ) < 3 ) { data[( i * 512 +
            //            j )] = 0; }
            else {

                data[( i * 512 + j )] = ( j / 2 ) % 256;
            }
        }
    }
    m_viewer.makeCurrent();
    auto& params = probe.m_textureScan->getParameters();
    memcpy( params.texels, data, 192 * 512 );
    probe.m_textureScan->initializeGL( false );
    m_viewer.doneCurrent();
}

// InputStream* scanStream = nullptr;
// InputStream* posStream  = nullptr;

//#define ONLY_POSE
