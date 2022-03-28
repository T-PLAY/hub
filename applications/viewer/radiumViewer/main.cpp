

// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

// include the custom material definition
#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/RawShaderMaterial.hpp>

// include the Viewer to demonstrate dynamic edition of materials
#include <Gui/Viewer/Viewer.hpp>

// Qt
#include <QTimer>

#include <Core/Asset/BlinnPhongMaterialData.hpp>

///**
// * Demonstrate the usage of RawShaderMaterial functionalities
// */
//// Vertex shader source code
//const std::string _vertexShaderSource { "#include \"TransformStructs.glsl\"\n"
//                                        "layout (location = 0) in vec3 in_position;\n"
//                                        "layout (location = 0) out vec3 out_pos;\n"
//                                        "uniform Transform transform;\n"
//                                        "void main(void)\n"
//                                        "{\n"
//                                        "    mat4 mvp    = transform.proj * transform.view * transform.model;\n"
//                                        "    out_pos     = in_position;\n"
//                                        "    gl_Position = mvp*vec4(in_position.xyz, 1.0);\n"
//                                        "}\n" };
//// Fragment shader source code
//const std::string _fragmentShaderSource {
//    "layout (location = 0) in  vec3 in_pos;\n"
//    "layout (location = 0) out vec4 out_color;\n"
//    "uniform vec4 aColorUniform;\n"
//    "uniform float aScalarUniform;\n"
//    "void main(void)\n"
//    "{\n"
//    "    out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
//    "}\n"
//};

//// Fragment shader source code
//const std::string _fragmentShaderSource2 {
//    "layout (location = 0) in  vec3 in_pos;\n"
//    "layout (location = 0) out vec4 out_color;\n"
//    "uniform vec4 aColorUniform;\n"
//    "uniform float aScalarUniform;\n"
//    "void main(void)\n"
//    "{\n"
//    "    out_color =  ( 1 + sin( 20 * ( in_pos.y + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
//    "}\n"
//};

//const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
//    { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, _vertexShaderSource },
//    { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource }
//};

//const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config2 {
//    { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, _vertexShaderSource },
//    { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, _fragmentShaderSource2 }
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
//    }
//    void setOrComputeTheParameterValues()
//    {
//        // client side computation of the parameters, e.g.
//        m_colorParameter = Ra::Core::Utils::Color::Red();
//        m_scalarParameter = .5_ra;
//    }

//private:
//    Ra::Core::Utils::Color m_colorParameter { Ra::Core::Utils::Color::Green() };
//    Scalar m_scalarParameter { 1 };
//};

#include <Engine/Data/TextureManager.hpp>
#include <glbinding/gl/enum.h>

#include <fstream>
#include <stream.h>
#include <Engine/Data/PlainMaterial.hpp>

