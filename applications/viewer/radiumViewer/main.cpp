
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
#include <Gui/Viewer/Viewer.hpp>

#include <QTimer>

#include <Engine/Rendering/RenderObjectManager.hpp>
#include <fstream>
#include <stream.h>

int main(int argc, char* argv[])
{
    //    InputStream scanStream("ULA-OP 256");
    //    InputStream posStream("Polhemus Patriot (probe)", "ULA-OP 256");
    InputStream posStream("Polhemus Patriot (probe)");

    //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]

    //! [Creating the quad]
    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });
    //! [Creating the quad]
    // [Add missing texture coordonates for to the quad]
    Ra::Core::Vector3Array tex_coords;
    tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
    tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
    tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
    tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
    quad.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD), tex_coords);
    // [Add missing texture coordonates for to the quad]

    //! [Creating a texture for the quad]
    unsigned char data[192 * 512];
    // fill with some function
    for (int i = 0; i < 192; ++i) {
        for (int j = 0; j < 512; j++) {
            data[(i * 512 + j)] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
        }
    }
    auto& myTexture = app.m_engine->getTextureManager()->addTexture("myTexture", 512, 192, data);
    myTexture.format = gl::GLenum::GL_RED;
    myTexture.internalFormat = gl::GLenum::GL_R8;
    //! [Creating a texture for the quad]

    //! [create the quad material]
    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
    // uncomment this to remove glossy highlight
    matData.m_specular = Ra::Core::Utils::Color::Black();
    matData.m_hasSpecular = true;
    matData.m_hasTexDiffuse = true;
    matData.m_texDiffuse = "myTexture";
    //! [create the quad material]

    //! [Create the engine entity for the quad]
    auto e = app.m_engine->getEntityManager()->createEntity("Textured quad");
    //! [Create the engine entity for the quad]

    //! [Create a geometry component with the quad]
    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);
    //! [Create a geometry component with the quad]

    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = app.m_engine->getSystem("GeometrySystem");
    geometrySystem->addComponent(e, c);
    //! [Register the entity/component association to the geometry system ]

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

    //![get the renderobject for further edition]
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(c->m_renderObjects[0]);

    //    // get scans
    //    std::fstream posFile(PROJECT_DIR "records/latest/Polhemus Patriot (probe).txt", std::ios::binary | std::ios::in);
    //    assert(posFile.is_open());
    //    const std::vector<Stream::Acquisition>& poses = InputStream(posFile).getAllAcquisition();
    //    posFile.close();

    //    std::fstream scanFile(PROJECT_DIR "records/latest/ULA-OP 256.txt", std::ios::binary | std::ios::in);
    //    assert(scanFile.is_open());
    //    const std::vector<Stream::Acquisition>& scans = InputStream(scanFile).getAllAcquisition();
    //    scanFile.close();
    //    assert(poses.size() == scans.size());
    //    assert(!poses.empty());
    //    std::cout << "nb acqs = " << scans.size() << std::endl;
    //    //        std::thread thread([=]() {

    auto close_timer = new QTimer(&app);
    close_timer->setInterval(25);
    int iAcquisition = 0;
    //    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &poses, &scans, &ro]() {
    //    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &scanStream, &posStream, &ro]() {
    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &posStream, &ro]() {
        std::cout << "update acquisition " << iAcquisition << std::endl;

        // update texture
//        {
//            Stream::Acquisition scanAcq;
//            scanStream >> scanAcq;
//            //            Ra::Engine::Data::TextureParameters textureParameters;
//            //            textureParameters.name = "myTexture";
//            //            auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
//            Ra::Engine::Data::TextureParameters textureParameters;
//            textureParameters.name = "myTexture";
//            auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
//            auto& params = texture->getParameters();
//            //                    params.texels = scans[iAcquisition].mData;
//            //        memcpy(params.texels, scans[iAcquisition].mData, 192 * 512);
//            memcpy(params.texels, scanAcq.mData, 192 * 512);
//            //            updateTexel(params.texels, dec);
//            app.m_mainWindow->getViewer()->makeCurrent();
//            texture->initializeGL(false);
//            app.m_mainWindow->getViewer()->doneCurrent();
//        }

        // update position and orientation
        {
            Stream::Acquisition posAcq;
            posStream >> posAcq;
            float* translation = (float*)posAcq.mData;
            float* quaternion = (float*)&posAcq.mData[12];
            //                std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
            //                std::cout << str << std::endl;

            Ra::Core::Transform T = Ra::Core::Transform::Identity();
            //                Ra::Core::Vector3 vec(1.0, 1.0, 1.0);

            Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
            T.rotate(quat);

            Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
            T.scale(vecScale);

            Ra::Core::Vector3 vec(translation[0], translation[1], translation[2]);
            vec /= 10.0;
            T.translate(vec);
            ro->setLocalTransform(T);
        }

        //            std::cout << "update data with dec = " << dec << std::endl;
        ++iAcquisition;
        //        if (iAcquisition == poses.size())
        //            iAcquisition = 0;
    });
    close_timer->start();

    return app.exec();
}

