#include <GuiMainWindow.h>
#include <GuiManager.h>

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

    // prepare the viewer to render the scene (i.e. build RenderTechniques for the active renderer)
    app.m_viewer->prepareDisplay();

    // Start the app.
    app.m_frameTimer->start();

    GuiMainWindow w( app.m_engine, app.m_viewer.get() );
    w.show();

    return app.exec();
}