

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
//     if (m_textureData != nullptr) {
//         delete m_textureData;
//         m_textureData = nullptr;
//         delete m_textureScan;
//         delete m_quad;
//         delete m_scanLine;
//     }
// }

ScanComponent::ScanComponent( const hub::InputSensor& inputSensor,
                              Ra::Engine::Scene::Entity* entity,
                              Ra::Engine::RadiumEngine& engine,
                              Ra::Gui::Viewer& viewer ) :
    //    Ra::Engine::Scene::Component( "Dof6 component", entity ) {}
    SensorComponent( inputSensor, entity ),
    m_engine( engine ),
    m_viewer( viewer ) {}

void ScanComponent::initialize() {
    SensorComponent::initialize();

    //    auto blinnPhongMaterial              = make_shared<BlinnPhongMaterial>( "Shaded Material"
    //    ); blinnPhongMaterial->m_perVertexColor = true; blinnPhongMaterial->m_ks = Color::White();
    //    blinnPhongMaterial->m_ns = 100_ra;

    //    auto lambertianMaterial              = make_shared<LambertianMaterial>( "Lambertian
    //    Material" ); lambertianMaterial->m_perVertexColor = true;

    //    auto plainMaterial              = make_shared<PlainMaterial>( "Plain Material" );
    m_scanLineMaterial                   = make_shared<PlainMaterial>( "Plain Material" );
    m_scanLineMaterial->m_perVertexColor = true;

    //    m_scanMaterial = std::make_unique<ScanMaterial>( "Scan Material" );
    //// setup ////

    const auto& sensorSpec  = m_inputSensor.m_spec;
    const auto& metaData    = sensorSpec.m_metaData;
    const auto& resolutions = sensorSpec.m_resolutions;
    //    if ( metaData.find( "type" ) != metaData.end() ) {
    //        const char* type = std::any_cast<const char*>( metaData.at( "type" ) );
    //        if ( strcmp( type, "record" ) == 0 ) {
    //            //            m_isRecordStream = true;
    //            m_isLiveStream = false;
    //        }

    if ( metaData.find( "nAcqs" ) != metaData.end() ) {
        //        assert( metaData.find( "nAcqs" ) != metaData.end() );
        m_nMaxScans = std::any_cast<unsigned int>( metaData.at( "nAcqs" ) );
    }
    if ( metaData.find( "transform" ) != metaData.end() ) {
        const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
        m_localTransform   = Eigen::Map<Eigen::Matrix4f>( (float*)array ); // Column-major
    }
    else if ( metaData.find( "parent" ) != metaData.end() ) {
        //        m_localTransform.block( 0, 0, 3, 3 ) = Eigen::Scaling( 100.0f, 100.0f, 100.0f );
        //        Ra::Core::Transform TLocal = Transform::Identity();
        //        TLocal.scale(0.01);
        //        for (int i = 0; i <3; ++i) {
        //            m_roAxes[i]->setLocalTransform(TLocal);
        //        }
    }
    else { m_localTransform.block( 0, 0, 3, 3 ) = Eigen::Scaling( 100.0f, 100.0f, 100.0f ); }

    // image only
    if ( resolutions.size() == 1 ) {
        addScan();
        //        m_iScan     = 0;
        auto& scan = m_scans.at( 0 );
        scan.m_quad->setVisible( true );
        scan.m_scanLine->setVisible( true );
        m_nMaxScans = 1;
        //        m_nScans    = 1;
    }
    else if ( resolutions.size() == 2 ) {
        for ( int i = 0; i < m_nMaxScans; ++i ) {
            addScan();
        }
    }
    else { assert( false ); }
    //    if ( m_isLiveStream ) {
    //        addScan();
    //        m_iScan = 0;
    //    }
    //    else {
    //        for ( int i = 0; i < m_nScans; ++i ) {
    //            addScan();
    //        }
    //    }

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
}

