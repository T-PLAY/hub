// Include Radium base application and its simple Gui
#include <Gui/BaseApplication.hpp>
#include <Gui/RadiumWindow/SimpleWindowFactory.hpp>

// include the Engine/entity/component interface
#include <Core/Geometry/MeshPrimitives.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometryComponent.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <QTimer>

#include <Core/Asset/FileLoaderInterface.hpp>
#include <DicomLoader/DicomLoader.hpp>

#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

//#define MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/"
//#define MRI_PATH2 "C:\\Users\\gbouyjou\\Downloads\\MRI"
#define DEMO_FILE

int main(int argc, char* argv[])
{
    //! [Creating the application]
    Ra::Gui::BaseApplication app(argc, argv);
    app.initialize(Ra::Gui::SimpleWindowFactory {});
    //! [Creating the application]

//    return app.exec();

// DEMO_FILE is set during configure time by cmake and defines the file to load in the demo
#ifndef DEMO_FILE
    //! [Creating the Volume]
    auto density = new Ra::Core::Geometry::VolumeGrid();
    int sx = 100;
    int sy = 100;
    int sz = 75;

    density->setSize(Ra::Core::Vector3i(sx, sy, sz));
    auto& data = density->data();
    // Radial density with value 1 at center
    auto densityField = [sx, sy, sz](int i, int j, int k, int r) {
        i -= sx / 2;
        j -= sy / 2;
        k -= sz / 2;
        Scalar d = (std::sqrt(i * i + j * j + k * k) - r) / r;
        if (d > 0) {
            return 0_ra;
        } else {
            return -d;
        }
    };
    for (int i = 0; i < sx; ++i) {
        for (int j = 0; j < sy; ++j) {
            for (int k = 0; k < sz; ++k) {
                data[i + sx * (j + sy * k)] = densityField(i, j, k, 50);
            }
        }
    }

    auto volume = new Ra::Core::Asset::VolumeData("DemoVolume");
    volume->volume = density;
    Scalar maxDim = std::max(std::max(sx, sy), sz);
    Ra::Core::Vector3 p0(0, 0, 0);
    Ra::Core::Vector3 p1(sx, sy, sz);
    volume->boundingBox = Ra::Core::Aabb(p0, p1);
    volume->densityToModel = Ra::Core::Transform::Identity();
    volume->modelToWorld = Eigen::Scaling(1_ra / maxDim); // In the scene, the volume has size 1^3
    //! [Creating the Volume]

    //! [Create the engine entity for the Volume]
    auto e = app.m_engine->getEntityManager()->createEntity("Volume demo");
    e->setTransform(Ra::Core::Transform { Ra::Core::Translation(0_ra, 0_ra, 0.5_ra) });
    //! [Create the engine entity for the Volume]

    //! [Create a geometry component with the Volume]
    auto c = new Ra::Engine::Scene::VolumeComponent("Volume Demo", e, volume);
    //! [Create a geometry component with the Volume]

    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = app.m_engine->getSystem("GeometrySystem");
    geometrySystem->addComponent(e, c);
    //! [Register the entity/component association to the geometry system ]
    //! [Creating the cube]
    auto cube = Ra::Core::Geometry::makeSharpBox({ 0.5f, 0.5f, 1.f });
    //! [Creating the cube]

    //! [Colorize the Cube]
    cube.addAttrib(
        "in_color",
        Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::White() });
    //! [Colorize the Cube]

    //! [Create the engine entity for the cube]
    auto ce = app.m_engine->getEntityManager()->createEntity("White cube");
    //! [Create the engine entity for the cube]

    //! [Create a geometry component with the cube]
    auto cc = new Ra::Engine::Scene::TriangleMeshComponent("Cube Mesh", ce, std::move(cube), nullptr);
    //! [Create a geometry component with the cube]

    //! [Register the entity/component association to the geometry system ]
    geometrySystem->addComponent(ce, cc);
    //! [Register the entity/component association to the geometry system ]

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]
#else

//    app.loadFile(DEMO_FILE);
    //    Ra::IO::DicomLoader DicomLoader;
    app.m_engine->registerFileLoader(std::shared_ptr<Ra::Core::Asset::FileLoaderInterface>(new Ra::IO::DicomLoader()));
