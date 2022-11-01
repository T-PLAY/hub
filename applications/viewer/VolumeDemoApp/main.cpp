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

int main( int argc, char* argv[] ) {
    //! [Creating the application]
    Ra::Gui::BaseApplication app( argc, argv );
    app.initialize( Ra::Gui::SimpleWindowFactory {} );
    //! [Creating the application]

// DEMO_FILE is set during configure time by cmake and defines the file to load in the demo
#ifndef DEMO_FILE
    //! [Creating the Volume]
    auto density = new Ra::Core::Geometry::VolumeGrid();
    int sx       = 100;
    int sy       = 100;
    int sz       = 75;

    density->setSize( Ra::Core::Vector3i( sx, sy, sz ) );
    auto& data = density->data();
    // Radial density with value 1 at center
    auto densityField = [sx, sy, sz]( int i, int j, int k, int r ) {
        i -= sx / 2;
        j -= sy / 2;
        k -= sz / 2;
        Scalar d = ( std::sqrt( i * i + j * j + k * k ) - r ) / r;
        if ( d > 0 ) { return 0_ra; }
        else { return -d; }
    };
    for ( int i = 0; i < sx; ++i ) {
        for ( int j = 0; j < sy; ++j ) {
            for ( int k = 0; k < sz; ++k ) {
                data[i + sx * ( j + sy * k )] = densityField( i, j, k, 50 );
            }
        }
    }

    auto volume    = new Ra::Core::Asset::VolumeData( "DemoVolume" );
    volume->volume = density;
    Scalar maxDim  = std::max( std::max( sx, sy ), sz );
    Ra::Core::Vector3 p0( 0, 0, 0 );
    Ra::Core::Vector3 p1( sx, sy, sz );
    volume->boundingBox    = Ra::Core::Aabb( p0, p1 );
    volume->densityToModel = Ra::Core::Transform::Identity();
    volume->modelToWorld = Eigen::Scaling( 1_ra / maxDim ); // In the scene, the volume has size 1^3
    //! [Creating the Volume]

    //! [Create the engine entity for the Volume]
    auto e = app.m_engine->getEntityManager()->createEntity( "Volume demo" );
    e->setTransform( Ra::Core::Transform { Ra::Core::Translation( 0_ra, 0_ra, 0.5_ra ) } );
    //! [Create the engine entity for the Volume]

    //! [Create a geometry component with the Volume]
    auto c = new Ra::Engine::Scene::VolumeComponent( "Volume Demo", e, volume );
    //! [Create a geometry component with the Volume]

    //! [Register the entity/component association to the geometry system ]
    auto geometrySystem = app.m_engine->getSystem( "GeometrySystem" );
    geometrySystem->addComponent( e, c );
    //! [Register the entity/component association to the geometry system ]
    //! [Creating the cube]
    auto cube = Ra::Core::Geometry::makeSharpBox( { 0.5f, 0.5f, 1.f } );
    //! [Creating the cube]

    //! [Colorize the Cube]
    cube.addAttrib(
        "in_color",
        Ra::Core::Vector4Array { cube.vertices().size(), Ra::Core::Utils::Color::White() } );
    //! [Colorize the Cube]

    //! [Create the engine entity for the cube]
    auto ce = app.m_engine->getEntityManager()->createEntity( "White cube" );
    //! [Create the engine entity for the cube]

    //! [Create a geometry component with the cube]
    auto cc =
        new Ra::Engine::Scene::TriangleMeshComponent( "Cube Mesh", ce, std::move( cube ), nullptr );
    //! [Create a geometry component with the cube]

    //! [Register the entity/component association to the geometry system ]
    geometrySystem->addComponent( ce, cc );
    //! [Register the entity/component association to the geometry system ]

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]
#else

    app.m_engine->registerFileLoader(
        std::shared_ptr<Ra::Core::Asset::FileLoaderInterface>( new Ra::IO::DicomLoader() ) );

    app.loadFile( QString( MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm" ) );
    // app.loadFile(QString("C:\\Users\\gbouyjou\\Downloads\\MRI"
    // "AXT2_ligaments_uterosacres/D0010525.dcm"));

    //![get the renderobject for further edition]

    const auto& roManager = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager();
    roManager->getRenderObjects();
    const auto nObjects = roManager->getRenderObjectsCount();
    for ( int i = 0; i < nObjects; ++i ) {
        std::cout << roManager->getRenderObject( i )->getName() << std::endl;
    }
    auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
        nObjects - 1 );

    RA_DISPLAY_AABB( ro->computeAabb(), Ra::Core::Utils::Color::Blue() );

    //! [Tell the window that something is to be displayed]
    app.m_mainWindow->prepareDisplay();
    //! [Tell the window that something is to be displayed]

#endif

    return app.exec();
}
