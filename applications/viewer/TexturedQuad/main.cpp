// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
#include <Core/Asset/BlinnPhongMaterialData.hpp>
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Data/TextureManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <Engine/Rendering/RenderObjectManager.hpp>
#include <QTimer>

#include <DicomLoader/DicomUtils.hpp>

#include <filesystem>

const char* vertexShaderFile   = PROJECT_DIR "applications/viewer/TexturedQuad/vertexShader.glsl";
const char* fragmentShaderFile = PROJECT_DIR "applications/viewer/TexturedQuad/fragmentShader.glsl";

//#define MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/"

int main( int argc, char* argv[] ) {
    //! [Creating the application]
    Ra::Gui::BaseApplication app( argc, argv );
    app.initialize( Ra::Gui::SimpleWindowFactory {} );
    //! [Creating the application]

    //! [Creating a quad geometry with texture coordinates]
#ifdef USE_GOT_PR
    auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
#else
    auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {} );
    Ra::Core::Vector3Array tex_coords;
    tex_coords.push_back( { 0_ra, 0_ra, 0_ra } );
    tex_coords.push_back( { 1_ra, 0_ra, 0_ra } );
    tex_coords.push_back( { 0_ra, 1_ra, 0_ra } );
    tex_coords.push_back( { 1_ra, 1_ra, 0_ra } );
    quad.addAttrib(
        Ra::Engine::Data::Mesh::getAttribName( Ra::Engine::Data::Mesh::VERTEX_TEXCOORD ),
        tex_coords );
    //! [Creating a quad geometry with texture coordinates]
#endif

    unsigned int imageWidth, imageHeight, nImages, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;


    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";

    auto volumeData = Ra::IO::DICOM::readDicomVolume( filename,
                                                      &imageWidth,
                                                      &imageHeight,
                                                      &nImages,
                                                      &bytePerVoxel,
                                                      &pixelSpacingWidth,
                                                      &pixelSpacingHeight,
                                                      &sliceThickness );

    assert( imageWidth == 512 );
    assert( imageHeight == 512 );
    assert( bytePerVoxel == 2 );

    //! [Creating a texture]
    // fill with some function
    auto& myTexture =
        app.m_engine->getTextureManager()->addTexture( "myTexture", 512, 512, volumeData );
    // these values will be used when engine initialize texture GL representation.
    myTexture.format = gl::GLenum::GL_RG;
    myTexture.internalFormat = gl::GLenum::GL_RG;
    //! [Creating a texture]

    //! [Create an entity and component to draw or data]
    auto e = app.m_engine->getEntityManager()->createEntity( "Textured quad" );

    Ra::Core::Asset::BlinnPhongMaterialData matData( "myMaterialData" );
    matData.m_hasTexDiffuse = true;
    // this name has to be the same as texManager added texture name
    matData.m_texDiffuse = "myTexture";

    // the entity get's this new component ownership. a bit wired since hidden in ctor.
    new Ra::Engine::Scene::TriangleMeshComponent( "Quad Mesh", e, std::move( quad ), &matData );

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    const auto& roManager = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    const auto nObjects   = roManager->getRenderObjectsCount();
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
        nObjects - 1 );
    auto renderTechnique = ro->getRenderTechnique();
    Ra::Engine::Data::ShaderConfiguration shaderConfig(
        "myShader", vertexShaderFile, fragmentShaderFile );
    renderTechnique->setConfiguration( shaderConfig );

    return app.exec();
}
