#ifndef RADIUMENGINE_MAINWINDOW_HPP
#define RADIUMENGINE_MAINWINDOW_HPP

#include <Gui/MainWindowInterface.hpp>
//#include <Gui/RaGui.hpp>
//#include <Gui/SelectionManager/SelectionManager.hpp>
//#include <Gui/TimerData/FrameTimerData.hpp>
//#include <Gui/TreeModel/EntityTreeModel.hpp>

#include "ui_MainWindow.h"
#include <QMainWindow>

#include <QEvent>
#include <qdebug.h>

#include <GuiManager.h>

namespace Ra {
namespace Gui {
class Viewer;
} // namespace Gui
} // namespace Ra

namespace Ra {
namespace Plugins {
class RadiumPluginInterface;
}
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
    Viewer* getViewer() override;

    /// Access the selection manager.
    Gui::SelectionManager* getSelectionManager() override;

    /// Access the timeline.
    Gui::Timeline* getTimeline() override;

    /// Update the ui from the plugins loaded.
    void updateUi( Plugins::RadiumPluginInterface* plugin ) override;

    /// Update the UI ( most importantly gizmos ) to the modifications of the engine
    void onFrameComplete() override;

    /// Add a renderer in the application: UI, viewer.
    void addRenderer( const std::string& name,
                      std::shared_ptr<Engine::Rendering::Renderer> e ) override;

  public slots:

    // Gizmo buttons slots
    //    void gizmoShowNone();
    //    void gizmoShowTranslate();
    //    void gizmoShowRotate();
    //    void gizmoShowScale();

    // Keymapping configuration tool
    //    void reloadConfiguration();
    //    void loadConfiguration();

    /// Slot for the tree view checkboxes
    //    void setROVisible( Core::Utils::Index roIndex, bool visible );

    /// Reset the camera to see all visible objects
    void fitCamera();
    /// clear selection, fit camera and update material name in ui
    void prepareDisplay() override;

    void on_action2D_triggered();
    void on_action3D_triggered();

    /// Slot for the "edit" button.
    //    void editRO();

    /// Cleanup resources.
    void cleanup() override;

    /// Show or hide all render objects
    //    void showHideAllRO();

  signals:
    /// Emitted when the frame loads
    void fileLoading( const QString path );

    /// Emitted when the user changes the timer box ("Frame average over count")
    void framescountForStatsChanged( int count );

    /// Emitted when a new item is selected. An invalid entry is sent when no item is selected.
    //    void selectedItem( const Engine::Scene::ItemEntry& entry );

  private:
    /// Connect qt signals and slots. Called once by the constructor.
    void createConnections();

    // Updates the Tracking info according to the selected feature, if any.
    void updateTrackedFeatureInfo();

    virtual void closeEvent( QCloseEvent* event ) override;

    /// Update displayed texture according to the current renderer
    //    void updateDisplayedTexture();

    /// Set the background color (updates viewer). If c is invalid, the color is fetch from
    /// QSettings.
    void updateBackgroundColor( QColor c = QColor() );

    /// After loading a file, set the first camera loaded (if any) as the active camera.
    /// if multiple files are loaded, use the first camera of the first loaded file
    //    void activateCamera( const std::string& sceneName );

  private slots:
    // Slot to init renderers once gl is ready
    void onGLInitialized();

    /// Slot to accept a new renderer
    void onRendererReady();

    /// Exports the mesh of the currently selected object to a file.
    //    void exportCurrentMesh();

    /// Remove the currently selected item (entity, component or ro)
    //    void deleteCurrentItem();

    /// Clears all entities and resets the camera.
    void resetScene();

    /// Allow to pick using a circle
    void toggleCirclePicking( bool on );

    /// set the current background color
    //    void on_m_currentColorButton_clicked();

    /// activate trackball camera manipulator
    void activateTrackballManipulator();

    /// activate flight-mode camera manipulator
    void activateFlightManipulator();

    /// Allow to manage registered plugin paths
    /// @todo : for now, only add a new path ... make full management available
    //    void addPluginPath();
    /// Remove all registered plugin directories
    //    void clearPluginPaths();

  private slots:
    /// \name Time events
    /// \{

  private:
    /// Viewer widget
    Ra::Gui::Viewer* m_viewer { nullptr };

    /// Observers id for engine event
    ///@{
    //    int m_entityAddObserverId {-1};
    //    int m_entityRemoveObserverId {-1};
    //    int m_componentAddObserverId {-1};
    //    int m_componentRemoveObserverId {-1};
    //    int m_roAddObserverId {-1};
    //    int m_roRemoveObserverId {-1};
    ///@}

    GuiManager m_guiManager;
};

} // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_MAINWINDOW_HPP
