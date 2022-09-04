

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

// constexpr int s_maxScans = 50;

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

    const auto& sensorSpec = m_inputSensor.m_spec;
    const auto& metaData   = sensorSpec.m_metaData;
    if ( metaData.find( "type" ) != metaData.end() ) {
        const char* type = std::any_cast<const char*>( metaData.at( "type" ) );
        if ( strcmp( type, "record" ) == 0 ) {
            //            m_isRecordStream = true;
            m_isLiveStream = false;
        }

        assert( metaData.find( "nAcqs" ) != metaData.end() );
        m_nScans = std::any_cast<unsigned int>( metaData.at( "nAcqs" ) );
    }
    if ( metaData.find( "transform" ) != metaData.end() ) {
        const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
        m_transform        = Eigen::Map<Eigen::Matrix4f>( (float*)array ); // Column-major
    }
    else {
        //        auto TLocal = Transform::Identity();
        //                TLocal.translate( pos );
        //                TLocal.rotate( orientation );
        //        TLocal.scale( Vector3( 100, 100, 100 ) );
        //        m_transform = Eigen::Scaling( 100.0, 100.0, 100.0 ).resize(4, 4);
        //        m_transform = Eigen::Matrix4f::Identity();
        m_transform.block( 0, 0, 3, 3 ) = Eigen::Scaling( 100.0f, 100.0f, 100.0f );
    }

    //    const auto& resolutions = sensorSpec.m_resolutions;
    //    if ( resolutions.size() == 1 ) {
    if ( m_isLiveStream ) {
        addScan();
        m_iScan = 0;
    }
    else {
        for ( int i = 0; i < m_nScans; ++i ) {
            addScan();
        }
    }
    //}

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

    //    const auto& metadata = m_inputSensor.m_spec.m_metaData;
    //    double scanWidth     = 1000; // mm
    //                                 //    if ( metadata.find( "scanWidth" ) != metadata.end() ) {
    //    //        scanWidth = std::any_cast<double>( metadata.at( "scanWidth" ) );
    //    //    }

    //    double scanDepth = 1000; // mm
    //    //    if ( metadata.find( "scanDepth" ) != metadata.end() ) {
    //    //        scanDepth = std::any_cast<double>( metadata.at( "scanDepth" ) );
    //    //    }
}

