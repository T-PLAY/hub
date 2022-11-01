#include "GuiManager.h"

#include <IO/Stream.hpp>

#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

//#include "./ui_MainWindow.h"
//#include <cassert>
//#include <Engine/Rendering/ForwardRenderer.hpp>

#include <MinimalComponent.hpp>
//#include <MinimalSystem.hpp>
#include <QDoubleSpinBox>
#include <QOpenGLContext>

#include <constants.h>
#include <filesystem>

#include <FormStreamViews.h>

#include <Core/Asset/FileLoaderInterface.hpp>
//#include <DicomLoader/DicomLoader.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

//#include <FormInputStreamViews.h>
#ifdef ENABLE_LOADER
#    include <FormWidgetLoader.h>
#endif

#include <Gui/TreeModel/EntityTreeModel.hpp>

#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/Component.hpp>
#include <QItemSelectionModel>

#include <QHeaderView>
#include <QPushButton>
#include <QTableView>

GuiManager::GuiManager( QObject* parent ) : QObject { parent } {}

// GuiManager::GuiManager()
//{

//}

GuiManager::~GuiManager() {
    std::cout << "[GuiManager] ~GuiManager() start" << std::endl;
    delete m_formStreamViews;
    m_formStreamViews = nullptr;

#ifdef ENABLE_IMAGE_VIEWER
    delete m_imageManipulator;
    m_imageManipulator = nullptr;
#endif

#ifdef ENABLE_LOADER
    delete m_formWidgetLoader;
    m_formWidgetLoader = nullptr;
#endif

    m_sceneManager.clear();

    std::cout << "[GuiManager] ~GuiManager() end" << std::endl;
}

void GuiManager::clear() {
    m_sceneManager.clear();
}

#include <QAction>
#include <QDockWidget>
#include <QToolBar>

void GuiManager::init() {
    assert( !m_initialized );
    assert( m_mainWindow != nullptr );
    m_dockLeft = new QDockWidget( m_mainWindow );
#ifdef ENABLE_IMAGE_VIEWER
    m_dockRight = new QDockWidget( m_mainWindow );
#endif
    m_dockBottom = new QDockWidget( m_mainWindow );
    assert( m_dockLeft != nullptr );
    assert( m_dockBottom != nullptr );

    assert( m_mdiArea != nullptr );

    assert( m_engine != nullptr );
    assert( m_viewer != nullptr );
    assert( m_system != nullptr );

    assert( m_stackedWidget != nullptr );
    assert( m_layout3DView != nullptr );

    //////////////////////////////////////// TOOL BAR

    m_actionAddSources = new QAction( "Add Source", m_mainWindow );
    m_toolBarTopLeft   = new QToolBar( m_mainWindow );
    m_toolBarTopLeft->setMinimumHeight( 40 );
    m_toolBarTopLeft->setMovable( false );
    m_toolBarTopLeft->addAction( m_actionAddSources );
    m_mainWindow->addToolBar( m_toolBarTopLeft );

    m_toolBarTopRight = new QToolBar( m_mainWindow );
    m_toolBarTopRight->setMovable( false );
    m_toolBarTopRight->setLayoutDirection( Qt::LayoutDirection::RightToLeft );
    m_actionSettings = new QAction( "Settings", m_mainWindow );
    m_toolBarTopRight->addAction( m_actionSettings );
    m_action3D = new QAction( "3D", m_mainWindow );
    QObject::connect( m_action3D, &QAction::triggered, this, &GuiManager::on_action3D_triggered );
    m_toolBarTopRight->addAction( m_action3D );
    m_toolBarTopRight->addSeparator();
    m_action2D = new QAction( "2D", m_mainWindow );
    QObject::connect( m_action2D, &QAction::triggered, this, &GuiManager::on_action2D_triggered );
    m_toolBarTopRight->addAction( m_action2D );
    m_mainWindow->addToolBar( m_toolBarTopRight );

    //////////////////////////////////////// 3D VIEW

    m_3DToolBox = new Form3DToolBox( m_mainWindow );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::pushButton_fitScene_clicked,
                      this,
                      &GuiManager::on_toolButton_fitScene_clicked );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::pushButton_fitSelected_clicked,
                      this,
                      &GuiManager::on_toolButton_fitSelected_clicked );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::pushButton_fitTrace_clicked,
                      this,
                      &GuiManager::on_toolButton_fitTrace_clicked );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::checkBox_showTrace_toggled,
                      this,
                      &GuiManager::on_checkBox_trace_toggled );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::checkBox_live_toggled,
                      this,
                      &GuiManager::on_checkBox_live_toggled );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::checkBox_showGrid_toggled,
                      this,
                      &GuiManager::on_checkBox_grid_toggled );

    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::doubleSpinBox_tune_valueChanged,
                      &m_sceneManager,
                      &SceneManager::on_tune_valueChanged );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::doubleSpinBox_tune2_valueChanged,
                      &m_sceneManager,
                      &SceneManager::on_tune2_valueChanged );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::doubleSpinBox_tune3_valueChanged,
                      &m_sceneManager,
                      &SceneManager::on_tune3_valueChanged );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::doubleSpinBox_tune4_valueChanged,
                      &m_sceneManager,
                      &SceneManager::on_tune4_valueChanged );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::comboBox_palettes_currentIndexChanged,
                      &m_sceneManager,
                      &SceneManager::on_palette_valueChanged );

    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::checkBox_debug_toggled,
                      m_viewer,
                      &Ra::Gui::Viewer::enableDebugDraw );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::checkBox_transparency_toggled,
                      &m_sceneManager,
                      &SceneManager::on_setTransparency );

    m_viewer->enableDebugDraw( 0 );

    m_layout3DView->insertWidget( 0, m_3DToolBox );

    //////////////////////////////////////// RIGHT
    //////////////////////////////////////////////////////