//    std::thread thread([&app, &scans, &poses, &texture]() {
//        //    changeTimer->start();

//        std::cout << "start playing" << std::endl;
//        while (true) {
//            const auto startRecord = poses[0].mBackendTimestamp;
//            const auto& startChrono = std::chrono::high_resolution_clock::now();

//            for (int i = 0; i < poses.size(); ++i) {
//                std::this_thread::sleep_until(startChrono + std::chrono::microseconds(poses[i].mBackendTimestamp - startRecord));
//                //            auto end = startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord);
//                //            while (std::chrono::high_resolution_clock::now() < end)
//                //                ; // wait

//                float* translation = (float*)poses[i].mData;
//                float* quaternion = (float*)&poses[i].mData[12];
//                //                std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
//                //                std::cout << str << std::endl;

//                Ra::Core::Transform T = Ra::Core::Transform::Identity();
//                //                Ra::Core::Vector3 vec(1.0, 1.0, 1.0);
//                Ra::Core::Vector3 vecScale(0.1, 1.0, 1.0);
//                T.scale(vecScale);

//                Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
//                //                Ra::Core::Quaternion quat(quaternion);
//                T.rotate(quat);

//                Ra::Core::Vector3 vec(translation[0] / 10.0, translation[1] / 10.0, translation[2] / 10.0);
//                T.translate(vec);
//                //                ro->setLocalTransform(T);

////                Ra::Engine::Data::TextureParameters textureParameters;
////                textureParameters.name = "myTexture";
////                auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
//                                auto& params = texture->getParameters();
////                        updateTexel(params.texels, dec);
////                                params.texels = scans[i].mData;
//                //                memcpy(params.texels, scans[i].mData, 192 * 512);
//                                app.m_mainWindow->getViewer()->makeCurrent();
//                                texture->initializeGL(false);
//                                app.m_mainWindow->getViewer()->doneCurrent();
//            }
//            std::cout << "end record, auto loop" << std::endl;
//        }
//    });
//    thread.detach();

//// This should be enough if you create your Blinn-Phong material after that as you do and give it to the component creation method.
//// But if you need some more sophisticated texture management (e.g. update texels data each frame or something like that) without changing
//// the whole material, you might request the registered texture from the manager using getOrLoadTexture to have a direct access to the texture
//// you associated with your material (and then changing its texels using Texture::updateData)

//// Include Radium base application and its simple Gui
//#include <Gui/BaseApplication.hpp>
//#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

//// include the Engine/entity/component interface
//#include <Core/Asset/BlinnPhongMaterialData.hpp>
//#include <Core/Geometry/MeshPrimitives.hpp>
//#include <Engine/Data/TextureManager.hpp>
//#include <Engine/Scene/EntityManager.hpp>
//#include <Engine/Scene/GeometryComponent.hpp>
//#include <Engine/Scene/GeometrySystem.hpp>

////#include <Engine/RadiumEngine.hpp>
//#include <Engine/Data/RawShaderMaterial.hpp>
//#include <Engine/Rendering/RenderObjectManager.hpp>

//#include <QTimer>

//#include <Engine/Data/PlainMaterial.hpp>

///**
// * Demonstrate the usage of RawShaderMaterial functionalities
// */
//// Vertex shader source code
//const std::string _vertexShaderSource { "#include \"TransformStructs.glsl\"\n"
//                                        "layout (location = 0) in vec3 in_position;\n"
//                                        "layout (location = 4) in vec3 in_texcoord;\n"
//                                        "layout (location = 0) out vec3 out_pos;\n"
//                                        "layout (location = 2) out vec3 out_texcoord;\n"
//                                        "uniform Transform transform;\n"
//                                        "void main(void)\n"
//                                        "{\n"
//                                        "    mat4 mvp    = transform.proj * transform.view * transform.model;\n"
//                                        "    out_pos     = in_position;\n"
//                                        "    gl_Position = mvp*vec4(in_position.xyz, 1.0);\n"
//                                        "    out_texcoord = in_texcoord;\n"
//                                        "}\n" };
//// Fragment shader source code
//const std::string _fragmentShaderSource {
//    "#include \"BlinnPhong.glsl\"\n"
//    "#include \"VertexAttribInterface.frag.glsl\"\n"
//    "layout (location = 0) out vec4 out_color;\n"
//    "uniform vec4 aColorUniform;\n"
//    "uniform float aScalarUniform;\n"
//    "void main(void)\n"
//    "{\n"
//    "   out_color =  vec4(texture(material.tex.kd, in_texcoord.xy).b, 0.0, 0.0, 1.0);\n"
//    "   // out_color =  vec4(in_texcoord.xy, 1.0, 1.0);\n"
//    "   // out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
//    "}\n"
//};