void ScanComponent::update( const hub::Acquisition& acq ) {

    const auto& measures = acq.getMeasures();
    const auto nMeasures = measures.size();
    //    if (! m_isLiveStream) {
    //    if ( nMeasures == 2 ) {
    if ( m_firstUpdate || acq.m_start != m_lastUpdateDate ) {
        m_firstUpdate    = false;
        m_lastUpdateDate = acq.m_start;
        if ( !m_isLiveStream ) ++m_iScan;
    }
    else { return; }
    //    }
    std::cout << "[ScanComponent] iScan = " << m_iScan << ", update acq : " << acq << std::endl;

    assert( !m_scans.empty() );
    assert( 0 <= m_iScan && m_iScan < m_nScans );
    assert( m_iScan < (int)m_scans.size() );
    auto& scan = m_scans.at( m_iScan );

    //    double scanWidth               = 250; // mm
    //    double scanDepth               = 250; // mm
    const unsigned char* imageData = nullptr;
    int imageSize;
    if ( nMeasures == 1 ) {
        const auto& image = measures.at( 0 );
        imageData         = image.m_data;
        imageSize         = image.m_size;
    }
    else if ( nMeasures == 2 ) {
        const hub::Measure& measure = measures.at( 0 );
        assert( measure.m_size == 28 );
        assert( measure.m_data != nullptr );
        const hub::Dof6& dof6 = acq.getMeasures().at( 0 );
        Ra::Core::Vector3 pos( dof6.m_x, dof6.m_y, dof6.m_z );
        Ra::Core::Quaternion orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );
        //        scan.m_quad.s
        // update quad
        {
            auto TLocal = Transform::Identity();
            TLocal.translate( pos );
            TLocal.rotate( orientation );
            //            TLocal.scale( Vector3( 100, 100, 100 ) );
            //            TLocal.scale( Vector3( scanDepth / 2.0, 1.0, scanWidth / 2.0 ) );
            //            TLocal.scale( Vector3( 100.0, 100.0, 100.0 ) );
            TLocal *= m_transform;
            //            TLocal.translate( Vector3( 1.0, 0.0, 0.0 ) );
            scan.m_scanLine->setLocalTransform( TLocal );
            if ( ! m_isLiveStream )
                TLocal *= Eigen::AngleAxis<float>( M_PIf, Vector3( 1.0, 0.0, 0.0 ) );
            TLocal *= Eigen::AngleAxis<float>( M_PI_2f, Vector3( 0.0, 1.0, 0.0 ) );
            scan.m_quad->setLocalTransform( TLocal );
            scan.m_quad->setVisible( true );
            //            if ( m_nScans == 1 ) scan.m_scanLine->setVisible( true );
            if ( m_isLiveStream ) scan.m_scanLine->setVisible( true );
            //            m_roAxes[0]->setLocalTransform( TLocal );
            //            m_roAxes[1]->setLocalTransform( TLocal );
            //            m_roAxes[2]->setLocalTransform( TLocal );
        }

        // update scan line
        //        {
        ////            const float lineRadius = 0.02 * scanDepth * 10;
        //            auto TLocal            = Transform::Identity();
        //            TLocal.translate( pos );
        //            TLocal.rotate( orientation );
        //            TLocal.scale( Vector3( 100, 100, 100));
        ////            TLocal.scale( Vector3( lineRadius, lineRadius, scanWidth ) );
        ////            TLocal.translate( Vector3( 0.0, 0.0, -0.5 ) );
        //            //        TLocal.translate(Vector3(0_ra, 0_ra, -iScan * 5));
        //            //        TLocal.scale(1000.0);
        //        }

        const auto& image = acq.getMeasures().at( 1 );
        imageData         = image.m_data;
        imageSize         = image.m_size;
    }
    else { assert( false ); }

    // update texture
    {
        //    const unsigned char* data = measure.m_data;
        memcpy( scan.m_textureData, imageData, imageSize );

        //    m_viewer.makeCurrent();
        //    auto& params = m_textureScan->getParameters();
        //    //    memcpy(params.texels, data, 192 * 512);
        //    memcpy(params.texels, data, m_inputSensor.getAcquisitionSize());
        //    m_textureScan->initializeGL(false);
        //    m_viewer.doneCurrent();

        auto* textureManager = m_engine.getTextureManager();
        //    const auto& sensorName = m_inputSensor.m_spec.m_sensorName;
        textureManager->updateTextureContent( scan.m_textureName, (void*)scan.m_textureData );
        //        scan.m_textureScan = m_engine.getTextureManager()->getOrLoadTexture(
        //        textureParameters );

        // PR GOT
        //    m_image->update(data, m_inputSensor.getAcquisitionSize());

        //        acqs.pop();
    }
}