int main(int argc, char* argv[])
{
//    //! [Creating the application]
//    Ra::Gui::BaseApplication app(argc, argv);
//    app.initialize(Ra::Gui::SimpleWindowFactory {});

////     Initialize all OpenGL state for the scene content
////    app.m_mainWindow->prepareDisplay();

//    //! [Creating the quad]
//    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });

//    //! [Create the engine entity for the quad]
//    auto e = app.m_engine->getEntityManager()->createEntity("Quad Entity");


//    // create texture
//    unsigned char data[192 * 512] = { 0 };

//    Ra::Engine::Data::TextureParameters textParam;
//    textParam.name = "myTexture";
//    textParam.width = 192;
//    textParam.height = 512;
//    textParam.texels = data;
//    textParam.format = gl::GLenum::GL_RED;
//    textParam.internalFormat = gl::GLenum::GL_R8;

//    Ra::Engine::Data::Texture texture(textParam);
////    texture.initializeGL();


//    // create material
//    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
//    matData.m_hasDiffuse = true;
//    matData.m_texDiffuse = "myTexture";


//    //! [Create a geometry component using the custom material]
//    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);

//    //! [Register the entity/component association to the geometry system ]
//    auto system = app.m_engine->getSystem("GeometrySystem");
//    system->addComponent(e, c);

//    // Initialize all OpenGL state for the scene content
//    app.m_mainWindow->prepareDisplay();


//    return app.exec();
//}


   //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]
    //!

    //    ! [add the custom material to the material system]
    //        Ra::Engine::Data::RawShaderMaterial::registerMaterial();
        Ra::Engine::Data::BlinnPhongMaterial::registerMaterial();

    //    ! [Populate the Radium ecosystem]
    //    auto ro = initQuad(app);

    // Initialize all OpenGL state for the scene content
    app.m_mainWindow->prepareDisplay();

    //! [Creating the quad]
    auto quad = Ra::Core::Geometry::makeZNormalQuad({ 1_ra, 1_ra });

    //! [Create the engine entity for the quad]
    auto e = app.m_engine->getEntityManager()->createEntity("Quad Entity");

    //! [Create Parameter provider for the shader]
//    auto paramProvider = std::make_shared<MyParameterProvider>();
//    paramProvider->setOrComputeTheParameterValues();

    //! [Create the shader material]
    //    Ra::Core::Asset::RawShaderMaterialData mat { "Quad Material", _config1, paramProvider };
//    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
//    matData.m_texDiffuse = "myTexture";
//    matData.m_hasTexDiffuse = true;


    unsigned char data[192 * 512] = { 0 };
    for (int i =0; i <192 * 512; ++i) {
        data[i] = i;
    }

    Ra::Engine::Data::TextureParameters textParam;
    textParam.name = "myTexture";
    textParam.width = 192;
    textParam.height = 512;
    textParam.texels = data;
    textParam.format = gl::GLenum::GL_RED;
    textParam.internalFormat = gl::GLenum::GL_R8;

    Ra::Engine::Data::Texture texture(textParam);
//    texture.initializeGL();

//    auto* textureManager = app.getEngine()->getTextureManager();
//    textureManager->addTexture("myTexture", 192, 512, data);

//        auto * texture = textureManager->loadTexture(textParam);
    //    auto * texture = Ra::Engine::Data::TextureManager::

//    Ra::Engine::Data::Texture texture;

    //        Ra::Engine::BlinnPhongMaterialData blinn;

    Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
    matData.m_hasDiffuse = true;
    matData.m_texDiffuse = "myTexture";

    //    auto blinn = std::make_shared<Ra::Core::Asset::BlinnPhongMaterialData>("myMaterial");
    //        blinn->addTexture(Ra::Engine::Data::BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, texture);

    //    blinn.addTexture();
    //    ro->setMaterial(blinn);

//    Ra::Engine::Data::BlinnPhongMaterial mat("myMaterial");
    using namespace Ra;
    using namespace Ra::Engine;
    using namespace Ra::Engine::Data;



    auto converter = Ra::Engine::Data::BlinnPhongMaterialConverter();
//    Ra::Engine::Data::Material * mat = converter(&matData);
    Ra::Engine::Data::BlinnPhongMaterial * mat = dynamic_cast<BlinnPhongMaterial*>(converter(&matData));
//    Ra::Engine::Data::BlinnPhongMaterial mat("myMaterial");
//    mat.addTexture(Ra::Engine::Data::BlinnPhongMaterial::TextureSemantic::TEX_DIFFUSE, &texture);

    //! [Create a geometry component using the custom material]
    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), &matData);
//    auto c = new Ra::Engine::Scene::TriangleMeshComponent("Quad Mesh", e, std::move(quad), mat);

    //! [Register the entity/component association to the geometry system ]
    auto system = app.m_engine->getSystem("GeometrySystem");
    system->addComponent(e, c);



    //![get the renderobject for further edition]
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
        c->m_renderObjects[0]);

//    auto matPtr = std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(mat);
    ro->setMaterial(std::make_shared<Ra::Engine::Data::BlinnPhongMaterial>(*mat));

//    auto builder =  Ra::Engine::Rendering::EngineRenderTechniques::getDefaultTechnique( "Default material" );
//    auto rt      = Ra::Core::make_shared<Ra::Engine::Rendering::RenderTechnique>();
//    // define the technique for rendering this RenderObject (here, using the default from Material
//    // name)
////    builder.second( *rt, matPtr->isTransparent() );
//    builder.second( *rt, false);
//    rt->setParametersProvider( matPtr );
//    ro->setRenderTechnique( rt );

    using namespace Ra;
    using namespace Ra::Engine;
    using namespace Ra::Engine::Data;

