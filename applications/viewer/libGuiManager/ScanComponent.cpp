

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

#include <Core/Geometry/StandardAttribNames.hpp>
#include <Eigen/src/Geometry/AlignedBox.h>
#include <Engine/Data/TextureManager.hpp>
#include <ScanMaterial/ScanMaterial.hpp>

#ifdef IO_USE_ASSIMP
#    include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

using namespace Ra;
using namespace Ra::Core;
using namespace Ra::Core::Utils;
using namespace Ra::Core::Geometry;
using namespace Ra::Engine;
using namespace Ra::Engine::Rendering;
using namespace Ra::Engine::Data;
using namespace Ra::Engine::Scene;

// Scan::~Scan() {
// }

ScanComponent::ScanComponent(
    //        const hub::InputSensor& inputSensor,
    const hub::SensorSpec& sensorSpec,
    Ra::Engine::Scene::Entity* entity,
    Ra::Engine::RadiumEngine& engine,
    Ra::Gui::Viewer& viewer ) :
    //    SensorComponent( inputSensor, entity ),
    SensorComponent( sensorSpec, entity ),
    m_engine( engine ),
    m_viewer( viewer ) {}

void ScanComponent::initialize() {
    SensorComponent::initialize();

    m_scanLineMaterial                   = make_shared<PlainMaterial>( "Plain Material" );
    m_scanLineMaterial->m_perVertexColor = true;

    //// setup ////

    //    const auto& sensorSpec  = m_inputSensor.m_spec;
    const auto& metaData    = m_sensorSpec.m_metaData;
    const auto& resolutions = m_sensorSpec.m_resolutions;

    if ( metaData.find( "nAcqs" ) != metaData.end() ) {
        m_nMaxScans = std::any_cast<unsigned int>( metaData.at( "nAcqs" ) );
    }
    if ( metaData.find( "transform" ) != metaData.end() ) {
        const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
        m_localTransform   = Eigen::Map<Eigen::Matrix4f>( (float*)array ); // Column-major
    }
    else if ( metaData.find( "parent" ) != metaData.end() ) {}
    else { m_localTransform.block( 0, 0, 3, 3 ) = Eigen::Scaling( 100.0f, 100.0f, 100.0f ); }

    // image only
    if ( resolutions.size() == 1 ) {
        addScan();
        auto& scan = m_scans.at( 0 );
        scan.m_quad->setVisible( true );
        scan.m_scanLine->setVisible( true );
        m_nMaxScans = 1;
    }
    else if ( resolutions.size() == 2 ) {

        const auto& format  = resolutions.at( 0 ).second;
        const auto& format2 = resolutions.at( 1 ).second;
        if ( format == hub::SensorSpec::Format::DOF6 ) { m_iImage = 1; }
        else {
            assert( format2 == hub::SensorSpec::Format::DOF6 );
            m_iImage = 0;
        }

        for ( int i = 0; i < m_nMaxScans; ++i ) {
            addScan();
        }
    }
    else { assert( false ); }
}