void ScanComponent::update( const hub::Acquisition& acq ) {
    //    std::cout << "[ScanComponent] update(Acquisition) receive acq : " << acq << std::endl;

    const auto& measures = acq.getMeasures();
    const auto nMeasures = measures.size();

    //    int iScan = 0;

    bool isNewScan = true;

    if ( m_traceEnabled ) {
        // image only
        if ( nMeasures == 1 ) {
            //        if ( iScan == -1 ) {
            //            addScan();
            //            iScan = 0;
            //        }
        }
        // scan (image + 6DOF)
        else if ( nMeasures == 2 ) {
            assert( !m_scans.empty() );
            assert( 0 <= m_iScan && m_iScan < m_nMaxScans );
            assert( m_iScan < (int)m_scans.size() );
            auto& lastScan = m_scans.at( m_iScan );
            //        lastScan.m_quad->setVisible(false);
            //        if ( !m_isLiveStream ) {
            //        lastScan.m_quad->setTransparent( true );
            lastScan.m_material->m_isTransparent = true;
            lastScan.m_material->needUpdate();
            lastScan.m_scanLine->setVisible( false );
            //        }

            //        addScan();
            //        ++iScan;
            if ( m_startScan2iScan.find( acq.m_start ) != m_startScan2iScan.end() ) {
                m_iScan   = m_startScan2iScan.at( acq.m_start );
                isNewScan = false;
            }
            else {
                m_iScan                        = m_nScans++;
                m_startScan2iScan[acq.m_start] = m_iScan;
                std::cout << "[ScanComponent] update() show new scan " << m_nScans << std::endl;
            }
        }
        else { assert( false ); }
    }
    //    if ( m_firstUpdate || acq.m_start != m_lastUpdateDate ) {
    //        m_firstUpdate    = false;
    //        m_lastUpdateDate = acq.m_start;
    //        if ( !m_isLiveStream ) ++m_iScan;
    //    }
    //    else { return; }
    //    }
    //    std::cout << "[ScanComponent] m_iScan = " << m_iScan << ", update acq : " << acq <<
    //    std::endl;

    assert( !m_scans.empty() );
    assert( 0 <= m_iScan && m_iScan < m_nMaxScans );
    assert( m_iScan < (int)m_scans.size() );
    auto& scan = m_scans.at( m_iScan );


    assert( 1 <= nMeasures && nMeasures <= 2 );

    // update scan properties
    if ( isNewScan ) {
        // find image data
        // image is always the last measure by convention
        const auto& image              = measures.at( nMeasures - 1 );
        const unsigned char* imageData = image.m_data;
        int imageSize                  = image.m_size;

        // update texture
        {
            memcpy( scan.m_textureData, imageData, imageSize );

            auto* textureManager = m_engine.getTextureManager();
            textureManager->updateTextureContent( scan.m_textureName, (void*)scan.m_textureData );
        }

        // update position and orientation
        if ( nMeasures == 2 ) {
            // update quad
            {
                const hub::Dof6& dof6 = measures.at( 0 );
                Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
                Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );
                auto TLocal = Transform::Identity();
                TLocal.translate( pos );
                TLocal.rotate( orientation );

                for ( int i = 0; i < 3; ++i ) {
                    m_roAxes[i]->setLocalTransform( TLocal );
                }

                //            if ( m_isLiveStream ) { m_entity->setTransform( TLocal ); }
                //            else {
                TLocal *= m_localTransform;
                scan.m_quad->setLocalTransform( TLocal );
                scan.m_scanLine->setLocalTransform( TLocal );
                //            }

                //            if ( m_isLiveStream ) scan.m_scanLine->setVisible( true );
            }
        }
    }

    if ( m_liveEnabled && nMeasures == 2 ) {
        scan.m_material->m_isTransparent = false;
        scan.m_material->needUpdate();
        scan.m_quad->setVisible( true );
        scan.m_scanLine->setVisible( true );
    }
}

