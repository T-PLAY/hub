#include "GuiManager.h"

#include <filesystem>

#include <QAction>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QOpenGLContext>
#include <QPushButton>
#include <QTableView>
#include <QToolBar>

#include <Core/Asset/FileLoaderInterface.hpp>
#include <Core/Utils/Color.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/Component.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/GeometrySystem.hpp>
#include <Gui/TreeModel/EntityTreeModel.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include <IO/Stream.hpp>

#include <FormStreamViews.h>
#include <MinimalComponent.hpp>

#ifdef ENABLE_LOADER
#    include <FormWidgetLoader.h>
#endif

GuiManager::GuiManager( QObject* parent ) : QObject { parent } {}

GuiManager::~GuiManager() {
    std::cout << "[GuiManager] ~GuiManager() start" << std::endl;

#ifdef ENABLE_IMAGE_VIEWER
    delete m_imageManipulator;
    m_imageManipulator = nullptr;
#endif

#ifdef ENABLE_LOADER
    delete m_formWidgetLoader;
    m_formWidgetLoader = nullptr;
#endif

    m_sceneManager.clear();

    delete m_formStreamViews;
    m_formStreamViews = nullptr;

    std::cout << "[GuiManager] ~GuiManager() end" << std::endl;
}

void GuiManager::clear() {
    m_sceneManager.clear();
}

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

    //////////////////////////////////////// TOOL BOX

    m_3DToolBox = new Form3DToolBox( m_mainWindow );
    QObject::connect( m_3DToolBox,
                      &Form3DToolBox::pushButton_fitScene_clicked,
                      this,
                      &GuiManager::on_toolButton_fitScene_clicked );
    //    QObject::connect( m_3DToolBox,
    //                      &Form3DToolBox::pushButton_fitSelected_clicked,
    //                      this,
    //                      &GuiManager::on_toolButton_fitSelected_clicked );
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

    m_layout3DView->insertWidget( 0, m_3DToolBox );

    //////////////////////////////////////// 3D VIEW

    m_viewer->enableDebugDraw( 0 );

    //////////////////////////////////////// RIGHT DOCKER

#ifdef ENABLE_IMAGE_VIEWER

    m_dockRight->setMinimumWidth( 500 );

    QWidget* rightContainer = new QWidget;
    m_dockRight->setWidget( rightContainer );

    QVBoxLayout* vLayout = new QVBoxLayout( rightContainer );
    m_dockRight->setLayout( vLayout );

    m_imageManipulator = new FormImageManipulator;
    vLayout->addWidget( m_imageManipulator );

#endif

    //////////////////////////////////////// LEFT DOCKER

    m_formStreamViews = new FormStreamViews( m_dockLeft );

    m_dockLeft->setWidget( m_formStreamViews );
    QObject::connect(
        m_formStreamViews, &FormStreamViews::startStream, this, &GuiManager::onStartStream );

    QObject::connect(
        m_formStreamViews, &FormStreamViews::stopStream, this, &GuiManager::onStopStream );

    QObject::connect( m_formStreamViews,
                      &FormStreamViews::serverDisconnected,
                      this,
                      &GuiManager::onServerDisconnected );
    QObject::connect( m_formStreamViews,
                      &FormStreamViews::serverConnected,
                      this,
                      &GuiManager::onServerConnected );
    //    QObject::connect(
    //        m_formStreamViews, &FormStreamViews::newAcquisition, this,
    //        &GuiManager::onNewAcquisition );

    //    auto _onNewStreamer = [this]( const std::string& streamerName,
    //                               const hub::SensorSpec& sensorSpec ) {
    //        return onNewStreamer( streamerName,
    //                       sensorSpec ); // no needs of gui main thread, direct call to update 3D
    //                       scene
    ////        emit newStreamer( streamerName, sensorSpec ); // gui update needs gui main thread
    ////        return true;
    //    };
    //    auto _onDelStreamer = [this]( const std::string& streamerName,
    //                               const hub::SensorSpec& sensorSpec ) {
    //        onDelStreamer( streamerName, sensorSpec );
    //    };
    //    auto _onServerConnected = [this]( const std::string& ipv4, int port ) {
    //        onServerConnected( ipv4, port );
    //    };
    //    auto _onServerDisconnected = [this]( const std::string& ipv4, int port ) {
    //        onServerDisconnected( ipv4, port );
    //    };
    //        auto _onNewAcquisition = [=]( const std::string& streamerName,
    //                                      const hub::Acquisition& acq ) {
    //            onNewAcquisition( streamerName, acq );
    //            emit newAcquisition( streamerName, acq );
    //        };
    auto _onNewAcquisition = [this]( const std::string& streamName,
                                     const hub::Acquisition& acq ) {
        onNewAcquisition( streamName, acq );
    };
    auto _newStreamerAdded = [this]( const std::string& streamName,
                                     const hub::SensorSpec& ) {
//        return false;
        m_sceneManager.getSensor(streamName);
        return true;
    };
    m_formStreamViews->initViewer( _newStreamerAdded, _onNewAcquisition );

    //////////////////////////////////////// BOTTOM DOCKER

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
    // init scene manager

    m_sceneManager.m_engine  = m_engine;
    m_sceneManager.m_sys     = m_system;
    m_sceneManager.m_viewer  = m_viewer;
    m_sceneManager.m_mdiArea = m_mdiArea;
