
#include "MinimalComponent.hpp"

#include <Core/Asset/BlinnPhongMaterialData.hpp>
#include <Core/Asset/FileData.hpp>
#include <Core/Containers/MakeShared.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Core/Geometry/TopologicalMesh.hpp>
#include <Core/Resources/Resources.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Core/Utils/Timer.hpp>

#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/LambertianMaterial.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <Engine/FrameInfo.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>

#include <Engine/Data/TextureManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <Gui/BaseApplication.hpp>
#include <Gui/Viewer/Viewer.hpp>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

#include <stream.h>

const char* vertexShaderFile   = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const char* fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

/**
 * This file contains a minimal radium/qt application which shows the geometrical primitives
 * supported by Radium
 */

MinimalComponent::MinimalComponent( Ra::Engine::Scene::Entity* entity,
                                    Ra::Engine::RadiumEngine& e ) :
    Ra::Engine::Scene::Component( "Minimal Component", entity ), m_engine( e ) {}


/// This function is called when the component is properly
/// setup, i.e. it has an entity.

void MinimalComponent::initialize() {
    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material" );
    blinnPhongMaterial->m_perVertexColor = true;
    blinnPhongMaterial->m_ks             = Color::White();
    blinnPhongMaterial->m_ns             = 100_ra;

    auto plainMaterial              = make_shared<PlainMaterial>( "Plain Material" );
    plainMaterial->m_perVertexColor = true;

    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian Material" );
    lambertianMaterial->m_perVertexColor = true;

    //// setup ////
    Scalar cellSize   = 1_ra;
    int nCellX        = 10;

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


    std::vector<std::shared_ptr<Engine::Data::Mesh>> meshAxis;

    // origin gizmo
    {
        constexpr Scalar arrowScale = 1_ra;
        constexpr Scalar axisWidth  = .05_ra;
        constexpr Scalar arrowFrac  = 0_ra;

        std::vector<Color> gizmoColors = { Color::Red(), Color::Green(), Color::Blue() };

        for ( uint i = 0; i < 3; ++i ) {
            Core::Vector3 cylinderEnd = Core::Vector3::Zero();
            cylinderEnd[i]            = ( 1_ra - arrowFrac );
            Core::Vector3 arrowEnd    = Core::Vector3::Zero();
            arrowEnd[i]               = 1_ra;
            Core::Geometry::TriangleMesh cylinder =
                Core::Geometry::makeCylinder( Core::Vector3::Zero(),
                                              arrowScale * cylinderEnd,
                                              arrowScale * axisWidth / 2_ra,
                                              32,
                                              gizmoColors[i] );

            std::shared_ptr<Engine::Data::Mesh> mesh(
                new Engine::Data::Mesh( "Translate Gizmo Arrow" ) );
            mesh->loadGeometry( std::move( cylinder ) );

            meshAxis.push_back( std::move( mesh ) );
        }

        // origin axis
        for ( uint i = 0; i < 3; ++i ) {
            auto gizmo = RenderObject::createRenderObject(
                "originAxis" + std::to_string( i ), this, RenderObjectType::Geometry, meshAxis[i] );

            gizmo->setMaterial( plainMaterial );
            addRenderObject( gizmo );
        }
    }

    // ref cube
    {
        std::shared_ptr<Mesh> cube1( new Mesh( "Cube" ) );
        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        auto box      = Core::Geometry::makeSharpBox( cubeSize * 0.7 / 2, Color::Grey() );
        cube1->loadGeometry( std::move( box ) );

        auto renderObject1 = RenderObject::createRenderObject(
            "refCube", this, RenderObjectType::Geometry, cube1, {} );
        renderObject1->setMaterial( lambertianMaterial );
        addRenderObject( renderObject1 );
    }

    // probe cube
    {
        std::shared_ptr<Mesh> cube2( new Mesh( "Cube" ) );

        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        cube2->loadGeometry( Geometry::makeSharpBox( cubeSize * 0.2 / 2 ) );
        cube2->getCoreGeometry().addAttrib(
            "in_color", Vector4Array { cube2->getNumVertices(), Color::Grey() } );
        //        cube2->setAttribNameCorrespondance("colour", "in_color");

        m_probe = RenderObject::createRenderObject(
            "probeCube", this, RenderObjectType::Geometry, cube2, {} );
        m_probe->setLocalTransform( Transform { Translation( Vector3( 0_ra, 2_ra, 0_ra ) ) } );
        m_probe->setMaterial( lambertianMaterial );
        addRenderObject( m_probe );
    }

    // probe axis
    {
        for ( uint i = 0; i < 3; ++i ) {
            m_probe_axis[i] = RenderObject::createRenderObject(
                "probeAxis" + std::to_string( i ), this, RenderObjectType::Geometry, meshAxis[i] );
            m_probe_axis[i]->setMaterial( plainMaterial );
            addRenderObject( m_probe_axis[i] );

            m_probe_axis[i]->setLocalTransform(
                Transform { Translation( Vector3( 0_ra, 2_ra, 0_ra ) ) } );
        }
    }

    // scan plane
    {
        auto quadTriangle = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
        //        Ra::Core::Vector3Array tex_coords;
        //        tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
        //        tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
        //        tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
        //        tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
        //        quadTriangle.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD),
        //        tex_coords);

        //! [Creating a texture for the slice]
        //        unsigned char data[192 * 512];
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
            m_engine.getTextureManager()->addTexture( "myTexture", 512, 192, data );
        textureParameters.format         = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the slice]

        std::shared_ptr<Engine::Data::Mesh> meshQuad( new Engine::Data::Mesh( "Scan plane" ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );

//        RenderTechnique renderTechnique = RenderTechnique::createDefaultRenderTechnique();
//        Ra::Engine::Data::ShaderConfiguration shaderConfig(
//            "myShader", vertexShaderFile, fragmentShaderFile );
//        renderTechnique.setConfiguration( shaderConfig );

        m_scan = RenderObject::createRenderObject(
            "echoPlane", this, RenderObjectType::Geometry, meshQuad );

        //        auto myMat = make_shared<BlinnPhongMaterial>("Shaded Material");
        Ra::Core::Asset::BlinnPhongMaterialData matData( "myMaterialData" );
        // uncomment this to remove glossy highlight
        matData.m_specular      = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular   = true;
        matData.m_hasTexDiffuse = true;
        matData.m_texDiffuse    = "myTexture";
        //        m_scan->setMaterial(matData);

        std::shared_ptr<Ra::Engine::Data::Material> roMaterial;
        auto converter =
            Ra::Engine::Data::EngineMaterialConverters::getMaterialConverter( matData.getType() );
        auto mat = converter.second( &matData );
        roMaterial.reset( mat );

//        roMaterial->needUpdate();
        m_scan->setMaterial( roMaterial );

//        m_scan->getRenderTechnique()->setConfiguration( shaderConfig );

        addRenderObject( m_scan );
    }

//    updateProbe(Vector3(0_ra, 2_ra, 0_ra));
}

