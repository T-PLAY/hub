#include <Gui/MainWindow.hpp>
#include <MainApplication.hpp>

//#include <Core/Asset/FileLoaderInterface.hpp>
//#include <Engine/Data/Material.hpp>
//#include <Engine/Data/MaterialConverters.hpp>
//#include <Engine/Data/Mesh.hpp>
#include <Engine/Rendering/ForwardRenderer.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
//#include <Engine/Scene/CameraManager.hpp>
//#include <Engine/Scene/Entity.hpp>
//#include <Engine/Scene/EntityManager.hpp>
//#include <Engine/Scene/SignalManager.hpp>
//#include <Gui/SkeletonBasedAnimation/SkeletonBasedAnimationUI.hpp>
//#include <Gui/Timeline/Timeline.hpp>
//#include <Gui/TreeModel/EntityTreeModel.hpp>
//#include <Gui/Utils/KeyMappingManager.hpp>
//#include <Gui/Utils/qt_utils.hpp>
#include <Gui/Viewer/FlightCameraManipulator.hpp>
//#include <Gui/Viewer/Gizmo/GizmoManager.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>
//#include <IO/deprecated/OBJFileManager.hpp>
//#include <PluginBase/RadiumPluginInterface.hpp>

//#include <Core/Asset/Camera.hpp>
//#include <Core/Utils/StringUtils.hpp>
//#include <Engine/Scene/CameraComponent.hpp>
//#include <Engine/Scene/SkeletonBasedAnimationSystem.hpp>
//#include <Engine/Scene/SystemDisplay.hpp>

//#include <QColorDialog>
//#include <QComboBox>
//#include <QFileDialog>
//#include <QPushButton>
//#include <QSettings>
//#include <QToolButton>

//using Ra::Engine::Scene::ItemEntry;

