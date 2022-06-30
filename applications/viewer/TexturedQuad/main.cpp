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

const char* vertexShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const char* fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";

#define MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/"

int main(int argc, char* argv[])
{
    //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]

    //! [Creating a quad geometry with texture coordinates]
    //    auto quad = Ra::Core::Geometry::makeZNormalQuad( { 1_ra, 1_ra }, {}, true );
    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra }, {});
    Ra::Core::Vector3Array tex_coords;
    tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
    tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
    tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
    tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
    quad.addAttrib(
        Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD),
        tex_coords);
    //! [Creating a quad geometry with texture coordinates]

    unsigned int imageWidth, imageHeight, nImages, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;

    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
    //    app.loadFile(QString(MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm"));
    //    app.loadFile(QString(MRI_PATH "Loc/D0010001.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_AX_T2/D0010551.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_Coro_T2/D0010710.dcm"));
    //    app.loadFile(QString(MRI_PATH "MPR_Sag_T2/D0010652.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_ABDO/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_AP/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_AXIAL_TAP/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "PACS_PARENCHYME/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "RapportDose/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "Sag_CUBE_T2/D0010031.dcm"));
    //    app.loadFile(QString(MRI_PATH "Scouts/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "ScreenSave/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "SerieSmartPrep/IM0001.dcm"));
    //    app.loadFile(QString(MRI_PATH "WATER:AX_LAVA-Flex/D0010333.dcm"));

    auto volumeData = Ra::IO::DICOM::readDicomVolume(filename,
        &imageWidth,
        &imageHeight,
        &nImages,
        &bytePerVoxel,
        &pixelSpacingWidth,
        &pixelSpacingHeight,
        &sliceThickness);

    assert(imageWidth == 512);
    assert(imageHeight == 512);
    assert(bytePerVoxel == 2);

    //! [Creating a texture]
    //    unsigned char data[512 * 512 * 2];
    // fill with some function
    //    for (int i = 0; i < 192; ++i) {
    //        for (int j = 0; j < 512; j++) {
    //            data[(i * 512 + j)] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
    //        }
    //    }
    auto& myTexture = app.m_engine->getTextureManager()->addTexture("myTexture", 512, 512, volumeData);
    // these values will be used when engine initialize texture GL representation.
//    myTexture.format = gl::GLenum::GL_RED;
    myTexture.format = gl::GLenum::GL_RED;
    myTexture.internalFormat = gl::GLenum::GL_R16;
//    myTexture.internalFormat = gl::GLenum::GL_R8;
    //! [Creating a texture]

    //! [Create an entity and component to draw or data]
    auto e = app.m_engine->getEntityManager()->createEntity("Textured quad");

    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
    matData.m_hasTexDiffuse = true;
    // this name has to be the same as texManager added texture name
    matData.m_texDiffuse = "myTexture";

    // the entity get's this new component ownership. a bit wired since hidden in ctor.
    new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]


    const auto& roManager = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    const auto nObjects = roManager->getRenderObjectsCount();
    //    for (int i = 0; i < nObjects; ++i) {
    //        std::cout << roManager->getRenderObject(i)->getName() << std::endl;
    //    }
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(nObjects - 1);
    //    ro->setVisible(false);
    auto renderTechnique = ro->getRenderTechnique();
    Ra::Engine::Data::ShaderConfiguration shaderConfig(
        "myShader", vertexShaderFile, fragmentShaderFile);
    renderTechnique->setConfiguration(shaderConfig);


    return app.exec();
}