//const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
//    { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, _vertexShaderSource },
//    { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource }
//};

//class MyParameterProvider : public Ra::Engine::Data::ShaderParameterProvider {
//public:
//    MyParameterProvider() { }
//    ~MyParameterProvider() { }
//    void updateGL() override
//    {
//        // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
//        // The name of the parameter corresponds to the shader's uniform name.
//        m_renderParameters.addParameter("aColorUniform", m_colorParameter);
//        m_renderParameters.addParameter("aScalarUniform", m_scalarParameter);
//        //        m_renderParameters.addParameter("material.tex.kd", m_texture);
//        //        m_renderParameters.addParameter("material.tex.hasKd", 1);
//    }
//    void setOrComputeTheParameterValues()
//    {
//        // client side computation of the parameters, e.g.
//        m_colorParameter = Ra::Core::Utils::Color::Red();
//        m_scalarParameter = .5_ra;
//        m_renderParameters.addParameter("material.tex.kd", m_texture);
//        m_renderParameters.addParameter("material.tex.hasKd", 1);
//    }

//    // private:
//    Ra::Core::Utils::Color m_colorParameter { Ra::Core::Utils::Color::Green() };
//    Scalar m_scalarParameter { 1 };
//    Ra::Engine::Data::Texture* m_texture = nullptr;
//};

//#include <Engine/Data/LambertianMaterial.hpp>

//using namespace Ra;
//using namespace Ra::Core;
//using namespace Ra::Core::Utils;
//using namespace Ra::Core::Geometry;
//using namespace Ra::Engine;
//using namespace Ra::Engine::Rendering;
//using namespace Ra::Engine::Data;
//using namespace Ra::Engine::Scene;

///// This is a very basic component which holds a spinning cube.
//struct MinimalComponent : public Ra::Engine::Scene::Component {

//    explicit MinimalComponent(Ra::Engine::Scene::Entity* entity, TextureParameters* texture);

//    /// This function is called when the component is properly
//    /// setup, i.e. it has an entity.
//    void initialize() override;

//    TextureParameters* m_texture = nullptr;
//};

//MinimalComponent::MinimalComponent(Ra::Engine::Scene::Entity* entity, TextureParameters* texture)
//    : Ra::Engine::Scene::Component("Minimal Component", entity)
//    , m_texture(texture)
//{
//}

///// This function is called when the component is properly
///// setup, i.e. it has an entity.
//void MinimalComponent::initialize()
//{
//    auto blinnPhongMaterial = make_shared<BlinnPhongMaterial>("Shaded Material");
//    //    blinnPhongMaterial->m_perVertexColor = true;
//    //    blinnPhongMaterial->m_ks = Color::White();
//    //    blinnPhongMaterial->m_ns = 100_ra;

//    auto plainMaterial = make_shared<PlainMaterial>("Plain Material");
//    //    plainMaterial->m_perVertexColor = true;

//    auto lambertianMaterial = make_shared<LambertianMaterial>("Lambertian Material");
//    //    lambertianMaterial->m_perVertexColor = true;

//    //// setup ////
//    Scalar colorBoost = 1_ra; /// since simple primitive are ambient only, boost their color
//    Scalar cellSize = 0.35_ra;
//    int nCellX = 7;
//    int nCellY = 7;
//    Vector3 cellCorner { -nCellX * cellSize / 2_ra, 0_ra, -nCellY * cellSize / 2_ra };
//    Vector3 toCellCenter { cellSize / 2_ra, cellSize / 2_ra, cellSize / 2_ra };
//    Scalar offset { 0.05_ra };
//    Vector3 offsetVec { offset, offset, offset };
//    std::random_device rd; // Will be used to obtain a seed for the random number engine
//    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
//    gen.seed(13371337);
//    std::uniform_real_distribution<Scalar> dis015(0_ra, cellSize - 2_ra * offset);
//    std::uniform_real_distribution<Scalar> dis01(0_ra, 1_ra);
//    std::uniform_real_distribution<Scalar> dis11(-1_ra, 1_ra);
//    std::uniform_int_distribution<uint> disInt(0, 128);
//    uint circleGridSize = 8;
//    uint numberOfSphere = 32;

//    //// CUBES ////
//    std::shared_ptr<Mesh> cube1(new Mesh("Cube"));
//    auto coord = cellSize / 8_ra;
//    cube1->loadGeometry(Geometry::makeSharpBox(Vector3 { coord, coord, coord }));
//    cube1->getCoreGeometry().addAttrib(
//        "in_color", Vector4Array { cube1->getNumVertices(), Color::Green() });

//    auto renderObject1 = RenderObject::createRenderObject(
//        "Cube1", this, RenderObjectType::Geometry, cube1, {});
//    renderObject1->setLocalTransform(Transform { Translation(cellCorner) });
//    renderObject1->setMaterial(blinnPhongMaterial);
//    addRenderObject(renderObject1);

