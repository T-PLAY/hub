
#include "MinimalComponent.hpp"

//#include <Core/Asset/BlinnPhongMaterialData.hpp>
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

#include <Engine/Scene/EntityManager.hpp>

//#define USE_GOT_CONTRIB

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#else
#    include <Core/Geometry/MeshPrimitives.hpp>
#endif

#include <random>
//#include <stream.h>

#include <ScanMaterial/ScanMaterial.hpp>

const char* vertexShaderFile   = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const char* fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

constexpr int g_nProbes = 20;

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
    unsigned char* m_data = nullptr;
};

static std::shared_ptr<BlinnPhongMaterial> g_blinnPhongMaterial;
static std::shared_ptr<PlainMaterial> g_plainMaterial;
static std::shared_ptr<LambertianMaterial> g_lambertianMaterial;

static std::vector<Probe> g_probes;
static std::vector<std::shared_ptr<Engine::Data::Mesh>> m_meshAxis;

static int g_iProbePose               = 0;
static long long g_timestampProbePose = 0;
static int g_iProbeScan               = 0;
static long long g_timestampProbeScan = 0;

static long g_iter = 0;
// static Ra::Engine::Rendering::RenderObject * g_roGrid = nullptr;

// Ra::Engine::Rendering::RenderObject* g_splineLines[3] = { nullptr, nullptr, nullptr };

////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This file contains a minimal radium/qt application which shows the geometrical primitives
 * supported by Radium
 */

// MinimalComponent::MinimalComponent(Ra::Engine::Scene::Entity* entity,
MinimalComponent::MinimalComponent( Ra::Engine::Scene::Entity* entity,
                                    Ra::Engine::RadiumEngine& e,
                                    Ra::Gui::Viewer& viewer )
    :
    Ra::Engine::Scene::Component( "Minimal Component", entity ),
    m_engine( e ),
    m_viewer( viewer )
{}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.

