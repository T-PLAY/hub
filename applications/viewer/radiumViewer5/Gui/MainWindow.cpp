#include <Gui/MainWindow.hpp>

//#include <MainApplication.hpp>
//#include <BaseApplication.hpp>
#include <Gui/BaseApplication.hpp>

#include <Engine/Rendering/ForwardRenderer.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Gui/Viewer/FlightCameraManipulator.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <QMenuBar>

/// Allow singleton-like access to the main app à la qApp.
#if defined( mainApp )
#    undef mainApp
#endif
#define mainApp ( static_cast<BaseApplication*>( qApp ) )

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
    viewerwidget->setAutoFillBackground( false );

    stackedWidget->setCurrentIndex( 1 );
    verticalLayout_3D->addWidget( viewerwidget );

    m_viewer->prepareDisplay();

    createConnections();

    // load default color from QSettings
    updateBackgroundColor();
}

MainWindow::~MainWindow() {
    // Child QObjects will automatically be deleted
}

void MainWindow::cleanup() {}

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

    // Connect picking results (TODO Val : use events to dispatch picking directly)
}

Viewer* MainWindow::getViewer() {
    return m_viewer;
}

Gui::SelectionManager* MainWindow::getSelectionManager() {
    return nullptr;
}

Gui::Timeline* MainWindow::getTimeline() {
    return nullptr;
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
    if ( !c.isValid() ) {
        // get the default color or an already existing one
        auto defColor = Core::Utils::Color::linearRGBTosRGB( m_viewer->getBackgroundColor() );
        auto bgk      = QColor::fromRgb(
            defColor.rgb()[0] * 255, defColor.rgb()[1] * 255, defColor.rgb()[2] * 255 );
        c = settings.value( "colors/background", bgk ).value<QColor>();
    }
    else { settings.setValue( "colors/background", c ); }

    // update the color of the button
    QString qss = QString( "background-color: %1" ).arg( c.name() );

    // update the background coolor of the viewer
    auto bgk = Core::Utils::Color::sRGBToLinearRGB( Core::Utils::Color(
        Scalar( c.redF() ), Scalar( c.greenF() ), Scalar( c.blueF() ), Scalar( 0 ) ) );
    m_viewer->setBackgroundColor( bgk );
}

// void Gui::MainWindow::setROVisible( Core::Utils::Index roIndex, bool visible ) {
// }

void Gui::MainWindow::updateUi( Plugins::RadiumPluginInterface* plugin ) {}

void MainWindow::onRendererReady() {}

void MainWindow::onFrameComplete() {}

void MainWindow::addRenderer( const std::string& name,
                              std::shared_ptr<Engine::Rendering::Renderer> e ) {
    int id = m_viewer->addRenderer( e );
    CORE_UNUSED( id );
}

void MainWindow::resetScene() {
    // Fix issue #378 : ask the viewer to switch back to the default camera
    m_viewer->resetToDefaultCamera();
    // To see why this call is important, please see deleteCurrentItem().
    fitCamera();
}

void MainWindow::fitCamera() {
    auto aabb = Engine::RadiumEngine::getInstance()->computeSceneAabb();
    if ( aabb.isEmpty() ) {
        m_viewer->getCameraManipulator()->resetCamera();
        mainApp->askForUpdate();
    }
    else { m_viewer->fitCameraToScene( aabb ); }
}

void MainWindow::prepareDisplay() {

    if ( m_viewer->prepareDisplay() ) { mainApp->askForUpdate(); }
}

void MainWindow::on_action2D_triggered() {
    stackedWidget->setCurrentIndex( 0 );
}

void MainWindow::on_action3D_triggered() {
    stackedWidget->setCurrentIndex( 1 );
}

void MainWindow::onGLInitialized() {
    // Connection to gizmos after their creation

    // set default renderer once OpenGL is configured
    std::shared_ptr<Engine::Rendering::Renderer> e( new Engine::Rendering::ForwardRenderer() );
    addRenderer( "Forward Renderer", e );

    m_guiManager.m_mdiArea = mdiArea;
    m_guiManager.m_engine  = mainApp->m_engine;
    m_guiManager.m_viewer  = m_viewer;

    m_guiManager.m_mainWindow    = this;
    m_guiManager.m_stackedWidget = stackedWidget;
    m_guiManager.m_layout3DView  = verticalLayout_3D;

    auto* sys = mainApp->getEngine()->getSystem( "GeometrySystem" );
    assert( sys != nullptr );
    m_guiManager.m_system = sys;

    m_guiManager.init();
}

} // namespace Gui
} // namespace Ra