namespace Ra {
namespace Gui {

using namespace Core::Utils; // log

MainWindow::MainWindow( QWidget* parent ) : MainWindowInterface( parent ) {
    // Note : at this point most of the components (including the Engine) are
    // not initialized. Listen to the "started" signal.

    setupUi( this );

    m_viewer = new Viewer();
    // Registers the application dependant camera manipulators
    auto keyMappingManager = Gui::KeyMappingManager::getInstance();
    keyMappingManager->addListener( Gui::FlightCameraManipulator::configureKeyMapping );

    connect( m_viewer, &Viewer::glInitialized, this, &MainWindow::onGLInitialized );
    connect( m_viewer, &Viewer::rendererReady, this, &MainWindow::onRendererReady );

    m_viewer->setObjectName( QStringLiteral( "m_viewer" ) );

    QWidget* viewerwidget = QWidget::createWindowContainer( m_viewer );
    //  viewerwidget->setMinimumSize( QSize( 800, 600 ) );
    viewerwidget->setAutoFillBackground( false );

    setCentralWidget( viewerwidget );

    createConnections();

    // load default color from QSettings
    updateBackgroundColor();
}

MainWindow::~MainWindow() {
    // Child QObjects will automatically be deleted
}

void MainWindow::cleanup() {
//    m_viewer->getGizmoManager()->cleanup();
}

void MainWindow::activateTrackballManipulator() {
    // set trackball manipulator (default)
    m_viewer->setCameraManipulator(
        new Gui::TrackballCameraManipulator( *( m_viewer->getCameraManipulator() ) ) );
}

void MainWindow::activateFlightManipulator() {
    // set flightmode manipulator
    m_viewer->setCameraManipulator(
        new Gui::FlightCameraManipulator( *( m_viewer->getCameraManipulator() ) ) );
}

// Connection to gizmos must be done after GL is initialized
void MainWindow::createConnections() {
    // Loading setup.
//    connect( this, &MainWindow::fileLoading, mainApp, &Ra::Gui::BaseApplication::loadFile );

    // Connect picking results (TODO Val : use events to dispatch picking directly)
//    connect( m_viewer, &Viewer::toggleBrushPicking, this, &MainWindow::toggleCirclePicking );
//    connect( m_viewer, &Viewer::rightClickPicking, this, &MainWindow::handlePicking );
}

Viewer* MainWindow::getViewer() {
    return m_viewer;
}

Gui::SelectionManager* MainWindow::getSelectionManager() {
    return nullptr;
//    return m_selectionManager;
}

Gui::Timeline* MainWindow::getTimeline() {
    return nullptr;
//    return m_timeline;
}

void Gui::MainWindow::toggleCirclePicking( bool on ) {
    centralWidget()->setMouseTracking( on );
}

void MainWindow::closeEvent( QCloseEvent* event ) {
    emit closed();
    event->accept();
}


void MainWindow::updateBackgroundColor( QColor c ) {
    // FIXME : sometime, settings does not define colors but Qt found one ....
    QSettings settings;
    // Get or set color from/to settings
    if ( !c.isValid() )
    {
        // get the default color or an already existing one
        auto defColor = Core::Utils::Color::linearRGBTosRGB( m_viewer->getBackgroundColor() );
        auto bgk      = QColor::fromRgb(
            defColor.rgb()[0] * 255, defColor.rgb()[1] * 255, defColor.rgb()[2] * 255 );
        c = settings.value( "colors/background", bgk ).value<QColor>();
    }
    else
    { settings.setValue( "colors/background", c ); }

    // update the color of the button
    QString qss = QString( "background-color: %1" ).arg( c.name() );
//    m_currentColorButton->setStyleSheet( qss );

    // update the background coolor of the viewer
    auto bgk = Core::Utils::Color::sRGBToLinearRGB( Core::Utils::Color(
        Scalar( c.redF() ), Scalar( c.greenF() ), Scalar( c.blueF() ), Scalar( 0 ) ) );
    m_viewer->setBackgroundColor( bgk );
}

//void Gui::MainWindow::setROVisible( Core::Utils::Index roIndex, bool visible ) {
//    mainApp->m_engine->getRenderObjectManager()->getRenderObject( roIndex )->setVisible( visible );
//    mainApp->askForUpdate();
//}

void Gui::MainWindow::updateUi( Plugins::RadiumPluginInterface* plugin ) {
}

void MainWindow::onRendererReady() {
//    updateDisplayedTexture();
}

void MainWindow::onFrameComplete() {
}

void MainWindow::addRenderer( const std::string& name,
                              std::shared_ptr<Engine::Rendering::Renderer> e ) {
    int id = m_viewer->addRenderer( e );
    CORE_UNUSED( id );
//    CORE_ASSERT( id == m_currentRendererCombo->count(), "Inconsistent renderer state" );
//    m_currentRendererCombo->addItem( QString::fromStdString( name ) );
}

void MainWindow::resetScene() {
    // Fix issue #378 : ask the viewer to switch back to the default camera
    m_viewer->resetToDefaultCamera();
    // To see why this call is important, please see deleteCurrentItem().
//    m_selectionManager->clear();
//    Engine::RadiumEngine::getInstance()->getEntityManager()->deleteEntities();
    fitCamera();
}

void MainWindow::fitCamera() {
    auto aabb = Engine::RadiumEngine::getInstance()->computeSceneAabb();
    if ( aabb.isEmpty() )
    {
        m_viewer->getCameraManipulator()->resetCamera();
        mainApp->askForUpdate();
    }
    else
    { m_viewer->fitCameraToScene( aabb ); }
}

void MainWindow::prepareDisplay() {
//    m_selectionManager->clear();
//    m_currentShaderBox->setEnabled( false );
//    m_currentShaderBox->setCurrentText( "" );

    if ( m_viewer->prepareDisplay() ) { mainApp->askForUpdate(); }
}

void MainWindow::onGLInitialized() {
    // Connection to gizmos after their creation
//    connect( actionToggle_Local_Global,
//             &QAction::toggled,
//             m_viewer->getGizmoManager(),
//             &GizmoManager::setLocal );
//    connect(
//        this, &MainWindow::selectedItem, m_viewer->getGizmoManager(), &GizmoManager::setEditable );

    // set default renderer once OpenGL is configured
    std::shared_ptr<Engine::Rendering::Renderer> e( new Engine::Rendering::ForwardRenderer() );
    addRenderer( "Forward Renderer", e );
}


} // namespace Gui
} // namespace Ra
