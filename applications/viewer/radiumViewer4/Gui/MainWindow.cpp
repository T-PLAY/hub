#include <Gui/MainWindow.hpp>
#include <MainApplication.hpp>

#include <Engine/Rendering/ForwardRenderer.hpp>
#include <Gui/SelectionManager/SelectionManager.hpp>
#include <Gui/TreeModel/EntityTreeModel.hpp>
#include <Gui/Viewer/CameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <QDockWidget>

namespace Ra {
using namespace Gui;
using namespace Engine;
using namespace Engine::Rendering;

namespace Gui {

MainWindow::MainWindow( QWidget* parent ) :
    MainWindowInterface( parent )
{
    setupUi( this );

    if ( objectName().isEmpty() ) setObjectName( QString::fromUtf8( "RadiumSimpleWindow" ) );

    // Initialize the minimum tools for a Radium-Guibased Application
    m_viewer = std::make_unique<Viewer>();
    connect( m_viewer.get(), &Viewer::glInitialized, this, &MainWindow::onGLInitialized );
    connect( m_viewer.get(), &Viewer::rendererReady, this, &MainWindow::onRendererReady );
    m_viewer->setObjectName( QStringLiteral( "m_viewer" ) );

    // Initialize the scene interactive representation
    m_sceneModel =
        std::make_unique<Ra::Gui::ItemModel>( Ra::Engine::RadiumEngine::getInstance(), this );
    m_selectionManager = std::make_unique<Ra::Gui::SelectionManager>( m_sceneModel.get(), this );

    // initialize Gui for the application
    auto viewerWidget = QWidget::createWindowContainer( m_viewer.get() );
    viewerWidget->setAutoFillBackground( false );
    setCentralWidget( viewerWidget );

    setWindowTitle( QString( "Radium player" ) );

    createConnections();
}

MainWindow::~MainWindow() {
}

Ra::Gui::Viewer* MainWindow::getViewer() {
    return m_viewer.get();
}

Ra::Gui::SelectionManager* MainWindow::getSelectionManager() {
    return m_selectionManager.get();
}

Ra::Gui::Timeline* MainWindow::getTimeline() {
    return nullptr;
}

void MainWindow::updateUi( Ra::Plugins::RadiumPluginInterface* ) {
    // no ui in the simple window, so, nothing to do
}

void MainWindow::onFrameComplete() {}

void MainWindow::addRenderer( const std::string&,
                              std::shared_ptr<Ra::Engine::Rendering::Renderer> e ) {
    m_viewer->addRenderer( e );
}

void MainWindow::prepareDisplay() {
    m_selectionManager->clear();
    if ( m_viewer->prepareDisplay() ) { emit frameUpdate(); }
}

void MainWindow::cleanup() {
    m_viewer.reset( nullptr );
}

void MainWindow::createConnections() {}

void MainWindow::displayHelpDialog() {
    m_viewer->displayHelpDialog();
}

void MainWindow::onGLInitialized() {
    // Connection to gizmos after their creation

    // set default renderer once OpenGL is configured
    std::shared_ptr<Engine::Rendering::Renderer> e( new Engine::Rendering::ForwardRenderer() );
    addRenderer( "Forward Renderer", e );
}

void MainWindow::onRendererReady() {
}

} // namespace Gui
} // namespace Ra