//    app.addApplicationExtension();
    //    app.loadFile("/home/gauthier/Downloads/DICOM/D0010001.dcm");
    //    constexpr auto irmPath = "/home/gauthier/tmpDicomViewer/IRM/";

    app.loadFile(QString(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm"));
//app.loadFile(QString("C:\\Users\\gbouyjou\\Downloads\\MRI" "AXT2_ligaments_uterosacres/D0010525.dcm"));
//    app.loadFile(QString(MRI_PATH2 "AXT2_ligaments_uterosacres/D0010525.dcm"));

//        app.loadFile(QString(MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm"));
//        app.loadFile(QString(MRI_PATH "Loc/D0010001.dcm"));
//        app.loadFile(QString(MRI_PATH "MPR_AX_T2/D0010551.dcm"));
//        app.loadFile(QString(MRI_PATH "MPR_Coro_T2/D0010710.dcm"));
//        app.loadFile(QString(MRI_PATH "MPR_Sag_T2/D0010652.dcm"));
//        app.loadFile(QString(MRI_PATH "PACS_AXIAL_ABDO/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "PACS_AXIAL_AP/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "PACS_AXIAL_TAP/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "PACS_PARENCHYME/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "RapportDose/IM0001.dcm")); // not loadable
//        app.loadFile(QString(MRI_PATH "Sag_CUBE_T2/D0010031.dcm"));
//        app.loadFile(QString(MRI_PATH "Scouts/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "ScreenSave/IM0001.dcm")); // not loadable
//        app.loadFile(QString(MRI_PATH "SerieSmartPrep/IM0001.dcm")); // 17 bits per voxel
//        app.loadFile(QString(MRI_PATH "WATER:AX_LAVA-Flex/D0010333.dcm"));

//    auto entities = app.m_engine->getEntityManager()->getEntities();
//    auto e = entities[entities.size() - 1];
//    Ra::Core::Transform tr = Ra::Core::Transform::Identity();
//    tr.rotate(Ra::Core::AngleAxis(-Ra::Core::Math::PiDiv2, Ra::Core::Vector3 { 1_ra, 0_ra, 0_ra }));
//    tr.translate(Ra::Core::Vector3 { 0_ra, 0_ra, 0.125_ra });
//    e->setTransform(tr);

    //    auto geometrySystem = app.m_engine->getSystem( "GeometrySystem" );
    //    //! [Creating the cube]
    //    auto cube = Ra::Core::Geometry::makeSharpBox( { 0.5_ra, 0.05_ra, 0.5_ra } );
    //    //! [Creating the cube]

    //    //! [Colorize the Cube]
    //    cube.addAttrib(
    //        "in_color",
    //        Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::White() } );
    //    //! [Colorize the Cube]

    //    //! [Create the engine entity for the cube]
    //    auto ce = app.m_engine->getEntityManager()->createEntity( "White cube" );
    //    //! [Create the engine entity for the cube]

    //    //! [Create a geometry component with the cube]
    //    auto cc =
    //        new Ra::Engine::Scene::TriangleMeshComponent( "Cube Mesh", ce, std::move( cube ), nullptr );
    //    //! [Create a geometry component with the cube]

    //    //! [Register the entity/component association to the geometry system ]
    //    geometrySystem->addComponent( ce, cc );
    //    //! [Register the entity/component association to the geometry system ]

    //![get the renderobject for further edition]
    //    const auto & ro = app.m_engine->getRenderObjectManager()->getRenderObject();
    //    std::cout << ro->getName() << std::endl;
    //    ro->computeAabb();

    const auto& roManager = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    roManager->getRenderObjects();
    const auto nObjects = roManager->getRenderObjectsCount();
    for (int i = 0; i < nObjects; ++i) {
        std::cout << roManager->getRenderObject(i)->getName() << std::endl;
    }
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(nObjects - 1);
    //    ro->setVisible(false);

    RA_DISPLAY_AABB(ro->computeAabb(), Ra::Core::Utils::Color::Blue());

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

#endif

    //    // terminate the app after 8 second (approximatively). Camera can be moved using mouse moves.
    //    auto close_timer = new QTimer( &app );
    //    close_timer->setInterval( 8000 );
    //    QObject::connect( close_timer, &QTimer::timeout, [&app]() { app.appNeedsToQuit(); } );
    //    // close_timer->start();

    return app.exec();
}
