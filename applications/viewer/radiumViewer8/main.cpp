#include <GuiManager.h>
#include <GuiMainWindow.h>

#include <Engine/RaEngine.hpp>

#include <QApplication>

#include <QOpenGLContext>

#include <Engine/Scene/EntityManager.hpp>

#include <AllPrimitivesComponent.hpp>
#include <minimalapp.hpp>



int main( int argc, char* argv[] ) {

    // Create app and show viewer window
    MinimalApp app( argc, argv );
    app.initialize();

    // process all events so that everithing is initialized
    QApplication::processEvents();

    // Create and initialize entity and component
    /// \todo Create one entity per object, instead of using the big "all primitive component"
//    Ra::Engine::Scene::Entity* e =
//        app.m_engine->getEntityManager()->createEntity( "All Primitives" );
//    AllPrimitivesComponent* c = new AllPrimitivesComponent( e );
//    c->initialize();

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    app.m_viewer->prepareDisplay();

    // Start the app.
    app.m_frameTimer->start();

//    GuiManager guiManager;
//    guiManager.m_engine = app.m_engine;
//    guiManager.m_viewer = app.m_viewer.get();
//    auto* sys = app.m_engine->getSystem("GeometrySystem");
//    guiManager.m_system = sys;
//    guiManager.m_mainWindow = app.m_viewer.getm

    GuiMainWindow w(app.m_engine, app.m_viewer.get());
    w.show();


    return app.exec();
}