void MinimalComponent::initialize() {
    ScanMaterial::registerMaterial();

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

        m_roGrid = RenderObject::createRenderObject(
            "test_grid", this, RenderObjectType::Geometry, gridPrimitive, {} );
        m_roGrid->setMaterial( Ra::Core::make_shared<PlainMaterial>( "Grid material" ) );
        m_roGrid->setPickable( false );
        addRenderObject( m_roGrid );
    }


    // origin gizmo
    {
        constexpr Scalar arrowScale = 1_ra;
        constexpr Scalar axisWidth  = .05_ra;
        constexpr Scalar arrowFrac  = 0_ra;

        std::vector<Color> gizmoColors = { Color::Red(),
                                           Color::Green(),
                                           Color::Blue(),
                                           Color::Cyan(),
                                           Color::Cyan(),
                                           Color::Cyan() };

        for ( uint i = 0; i < 6; ++i ) {
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

    g_probes.reserve( g_nProbes );
    for ( int i = 0; i < g_nProbes; ++i ) {
        addProbe();
    }

    {
        Ra::Core::Geometry::Spline<3, 3> spline;
        Core::VectorArray<Vector3> points;
        points.push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
        points.push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
        points.push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
        spline.setCtrlPoints( points );

        for ( int i = 0; i < 3; ++i ) {
            m_roTraces[i] = RenderObject::createRenderObject(
                "test_line",
                this,
                RenderObjectType::Geometry,
#ifdef USE_GOT_CONTRIB
                DrawPrimitives::Spline( spline, 10, Color::Green(), Color::Red() ),
#else
                DrawPrimitives::Spline( spline, 10, Color::Green() ),
#endif
                {} );
            m_roTraces[i]->setMaterial( g_plainMaterial );
            addRenderObject( m_roTraces[i] );
        }
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
        }

        probe.m_scanLine = RenderObject::createRenderObject(
            "scanLine", this, RenderObjectType::Geometry, m_meshAxis[5] );
        probe.m_scanLine->setMaterial( g_plainMaterial );
        addRenderObject( probe.m_scanLine );

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
        probe.m_data = new unsigned char[192 * 512];
        // fill with some function
        for ( int i = 0; i < 192; ++i ) {
            for ( int j = 0; j < 512; j++ ) {
                if ( std::abs( i - 20 - iProbe * 10 ) < 3 ||
                     std::abs( j - 20 - iProbe * 10 ) < 3 ) {
                    probe.m_data[( i * 512 + j )] = 0;
                }
                else {
                    probe.m_data[( i * 512 + j )] = ( j / 2 ) % 256;
                }
            }
        }
        probe.m_textureName = std::string( "myTexture" ) + std::to_string( iProbe );
        auto& textureParameters =
            m_engine.getTextureManager()->addTexture(
                probe.m_textureName.c_str(), 256, 256, probe.m_data );
        textureParameters.format         = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        assert( probe.m_textureName == textureParameters.name );
        //! [Creating a texture for the slice]

        std::shared_ptr<Engine::Data::Mesh> meshQuad(
            new Engine::Data::Mesh( std::string( "Scan plane" ) + std::to_string( iProbe ) ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );


        probe.m_scan =
            RenderObject::createRenderObject( std::string( "echoPlane" ) + std::to_string( iProbe ),
                                              this,
                                              RenderObjectType::Geometry,
                                              meshQuad );




        {
            auto mat = make_shared<ScanMaterial>(
                ( std::string( "Scan Material" ) + std::to_string( iProbe ) ).c_str() );

            mat->m_perVertexColor = true;
            mat->addTexture( ScanMaterial::TextureSemantic::TEX_DIFFUSE, textureParameters );

            probe.m_scan->setMaterial( mat );

        }

        auto TLocal = Transform::Identity();
        TLocal.translate( Vector3( 0_ra, 0_ra, -iProbe * 5 ) );
        TLocal.scale( 10.0 );
        probe.m_scan->setLocalTransform( TLocal );

        addRenderObject( probe.m_scan );
    }

    g_probes.push_back( std::move( probe ) );

}

void MinimalComponent::updateShader() {
    int iProbe = 0;
    for ( auto& probe : g_probes ) {

        auto& scan = *probe.m_scan;


        Ra::Engine::Data::TextureParameters textureParameters;
        textureParameters.name = probe.m_textureName;
        m_viewer.makeCurrent();
        probe.m_textureScan = m_engine.getTextureManager()->getOrLoadTexture( textureParameters );
        m_viewer.doneCurrent();

        std::cout << "iProbe : " << iProbe << ", texture = " << probe.m_textureScan << std::endl;
        ++iProbe;
    }

    for ( int i = 0; i < g_nProbes; ++i ) {
        initPose( i );
        initScan( i );
    }
}

void MinimalComponent::updatePose( const hub::Acquisition& acq ) {

    // void MinimalComponent::updatePose( Acquisitions& acqs ) {
    // void MinimalComponent::updatePose( hub::Acquisition && acq) {

    if ( acq.m_start == g_timestampProbePose ) { ++g_iProbePose; }
    else {
        for ( int i = 1; i <= g_iProbePose; ++i ) {
            initPose( i );
        }
        g_iProbePose         = 0;
        g_timestampProbePose = acq.m_start;
    }




    auto& probe = g_probes.at( g_iProbePose );

    assert( acq.getMeasures().size() == 1 );
    hub::Dof6 dof6 = acq.getMeasures().at( 0 );

    Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );

    Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );
    std::cout << "update pose orientation " << dof6.m_w0 << ", " << dof6.m_w1 << ", " << dof6.m_w2
              << ", " << dof6.m_w3 << std::endl;

    // change to Radium base reference
    Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();

    // orientation
    Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
    TOrientation.rotate( orientation );

    // World transform
    Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();
    TWorld.translate( pos );

    Ra::Core::Transform TLocal3 = Ra::Core::Transform::Identity();


    // worked
    // end worked

    TLocal3.rotate(
        Eigen::AngleAxis( 0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3( 0.0, 1.0, 0.0 ) ) );
    TLocal3.rotate(
        Eigen::AngleAxis( 0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3( 1.0, 0.0, 0.0 ) ) );
    // end worked






    Ra::Core::Transform TLocal4 = Ra::Core::Transform::Identity();


    const float lineRadius = m_scanner.width / 10.0;
    {
        Ra::Core::Transform TLocal = Transform::Identity();
        TLocal.scale( lineRadius );


        probe.m_refCube->setLocalTransform( TRadium * TWorld * TLocal4 * TOrientation * TLocal3 );

        for ( int i = 0; i < 3; ++i ) {
            probe.m_refAxis[i]->setLocalTransform( TRadium * TWorld * TOrientation * TLocal );

        }
    }

    {
        // Local transform scan
        Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();


        TLocal.translate( Ra::Core::Vector3( 0.0, m_scanner.depth / 2.0, 0.0 ) + m_scanner.pos );


        Ra::Core::Vector3 vecScale( m_scanner.depth / 2.0, m_scanner.width / 2.0, 1.0 );
        TLocal.scale( vecScale );

        probe.m_scan->setLocalTransform( TRadium * TWorld * TLocal4 * TOrientation * TLocal3 *
                                         TLocal );
        if ( g_iProbePose > 0 ) probe.m_scan->setVisible( true );

        Ra::Core::Transform TLocal2 = Ra::Core::Transform::Identity();
        TLocal2.translate( Ra::Core::Vector3( 0.0, 0.0, -m_scanner.width / 2.0 ) + m_scanner.pos );
        Ra::Core::Vector3 vecScale2( lineRadius, lineRadius, m_scanner.width );
        TLocal2.scale( vecScale2 );

        probe.m_scanLine->setLocalTransform( TRadium * TWorld * TOrientation * TLocal2 );
        if ( g_iProbePose > 0 ) probe.m_scanLine->setVisible( true );
    }

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

    if ( iProbe == 0 ) initPoseTraces( {} );
}

