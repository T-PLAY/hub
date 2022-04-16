
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

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Engine/Data/RawShaderMaterial.hpp>
#include <QThread>

#include <Engine/Data/PlainMaterial.hpp>
#include <fstream>

#include <Engine/Scene/CameraManager.hpp>

#define SENSOR

// std::string loadShaderSource(const std::string& vertexPath)
//{
// }

///**
// * Demonstrate the usage of RawShaderMaterial functionalities
// */
// Vertex shader source code
const std::string vertexShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
// const std::string vertexShaderSource = loadShaderSource(vertexShaderFile);
//  const std::string _vertexShaderSource { "#include \"TransformStructs.glsl\"\n"
const std::string fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";
// const std::string fragmentShaderSource = loadShaderSource(fragmentShaderFile);
//  const std::string _fragmentShaderSource {
//  };

// const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
// };

// const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
// };

// class MyParameterProvider : public Ra::Engine::Data::ShaderParameterProvider {
// public:

//};

int main(int argc, char* argv[])
{

    //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]

    //    ! [add the custom material to the material system]
    Ra::Engine::Data::RawShaderMaterial::registerMaterial();

    std::shared_ptr<Ra::Engine::Rendering::RenderObject> roQuad;
    // quad texture
    {
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
                if (std::abs(i - 20) < 3 || std::abs(j - 20) < 3) {
                    data[(i * 512 + j)] = 0;
                } else {

                    data[(i * 512 + j)] = (j / 2) % 256;
                }
            }
        }
        auto& textureParameters = app.m_engine->getTextureManager()->addTexture("myTexture", 512, 192, data);
        textureParameters.format = gl::GLenum::GL_RED;
        textureParameters.internalFormat = gl::GLenum::GL_R8;
        //! [Creating a texture for the quad]

        //! [create the quad material]
        Ra::Core::Asset::BlinnPhongMaterialData matData("myMaterialData");
        // uncomment this to remove glossy highlight
        matData.m_specular = Ra::Core::Utils::Color::Black();
        matData.m_hasSpecular = true;
        matData.m_hasTexDiffuse = true;
        matData.m_texDiffuse = "myTexture";

        //! [Create Parameter provider for the shader]
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

        auto& renderTechnique = *ro->getRenderTechnique();
        Ra::Engine::Data::ShaderConfiguration shaderConfig("myShader", vertexShaderFile, fragmentShaderFile);
        renderTechnique.setConfiguration(shaderConfig);

        roQuad = ro;
    }

    Ra::Engine::Scene::CameraManager* cameraManager = static_cast<Ra::Engine::Scene::CameraManager*>(app.m_engine->getInstance()->getSystem("DefaultCameraManager"));
    cameraManager->defaultCamera.setPosition(Ra::Core::Vector3 { 100_ra, 100_ra, 100_ra });
    //    cameraManager->updateActiveCameraData();
    //    roQuad->setVisible(false);

    //    return app.exec();

#ifdef SENSOR
    InputStream* scanStream = nullptr;
    InputStream* posStream = nullptr;

//#define POSE_ONLY
#ifdef POSE_ONLY
    InputStream posStream("Polhemus Patriot (probe)");
#else

    try {
        scanStream = new InputStream("ULA-OP 256", "");
    } catch (std::exception& e) {
        std::cout << "[main] catch exception " << e.what() << std::endl;
        scanStream = nullptr;
    }

    try {
        if (scanStream != nullptr) {
            posStream = new InputStream("Polhemus Patriot (probe)", "ULA-OP 256");
        } else {
            posStream = new InputStream("Polhemus Patriot (probe)");
        }
    } catch (std::exception& e) {
        std::cout << "[main] catch exception " << e.what() << std::endl;
        posStream = nullptr;
    }

    if (posStream == nullptr && scanStream == nullptr) {
        return 0;
    }

#endif

#endif

    auto close_timer = new QTimer(&app);
#ifdef WIN32
    close_timer->setInterval(10); // win
#else
    close_timer->setInterval(20); // linux
#endif
    int iAcquisition = 0;
    int nThread = 0;

#ifdef SENSOR
    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &posStream = posStream, &roQuad, &nThread, &scanStream = scanStream]() {
#else
    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &nThread]() {
#endif
        ++nThread;
        if (nThread > 1) {
            std::cout << "thread pending ---------------------" << std::endl;
            return;
        }
        std::cout << "update acquisition " << iAcquisition << std::endl;

        //#ifndef POSE_ONLY
        // update texture

#ifdef SENSOR
        if (scanStream != nullptr) {
            Stream::Acquisition scanAcq;
            *scanStream >> scanAcq;

            unsigned char* data = scanAcq.mData;
#else
        unsigned char data[192 * 512] = { 0 };
        for (int i = 0; i < 192; ++i) {
            for (int j = 0; j < 512; j++) {
                data[i * 512 + j] = (iAcquisition + j) % 256;
            }
        }

#endif

            Ra::Engine::Data::TextureParameters textureParameters;
            textureParameters.name = "myTexture";
            auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
            auto& params = texture->getParameters();
            memcpy(params.texels, data, 192 * 512);
            app.m_mainWindow->getViewer()->makeCurrent();
            texture->initializeGL(false);
            app.m_mainWindow->getViewer()->doneCurrent();

#ifdef SENSOR
        }
#endif
        //#endif

#ifdef SENSOR
        // update position and orientation
        if (posStream != nullptr) {
            Stream::Acquisition posAcq;
            *posStream >> posAcq;
            float* translation = (float*)posAcq.mData;
            float* quaternion = (float*)&posAcq.mData[12];

            // change to Radium base reference
            Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
            TRadium.rotate(Eigen::AngleAxis(1.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
            TRadium.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));

            // orientation
            Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
            Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
            TOrientation.rotate(quat);


            // World transform
            Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();

            Ra::Core::Vector3 vecPos(-translation[0], -translation[1], -translation[2]);

            vecPos /= 5.0;
            TWorld.translate(vecPos);

            // Local transform
            Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
            TLocal.translate(Ra::Core::Vector3(1.0, 0.0, 0.0));
            Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
            TLocal.scale(vecScale);

            roQuad->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        }
#endif

        ++iAcquisition;
        --nThread;
    });
    close_timer->start();

    return app.exec();

#ifdef SENSOR
    if (posStream != nullptr)
        delete posStream;
    if (scanStream != nullptr)
        delete scanStream;
#endif
}
