
#include <minimalradium.hpp>

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

#include <Gui/Viewer/Viewer.hpp>
#include <Gui/BaseApplication.hpp>

#ifdef IO_USE_ASSIMP
#include <IO/AssimpLoader/AssimpFileLoader.hpp>
#endif

#include <random>

const std::string vertexShaderFile = PROJECT_DIR "applications/viewer/simpleRadiumViewer/vertexShader.glsl";
const std::string fragmentShaderFile = PROJECT_DIR "applications/viewer/simpleRadiumViewer/fragmentShader.glsl";

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

MinimalComponent::MinimalComponent(Ra::Engine::Scene::Entity* entity, MinimalApp* app)
    : Ra::Engine::Scene::Component("Minimal Component", entity)
    , m_app(app)
{
}

void updateCellCorner(Vector3& cellCorner, const Scalar cellSize, const int nCellX, const int)
{

    cellCorner[0] += cellSize;
    if (cellCorner[0] > cellSize * ((2 * nCellX - 1) / 4_ra)) {
        cellCorner[0] = -nCellX * cellSize / 2_ra;
        cellCorner[2] += cellSize;
    }
}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void MinimalComponent::initialize()
{
    auto blinnPhongMaterial = make_shared<BlinnPhongMaterial>("Shaded Material");
    blinnPhongMaterial->m_perVertexColor = true;
    blinnPhongMaterial->m_ks = Color::White();
    blinnPhongMaterial->m_ns = 100_ra;

    auto plainMaterial = make_shared<PlainMaterial>("Plain Material");
    plainMaterial->m_perVertexColor = true;

    auto lambertianMaterial = make_shared<LambertianMaterial>("Lambertian Material");
    lambertianMaterial->m_perVertexColor = true;

    //// setup ////
    Scalar colorBoost = 1_ra; /// since simple primitive are ambient only, boost their color
    Scalar cellSize = 1_ra;
    int nCellX = 10;
    int nCellY = 10;
    Vector3 cellCorner { -nCellX * cellSize / 2_ra, 0_ra, -nCellY * cellSize / 2_ra };
    Vector3 toCellCenter { cellSize / 2_ra, cellSize / 2_ra, cellSize / 2_ra };
    Scalar offset { 0.05_ra };
    Vector3 offsetVec { offset, offset, offset };
    std::random_device rd; // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    gen.seed(13371337);
    std::uniform_real_distribution<Scalar> dis015(0_ra, cellSize - 2_ra * offset);
    std::uniform_real_distribution<Scalar> dis01(0_ra, 1_ra);
    std::uniform_real_distribution<Scalar> dis11(-1_ra, 1_ra);
    std::uniform_int_distribution<uint> disInt(0, 128);
    uint circleGridSize = 8;
    uint numberOfSphere = 32;

    //// GRID ////
    {

        auto gridPrimitive = DrawPrimitives::Grid(Vector3::Zero(),
            Vector3::UnitX(),
            Vector3::UnitZ(),
            Color::Grey(0.6f),
            cellSize,
            nCellX);

        auto gridRo = RenderObject::createRenderObject(
            "test_grid", this, RenderObjectType::Geometry, gridPrimitive, {});
        gridRo->setMaterial(Ra::Core::make_shared<PlainMaterial>("Grid material"));
        gridRo->setPickable(false);
        addRenderObject(gridRo);
    }

    //    auto xLine = RenderObject::createRenderObject(
    //        "x_line",
    //        this,
    //        RenderObjectType::Geometry,
    //        //        DrawPrimitives::Line(Vector3 { 0_ra, 0_ra, 0_ra }, Vector3 { 1_ra, 0_ra, 0_ra }, Color::Red()),
    //        DrawPrimitives::Ray(Ray(Vector3 { 0_ra, 0_ra, 0_ra }, Vector3 { 1_ra, 0_ra, 0_ra }), Color::Red(), 1_ra),
    //        {});
    //    xLine->setMaterial(plainMaterial);
    //    addRenderObject(xLine);

    std::vector<std::shared_ptr<Engine::Data::Mesh>> meshAxis;

    // gizmo
    {
        constexpr Scalar arrowScale = 1_ra;
        constexpr Scalar axisWidth = .05_ra;
        constexpr Scalar arrowFrac = 0_ra;

        std::vector<Color> gizmoColors = { Color::Red(), Color::Green(), Color::Blue() };

        for (uint i = 0; i < 3; ++i) {
            Core::Vector3 cylinderEnd = Core::Vector3::Zero();
            cylinderEnd[i] = (1_ra - arrowFrac);
            Core::Vector3 arrowEnd = Core::Vector3::Zero();
            arrowEnd[i] = 1_ra;
            Core::Geometry::TriangleMesh cylinder = Core::Geometry::makeCylinder(
                Core::Vector3::Zero(), arrowScale * cylinderEnd, arrowScale * axisWidth / 2_ra, 32, gizmoColors[i]);
            //            Core::Geometry::TriangleMesh cone = Core::Geometry::makeCone(
            //                arrowScale * cylinderEnd, arrowScale * arrowEnd, arrowScale * arrowFrac / 2_ra, 32, gizmoColors[i]);
            //            cylinder.append(cone);
            //            cone.append(cylinder);
            //            triangleMeshAxis.push_back(cylinder);

            std::shared_ptr<Engine::Data::Mesh> mesh(
                new Engine::Data::Mesh("Translate Gizmo Arrow"));
            mesh->loadGeometry(std::move(cylinder));

            meshAxis.push_back(std::move(mesh));

            //            mesh->loadGeometry(std::move(cone));
        }

        for (uint i = 0; i < 3; ++i) {
            auto gizmo = RenderObject::createRenderObject("originAxis" + std::to_string(i), this, RenderObjectType::Geometry, meshAxis[i]);

            gizmo->setMaterial(plainMaterial);
            addRenderObject(gizmo);
        }
    }

    //// CUBES ////
    {
        std::shared_ptr<Mesh> cube1(new Mesh("Cube"));
        //        auto coord = cellSize / 8_ra;
        auto cubeSize = Vector3 { 1_ra, 1_ra, 1_ra };
        auto box = Core::Geometry::makeSharpBox(cubeSize * 0.7 / 2, Color::Grey());
        //        assert(ret);
        cube1->loadGeometry(std::move(box));

        auto renderObject1 = RenderObject::createRenderObject(
            "refCube", this, RenderObjectType::Geometry, cube1, {});
        //        renderObject1->setLocalTransform(Transform { Translation(cellCorner) });
        renderObject1->setMaterial(lambertianMaterial);
        //        renderObject1->setMaterial(blinnPhongMaterial);
        addRenderObject(renderObject1);

        // another cube
        std::shared_ptr<Mesh> cube2(new Mesh("Cube"));
        //        coord = cellSize / 4_ra;
        //        coord = Vector3 {0_ra, 2_ra, 0_ra};
        cube2->loadGeometry(Geometry::makeSharpBox(cubeSize * 0.2 / 2));
        cube2->getCoreGeometry().addAttrib(
            "in_color", Vector4Array { cube2->getNumVertices(), Color::Grey() });
        //        cube2->setAttribNameCorrespondance("colour", "in_color");

        auto renderObject2 = RenderObject::createRenderObject(
            "probeCube", this, RenderObjectType::Geometry, cube2, {});
        //        coord = cellSize / 2_ra;
        renderObject2->setLocalTransform(
            Transform { Translation(Vector3(0_ra, 2_ra, 0_ra)) });
        //                renderObject2->setMaterial(lambertianMaterial);
        renderObject2->setMaterial(blinnPhongMaterial);
        addRenderObject(renderObject2);

        for (uint i = 0; i < 3; ++i) {
            auto gizmo = RenderObject::createRenderObject("probeAxis" + std::to_string(i), this, RenderObjectType::Geometry, meshAxis[i]);
            gizmo->setMaterial(plainMaterial);
            addRenderObject(gizmo);
            gizmo->setLocalTransform(
                Transform { Translation(Vector3(0_ra, 2_ra, 0_ra)) });
            //            axis.push_back(gizmo);
        }
    }

    // scan plane
    {
        auto quadTriangle = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });
        Ra::Core::Vector3Array tex_coords;
        tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
        tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
        tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
        tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
        quadTriangle.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD), tex_coords);

        //! [Creating a texture for the quad]
        //        unsigned char data[192 * 512];
        unsigned char* data = new unsigned char[192 * 512];
        // fill with some function
        for (int i = 0; i < 192; ++i) {
            for (int j = 0; j < 512; j++) {
                if (std::abs(i - 20) < 3 || std::abs(j - 20) < 3) {
                    data[(i * 512 + j)] = 0;
                } else {

                    data[(i * 512 + j)] = (j / 2) % 256;
                }
            }
        }
        auto& textureParameters = m_app->m_engine->getTextureManager()->addTexture("myTexture", 512, 192, data);
        textureParameters.format = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the quad]

        std::shared_ptr<Engine::Data::Mesh> meshQuad(
            new Engine::Data::Mesh("Scan plane"));
        meshQuad->loadGeometry(std::move(quadTriangle));

        RenderTechnique renderTechnique = RenderTechnique::createDefaultRenderTechnique();
        Ra::Engine::Data::ShaderConfiguration shaderConfig("myShader", vertexShaderFile, fragmentShaderFile);
        renderTechnique.setConfiguration(shaderConfig);
        auto quad = RenderObject::createRenderObject("echoPlane", this, RenderObjectType::Geometry, meshQuad, renderTechnique);

        //        auto myMat = make_shared<BlinnPhongMaterial>("Shaded Material");
        Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
        // uncomment this to remove glossy highlight
        matData.m_specular = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular = true;
        matData.m_hasTexDiffuse = true;
        matData.m_texDiffuse = "myTexture";
        //        quad->setMaterial(matData);

        std::shared_ptr<Ra::Engine::Data::Material> roMaterial;
        auto converter = Ra::Engine::Data::EngineMaterialConverters::getMaterialConverter(matData.getType());
        auto mat = converter.second(&matData);
        roMaterial.reset(mat);

        roMaterial->needUpdate();
        quad->setMaterial(roMaterial);

        //        quad->getRenderTechnique()->setConfiguration(shaderConfig);

        addRenderObject(quad);
    }
}

