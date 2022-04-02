
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

std::string loadShaderSource(const std::string& vertexPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::ifstream vShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        std::stringstream vShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return "";
    }
    return vertexCode;
}

/**
 * Demonstrate the usage of RawShaderMaterial functionalities
 */
// Vertex shader source code
const std::string vertexShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/vertexShader.glsl";
const std::string vertexShaderSource = loadShaderSource(vertexShaderFile);
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
//  Fragment shader source code
const std::string fragmentShaderFile = PROJECT_DIR "applications/viewer/radiumViewer/fragmentShader.glsl";
const std::string fragmentShaderSource = loadShaderSource(fragmentShaderFile);
// const std::string _fragmentShaderSource {
//     "#include \"BlinnPhong.glsl\"\n"
//     "#include \"VertexAttribInterface.frag.glsl\"\n"
//     "layout (location = 0) out vec4 out_color;\n"
//     "uniform vec4 aColorUniform;\n"
//     "uniform float aScalarUniform;\n"
//     "void main(void)\n"
//     "{\n"
//     "   // out_color =  vec4(texture(material.tex.kd, in_texcoord.xy).b, 0.0, 0.0, 1.0);\n"
//     "   out_color =  vec4(in_texcoord.xy, 1.0, 1.0);\n"
//     "   // out_color =  ( 1 + cos( 20 * ( in_pos.x + aScalarUniform ) ) ) * 0.5 * aColorUniform;\n"
//     "}\n"
// };

const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
    { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, vertexShaderSource },
    { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, fragmentShaderSource }
};

// const std::vector<std::pair<Ra::Engine::Data::ShaderType, std::string>> _config1 {
//     { Ra::Engine::Data::ShaderType::ShaderType_VERTEX, vertexShaderFile },
//     { Ra::Engine::Data::ShaderType::ShaderType_FRAGMENT, fragmentShaderFile }
// };

class MyParameterProvider : public Ra::Engine::Data::ShaderParameterProvider {
public:
    MyParameterProvider() { }
    ~MyParameterProvider() { }
    void updateGL() override
    {
        assert(m_texture != nullptr);
        // Method called before drawing each frame in Renderer::updateRenderObjectsInternal.
        // The name of the parameter corresponds to the shader's uniform name.
        m_renderParameters.addParameter("aColorUniform", m_colorParameter);
        m_renderParameters.addParameter("aScalarUniform", m_scalarParameter);
        //        m_renderParameters.addParameter("material.tex.kd", m_texture);
        //        m_renderParameters.addParameter("material.tex.hasKd", 1);
    }
    void setOrComputeTheParameterValues()
    {
        assert(m_texture != nullptr);
        // client side computation of the parameters, e.g.
        m_colorParameter = Ra::Core::Utils::Color::Red();
        m_scalarParameter = .5_ra;
        m_renderParameters.addParameter("material.tex.kd", m_texture);
        m_renderParameters.addParameter("material.tex.hasKd", 1);
    }

    // private:
    Ra::Core::Utils::Color m_colorParameter { Ra::Core::Utils::Color::Green() };
    Scalar m_scalarParameter { 1 };
    Ra::Engine::Data::Texture* m_texture = nullptr;
};