//    // quad
//    std::shared_ptr<Mesh> quad(new Mesh("Quad"));
//    coord = cellSize / 4_ra;
//    //    quad->loadGeometry(Geometry::makeSharpBox(Vector3 { coord, coord, coord }));
//    quad->loadGeometry(Geometry::makeZNormalQuad({ 1_ra, 1_ra }));
//    Ra::Core::Vector3Array tex_coords;
//    tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
//    tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
//    tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
//    tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
//    quad->addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD), tex_coords);
//    //    quad->getCoreGeometry().addAttrib(
//    //        "colour", Vector4Array { quad->getNumVertices(), Color::Red() });

//    //    blinnPhongMaterial->addTexture(BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, m_texture);
//    //    blinnPhongMaterial->addTexture(BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, *m_texture);
//    //    auto builder = EngineRenderTechniques::getDefaultTechnique( m_paint->getMaterialName() );
//    //      auto rt      = Core::make_shared<RenderTechnique>();
//    //      // define the technique for rendering this RenderObject (here, using the default from Material
//    //      // name)
//    //      builder.second( *rt, m_paint->isTransparent() );
//    //      rt->setParametersProvider( m_paint );
//    //      ro->setRenderTechnique( rt );

//    //    quad->setAttribNameCorrespondance("colour", "in_color");
//    //    std::shared_ptr<Data::ShaderConfiguration> shaderConf(new Data::ShaderConfiguration("BlinnCopy", _vertexShaderSource, _fragmentShaderSource));
//    //    std::shared_ptr<RenderTechnique> renderTechnique;
//    //    renderTechnique->setConfiguration(*shaderConf);

//    auto renderObject2 = RenderObject::createRenderObject(
//        "Quad", this, RenderObjectType::Geometry, quad, {});
//    coord = cellSize / 2_ra;
//    renderObject2->setLocalTransform(
//        Transform { Translation(cellCorner + Vector3(coord, coord, coord)) });
//    //    lambertianMaterial->addTexture(LambertianMaterial::TextureSemantic::TEX_COLOR, *m_texture);
//    //    renderObject2->setMaterial(lambertianMaterial);
//    plainMaterial->addTexture(PlainMaterial::TextureSemantic::TEX_COLOR, *m_texture);
//    renderObject2->setMaterial(plainMaterial);
//    //    renderObject2->setMaterial(blinnPhongMaterial);
//    addRenderObject(renderObject2);
//}

//int main(int argc, char* argv[])
//{

//    // Create app and show viewer window
//    Gui::BaseApplication app(argc, argv);
//    app.initialize(Ra::Gui::SimpleWindowFactory {});