// void MinimalComponent::updateScan( Acquisitions& acqs ) {
void MinimalComponent::updateScan( const hub::Acquisition& acq ) {

    if ( acq.m_start == g_timestampProbeScan ) { ++g_iProbeScan; }
    else {
        for ( int i = 1; i <= g_iProbeScan; ++i ) {
            initScan( i );
        }
        g_iProbeScan         = 0;
        g_timestampProbeScan = acq.m_start;
    }




    auto& probe = g_probes.at( g_iProbeScan );

    assert( probe.m_textureScan != nullptr );

    assert( acq.getMeasures().size() == 1 );
    const unsigned char* data = acq.getMeasures().at( 0 ).m_data;

    m_viewer.makeCurrent();
    auto& params = probe.m_textureScan->getParameters();
    memcpy( params.texels, data, 256 * 256 );
    probe.m_textureScan->initializeGL( false );
    m_viewer.doneCurrent();

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
            else { data[( i * 512 + j )] = ( j / 2 ) % 256; }
        }
    }
    m_viewer.makeCurrent();
    auto& params = probe.m_textureScan->getParameters();
    memcpy( params.texels, data, 256 * 256 );
    probe.m_textureScan->initializeGL( false );
    m_viewer.doneCurrent();
}

void MinimalComponent::setupScanner( double scanWidth,
                                     double scanDepth,
                                     double x,
                                     double y,
                                     double z ) {
    m_scanner.width = scanWidth;
    m_scanner.depth = scanDepth;
    m_scanner.pos   = Ra::Core::Vector3( x, y, z );
}

void MinimalComponent::initPoseTraces( const std::vector<hub::Acquisition>& poseAcqs ) {

    Ra::Core::Geometry::Spline<3, 3> splines[3];
    Core::VectorArray<Vector3> points[3];

    for ( const auto& acq : poseAcqs ) {




        assert( acq.getMeasures().size() == 1 );
        hub::Dof6 dof6 = acq.getMeasures().at( 0 );

        Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );

        Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

        // change to Radium base reference
        Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();

        // orientation
        Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
        TOrientation.rotate( orientation );

        // World transform
        Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();
        TWorld.translate( pos );

        {
            // Local transform scan
            Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();


            TLocal.translate( Ra::Core::Vector3( m_scanner.depth / 2.0, 0.0, 0.0 ) +
                              m_scanner.pos );



            Ra::Core::Transform TLocal2 = Ra::Core::Transform::Identity();
            TLocal2.translate( Ra::Core::Vector3( 0.0, -m_scanner.width / 2.0, 0.0 ) +
                               m_scanner.pos );
            const auto& transform = TRadium * TWorld * TOrientation * TLocal2;

            for ( int i = 0; i < g_nTraces; ++i ) {
                Vector3 pos2 = transform * Vector3( 0, i * m_scanner.width / 2.0, 0 );
                points[i].push_back( pos2 );
            }

        }
    }

    if ( poseAcqs.empty() ) {
        for ( int i = 0; i < 3; ++i ) {
            points[i].push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
            points[i].push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
            points[i].push_back( Vector3( 0_ra, 0_ra, 0_ra ) );
        }
    }

    std::cout << "[MinimalComponent] splines added -----------------" << std::endl;

    m_viewer.makeCurrent();

    for ( int i = 0; i < g_nTraces; ++i ) {
        splines[i].setCtrlPoints( points[i] );
        m_roTraces[i]->setMesh(
#ifdef USE_GOT_CONTRIB
            DrawPrimitives::Spline( splines[i],
                                    points[i].size(),
                                    Color::Cyan() * ( i + 1 ) / 3.0,
                                    Color::Black() * ( i + 1 ) / 3.0 ) );
#else
            DrawPrimitives::Spline(
                splines[i], points[i].size(), Color::Cyan() * ( i + 1 ) / 3.0 ) );
#endif
        m_roTraces[i]->setTransparent( true );
    }

    m_viewer.doneCurrent();



}

void MinimalComponent::incIter() {
    g_iter++;
    std::cout << "iter = " << g_iter << std::endl;
}

/////////////////////// GETTERS AND SETTERS

void MinimalComponent::traceSetVisible( bool visible ) {
    for ( auto& roTrace : m_roTraces ) {
        roTrace->setVisible( visible );
    }
}

RenderObject& MinimalComponent::getRoGrid() {
    return *m_roGrid;
}

Traces& MinimalComponent::getRoTraces() {
    return m_roTraces;
}

// InputStream* scanStream = nullptr;
// InputStream* posStream  = nullptr;

//#define ONLY_POSE