Aabb ScanComponent::getAabb() const {
    //    Aabb aabb = m_scans.at(0).m_quad->computeAabb();
    Aabb aabb;
    if ( m_nScans == 0 ) {
        aabb = m_entity->computeAabb();
        aabb.transform( m_entity->getTransform() );
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
    for (int iScan = 0; iScan < m_nScans; ++iScan) {
//    for (auto & scan : m_scans) {
        auto & scan = m_scans.at(iScan);
        scan.m_quad->setVisible(m_traceEnabled);
    }
}

void ScanComponent::enableLive( bool enable ) {
    m_liveEnabled = enable;
    auto& scan    = m_scans.at( m_iScan );
    scan.m_quad->setVisible( m_liveEnabled );
    scan.m_scanLine->setVisible( m_liveEnabled );
}

void ScanComponent::addScan() {
    //    std::cout << "[ScanComponent] addScan " << m_scans.size() << std::endl;
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
        //        tex_coords.push_back( { 0_ra, 0_ra, 0_ra } );
        //        tex_coords.push_back( { 1_ra, 0_ra, 0_ra } );
        //        tex_coords.push_back( { 0_ra, 1_ra, 0_ra } );
        //        tex_coords.push_back( { 1_ra, 1_ra, 0_ra } );
        quadTriangle.addAttrib(
            Ra::Core::Geometry::getAttribName( Ra::Core::Geometry::VERTEX_TEXCOORD ), tex_coords );

        const auto& sensorName  = m_inputSensor.m_spec.m_sensorName;
        const auto& sensorSpec  = m_inputSensor.m_spec;
        const auto& resolutions = sensorSpec.m_resolutions;
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
            imageSize = sensorSpec.m_acquisitionSize;
        }
        else if ( resolutionsSize == 2 ) {
            assert( resolutions.at( 0 ).second == hub::SensorSpec::Format::DOF6 );
            assert( resolutions.at( 1 ).first.size() == 2 );
            dims      = resolutions.at( 1 ).first;
            format    = resolutions.at( 1 ).second;
            width     = dims.at( 0 );
            height    = dims.at( 1 );
            imageSize = hub::SensorSpec::computeAcquisitionSize( resolutions.at( 1 ) );
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
                    //                    if ( std::abs( i - 40 - iScan ) < 5 || std::abs( j - 40 -
                    //                    iScan ) < 5 ) {
                    //                        scan.m_textureData[( i * height + j ) * nChannels + k]
                    //                        = 0;
                    //                    }
                    //                    else {
                    //                        scan.m_textureData[( i * height + j ) * nChannels + k]
                    //                        = ( i / 2 ) % 256;
                    //                    }
                }
            }
        }

        scan.m_textureName      = sensorName + std::to_string( iScan );
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
        scan.m_material->m_pimp.x()  = 0.5;
        scan.m_material->m_pimp.y()  = 1.0;
        scan.m_material->m_nChannels = nChannels;
#endif
        scan.m_material->addTexture( CurrentMaterial::TextureSemantic::TEX_DIFFUSE,
                                     textureParameters );

        scan.m_quad->setMaterial( scan.m_material );
        //        if ( !m_isLiveStream ) {
        scan.m_quad->setTransparent( true );
        scan.m_material->m_isTransparent = false;
        scan.m_material->needUpdate();
        //        }

        scan.m_quad->setVisible( false );

        //        scan.m_quad->setLocalTransform( m_localTransform );
        //        if ( !m_isLiveStream ) { scan.m_quad->setVisible( false ); }

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

        //        scan.m_scanLine->setLocalTransform( m_localTransform );
        //        if ( !m_isLiveStream ) { scan.m_scanLine->setVisible( false ); }
        scan.m_scanLine->setVisible( false );

        addRenderObject( scan.m_scanLine );
    }

    //    if ( m_isLiveStream ) {
    auto TLocal = Transform::Identity();
    TLocal *= m_localTransform;
    scan.m_scanLine->setLocalTransform( TLocal );
    scan.m_quad->setLocalTransform( TLocal );
    //    }

    m_scans.push_back( scan );
}

void ScanComponent::on_tune_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune_valueChanged : " << arg1 << std::endl;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.x() = arg1;
        scan.m_material->needUpdate();
    }
#endif
}

void ScanComponent::on_tune2_valueChanged( double arg1 ) {
#ifndef USE_BLINN_PHONG_MATERIAL
    std::cout << "[ScanComponent] on_tune_valueChanged : " << arg1 << std::endl;
    for ( auto& scan : m_scans ) {
        scan.m_material->m_pimp.y() = arg1;
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
    //    if ( !m_isLiveStream ) {
    for ( auto& scan : m_scans ) {
        scan.m_material->m_isTransparent = isTransparent;
        scan.m_material->needUpdate();
    }
//    }
#endif
}