int main(int argc, char* argv[])
{

    //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]

    //! [add the custom material to the material system]
    Ra::Engine::Data::RawShaderMaterial::registerMaterial();

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
            //            data[(i * 512 + j)] = (unsigned char)(255.0 * std::abs(std::sin(j * i * M_PI / 64.0) * std::cos(j * i * M_PI / 96.0)));
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
    //    auto paramProvider = std::make_shared<MyParameterProvider>();
    //    //            Ra::Engine::Data::TextureParameters textureParameters;
    //    //            textureParameters.name = "myTexture";
    //    auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
    //                        app.m_mainWindow->getViewer()->makeCurrent();
    //                        texture->initializeGL(false);
    //                        app.m_mainWindow->getViewer()->doneCurrent();
    //    paramProvider->m_texture = texture;
    //    paramProvider->setOrComputeTheParameterValues();
    //    Ra::Core::Asset::RawShaderMaterialData matData("myMaterialData", _config1, paramProvider);
    //    Ra::Core::Asset::RawShaderMaterialData matData("myMaterialData", _config1, paramProvider);
    //    Ra::Engine::Data::PlainMaterial matData;
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

    //    auto& mat = *ro->getMaterial();
    auto& renderTechnique = *ro->getRenderTechnique();
    Ra::Engine::Data::ShaderConfiguration shaderConfig("myShader", vertexShaderFile, fragmentShaderFile);
    //    Ra::Engine::Data::ShaderProgram shaderProgram(shaderConfig);
    renderTechnique.setConfiguration(shaderConfig);

    //        return app.exec();

    InputStream scanStream("ULA-OP 256");
    //    InputStream posStream("Polhemus Patriot (probe)", "ULA-OP 256");
    InputStream posStream("Polhemus Patriot (probe)");
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
    //        close_timer->setInterval(10);
    close_timer->setInterval(20);
    //    close_timer->setSingleShot(true);
    int iAcquisition = 0;
    int nThread = 0;
    //    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &poses, &scans, &ro]() {
    //    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &scanStream, &posStream, &ro]() {
    //        QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &posStream, &ro, &nThread]() {
    QObject::connect(close_timer, &QTimer::timeout, [&app, &iAcquisition, &posStream, &ro, &nThread, &scanStream]() {
        ++nThread;
        if (nThread > 1) {
            std::cout << "thread pending ---------------------" << std::endl;
            return;
        }
        //        while (true) {
        std::cout << "update acquisition " << iAcquisition << std::endl;

        // update texture
        {
            Stream::Acquisition scanAcq;
            scanStream >> scanAcq;

            Ra::Engine::Data::TextureParameters textureParameters;
            textureParameters.name = "myTexture";
            auto texture = app.m_engine->getTextureManager()->getOrLoadTexture(textureParameters);
            auto& params = texture->getParameters();
            //                    params.texels = scans[iAcquisition].mData;
            //        memcpy(params.texels, scans[iAcquisition].mData, 192 * 512);
            memcpy(params.texels, scanAcq.mData, 192 * 512);
            //            updateTexel(params.texels, dec);
            app.m_mainWindow->getViewer()->makeCurrent();
            texture->initializeGL(false);
            app.m_mainWindow->getViewer()->doneCurrent();
        }

        // update position and orientation
        {
            Stream::Acquisition posAcq;
            posStream >> posAcq;
            float* translation = (float*)posAcq.mData;
            //            std::cout << "x = " << translation[0] << ",\ty = " << translation[1] << ",\tz = " << translation[2] << std::endl;
            float* quaternion = (float*)&posAcq.mData[12];
            //                float* euler = (float*)&posAcq.mData[28];
            //                float az = euler[0]; // yaw
            //                float el = euler[1]; // pitch
            //                float roll = euler[2];
            //                std::cout << "az = " << az << ", el = " << el << ", roll = " << roll << std::endl;

            //  , PDI_MODATA_ORI           // az, el, ro Euler ori angles
            //                std::string str = std::string("x:") + std::to_string(translation[0]) + ", y:" + std::to_string(translation[1]) + ", z:" + std::to_string(translation[2]) + "\naz:" + std::to_string(quaternion[0]) + ", el:" + std::to_string(quaternion[1]) + ", ro:" + std::to_string(quaternion[2]) + ", q4:" + std::to_string(quaternion[3]);
            //                std::cout << str << std::endl;

            // change to Radium base reference
            Ra::Core::Transform TRadium = Ra::Core::Transform::Identity();
            TRadium.rotate(Eigen::AngleAxis(1.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
            TRadium.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));

            // orientation
            Ra::Core::Transform TOrientation = Ra::Core::Transform::Identity();
            Ra::Core::Quaternion quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
            //                Ra::Core::Transform T2 = Ra::Core::Transform::Identity();
            //                T2.rotate(quat);
            //                Ra::Core::Matrix4 mat = Ra::Core::Matrix4::Zero();
            //                mat.col(0) << T2;
            //                quat = quat.inverse();
            //                Ra::Core::Quaternion::inverse(q)
            //            quat = quat.toRotationMatrix().transpose();
            TOrientation.rotate(quat);
            //                TOrientation.rotate(Eigen::AngleAxis(az / 180.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 1.0, 0.0)));
            //                TOrientation.rotate(Eigen::AngleAxis(el / 180.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));
            //                TOrientation.rotate(Eigen::AngleAxis(roll / 180.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));

            // World transform
            Ra::Core::Transform TWorld = Ra::Core::Transform::Identity();
            //                                TWorld.translate(Ra::Core::Vector3(0.0, 0.0, 1.0));
            //                TWorld.rotate(Eigen::AngleAxis(1.0f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
            //                TWorld.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));
            //                Ra::Core::Vector3 vec(-translation[0], -translation[2], -translation[1]);
            Ra::Core::Vector3 vecPos(translation[0], translation[1], translation[2]);
            //                Ra::Core::Vector3 vecPos(-translation[0], -translation[1], -translation[2]);
            vecPos /= 3.0;
            TWorld.translate(vecPos);

            // Local transform
            Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
            //                TLocal.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));
            //                TLocal.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 1.0, 0.0)));
            //                TLocal.rotate(Eigen::AngleAxis(0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
            TLocal.translate(Ra::Core::Vector3(1.0, 0.0, 0.0));
            //                                TLocal.rotate(Eigen::AngleAxis(0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(0.0, 0.0, 1.0)));
            //                                TLocal.rotate(Eigen::AngleAxis(-0.5f * Ra::Core::Math::Pi, Ra::Core::Vector3(1.0, 0.0, 0.0)));
            Ra::Core::Vector3 vecScale(1.0, 192.0 / 512, 1.0);
            TLocal.scale(vecScale);

            //                ro->setLocalTransform(TWorld * TOrientation * TLocal);
            ro->setLocalTransform(TRadium * TWorld * TOrientation * TLocal);
        }

        //            std::cout << "update data with dec = " << dec << std::endl;
        ++iAcquisition;
        //        if (iAcquisition == poses.size())
        //            iAcquisition = 0;
        //        }
        --nThread;
    });
    close_timer->start();

    return app.exec();
}