//    //! [Creating a texture for the quad]
//    constexpr int width = 192;
//    constexpr int height = 512;
//    constexpr int nbComponent = 3;
//    unsigned char data[width * height * nbComponent];
//    // fill with some function
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; ++j) {
//            //            for (int k = 0; k < nbComponent; ++k) {
//            //            data[(i * height + j) * nbComponent + 0] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
//            //            data[(i * height + j) * nbComponent + 1] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
//            //            data[(i * height + j) * nbComponent + 2] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
//            data[(i * height + j) * nbComponent + 0] = (i * 2) % 255;
//            data[(i * height + j) * nbComponent + 1] = (i * 2) % 255;
//            data[(i * height + j) * nbComponent + 2] = (i * 2) % 255;
//            //            data[(i * height + j)] = i % 255;
//            //                        data[(i * height + j) * nbComponent + 1] = 0;
//            //                        data[(i * height + j) * nbComponent + 2] = 0;
//            //            }
//        }
//    }
//    const auto& textureManager = app.m_engine->getTextureManager();
//    auto& textureParameters = textureManager->addTexture("myTexture", 192, 512, data);
////    textureParameters.format = gl::GLenum::GL_RED;
////    textureParameters.internalFormat = gl::GLenum::GL_R8;
//    //! [Creating a texture for the quad]
////    auto* texture = textureManager->getOrLoadTexture(textureParameters);
//    auto* texture = textureManager->getOrLoadTexture(textureParameters);
//    assert(texture != nullptr);
//    texture->updateData(data);

//    // Create and initialize entity and component
//    Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity("Cube");
//    MinimalComponent* c = new MinimalComponent(e, &textureParameters);
//    auto geometrySystem = app.m_engine->getSystem("GeometrySystem");
//    geometrySystem->addComponent(e, c);
//    c->initialize();

//    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
//    app.m_mainWindow->prepareDisplay();

////    int dec = 127;
////    for (int i = 0; i < width; ++i) {
////        for (int j = 0; j < height; ++j) {
////            //            for (int k = 0; k < nbComponent; ++k) {
////            //            data[(i * height + j) * nbComponent + 0] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
////            //            data[(i * height + j) * nbComponent + 1] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
////            //            data[(i * height + j) * nbComponent + 2] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
////            data[(i * height + j) * nbComponent + 0] = 255;
////            data[(i * height + j) * nbComponent + 1] = 0;
////            data[(i * height + j) * nbComponent + 2] = 0;
////            //            data[(i * height + j)] = i % 255;
////            //                        data[(i * height + j) * nbComponent + 1] = 0;
////            //                        data[(i * height + j) * nbComponent + 2] = 0;
////            //            }
////        }
////    }

//    return app.exec();
//}

// int main(int argc, char* argv[])
//{
//     //! [Creating the application]
//     Ra::Gui::BaseApplication app(argc, argv);
//     app.initialize(Ra::Gui::SimpleWindowFactory {});
//     //! [Creating the application]

//    //! [add the custom material to the material system]
//    Ra::Engine::Data::RawShaderMaterial::registerMaterial();

//    //! [Creating the quad]
//    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });
//    //! [Creating the quad]
//    // [Add missing texture coordonates for to the quad]
//    Ra::Core::Vector3Array tex_coords;
//    tex_coords.push_back({ 0_ra, 0_ra, 0_ra });
//    tex_coords.push_back({ 1_ra, 0_ra, 0_ra });
//    tex_coords.push_back({ 0_ra, 1_ra, 0_ra });
//    tex_coords.push_back({ 1_ra, 1_ra, 0_ra });
//    quad.addAttrib(Ra::Engine::Data::Mesh::getAttribName(Ra::Engine::Data::Mesh::VERTEX_TEXCOORD), tex_coords);
//    // [Add missing texture coordonates for to the quad]

//    //! [Creating a texture for the quad]
//    constexpr int width = 192;
//    constexpr int height = 512;
//    constexpr int nbComponent = 3;
//    unsigned char data[width * height * nbComponent];
//    // fill with some function
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; j++) {
//            //            for (int k = 0; k < nbComponent; ++k) {
//            //                data[(i * height + j) * nbComponent + 0] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
//            data[(i * height + j) * nbComponent + 0] = 255;
//            data[(i * height + j) * nbComponent + 1] = 0;
//            data[(i * height + j) * nbComponent + 2] = 0;
//            //            }
//        }
//    }
//    const auto& textureManager = app.m_engine->getTextureManager();
//    auto& myTexture = textureManager->addTexture("myTexture", 192, 512, data);
//    //    myTexture.format = gl::GLenum::GL_RED;
//    //    myTexture.internalFormat = gl::GLenum::GL_R8;
//    //! [Creating a texture for the quad]

//    //    //! [create the quad material]
//    //    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
//    //    // uncomment this to remove glossy highlight
//    //    matData.m_specular = Ra::Core::Utils::Color::Black();
//    //    matData.m_hasSpecular = true;
//    //    matData.m_hasTexDiffuse = true;
//    //    matData.m_texDiffuse = "myTexture";
//    //    //! [create the quad material]
//    //    Ra::Engine::Data::TextureManager::getOrLoadTexture(myTexture, );

//    //! [Create Parameter provider for the shader]
//    auto paramProvider = std::make_shared<MyParameterProvider>();
//    const auto& texture = textureManager->getOrLoadTexture(myTexture, true);
//    assert(texture != nullptr);
//    paramProvider->m_texture = texture;
//    paramProvider->setOrComputeTheParameterValues();

//    Ra::Core::Asset::RawShaderMaterialData matData("myMaterialData", _config1, paramProvider);

//    //! [Create the engine entity for the quad]
//    auto e = app.m_engine->getEntityManager()->createEntity("Textured quad");
//    //! [Create the engine entity for the quad]

//    //! [Create a geometry component with the quad]
//    //    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);
//    auto c = Ra::Engine::Scene::TriangleMeshComponent("myMeshComponent", e, std::move(quad));

//    //    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), nullptr);
//    //! [Create a geometry component with the quad]

//    //! [Register the entity/component association to the geometry system ]
//    auto geometrySystem = app.m_engine->getSystem("GeometrySystem");
//    geometrySystem->addComponent(e, &c);
//    //! [Register the entity/component association to the geometry system ]

//    //! [Tell the window that something is to be displayed]
//    app.m_mainWindow->prepareDisplay();
//    //! [Tell the window that something is to be displayed]

//    // terminate the app after 4 second (approximatively). Camera can be moved using mouse moves.
//    //    auto close_timer = new QTimer( &app );
//    //    close_timer->setInterval( 4000 );
//    //    QObject::connect( close_timer, &QTimer::timeout, [&app]() { app.appNeedsToQuit(); } );
//    //    close_timer->start();

//    //    Ra::Engine::Data::PlainMaterial mat("myMaterialData");
//    //    Ra::Core::Asset::M
//    //    mat.addTexture(Ra::Engine::Data::PlainMaterial::TextureSemantic::TEX_COLOR, myTexture);

//    //    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
//    //        c->m_renderObjects[0]);

//    //    ro->setMaterial(std::make_shared<Ra::Engine::Data::PlainMaterial>(mat));

//    //    ////    auto matPtr = std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(mat);
//    //    //    ro->setMaterial(std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(*mat));

//    return app.exec();
//}

//// Include Radium base application and its simple Gui
//#include <Gui/BaseApplication.hpp>
//#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

//// include the Engine/entity/component interface
//#include <Core/Geometry/MeshPrimitives.hpp>
//#include <Engine/Rendering/RenderObjectManager.hpp>
//#include <Engine/Scene/EntityManager.hpp>
//#include <Engine/Scene/GeometryComponent.hpp>
//#include <Engine/Scene/GeometrySystem.hpp>

//// include the custom material definition
//#include <Engine/Data/BlinnPhongMaterial.hpp>
//#include <Engine/Data/RawShaderMaterial.hpp>

//// include the Viewer to demonstrate dynamic edition of materials
//#include <Gui/Viewer/Viewer.hpp>

//// Qt
//#include <QTimer>

//#include <Core/Asset/BlinnPhongMaterialData.hpp>

///**
// * Demonstrate the usage of RawShaderMaterial functionalities
// */
//// Vertex shader source code
// const std::string _vertexShaderSource { "#include \"TransformStructs.glsl\"\n"
//                                         "layout (location = 0) in vec3 in_position;\n"
//                                         "layout (location = 4) in vec3 in_texcoord;\n"
//                                         "layout (location = 0) out vec3 out_pos;\n"
//                                         "layout (location = 2) out vec3 out_texcoord;\n"
//                                         "uniform Transform transform;\n"
//                                         "void main(void)\n"
//                                         "{\n"
//                                         "    mat4 mvp    = transform.proj * transform.view * transform.model;\n"
//                                         "    out_pos     = in_position;\n"
//                                         "    gl_Position = mvp*vec4(in_position.xyz, 1.0);\n"
//                                         "    out_texcoord = in_texcoord;\n"
//                                         "}\n" };
//// Fragment shader source code
// const std::string _fragmentShaderSource {
//     "#include \"BlinnPhong.glsl\"\n"
//     "#include \"VertexAttribInterface.frag.glsl\"\n"
//     "layout (location = 0) out vec4 out_color;\n"
//     "uniform vec4 aColorUniform;\n"
//     "uniform float aScalarUniform;\n"
//     "void main(void)\n"
//     "{\n"
//     "   //out_color =  vec4(texture(material.tex.kd, in_texcoord.xy).rgb, 1.0);\n"
//     "   out_color =  vec4(in_texcoord.xy, 1.0, 1.0);\n"
//     "   // out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
//     "}\n"
// };

////// Fragment shader source code
//// const std::string _fragmentShaderSource2 {
////     "layout (location = 0) in  vec3 in_pos;\n"
////     "layout (location = 0) out vec4 out_color;\n"
////     "uniform vec4 aColorUniform;\n"
////     "uniform float aScalarUniform;\n"
////     "void main(void)\n"
////     "{\n"
////     "    out_color =  ( 1 + sin( 20 * ( in_pos.y + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
////     "}\n"
//// };

// const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
//     { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, _vertexShaderSource },
//     { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource }
// };

//// const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config2 {
////     { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, _vertexShaderSource },
////     { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource2 }
//// };

// class MyParameterProvider : public Ra::Engine::Data::ShaderParameterProvider {
// public:
//     MyParameterProvider() { }
//     ~MyParameterProvider() { }
//     void updateGL() override
//     {
//         // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
//         // The name of the parameter corresponds to the shader's uniform name.
//         m_renderParameters.addParameter("aColorUniform", m_colorParameter);
//         m_renderParameters.addParameter("aScalarUniform", m_scalarParameter);
//     }
//     void setOrComputeTheParameterValues()
//     {
//         // client side computation of the parameters, e.g.
//         m_colorParameter = Ra::Core::Utils::Color::Red();
//         m_scalarParameter = .5_ra;
//     }

// private:
//     Ra::Core::Utils::Color m_colorParameter { Ra::Core::Utils::Color::Green() };
//     Scalar m_scalarParameter { 1 };
// };

//#include <Engine/Data/TextureManager.hpp>
//#include <glbinding/gl/enum.h>

//#include <Engine/Data/PlainMaterial.hpp>
//#include <fstream>
//#include <stream.h>

// int main(int argc, char* argv[])
//{
//     //! [Creating the application]
//     Ra::Gui::BaseApplication app(argc, argv);
//     app.initialize(Ra::Gui::SimpleWindowFactory {});
//     //! [Creating the application]
//     //!

//    //    ! [add the custom material to the material system]
//    Ra::Engine::Data::RawShaderMaterial::registerMaterial();
//    //        Ra::Engine::Data::BlinnPhongMaterial::registerMaterial();

//    //    ! [Populate the Radium ecosystem]
//    //    auto ro = initQuad(app);

//    // Initialize all OpenGL state for the scene content
//    //    app.m_mainWindow->prepareDisplay();

//    //! [Creating the quad]
//    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });

//    //! [Create the engine entity for the quad]
//    auto e = app.m_engine->getEntityManager()->createEntity("Quad Entity");

//    //    unsigned char data[192 * 512] = { 0 };
//    constexpr int width = 10;
//    constexpr int height = 10;
//    unsigned char data[width * height * 3] = { 0 };
//    for (int i = 0; i < width; ++i) {
//        for (int j = 0; j < height; ++j) {
//            data[(i * width + j) * 3] = 255;
//            data[(i * width + j) * 3 + 1] = 0;
//            data[(i * width + j) * 3 + 2] = 0;
//        }
//    }

//    //    Ra::Engine::Data::TextureParameters textParam;
//    //    textParam.name = "myTexture";
//    //    textParam.width = 192;
//    //    textParam.height = 512;
//    //    textParam.texels = data;
//    //    textParam.format = gl::GLenum::GL_RED;
//    //    textParam.internalFormat = gl::GLenum::GL_R8;

//    //    Ra::Engine::Data::Texture texture(textParam);
//    ////    texture.initializeGL();

//    //    auto& myTexture = app.m_engine->getTextureManager()->addTexture("myTexture", 192, 512, data);
//    auto& myTexture = app.m_engine->getTextureManager()->addTexture("myTexture", 10, 10, data);
//        myTexture.format = gl::GLenum::GL_RED;
//        myTexture.internalFormat = gl::GLenum::GL_R8;

//    //    auto* textureManager = app.getEngine()->getTextureManager();
//    //    textureManager->addTexture("myTexture", 192, 512, data);

//    //        auto * texture = textureManager->loadTexture(textParam);
//    //    auto * texture = Ra::Engine::Data::TextureManager::

//    //    Ra::Engine::Data::Texture texture;

//    //        Ra::Engine::BlinnPhongMaterialData blinn;

//    //    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
//    //    matData.m_hasDiffuse = true;
//    //    matData.m_texDiffuse = "myTexture";

////    //! [Create Parameter provider for the shader]
////    auto paramProvider = std::make_shared<MyParameterProvider>();
////    paramProvider->setOrComputeTheParameterValues();

////    ! [Create the shader material]
////    Ra::Core::Asset::RawShaderMaterialData matData { "Quad Material", _config1, paramProvider };

//    //    auto blinn = std::make_shared<Ra::Core::Asset::BlinnPhongMaterialData>("myMaterial");
//    //        blinn->addTexture(Ra::Engine::Data::BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, texture);

//    //    blinn.addTexture();
//    //    ro->setMaterial(blinn);

//    //        Ra::Engine::Data::BlinnPhongMaterial mat("myMaterial");
//    //    using namespace Ra;
//    //    using namespace Ra::Engine;
//    //    using namespace Ra::Engine::Data;

//        Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
//        matData.m_texDiffuse = "myTexture";
//        matData.m_hasTexDiffuse = true;

//    //    auto converter = Ra::Engine::Data::BlinnPhongMaterialConverter();
//    ////    Ra::Engine::Data::Material * mat = converter(&matData);
//    //    Ra::Engine::Data::BlinnPhongMaterial * mat = dynamic_cast<BlinnPhongMaterial*>(converter(&matData));
//    ////    Ra::Engine::Data::BlinnPhongMaterial mat("myMaterial");
//    ////    mat.addTexture(Ra::Engine::Data::BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, &texture);

//    //! [Create a geometry component using the custom material]
//    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);
//    //    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &rawMatData);

//    //    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), mat);

//    //! [Register the entity/component association to the geometry system ]
//    auto system = app.m_engine->getSystem("GeometrySystem");
//    system->addComponent(e, c);

//    //    //![get the renderobject for further edition]
//    //    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
//    //        c->m_renderObjects[0]);

//    ////    auto matPtr = std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(mat);
//    //    ro->setMaterial(std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(*mat));

//    //    auto builder =  Ra::Engine::Rendering::EngineRenderTechniques::getDefaultTechnique( "Default material" );
//    //    auto rt      = Ra::Core::make_shared<Ra::Engine::Rendering::RenderTechnique>();
//    //    // define the technique for rendering this RenderObject (here, using the default from Material
//    //    // name)
//    ////    builder.second( *rt, matPtr->isTransparent() );
//    //    builder.second( *rt, false);
//    //    rt->setParametersProvider( matPtr );
//    //    ro->setRenderTechnique( rt );

//    //    using namespace Ra;
//    //    using namespace Ra::Engine;
//    //    using namespace Ra::Engine::Data;

//    //    Rendering::RenderTechnique rt;
//    //    auto builder = Rendering::EngineRenderTechniques::getDefaultTechnique( "Plain" );
//    //    builder.second( rt, false );
//    //    auto roMaterial = Core::make_shared<PlainMaterial>( "Default material" );
//    //    roMaterial->m_perVertexColor =
//    //        mesh->getAttribArrayGeometry().hasAttrib( Mesh::getAttribName( Mesh::VERTEX_COLOR ) );
//    //    rt.setParametersProvider( roMaterial );
//    //    rt.setParametersProvider( *mat );

//    ////    auto ro = Rendering::RenderObject::createRenderObject(
//    ////        mesh->getName(), component, Rendering::RenderObjectType::Debug, mesh, rt );
//    //    ro->setMaterial( roMaterial );
//    ////    ro->setMaterial(mat);
//    ////    return ro;

//    //        auto renderer = app.m_mainWindow->getViewer()->getRenderer();
//    ////        auto mat2 = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
//    ////        mat2->updateShaders();
//    //        renderer->buildRenderTechnique(ro.get());

//    // Initialize all OpenGL state for the scene content
//    app.m_mainWindow->prepareDisplay();

//    return app.exec();
//}

////        auto renderer = app.m_mainWindow->getViewer()->getRenderer();
////    ! [Change the shader after 3 seconds]
////    auto renderer = app.m_mainWindow->getViewer()->getRenderer();
////    auto changeTimer = new QTimer(&app);
////    changeTimer->setInterval(3000);
////    QObject::connect(changeTimer, &QTimer::timeout, [ro, renderer]() {
////        auto paramProvider = std::make_shared<MyParameterProvider>();
////        auto mat = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
////        mat->updateShaders(_config2, paramProvider);
////        renderer->buildRenderTechnique(ro.get());
////    });
////    auto mat = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
////    mat->updateShaders()
////    mat->needUpdate();
////    renderer->buildRenderTechnique(ro.get());

////    //! [terminate the app after 6 second (approximatively).]
////    auto close_timer = new QTimer(&app);
////    close_timer->setInterval(6000);
////    QObject::connect(close_timer, &QTimer::timeout, [&app]() { app.appNeedsToQuit(); });
////    close_timer->start();

////    // get scans
////    std::fstream posFile(PROJECT_DIR "records/latest/Polhemus Patriot (probe).txt", std::ios::binary | std::ios::in);
////    assert(posFile.is_open());
////    const std::vector<Stream::Acquisition> & poses = InputStream(posFile).getAllAcquisition();
////    posFile.close();

////    std::fstream scanFile(PROJECT_DIR "records/latest/ULA-OP 256.txt", std::ios::binary | std::ios::in);
////    assert(scanFile.is_open());
////    const std::vector<Stream::Acquisition> & scans = InputStream(scanFile).getAllAcquisition();
////    scanFile.close();

////    assert(poses.size() == scans.size());
////    std::cout << "nb acqs = " << scans.size() << std::endl;
////    //        std::thread thread([=]() {

////    std::thread thread([&]() {
////        //    changeTimer->start();

////        std::cout << "start playing" << std::endl;
////        while (true) {
////            const auto startRecord = poses[0].mBackendTimestamp;
////            const auto& startChrono = std::chrono::high_resolution_clock::now();

////            for (int i = 0; i < poses.size(); ++i) {
////                //            std::cout << "send snapshot" << std::endl;
////                //            const auto& elapsedTime = std::chrono::high_resolution_clock::now() - startChrono;
////                std::this_thread::sleep_until(startChrono + std::chrono::microseconds(poses[i].mBackendTimestamp - startRecord));
////                //            auto end = startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord);
////                //            while (std::chrono::high_resolution_clock::now() < end)
////                //                ; // wait

////                float* translation = (float*)poses[i].mData;
////                float* quaternion = (float*)&poses[i].mData[12];
////                //                std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
////                //                std::cout << str << std::endl;

////                Ra::Core::Transform T = Ra::Core::Transform::Identity();
////                //                Ra::Core::Vector3 vec(1.0, 1.0, 1.0);
////                Ra::Core::Vector3 vecScale(0.1, 0.1, 0.1);
////                T.scale(vecScale);

////                Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
////                //                Ra::Core::Quaternion quat(quaternion);
////                T.rotate(quat);

////                Ra::Core::Vector3 vec(translation[0] / 10.0, translation[1] / 10.0, translation[2] / 10.0);
////                T.translate(vec);
////                ro->setLocalTransform(T);
////            }
////            std::cout << "end record, auto loop" << std::endl;
////        }
////    });
////    thread.detach();

////    return app.exec();
////}