#ifdef ENABLE_IMAGE_VIEWER

    m_dockRight->setMinimumWidth( 500 );

    QWidget* rightContainer = new QWidget;
    m_dockRight->setWidget( rightContainer );

    QVBoxLayout* vLayout = new QVBoxLayout( rightContainer );
    m_dockRight->setLayout( vLayout );

    m_imageManipulator = new FormImageManipulator;
    vLayout->addWidget( m_imageManipulator );

#endif

    //////////////////////////////////////// TOP
    //////////////////////////////////////////////////////////
    assert( m_mdiArea != nullptr );

    //////////////////////////////////////// LEFT
    /////////////////////////////////////////////////////////////////

    m_formStreamViews = new FormStreamViews( m_dockLeft );

    m_dockLeft->setWidget( m_formStreamViews );
    QObject::connect( m_formStreamViews,
                      &FormStreamViews::streamingStarted,
                      this,
                      &GuiManager::onServerStreamStarted );

    QObject::connect( m_formStreamViews,
                      &FormStreamViews::streamingStopped,
                      this,
                      &GuiManager::onServerStreamStopped );

    QObject::connect( m_formStreamViews,
                      &FormStreamViews::serverDisconnected,
                      this,
                      &GuiManager::onServerDisconnected );
    QObject::connect( m_formStreamViews,
                      &FormStreamViews::serverConnected,
                      this,
                      &GuiManager::onServerConnected );

    //////////////////////////////////////// BOTTOM
    //////////////////////////////////////////////////////////////

    QWidget* bottomContainer = new QWidget( m_dockBottom );
    m_dockBottom->setWidget( bottomContainer );

    QHBoxLayout* hLayout = new QHBoxLayout( bottomContainer );

    ////////////// SENSORS VIEW

    m_sensorsView = new QTableView( bottomContainer );
    m_sensorsView->setModel( &m_sceneManager.m_sensorModel );
    m_sensorsView->show();
    hLayout->addWidget( m_sensorsView );

    QObject::connect( m_sensorsView->selectionModel(),
                      &QItemSelectionModel::selectionChanged,
                      this,
                      &GuiManager::on_sensorsView_selectionChanged );
    QObject::connect( m_sensorsView,
                      &QTableView::doubleClicked,
                      this,
                      &GuiManager::on_sensorsView_doubleClicked );

    m_sensorsView->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

    m_sceneManager.m_sensorsView = m_sensorsView;

    ////////////// LOADER

