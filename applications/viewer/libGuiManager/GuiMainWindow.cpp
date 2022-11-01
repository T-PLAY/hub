#include "GuiMainWindow.h"

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include "./ui_GuiMainWindow.h"
//#include <cassert>
//#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
//#include <MinimalSystem.hpp>
#include <QDoubleSpinBox>
#include <QOpenGLContext>

#include <constants.h>
#include <filesystem>

//#include <FormSensorViews.h>

//#include <Core/Asset/FileLoaderInterface.hpp>
//#include <DicomLoader/DicomLoader.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

//#include <Core/Geometry/MeshPrimitives.hpp>
//#include <Engine/Scene/EntityManager.hpp>
//#include <Engine/Scene/GeometryComponent.hpp>
//#include <Engine/Scene/GeometrySystem.hpp>

//#include <ImplicitVolume.h>

//#include <QEvent>
// the default priority for systems created here.
constexpr int defaultSystemPriority = 1000;

//#define CUBE_VOLUME

GuiMainWindow::GuiMainWindow( Ra::Engine::RadiumEngine* engine,
                              Ra::Gui::Viewer* viewer,
                              QWidget* parent ) :
    QMainWindow( parent ), ui( new Ui::GuiMainWindow ) {
    ui->setupUi( this );


    // configure radium
    // Create app and show viewer window
    assert( engine != nullptr );
    assert( viewer != nullptr );
    m_guiManager.m_engine = engine;
    m_guiManager.m_viewer = viewer;

    QWidget* viewerWidget = QWidget::createWindowContainer( m_guiManager.m_viewer );
    ui->layout3dView->addWidget( viewerWidget );
    this->show();

    CORE_ASSERT( m_guiManager.m_viewer->getContext()->isValid(), "OpenGL was not initialized" );
    // process all events so that everithing is initialized
    QApplication::processEvents();

    // Create one system
    m_guiManager.m_engine->registerSystem(
        "GeometrySystem", new Ra::Engine::Scene::GeometrySystem, defaultSystemPriority );
    auto* sys = m_guiManager.m_engine->getSystem( "GeometrySystem" );

    // Create and initialize entity and component


    // prepare the viewer to render the scene (i.e. build RenderTechniques for the
    // active renderer)
    m_guiManager.m_viewer->prepareDisplay();



    // Start the app.



    m_guiManager.m_mdiArea = ui->mdiArea;
    m_guiManager.m_system = sys;

    m_guiManager.m_mainWindow    = this;
    m_guiManager.m_stackedWidget = ui->stackedWidget;
    m_guiManager.m_layout3DView  = ui->layout3dView;

    m_guiManager.init();

    connect( ui->actionreloadShaders,
             &QAction::triggered,
             m_guiManager.m_viewer,
             &Ra::Gui::Viewer::reloadShaders );





}

GuiMainWindow::~GuiMainWindow() {
    std::cout << "[GuiMainWindow] ~GuiMainWindow() start" << std::endl;

    m_guiManager.clear();

    delete ui;
    std::cout << "[GuiMainWindow] ~GuiMainWindow() end" << std::endl;
}