void ScanComponent::update( const hub::Acquisition& acq ) {

    //    std::cout << "[ScanComponent] update(" << acq << ")" << std::endl;

    if ( acq.m_start == -1 ) {
        std::cout << "[ScanComponent] receive reset acq : " << acq << std::endl;
        for ( int iScan = 0; iScan < m_nScans; ++iScan ) {
            auto& scan = m_scans.at( iScan );
            scan.m_quad->setVisible( m_traceEnabled );
        }
        m_iScan  = 0;
        m_nScans = 0;
        m_startScan2iScan.clear();
        return;
    }

    const auto& measures = acq.getMeasures();
    //    const auto nMeasures = measures.size();
    const auto nMeasures = m_sensorSpec.m_resolutions.size();

    bool isNewScan = true;

    if ( m_traceEnabled ) {
        // image only
        if ( nMeasures == 1 ) {}
        // scan (image + 6DOF)
        else if ( nMeasures == 2 ) {
            assert( !m_scans.empty() );
            assert( 0 <= m_iScan && m_iScan < m_nMaxScans );
            assert( m_iScan < (int)m_scans.size() );
            auto& lastScan                       = m_scans.at( m_iScan );
            lastScan.m_material->m_isTransparent = true;
            lastScan.m_material->needUpdate();
            lastScan.m_scanLine->setVisible( false );

            if ( m_startScan2iScan.find( acq.m_start ) != m_startScan2iScan.end() ) {
                m_iScan   = m_startScan2iScan.at( acq.m_start );
                isNewScan = false;
            }
            else {
                m_iScan                        = m_nScans++;
                m_startScan2iScan[acq.m_start] = m_iScan;
                std::cout << "[ScanComponent] update() show new scan " << m_iScan << std::endl;
            }
        }
        else { assert( false ); }
    }

    assert( !m_scans.empty() );
    assert( 0 <= m_iScan && m_iScan < m_nMaxScans );
    assert( m_iScan < (int)m_scans.size() );
    auto& scan = m_scans.at( m_iScan );

    assert( 1 <= nMeasures && nMeasures <= 2 );

    // update scan properties
    if ( isNewScan ) {
        // find image data
        // image is always the last measure by convention
        const auto& image              = measures.at( m_iImage );
        const unsigned char* imageData = image.m_data;
        int imageSize                  = image.m_size;

        // update texture
        {
            memcpy( scan.m_textureData, imageData, imageSize );
            //            memset(scan.m_textureData, 127, imageSize);

#ifdef USE_PR_UPDATE_TEXTURE
            scan.m_textureScan->updateData( scan.m_textureData );
#else
            auto* textureManager = m_engine.getTextureManager();
            textureManager->updateTextureContent( scan.m_textureName, (void*)scan.m_textureData );
#endif
            //            std::cout << "[ScanComponent] update scan" << std::endl;
        }

        // update position and orientation
        if ( nMeasures == 2 ) {
            // update quad
            {
                const hub::Dof6& dof6 = measures.at( 1 - m_iImage );
                Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
                Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );
                auto TLocal = Transform::Identity();
                TLocal.translate( pos );
                TLocal.rotate( orientation );

                TLocal *= m_localTransform;
                scan.m_quad->setLocalTransform( TLocal );
                scan.m_scanLine->setLocalTransform( TLocal );
            }
        }
    }

    if ( m_liveEnabled && nMeasures == 2 ) {
        scan.m_material->m_isTransparent = false;
        scan.m_material->needUpdate();
        scan.m_quad->setVisible( true );
        scan.m_scanLine->setVisible( true );

        const hub::Dof6& dof6 = measures.at( 1 - m_iImage );
        Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
        Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );
        auto TLocal = Transform::Identity();
        TLocal.translate( pos );
        TLocal.rotate( orientation );

        for ( int i = 0; i < 3; ++i ) {
            m_roAxes[i]->setLocalTransform( TLocal );
        }
    }
}

Aabb ScanComponent::getAabb() const {
    Aabb aabb;
    if ( m_nScans == 0 ) {
//        aabb                  = m_entity->computeAabb();
        aabb = m_scans.at(0).m_quad->computeAabb();

        const auto& transform = m_entity->getTransform();
        //    aabb.transform( m_entity->getTransform() );
        const Eigen::AlignedBox3f::VectorType rotated_extent_2 =
            transform.linear().cwiseAbs() * aabb.sizes();
        const Eigen::AlignedBox3f::VectorType rotated_center_2 =
            transform.linear() * ( aabb.max() + aabb.min() ) +
            Scalar( 2 ) * transform.translation();
        Eigen::AlignedBox3f::VectorType min, max;
        max = ( rotated_center_2 + rotated_extent_2 ) / Scalar( 2 );
        min = ( rotated_center_2 - rotated_extent_2 ) / Scalar( 2 );
        return Aabb( min, max );
        //        aabb.transform( m_entity->getTransform() );
        //        aabb.translate( m_entity->getTransformAsMatrix() );
    }
    else {
        for ( int i = 0; i < m_nScans; ++i ) {
            aabb.extend( m_scans.at( i ).m_quad->computeAabb() );
        }
    }
    return aabb;
}