void MinimalComponent::updateShader() {
    auto& scan = *m_scan;

    auto renderTechnique = scan.getRenderTechnique();
    Ra::Engine::Data::ShaderConfiguration shaderConfig(
        "myShader", vertexShaderFile, fragmentShaderFile );
    renderTechnique->setConfiguration( shaderConfig );
}

void MinimalComponent::updateProbe(Ra::Core::Vector3 pos, Ra::Core::Quaternion orientation)
{

        // change to Radium base reference
        Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
        TRadium.rotate(Eigen::AngleAxis(1.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
        TRadium.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));

        // orientation
        Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
//        Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
        TOrientation.rotate(orientation);

        // World transform
        Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();

//        Ra::Core::Vector3 vecPos(-translation[0], -translation[1], -translation[2]);

        pos /= 5.0;
        TWorld.translate(pos);

        //        g_scan->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        //        g_probe->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        m_probe->setLocalTransform(TRadium * TWorld * TOrientation);
        for (int i = 0; i < 3; ++i) {
            m_probe_axis[i]->setLocalTransform(TRadium * TWorld * TOrientation);
        }

        // Local transform scan
        Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
        TLocal.translate(Ra::Core::Vector3(1.0, 0.0, 2.0));
        Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
        TLocal.scale(vecScale);
        m_scan->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
}

InputStream* scanStream = nullptr;
InputStream* posStream  = nullptr;

//#define ONLY_POSE