/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
MinimalSystem::MinimalSystem(MinimalApp* app)
    : m_app(app)
{
}

void MinimalSystem::generateTasks(Ra::Core::TaskQueue*, const Ra::Engine::FrameInfo&)
{
    std::cout << "[MinimalSystem] generateTasks" << std::endl;

    // update texture
    {
        Ra::Engine::Data::TextureParameters textureParameters;
        textureParameters.name = "myTexture";
        auto texture = m_app->m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
        auto& params = texture->getParameters();
        unsigned char* data = new unsigned char[192 * 512];
        for (int i = 0; i < 192; ++i) {
            for (int j = 0; j < 512; j++) {
                if (std::abs(i - 20) < 3 || std::abs(j - 20) < 3) {
                    data[(i * 512 + j)] = 0;
                } else {

                    data[(i * 512 + j)] = (j / 2) % 256;
                }
            }
        }
        memcpy(params.texels, data, 192 * 512);
        //            memcpy(params.texels, scanAcq.mData, 192 * 512);

        m_app->m_viewer->makeCurrent();
        texture->initializeGL(false);
        m_app->m_viewer->doneCurrent();
    }
}

void MinimalSystem::addComponent(Ra::Engine::Scene::Entity* ent, MinimalComponent* comp)
{
    registerComponent(ent, comp);
}