//    Rendering::RenderTechnique rt;
//    auto builder = Rendering::EngineRenderTechniques::getDefaultTechnique( "Plain" );
//    builder.second( rt, false );
//    auto roMaterial = Core::make_shared<PlainMaterial>( "Default material" );
//    roMaterial->m_perVertexColor =
//        mesh->getAttribArrayGeometry().hasAttrib( Mesh::getAttribName( Mesh::VERTEX_COLOR ) );
//    rt.setParametersProvider( roMaterial );
//    rt.setParametersProvider( *mat );

////    auto ro = Rendering::RenderObject::createRenderObject(
////        mesh->getName(), component, Rendering::RenderObjectType::Debug, mesh, rt );
//    ro->setMaterial( roMaterial );
////    ro->setMaterial(mat);
////    return ro;


//        auto renderer = app.m_mainWindow->getViewer()->getRenderer();
////        auto mat2 = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
////        mat2->updateShaders();
//        renderer->buildRenderTechnique(ro.get());


    return app.exec();
}



//        auto renderer = app.m_mainWindow->getViewer()->getRenderer();
    //    ! [Change the shader after 3 seconds]
    //    auto renderer = app.m_mainWindow->getViewer()->getRenderer();
    //    auto changeTimer = new QTimer(&app);
    //    changeTimer->setInterval(3000);
    //    QObject::connect(changeTimer, &QTimer::timeout, [ro, renderer]() {
    //        auto paramProvider = std::make_shared<MyParameterProvider>();
    //        auto mat = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
    //        mat->updateShaders(_config2, paramProvider);
    //        renderer->buildRenderTechnique(ro.get());
    //    });
    //    auto mat = static_cast<Ra::Engine::Data::RawShaderMaterial*>(ro->getMaterial().get());
    //    mat->updateShaders()
    //    mat->needUpdate();
    //    renderer->buildRenderTechnique(ro.get());

    //    //! [terminate the app after 6 second (approximatively).]
    //    auto close_timer = new QTimer(&app);
    //    close_timer->setInterval(6000);
    //    QObject::connect(close_timer, &QTimer::timeout, [&app]() { app.appNeedsToQuit(); });
    //    close_timer->start();

//    // get scans
//    std::fstream posFile(PROJECT_DIR "records/latest/Polhemus Patriot (probe).txt", std::ios::binary | std::ios::in);
//    assert(posFile.is_open());
//    const std::vector<Stream::Acquisition> & poses = InputStream(posFile).getAllAcquisition();
//    posFile.close();

//    std::fstream scanFile(PROJECT_DIR "records/latest/ULA-OP 256.txt", std::ios::binary | std::ios::in);
//    assert(scanFile.is_open());
//    const std::vector<Stream::Acquisition> & scans = InputStream(scanFile).getAllAcquisition();
//    scanFile.close();



//    assert(poses.size() == scans.size());
//    std::cout << "nb acqs = " << scans.size() << std::endl;
//    //        std::thread thread([=]() {

//    std::thread thread([&]() {
//        //    changeTimer->start();

//        std::cout << "start playing" << std::endl;
//        while (true) {
//            const auto startRecord = poses[0].mBackendTimestamp;
//            const auto& startChrono = std::chrono::high_resolution_clock::now();

//            for (int i = 0; i < poses.size(); ++i) {
//                //            std::cout << "send snapshot" << std::endl;
//                //            const auto& elapsedTime = std::chrono::high_resolution_clock::now() - startChrono;
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
//                Ra::Core::Vector3 vecScale(0.1, 0.1, 0.1);
//                T.scale(vecScale);

//                Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
//                //                Ra::Core::Quaternion quat(quaternion);
//                T.rotate(quat);

//                Ra::Core::Vector3 vec(translation[0] / 10.0, translation[1] / 10.0, translation[2] / 10.0);
//                T.translate(vec);
//                ro->setLocalTransform(T);
//            }
//            std::cout << "end record, auto loop" << std::endl;
//        }
//    });
//    thread.detach();

//    return app.exec();
//}
