

#include <ScanComponent.hpp>

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
#include <ScanMaterial/ScanMaterial.hpp>
//#include <Engine/Scene/EntityManager.hpp>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

const bool ENABLE_GRID = true;

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

ScanComponent::ScanComponent( const hub::InputSensor& inputStream,
                              Ra::Engine::Scene::Entity* entity,
                              Ra::Engine::RadiumEngine& engine,
                              Ra::Gui::Viewer& viewer ) :
    //    Ra::Engine::Scene::Component( "Dof6 component", entity ) {}
    StreamComponent( inputStream, entity ),
    m_engine( engine ),
    m_viewer( viewer ) {}

void ScanComponent::initialize() {
    StreamComponent::initialize();

    //    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material"
    //    ); blinnPhongMaterial->m_perVertexColor = true; blinnPhongMaterial->m_ks = Color::White();
    //    blinnPhongMaterial->m_ns = 100_ra;

    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian Material" );
    lambertianMaterial->m_perVertexColor = true;

    auto plainMaterial              = make_shared<PlainMaterial>( "Plain Material" );
    plainMaterial->m_perVertexColor = true;

    //// setup ////

    //    // origin ref cube
    //    {
    //        assert(m_ro == nullptr);
    //        std::shared_ptr<Mesh> cube1(new Mesh("Cube"));
    //        const Scalar cubeSide = 50.0;
    //        auto box = Core::Geometry::makeSharpBox(Vector3 { 1_ra, 1_ra, 1_ra } * cubeSide / 2.0,
    //        Color::Grey()); cube1->loadGeometry(std::move(box));

    //        m_ro = RenderObject::createRenderObject(
    //            "refCube", this, RenderObjectType::Geometry, cube1, {});
    //        m_ro->setMaterial(lambertianMaterial);
    //        addRenderObject(m_ro);
    //    }

    const auto& metadata = m_inputStream.m_spec.m_metaData;
    double scanWidth     = 1000; // mm
//    if ( metadata.find( "scanWidth" ) != metadata.end() ) {
//        scanWidth = std::any_cast<double>( metadata.at( "scanWidth" ) );
//    }

    double scanDepth = 1000; // mm
//    if ( metadata.find( "scanDepth" ) != metadata.end() ) {
//        scanDepth = std::any_cast<double>( metadata.at( "scanDepth" ) );
//    }

    // scan plane
    {
#ifdef USE_GOT_PR
        auto quadTriangle = Ra::Core::Geometry::makeYNormalQuad( { 1_ra, 1_ra }, {}, true );
#else
        auto quadTriangle = Ra::Core::Geometry::makeYNormalQuad( { 1_ra, 1_ra }, {} );
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
        //        unsigned char* data = new unsigned char[192 * 512];
        const auto& sensorName = m_inputStream.m_spec.m_sensorName;
        assert( m_inputStream.m_spec.m_dims.size() == 2 );
        const int width    = m_inputStream.m_spec.m_dims.at( 0 );
        const int height   = m_inputStream.m_spec.m_dims.at( 1 );
        const int sizeData = m_inputStream.m_spec.m_acquisitionSize;
        int nChannel = 0;
        switch (m_inputStream.m_spec.m_format) {
        case hub::SensorSpec::Format::Y8:
            nChannel = 1;
            break;
        case hub::SensorSpec::Format::Y16:
            nChannel = 2;
            break;
        default:
            assert(false);
            break;
        }
        assert( sizeData == width * height * nChannel );
        int iProbe = 0;

        m_data = new unsigned char[sizeData];
        // fill with some function
        for ( int i = 0; i < width; ++i ) {
            for ( int j = 0; j < height; j++ ) {
                for ( int k = 0; k < nChannel; ++k ) {
                    if ( std::abs( i - 40 - iProbe * 10 ) < 5 ||
                         std::abs( j - 40 - iProbe * 10 ) < 5 ) {
                        //                    data[( i * 512 + j )] = 0;
                        m_data[( i * height + j ) * nChannel]     = 0;
                        m_data[( i * height + j ) * nChannel + 1] = 0;
                    }
                    else {
                        //                    data[( i * 512 + j )] = ( j / 2 ) % 256;
                        m_data[( i * height + j ) * nChannel]     = ( j / 2 ) % 256;
                        m_data[( i * height + j ) * nChannel + 1] = ( j / 2 ) % 256;
                    }
                }
            }
        }
        //        m_textureName = std::string("myTexture") + std::to_string(iProbe);
        m_textureName = sensorName;
        auto& textureParameters =
            //            m_engine.getTextureManager()->addTexture( probe.m_textureName.c_str(),
            //            512, 192, data );
            //            m_engine.getTextureManager()->addTexture(probe.m_textureName.c_str(), 512,
            //            192, probe.m_data);
            m_engine.getTextureManager()->addTexture(
                m_textureName.c_str(), height, width, m_data );

        if ( nChannel == 1 ) {
            textureParameters.format         = gl::GLenum::GL_RED;
            textureParameters.internalFormat = gl::GLenum::GL_R8;
        }
        else if ( nChannel == 2 ) {
            textureParameters.format         = gl::GLenum::GL_RG;
            textureParameters.internalFormat = gl::GLenum::GL_RG;
        }

        //        textureParameters.minFilter = gl::GLenum::GL_LINEAR;
        //        textureParameters.magFilter = gl::GLenum::GL_LINEAR;
        assert( m_textureName == textureParameters.name );
        //! [Creating a texture for the slice]

        std::shared_ptr<Engine::Data::Mesh> meshQuad(
            new Engine::Data::Mesh( std::string( "Scan plane" ) + std::to_string( iProbe ) ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );

        m_ro = RenderObject::createRenderObject(
            "echoPlane", this, RenderObjectType::Geometry, meshQuad );

        //            auto mat                   = make_shared<PlainMaterial>( (std::string("Plain
        //            Material") + std::to_string(iProbe)).c_str() );
        auto mat = make_shared<ScanMaterial>( "Scan Material" );
        //            auto mat                   = make_shared<BlinnPhongMaterial>(
        //            (std::string("Plain Material") + std::to_string(iProbe)).c_str() );

        mat->m_perVertexColor = true;
        mat->addTexture( ScanMaterial::TextureSemantic::TEX_COLOR, textureParameters );

        //            mat->addTexture(PlainMaterial::TextureSemantic::TEX_COLOR, textureParameters);
        //            mat->addTexture(BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE,
        //            textureParameters);
        m_ro->setMaterial( mat );

        auto TLocal = Transform::Identity();
        TLocal.scale( Vector3( scanDepth / 2.0, 1.0, scanWidth / 2.0 ) );
        TLocal.translate( Vector3( 1.0, 0.0, 0.0 ) );
        //        TLocal.scale(1000.0);
        m_ro->setLocalTransform( TLocal );
        //                m_ro->getRenderTechnique()->setConfiguration( shaderConfig );

        addRenderObject( m_ro );

        m_viewer.makeCurrent();
        m_textureScan = m_engine.getTextureManager()->getOrLoadTexture( textureParameters );
        m_viewer.doneCurrent();

        //        Ra::Core::Asset::ImageSpec imgSpec(width, height, 1);
        //        m_image = std::make_shared<Ra::Core::Asset::Image>(imgSpec, nullptr);
        //        m_textureScan->attachImage(m_image);
    }

    // scan line
    {
        m_scanLine = RenderObject::createRenderObject(
            "scanLine", this, RenderObjectType::Geometry, m_meshAxis[5] ); // z axis
        m_scanLine->setMaterial( plainMaterial );

        const float lineRadius = 0.02 * scanDepth * 10;

        auto TLocal = Transform::Identity();
        TLocal.scale( Vector3( lineRadius, lineRadius, scanWidth ) );
        TLocal.translate( Vector3( 0.0, 0.0, -0.5 ) );
        //        TLocal.translate(Vector3(0_ra, 0_ra, -iProbe * 5));
        //        TLocal.scale(1000.0);
        m_scanLine->setLocalTransform( TLocal );

        addRenderObject( m_scanLine );
    }
}

void ScanComponent::update( const hub::Acquisition& acq ) {
    const unsigned char* data = acq.m_data;
    memcpy( m_data, data, m_inputStream.m_spec.m_acquisitionSize );

    //    m_viewer.makeCurrent();
    //    auto& params = m_textureScan->getParameters();
    //    //    memcpy(params.texels, data, 192 * 512);
    //    memcpy(params.texels, data, m_inputStream.getAcquisitionSize());
    //    m_textureScan->initializeGL(false);
    //    m_viewer.doneCurrent();

    auto* textureManager   = m_engine.getTextureManager();
    const auto& sensorName = m_inputStream.m_spec.m_sensorName;
    textureManager->updateTextureContent( sensorName, (void*)m_data );

    // PR GOT
    //    m_image->update(data, m_inputStream.getAcquisitionSize());

    //        acqs.pop();
}