#ifdef ENABLE_LOADER
    m_formWidgetLoader = new FormWidgetLoader(
        m_formStreamViews->getIpv4(), m_formStreamViews->getPort(), bottomContainer );
    m_formWidgetLoader->setEnabled( false );
    hLayout->addWidget( m_formWidgetLoader );
#endif

    //////////////////////////////////////// INIT DOCKERS

    m_mainWindow->addDockWidget( Qt::DockWidgetArea::LeftDockWidgetArea, m_dockLeft );
    m_mainWindow->setCorner( Qt::Corner::BottomLeftCorner, Qt::DockWidgetArea::LeftDockWidgetArea );
#ifdef ENABLE_IMAGE_VIEWER
    m_mainWindow->addDockWidget( Qt::DockWidgetArea::RightDockWidgetArea, m_dockRight );
#endif
    m_mainWindow->addDockWidget( Qt::DockWidgetArea::BottomDockWidgetArea, m_dockBottom );

    //////////////////////////////////////// INIT 3D ENVIRONMENT
    // Create and initialize entity and component

    m_sceneManager.m_engine  = m_engine;
    m_sceneManager.m_sys     = m_system;
    m_sceneManager.m_viewer  = m_viewer;
    m_sceneManager.m_mdiArea = m_mdiArea;
#ifdef ENABLE_IMAGE_VIEWER
    m_sceneManager.m_imageManipulator = m_imageManipulator;
#endif
    m_sceneManager.init();

    ///////////////////////////////////////////////////////

    m_comboBoxDisplayedTexture = m_3DToolBox->m_comboBoxDisplayedTexture;
    auto texs                  = m_viewer->getRenderer()->getAvailableTextures();
    for ( const auto& tex : texs ) {
        m_comboBoxDisplayedTexture->addItem( tex.c_str() );
    }
    QObject::connect( m_comboBoxDisplayedTexture,
                      &QComboBox::currentTextChanged,
                      m_viewer,
                      &Ra::Gui::Viewer::displayTexture );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::pushButton_reloadShaders_clicked,
                      m_viewer,
                      &Ra::Gui::Viewer::reloadShaders );

    m_initialized = true;
}

// void GuiManager::incIter()
//{
// }
// void GuiManager::onRecordLoaderPathUnloaded() {
//}

void GuiManager::onRecordLoaderPathLoaded() {}

void GuiManager::onSnapshotLoaderPathLoaded() {}

void GuiManager::on_action2D_triggered() {
    m_stackedWidget->setCurrentIndex( 0 );
}

void GuiManager::on_action3D_triggered() {
    m_stackedWidget->setCurrentIndex( 1 );
}

void GuiManager::onServerStreamStarted( const std::string& streamName,
                                        const std::string& syncStreamName ) {
    std::cout << "[GuiManager] onServerStreamStarted(" << streamName << ", " << syncStreamName
              << ")" << std::endl;

    const auto& ipv4 = m_formStreamViews->getIpv4();
    const auto port  = m_formStreamViews->getPort();

    m_sceneManager.addSensor(
        hub::io::InputStream( streamName, syncStreamName, hub::net::ClientSocket( ipv4, port ) ),
        streamName );
}

void GuiManager::onServerStreamStopped( const std::string& streamName,
                                        const hub::SensorSpec& sensorSpec ) {
    std::cout << "[GuiManager] onServerStreamStopped()" << std::endl;

    m_sceneManager.delSensor( streamName );
}

void GuiManager::onServerConnected() {
#ifdef ENABLE_LOADER
    m_formWidgetLoader->setEnabled( true );
#endif
}

void GuiManager::onServerDisconnected() {
#ifdef ENABLE_LOADER
    m_formWidgetLoader->setEnabled( false );
#endif
}

