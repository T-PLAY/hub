#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>
#include <cassert>
#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
#include <MinimalSystem.hpp>
#include <QOpenGLContext>

#include <QEvent>


MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    // Create app and show viewer window
    m_app     = new MinimalApp;
    auto& app = *m_app;

    QWidget * viewerWidget = QWidget::createWindowContainer(app.m_viewer.get());
//    viewerWidget->setAutoFillBackground(false);
//    ui->verticalLayout->addWidget(viewerWidget);
//    ui->stackedWidget->update();
    const int currentIndex = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(1);
    ui->page3D->layout()->addWidget(viewerWidget);
    this->show();

//    app.m_viewer->show();
//    app.m_viewer->resize( { 500, 500 } );
    CORE_ASSERT( app.m_viewer->getContext()->isValid(), "OpenGL was not initialized" );
    // process all events so that everithing is initialized
    QApplication::processEvents();

    // Create one system
    MinimalSystem* sys = new MinimalSystem;
    app.m_engine->registerSystem( "Minimal system", sys );

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* e = app.m_engine->getEntityManager()->createEntity( "Cube" );
    m_comp                       = new MinimalComponent( e, *app.m_engine );
    sys->addComponent( e, m_comp );
    m_comp->initialize();

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    app.m_viewer->prepareDisplay();

    m_comp->updateShader();

    auto keyMappingManager = Ra::Gui::KeyMappingManager::getInstance();
    // Add default manipulator listener
    keyMappingManager->addListener(
        Ra::Gui::RotateAroundCameraManipulator::KeyMapping::configureKeyMapping );

    // Start the app.
    app.m_frame_timer->start();
    app.m_viewer->setCameraManipulator( new Ra::Gui::RotateAroundCameraManipulator(
        *( app.m_viewer->getCameraManipulator() ), app.m_viewer.get() ) );


    ui->stackedWidget->setCurrentIndex(currentIndex);
}

MainWindow::~MainWindow() {
    delete m_app;
}


void MainWindow::on_action2D_triggered()
{
//    ui->page3D->hide();
//    ui->page2D->show();
//    ui->page2D->update();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_action3D_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
//    ui->page2D->hide();
//    ui->page3D->show();
//    ui->stackedWidget->update();
}
