
#pragma once

#include <Net/Utils.hpp>
//#define NOMINMAX

//#ifndef _WINDOWS_
//#define WIN32_LEAN_AND_MEAN
//#include <windows.h>
//#undef WIN32_LEAN_AND_MEAN
//#endif

//#include <winsock2.h>

//#pragma comment(lib, "ws2_32.lib")

//#define WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>
//#include <WS2tcpip.h>
//#include <Windows.h>
//#endif

#include <SceneManager.h>

//#include <QObject>
#include <QApplication>

//#include <FormInputStreamViews.h>

#include <FormStreamViews.h>

#ifdef ENABLE_LOADER
#    include <FormWidgetLoader.h>
#endif
//#include <MinimalComponent.hpp>
//#include <Imagema

#include <Form3DToolBox.h>
#ifdef ENABLE_IMAGE_VIEWER
#    include <FormImageManipulator.h>
#endif

#include <QStackedWidget>

class GuiManager : public QObject
{
    Q_OBJECT

  public:
    explicit GuiManager( QObject* parent = nullptr );

    ~GuiManager();

    void init();
    void clear();

  signals:

  public slots:

    void onRecordLoaderPathLoaded();
    void onSnapshotLoaderPathLoaded();

  public:
  private slots:
    void on_action2D_triggered();
    void on_action3D_triggered();

    void onServerStreamStarted( const std::string& streamName, const std::string& syncStreamName );
    void onServerStreamStopped( const std::string& streamName, const hub::SensorSpec& sensorSpec );
    void onServerDisconnected();
    void onServerConnected();

    void onInit( const std::string& sensorName );

    void onNewAcquisition( const std::string& sensorName, const std::string& sourceType );

    void onSelectedSourceChanged( const std::string& sensorName, const std::string& sourceType );

    void on_checkBox_grid_toggled( bool checked );
    void on_checkBox_trace_toggled( bool checked );
    void on_checkBox_live_toggled( bool checked );
    void on_toolButton_fitScene_clicked();
    void on_toolButton_fitSelected_clicked();

    void on_toolButton_fitTrace_clicked();

    void loadFile( QString path );

    void on_sensorsView_selectionChanged( const QItemSelection& selected,
                                          const QItemSelection& deselected );
    void on_sensorsView_doubleClicked( const QModelIndex& index );

  public:
    QMdiArea* m_mdiArea = nullptr;

    Ra::Engine::RadiumEngine* m_engine  = nullptr;
    Ra::Gui::Viewer* m_viewer           = nullptr;
    Ra::Engine::Scene::System* m_system = nullptr;

    QMainWindow* m_mainWindow       = nullptr;
    QStackedWidget* m_stackedWidget = nullptr;
    QVBoxLayout* m_layout3DView     = nullptr;

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

#ifdef ENABLE_LOADER
    FormWidgetLoader* m_formWidgetLoader = nullptr;
#endif

    FormStreamViews* m_formStreamViews = nullptr;

#ifdef ENABLE_IMAGE_VIEWER
    FormImageManipulator* m_imageManipulator = nullptr;
#endif
    Form3DToolBox* m_3DToolBox            = nullptr;
    QComboBox* m_comboBoxDisplayedTexture = nullptr;

    QTableView* m_sensorsView = nullptr;

    bool m_initialized = false;
};