#ifdef ENABLE_IMAGE_VIEWER
    m_sceneManager.m_imageManipulator = m_imageManipulator;
#endif
    m_sceneManager.init();

    ///////////////////////////////////////////////////////

    m_initialized = true;
}

void GuiManager::on_action2D_triggered() {
    m_stackedWidget->setCurrentIndex( 0 );
}

void GuiManager::on_action3D_triggered() {
    m_stackedWidget->setCurrentIndex( 1 );
}

// void GuiManager::onServerStreamStarted( const std::string& streamName,
//                                         const std::string& syncStreamName ) {
// void GuiManager::onServerStreamStarted( const std::string& streamName,
//                                        const hub::SensorSpec& sensorSpec ) {
//    //    std::cout << "[GuiManager] onServerStreamStarted(" << streamName << ", " <<
//    syncStreamName std::cout << "[GuiManager] onServerStreamStarted(" << streamName << ", " <<
//    sensorSpec << ")"
//              << std::endl;

//    const auto& ipv4 = m_formStreamViews->getIpv4();
//    const auto port  = m_formStreamViews->getPort();

//    //    m_sceneManager.addSensor(
//    //        hub::io::InputStream( streamName, syncStreamName, hub::net::ClientSocket( ipv4, port
//    )
//    //        ), streamName );

//    m_sceneManager.addSensor( streamName, sensorSpec );
//}

// void GuiManager::onServerStreamStopped( const std::string& streamName,
//                                         const hub::SensorSpec& sensorSpec ) {
//     std::cout << "[GuiManager] onServerStreamStopped()" << std::endl;

//    m_sceneManager.delSensor( streamName );
//}

// void GuiManager::onServerConnected() {
//#ifdef ENABLE_LOADER
//     m_formWidgetLoader->setEnabled( true );
//#endif
// }

bool GuiManager::onStartStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string & syncStreamName) {
        std::cout << "[GuiManager] onStartStream(" << streamName << ", " << sensorSpec << ", " << syncStreamName << ")"
                  << std::endl;

//    const auto& ipv4 = m_formStreamViews->getIpv4();
//    const auto port  = m_formStreamViews->getPort();

//    //    m_sceneManager.addSensor(
//    //        hub::io::InputStream( streamName, syncStreamName, hub::net::ClientSocket( ipv4, port
//    )
//    //        ), streamName );


    m_sceneManager.addSensor( streamName, sensorSpec, syncStreamName );
//        return false;
    return true;
}

void GuiManager::onStopStream( const std::string& streamName, const hub::SensorSpec& sensorSpec, const std::string & syncStreamName ) {
        std::cout << "[GuiManager] onStopStream(" << streamName << ", " << sensorSpec << ", " << syncStreamName << ")"
                  << std::endl;

    m_sceneManager.delSensor( streamName, sensorSpec, syncStreamName );
}

void GuiManager::onServerConnected( const std::string& ipv4, int port ) {
#ifdef ENABLE_LOADER
    m_formWidgetLoader->setEnabled( true );
#endif
}

void GuiManager::onServerDisconnected( const std::string& ipv4, int port ) {
#ifdef ENABLE_LOADER
    m_formWidgetLoader->setEnabled( false );
#endif
}

// void GuiManager::onServerDisconnected() {
//#ifdef ENABLE_LOADER
//     m_formWidgetLoader->setEnabled( false );
//#endif
// }

void GuiManager::onNewAcquisition( const std::string& streamName, const hub::Acquisition& acq ) {
    //    std::cout << "[GuiManager] GuiManager::onNewAcquisition '" << streamName << "' : " << acq
    //              << std::endl;
    m_sceneManager.newAcquisition( streamName, acq );
}

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

// void GuiManager::on_toolButton_fitSelected_clicked() {
//     const auto& indexes = m_sensorsView->selectionModel()->selectedIndexes();

//    if ( !indexes.empty() ) {

//        const auto& current = indexes.first();

//        const int row = current.row();

//        std::cout << "[GuiManager] row clicked " << row << std::endl;

//        const std::string streamName = m_sensorsView->model()
//                                           ->data( m_sensorsView->model()->index( row, 0 ) )
//                                           .toString()
//                                           .toStdString();
//        m_sceneManager.fitView( streamName );
//    }
//}

// void GuiManager::loadFile( QString path ) {
//     std::string filename = path.toLocal8Bit().data();
//     bool res             = m_engine->loadFile( filename );

//    if ( !res ) { std::cout << "Aborting file loading !"; }

//    m_engine->releaseFile();
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
