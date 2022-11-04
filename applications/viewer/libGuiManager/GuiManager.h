
#pragma once

#include <QApplication>
#include <QStackedWidget>

#include <Net/Utils.hpp>

#include <Form3DToolBox.h>
#include <FormStreamViews.h>
#include <SceneManager.h>

#ifdef ENABLE_LOADER
#    include <FormWidgetLoader.h>
#endif

#ifdef ENABLE_IMAGE_VIEWER
#    include <FormImageManipulator.h>
#endif

class GuiManager : public QObject
{
    Q_OBJECT

  public:
    explicit GuiManager( QObject* parent = nullptr );

    ~GuiManager();

    void init();
    void clear();

  public:
    QMdiArea* m_mdiArea = nullptr;

    Ra::Engine::RadiumEngine* m_engine  = nullptr;
    Ra::Gui::Viewer* m_viewer           = nullptr;
    Ra::Engine::Scene::System* m_system = nullptr;

    QMainWindow* m_mainWindow       = nullptr;
    QStackedWidget* m_stackedWidget = nullptr;
    QVBoxLayout* m_layout3DView     = nullptr;

    ////////////////////////////////////////////////////////////////////////////////

  private:
    QDockWidget* m_dockLeft   = nullptr;
    QDockWidget* m_dockTop    = nullptr;
    QDockWidget* m_dockRight  = nullptr;
    QDockWidget* m_dockBottom = nullptr;

    QToolBar* m_toolBarTopLeft  = nullptr;
    QToolBar* m_toolBarTopRight = nullptr;
    QAction* m_action3D         = nullptr;
    QAction* m_action2D         = nullptr;
    QAction* m_actionSettings   = nullptr;
    QAction* m_actionAddSources = nullptr;

    SceneManager m_sceneManager;

    Form3DToolBox* m_3DToolBox            = nullptr;
    QComboBox* m_comboBoxDisplayedTexture = nullptr;
    FormStreamViews* m_formStreamViews = nullptr;

#ifdef ENABLE_IMAGE_VIEWER
    FormImageManipulator* m_imageManipulator = nullptr;
#endif

    QTableView* m_sensorsView = nullptr;

#ifdef ENABLE_LOADER
    FormWidgetLoader* m_formWidgetLoader = nullptr;
#endif

    bool m_initialized = false;

  private slots:
    // tool bar
    void on_action2D_triggered();
    void on_action3D_triggered();

    // tool box
    void on_checkBox_grid_toggled( bool checked );
    void on_checkBox_trace_toggled( bool checked );
    void on_checkBox_live_toggled( bool checked );
    void on_toolButton_fitScene_clicked();

    // server view
//    void onServerStreamStarted( const std::string& streamName, const std::string& syncStreamName );
//    void onServerStreamStarted( const std::string& streamName, const hub::SensorSpec& sensorSpec );
//    void onServerStreamStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );
//    void onServerDisconnected();
//    void onServerConnected();

    bool onNewStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onDelStreamer( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onServerConnected(const std::string& ipv4, int port);
    void onServerDisconnected(const std::string& ipv4, int port);
    void onNewAcquisition( const std::string& sensorName, const hub::Acquisition& acq );

//    void on_toolButton_fitSelected_clicked();
//    void loadFile( QString path );

    // stream views
    void on_sensorsView_selectionChanged( const QItemSelection& selected,
                                          const QItemSelection& deselected );
    void on_sensorsView_doubleClicked( const QModelIndex& index );
};