void ScanComponent::enableTrace( bool enable ) {
    m_traceEnabled = enable;
    for ( int iScan = 0; iScan < m_nScans; ++iScan ) {
        auto& scan = m_scans.at( iScan );
        scan.m_quad->setVisible( m_traceEnabled );
    }
}

void ScanComponent::enableLive( bool enable ) {
    m_liveEnabled = enable;
    auto& scan    = m_scans.at( m_iScan );
    scan.m_quad->setVisible( m_liveEnabled );
    scan.m_scanLine->setVisible( m_liveEnabled );
}

void ScanComponent::addScan() {
    const int iScan = m_scans.size();

    Scan scan;

    // scan plane
    {
        auto quadTriangle = Ra::Core::Geometry::makeYNormalQuad( { 1_ra, 1_ra }, {} );
        Ra::Core::Vector3Array tex_coords;
        tex_coords.push_back( { 1_ra, 0_ra, 0_ra } );
        tex_coords.push_back( { 0_ra, 0_ra, 0_ra } );
        tex_coords.push_back( { 1_ra, 1_ra, 0_ra } );
        tex_coords.push_back( { 0_ra, 1_ra, 0_ra } );
        quadTriangle.addAttrib(
            Ra::Core::Geometry::getAttribName( Ra::Core::Geometry::VERTEX_TEXCOORD ), tex_coords );

        const auto& sensorName = m_sensorSpec.m_sensorName;
        //        const auto& sensorSpec  = m_inputSensor.m_spec;
        const auto& resolutions = m_sensorSpec.m_resolutions;
        int width;
        int height;
        int imageSize;
        hub::SensorSpec::Dims dims;
        hub::SensorSpec::Format format;
        const int resolutionsSize = resolutions.size();
        if ( resolutionsSize == 1 ) {
            dims      = resolutions.at( 0 ).first;
            format    = resolutions.at( 0 ).second;
            width     = dims.at( 0 );
            height    = dims.at( 1 );
            imageSize = m_sensorSpec.m_acquisitionSize;
        }
        else if ( resolutionsSize == 2 ) {
            assert( resolutions.at( 1 - m_iImage ).second == hub::SensorSpec::Format::DOF6 );
            assert( resolutions.at( m_iImage ).first.size() == 2 );
            dims      = resolutions.at( m_iImage ).first;
            format    = resolutions.at( m_iImage ).second;
            width     = dims.at( 0 );
            height    = dims.at( 1 );
            imageSize = hub::SensorSpec::computeAcquisitionSize( resolutions.at( m_iImage ) );
        }
        else { assert( false ); }
        assert( dims.size() == 2 );
        int nChannels = 0;
        switch ( format ) {
        case hub::SensorSpec::Format::Y8:
            nChannels = 1;
            break;
        case hub::SensorSpec::Format::Y16:
            nChannels = 2;
            break;
        case hub::SensorSpec::Format::BGR8:
            nChannels = 3;
            break;
        case hub::SensorSpec::Format::RGBA8:
            nChannels = 4;
            break;
        default:
            assert( false );
            break;
        }
        assert( imageSize == width * height * nChannels );

        scan.m_textureData = new unsigned char[imageSize];
        // fill with some function
        for ( int i = 0; i < width; ++i ) {
            for ( int j = 0; j < height; j++ ) {
                for ( int k = 0; k < nChannels; ++k ) {
                    scan.m_textureData[( i * height + j ) * nChannels + k] = 0;
                }
            }
        }

        scan.m_textureName      = sensorName + " " + std::to_string( iScan );
        auto& textureParameters = m_engine.getTextureManager()->addTexture(
            scan.m_textureName.c_str(), width, height, scan.m_textureData );

        if ( nChannels == 1 ) {
            textureParameters.format         = gl::GLenum::GL_RED;
            textureParameters.internalFormat = gl::GLenum::GL_R8;
        }
        else if ( nChannels == 2 ) {
            textureParameters.format         = gl::GLenum::GL_RG;
            textureParameters.internalFormat = gl::GLenum::GL_RG;
        }
        else if ( nChannels == 3 ) {
            textureParameters.format         = gl::GLenum::GL_RGB;
            textureParameters.internalFormat = gl::GLenum::GL_RGB;
        }
        else if ( nChannels == 4 ) {
            textureParameters.format         = gl::GLenum::GL_RGBA;
            textureParameters.internalFormat = gl::GLenum::GL_RGBA;
        }
        else { assert( false ); }

        assert( scan.m_textureName == textureParameters.name );

        std::shared_ptr<Engine::Data::Mesh> meshQuad(
            new Engine::Data::Mesh( std::string( "Scan plane" ) + std::to_string( iScan ) ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );

        scan.m_quad = RenderObject::createRenderObject(
            "echoPlane", this, RenderObjectType::Geometry, meshQuad );

        scan.m_material = make_shared<CurrentMaterial>(
            ( std::string( "Scan material " ) + std::to_string( iScan ) ).c_str() );

        scan.m_material->m_ks = Utils::Color::Black();
        scan.m_material->setMaterialAspect( ScanMaterial::MaterialAspect::MAT_TRANSPARENT );

#ifndef USE_BLINN_PHONG_MATERIAL
        scan.m_material->m_pimp.x()  = m_tune0;
        scan.m_material->m_pimp.y()  = m_tune1;
        scan.m_material->m_nChannels = nChannels;
#endif
        scan.m_material->addTexture( CurrentMaterial::TextureSemantic::TEX_DIFFUSE,
                                     textureParameters );

        scan.m_quad->setMaterial( scan.m_material );
        scan.m_quad->setTransparent( true );
        scan.m_material->m_isTransparent = false;
        scan.m_material->needUpdate();

        scan.m_quad->setVisible( false );

        addRenderObject( scan.m_quad );

        m_viewer.makeCurrent();
        scan.m_textureScan = m_engine.getTextureManager()->getOrLoadTexture( textureParameters );
        m_viewer.doneCurrent();
    }

    // scan line
    {
        Core::Geometry::TriangleMesh cylinder =
            Core::Geometry::makeCylinder( Core::Vector3( -1.0, 0, -1.0 ),
                                          Core::Vector3( -1.0, 0.0, 1.0 ),
                                          0.005,
                                          32,
                                          Color::Cyan() );

        std::shared_ptr<Engine::Data::Mesh> mesh(
            new Engine::Data::Mesh( "Translate Gizmo Arrow" ) );
        mesh->loadGeometry( std::move( cylinder ) );

        scan.m_scanLine = RenderObject::createRenderObject(
            "scanLine", this, RenderObjectType::Geometry, mesh ); // z axis
        scan.m_scanLine->setMaterial( m_scanLineMaterial );

        scan.m_scanLine->setVisible( false );

        addRenderObject( scan.m_scanLine );
    }

    auto TLocal = Transform::Identity();
    TLocal *= m_localTransform;
    scan.m_scanLine->setLocalTransform( TLocal );
    scan.m_quad->setLocalTransform( TLocal );

    m_scans.push_back( scan );
}

void ScanComponent::on_tune_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune_valueChanged : " << arg1 << std::endl;
    m_tune0 = arg1;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.x() = m_tune0;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_tune2_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune2_valueChanged : " << arg1 << std::endl;
    m_tune1 = arg1;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.y() = m_tune1;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_tune3_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune_valueChanged : " << arg1 << std::endl;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.z() = arg1;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_tune4_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune_valueChanged : " << arg1 << std::endl;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.w() = arg1;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_palette_valueChanged( int palette ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    for ( auto& scan : m_scans ) {
        scan.m_material->m_iPalette = palette;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_setTransparency( bool isTransparent ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    for ( auto& scan : m_scans ) {
        scan.m_material->m_isTransparent = isTransparent;
        scan.m_material->needUpdate();
    }
#endif
}
