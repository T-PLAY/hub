#include <MainApplication.hpp>

#include <Gui/Utils/KeyMappingManager.hpp>

#include <Gui/MainWindow.hpp>

class MainWindowFactory : public Ra::Gui::BaseApplication::WindowFactory
{
  public:
    using Ra::Gui::BaseApplication::WindowFactory::WindowFactory;
    Ra::Gui::MainWindowInterface* createMainWindow() const override {
        return new Ra::Gui::MainWindow();
    }
};

int main( int argc, char** argv ) {
    Ra::MainApplication app( argc, argv );
    app.initialize( MainWindowFactory() );
    app.setContinuousUpdate( false );
    return app.exec();
}
