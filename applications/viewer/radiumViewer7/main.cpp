#include <MainApplication.hpp>

#include <Gui/Utils/KeyMappingManager.hpp>

#include <Gui/MainWindow.hpp>

#include <Core/Asset/FileLoaderInterface.hpp>
#include <DicomLoader/DicomLoader.hpp>
#include <Engine/RadiumEngine.hpp>


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

//    app.m_mainWindow->update();
//    app.m_engine->registerFileLoader(std::shared_ptr<Ra::Core::Asset::FileLoaderInterface>(new Ra::IO::DicomLoader()));
//    app.m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");
//    app.m_engine->loadFile("/home/gauthier/document/projet/Clone_css/resourcesCStrike/maps/de_aztec/de_aztec.obj");
//    app.askForUpdate();
    app.m_mainWindow->prepareDisplay();

    return app.exec();
}