void GuiManager::onInit( const std::string& streamName ) {}

// void GuiManager::onInitPose()
//{
//}

// void GuiManager::onInitScan()
//{
//}

void GuiManager::onNewAcquisition( const std::string& streamName, const std::string& sourceType ) {}

void GuiManager::onSelectedSourceChanged( const std::string& streamName,
                                          const std::string& sourceType ) {}

// void GuiManager::onUpdatePose()
//{
//}

// void GuiManager::onUpdateScan()
//{
//}

// void GuiManager::on_comboBox_scan_currentTextChanged(const QString& sourceType)
//{
// }

// void GuiManager::on_comboBox_pose_currentTextChanged(const QString& sourceType)
//{
// }

// void GuiManager::onNewScanAcquisition()
//{
// }

// void GuiManager::onNewPoseAcquisition()
//{
// }

void GuiManager::on_checkBox_grid_toggled( bool checked ) {
    m_sceneManager.m_sceneComponent->setVisible( checked );
}

void GuiManager::on_checkBox_trace_toggled( bool checked ) {
    m_sceneManager.enableTrace( checked );
}

void GuiManager::on_checkBox_live_toggled( bool checked ) {
    m_sceneManager.enableLive( checked );
}

void GuiManager::on_toolButton_fitScene_clicked() {
    m_viewer->fitCamera();
}

void GuiManager::on_toolButton_fitSelected_clicked() {
    const auto& indexes = m_sensorsView->selectionModel()->selectedIndexes();

    if ( !indexes.empty() ) {

        const auto& current = indexes.first();

        const int row = current.row();

        std::cout << "[GuiManager] row clicked " << row << std::endl;

        const std::string streamName = m_sensorsView->model()
                                           ->data( m_sensorsView->model()->index( row, 0 ) )
                                           .toString()
                                           .toStdString();
        m_sceneManager.fitView( streamName );
    }
}

#include <Core/Utils/Color.hpp>
void GuiManager::on_toolButton_fitTrace_clicked() {}

void GuiManager::loadFile( QString path ) {
    std::string filename = path.toLocal8Bit().data();
    bool res             = m_engine->loadFile( filename );

    if ( !res ) { std::cout << "Aborting file loading !"; }

    m_engine->releaseFile();
}

// void GuiManager::on_sensorsView_clicked(const QModelIndex& index)
//{
//}

void GuiManager::on_sensorsView_selectionChanged( const QItemSelection& selected,
                                                  const QItemSelection& deselected ) {
    const auto& indexes = m_sensorsView->selectionModel()->selectedIndexes();

    if ( indexes.empty() ) {
        const auto& index = deselected.indexes().first();
        const int row     = index.row();

        const std::string streamName = m_sensorsView->model()
                                           ->data( m_sensorsView->model()->index( row, 0 ) )
                                           .toString()
                                           .toStdString();
        std::cout << "[GuiManager] stream " << streamName << " deselected" << std::endl;

#ifdef ENABLE_IMAGE_VIEWER
        m_sceneManager.detachSensorFromImageManipulator( streamName );
#endif
    }
    else {

        const auto& current = indexes.first();
        const int row       = current.row();

        const std::string streamName = m_sensorsView->model()
                                           ->data( m_sensorsView->model()->index( row, 0 ) )
                                           .toString()
                                           .toStdString();
        std::cout << "[GuiManager] stream " << streamName << " clicked" << std::endl;

#ifdef ENABLE_IMAGE_VIEWER
        m_sceneManager.attachSensorFromImageManipulator( streamName );
#endif
    }
}

void GuiManager::on_sensorsView_doubleClicked( const QModelIndex& index ) {

    const int row = index.row();

    std::cout << "[GuiManager] row clicked " << row << std::endl;

    const std::string streamName = m_sensorsView->model()
                                       ->data( m_sensorsView->model()->index( row, 0 ) )
                                       .toString()
                                       .toStdString();
    m_sceneManager.fitView( streamName );
}