void ScanComponent::addScan() {
    const int iScan = m_scans.size();

    //    double scanWidth = 1000; // mm
    //    if ( metadata.find( "scanWidth" ) != metadata.end() ) {
    //        scanWidth = std::any_cast<double>( metadata.at( "scanWidth" ) );
    //    }

    //    double scanDepth = 1000; // mm
    //    if ( metadata.find( "scanDepth" ) != metadata.end() ) {

    Scan scan;

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
        const auto& sensorName = m_inputSensor.m_spec.m_sensorName;
        const auto& sensorSpec = m_inputSensor.m_spec;
        //        assert(sensorSpec.m_resolutions.size() == 1);
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
        //        int iProbe = 0;

        scan.m_textureData = new unsigned char[imageSize];
        // fill with some function
        for ( int i = 0; i < width; ++i ) {
            for ( int j = 0; j < height; j++ ) {
                for ( int k = 0; k < nChannels; ++k ) {
                    if ( std::abs( i - 40 - iScan ) < 5 || std::abs( j - 40 - iScan ) < 5 ) {
                        //                    data[( i * 512 + j )] = 0;
                        scan.m_textureData[( i * height + j ) * nChannels + k] = 0;
                        //                        scan.m_textureData[( i * height + j ) * nChannels
                        //                        + 1] = 0;
                    }
                    else {
                        //                    data[( i * 512 + j )] = ( j / 2 ) % 256;
                        scan.m_textureData[( i * height + j ) * nChannels + k] = ( i / 2 ) % 256;
                        //                        scan.m_textureData[( i * height + j ) * nChannels
                        //                        + 1] = ( i / 2 ) % 256;
                    }
                }
            }
        }
        //        m_textureName = std::string("myTexture") + std::to_string(iScan);
        scan.m_textureName = sensorName + std::to_string( iScan );
        auto& textureParameters =
            //            m_engine.getTextureManager()->addTexture( probe.m_textureName.c_str(),
            //            512, 192, data );
            //            m_engine.getTextureManager()->addTexture(probe.m_textureName.c_str(), 512,
            //            192, probe.scan.m_textureData);
            m_engine.getTextureManager()->addTexture(
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

        //        textureParameters.minFilter = gl::GLenum::GL_LINEAR;
        //        textureParameters.magFilter = gl::GLenum::GL_LINEAR;
        assert( scan.m_textureName == textureParameters.name );
        //! [Creating a texture for the slice]

        std::shared_ptr<Engine::Data::Mesh> meshQuad(
            new Engine::Data::Mesh( std::string( "Scan plane" ) + std::to_string( iScan ) ) );
        meshQuad->loadGeometry( std::move( quadTriangle ) );

        scan.m_quad = RenderObject::createRenderObject(
            "echoPlane", this, RenderObjectType::Geometry, meshQuad );

        //            auto mat                   = make_shared<PlainMaterial>( (std::string("Plain
        //            Material") + std::to_string(iScan)).c_str() );
        //        scan.m_material = make_shared<ScanMaterial>( "Scan Material" );
        scan.m_material = make_shared<CurrentMaterial>(
            ( std::string( "Scan material " ) + std::to_string( iScan ) ).c_str() );
        //            auto mat                   = make_shared<BlinnPhongMaterial>(
        //            (std::string("Plain Material") + std::to_string(iScan)).c_str() );

        //        scan.m_material->m_perVertexColor = true;
        scan.m_material->m_ks = Utils::Color::Black();
        scan.m_material->setMaterialAspect( ScanMaterial::MaterialAspect::MAT_TRANSPARENT );
//        scan.m_material->setMaterialAspect( Material::MaterialAspect::MAT_OPAQUE );
//        scan.m_material->m_alpha = 0.5;
//        scan.m_material->m_color.x() = 0.25;
#ifndef USE_BLINN_PHONG_MATERIAL
        scan.m_material->m_pimp.x()  = 0.5;
        scan.m_material->m_pimp.y()  = 1.0;
        scan.m_material->m_nChannels = nChannels;
#endif
        //        scan.m_material->m_color.y() = 0.65;
        scan.m_material->addTexture( CurrentMaterial::TextureSemantic::TEX_DIFFUSE,
                                     textureParameters );
        scan.m_material->needUpdate();
        //        assert( scan.m_material->isTransparent() == true );

        //            mat->addTexture(PlainMaterial::TextureSemantic::TEX_COLOR, textureParameters);
        //            mat->addTexture(BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE,
        //            textureParameters);
        scan.m_quad->setMaterial( scan.m_material );
        if ( !m_isLiveStream ) {
            scan.m_quad->setTransparent( true );
            scan.m_material->m_isTransparent = true;
        }

        //        auto TLocal = Transform::Identity();
        //        TLocal.scale( Vector3( scanDepth / 2.0, 1.0, scanWidth / 2.0 ) );
        //        TLocal.translate( Vector3( 1.0, iScan * 100.0, 0.0 ) );
        //        TLocal.scale( 1000.0 );
        //        scan.m_quad->setLocalTransform( TLocal );
        scan.m_quad->setLocalTransform( m_transform );
        if ( !m_isLiveStream ) { scan.m_quad->setVisible( false ); }
        //        //                scan.m_quad->getRenderTechnique()->setConfiguration(
        //        shaderConfig );

        addRenderObject( scan.m_quad );

        m_viewer.makeCurrent();
        scan.m_textureScan = m_engine.getTextureManager()->getOrLoadTexture( textureParameters );
        m_viewer.doneCurrent();

        //        Ra::Core::Asset::ImageSpec imgSpec(width, height, 1);
        //        m_image = std::make_shared<Ra::Core::Asset::Image>(imgSpec, nullptr);
        //        m_textureScan->attachImage(m_image);
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

        //        const float lineRadius = 0.02 * scanDepth * 10;
        //        auto TLocal = Transform::Identity();
        //        TLocal.scale( Vector3( lineRadius, lineRadius, scanWidth ) );
        //        TLocal.translate( Vector3( 0.0, 0.0, -0.5 ) );
        //        TLocal.translate(Vector3(0_ra, 0_ra, -iScan * 5));
        //        TLocal.scale(1000.0);
        //        scan.m_scanLine->setLocalTransform( TLocal );
        scan.m_scanLine->setLocalTransform( m_transform );
        if ( !m_isLiveStream ) { scan.m_scanLine->setVisible( false ); }

        addRenderObject( scan.m_scanLine );
    }

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
    if ( !m_isLiveStream ) {
        for ( auto& scan : m_scans ) {
            scan.m_material->m_isTransparent = isTransparent;
            scan.m_material->needUpdate();
        }
    }
#endif
}
