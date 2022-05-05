#ifndef RADIUMENGINE_MAINWINDOW_HPP
#define RADIUMENGINE_MAINWINDOW_HPP

#include <Gui/MainWindowInterface.hpp>
#include <Gui/RaGui.hpp>
#include <Gui/SelectionManager/SelectionManager.hpp>
#include <Gui/TimerData/FrameTimerData.hpp>
#include <Gui/TreeModel/EntityTreeModel.hpp>

//#include "ui_MainWindow.h"
#include "ui_MainWindow.h"
#include <QMainWindow>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE



#include <QEvent>
#include <qdebug.h>

namespace Ra {
namespace Gui {
class Viewer;
} // namespace Gui
} // namespace Ra

namespace Ra {
namespace Gui {

/// This class manages most of the GUI of the application :
/// top menu, side toolbar and side dock.
class MainWindow : public Ra::Gui::MainWindowInterface, private Ui::MainWindow
{
    Q_OBJECT

  public:
    /// Constructor and destructor.
    explicit MainWindow( QWidget* parent = nullptr );
    virtual ~MainWindow();

    /// Access the viewer, i.e. the rendering widget.
    Ra::Gui::Viewer* getViewer() override;

    /// Access the selection manager.
    Ra::Gui::SelectionManager* getSelectionManager() override;

    /// Access the timeline.
    Ra::Gui::Timeline* getTimeline() override;

    /// Update the ui from the plugins loaded.
    void updateUi( Ra::Plugins::RadiumPluginInterface* plugin ) override;

    /// Update the UI ( most importantly gizmos ) to the modifications of the
    /// engine/
    void onFrameComplete() override;

    /// Add render in the application: UI, viewer - this method does not associate a control panel.
    void addRenderer( const std::string& name,
                      std::shared_ptr<Ra::Engine::Rendering::Renderer> e ) override;

  public slots:
    /**
     * Called when a scene is ready to display to parameterize the application window and the
     * viewer.
     */
    void prepareDisplay() override;

    /// Cleanup resources.
    void cleanup() override;

    // Display help dialog about Viewer key-bindings
    void displayHelpDialog() override;

  signals:
    /// Emitted when frame must be updated
    void frameUpdate();

private slots:
    // Slot to init renderers once gl is ready
    void onGLInitialized();

    /// Slot to accept a new renderer
    void onRendererReady();





  private:
    /// create the UI connections
    void createConnections();

    /// viewer widget
    std::unique_ptr<Ra::Gui::Viewer> m_viewer;

    /// Stores and manages the current selection.
    /// Even if no selection is provided by this application, used plugins require this
    std::unique_ptr<Ra::Gui::SelectionManager> m_selectionManager;

    /// Stores the internal model of engine objects for selection and visibility.
    std::unique_ptr<Ra::Gui::ItemModel> m_sceneModel;

//    Ui::MainWindow *ui;
};

} // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_MAINWINDOW_HPP
