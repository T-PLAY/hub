#include <Gui/MainWindow.hpp>
#include <MainApplication.hpp>

#include <Core/Asset/FileLoaderInterface.hpp>
#include <Engine/Data/Material.hpp>
#include <Engine/Data/MaterialConverters.hpp>
#include <Engine/Data/Mesh.hpp>
#include <Engine/Rendering/ForwardRenderer.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Rendering/RenderObjectManager.hpp>
#include <Engine/Scene/CameraManager.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Engine/Scene/SignalManager.hpp>
#include <Gui/SkeletonBasedAnimation/SkeletonBasedAnimationUI.hpp>
#include <Gui/Timeline/Timeline.hpp>
#include <Gui/TreeModel/EntityTreeModel.hpp>
#include <Gui/Utils/KeyMappingManager.hpp>
#include <Gui/Utils/qt_utils.hpp>
#include <Gui/Viewer/FlightCameraManipulator.hpp>
#include <Gui/Viewer/Gizmo/GizmoManager.hpp>
#include <Gui/Viewer/TrackballCameraManipulator.hpp>
#include <Gui/Viewer/Viewer.hpp>
#include <IO/deprecated/OBJFileManager.hpp>
#include <PluginBase/RadiumPluginInterface.hpp>

#include <Core/Asset/Camera.hpp>
#include <Core/Utils/StringUtils.hpp>
#include <Engine/Scene/CameraComponent.hpp>
#include <Engine/Scene/SkeletonBasedAnimationSystem.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>

using Ra::Engine::Scene::ItemEntry;

namespace Ra {
namespace Gui {

using namespace Core::Utils; // log

MainWindow::MainWindow( QWidget* parent ) : MainWindowInterface( parent ) {
    // Note : at this point most of the components (including the Engine) are
    // not initialized. Listen to the "started" signal.

    setupUi( this );

    m_viewer = new Viewer();
    // Registers the application dependant camera manipulators
    auto keyMappingManager = Gui::KeyMappingManager::getInstance();
    keyMappingManager->addListener( Gui::FlightCameraManipulator::configureKeyMapping );

    connect( m_viewer, &Viewer::glInitialized, this, &MainWindow::onGLInitialized );
    connect( m_viewer, &Viewer::rendererReady, this, &MainWindow::onRendererReady );

    m_viewer->setObjectName( QStringLiteral( "m_viewer" ) );

    QWidget* viewerwidget = QWidget::createWindowContainer( m_viewer );
    //  viewerwidget->setMinimumSize( QSize( 800, 600 ) );
    viewerwidget->setAutoFillBackground( false );

    setCentralWidget( viewerwidget );

    // Register the timeline
    m_timeline = new Ra::Gui::Timeline( this );
    m_timeline->onChangeEnd( Ra::Engine::RadiumEngine::getInstance()->getEndTime() );
    dockWidget_2->setWidget( m_timeline );

    // Register the Skeleton-based animation UI
    auto animSystem = static_cast<Ra::Engine::Scene::SkeletonBasedAnimationSystem*>(
        mainApp->getEngine()->getSystem( "SkeletonBasedAnimationSystem" ) );
    m_skelAnim = new Ra::Gui::SkeletonBasedAnimationUI( animSystem, m_timeline );
    toolBox->addTab( m_skelAnim, "SkeletonBased Animation" );
    for ( int i = 0; i < m_skelAnim->getActionNb(); ++i )
    {
        toolBar->addAction( m_skelAnim->getAction( i ) );
    }

    setWindowIcon( QPixmap( ":/Resources/Icons/RadiumIcon.png" ) );
    setWindowTitle( QString( "Radium Engine Sandbox" ) );

    QStringList headers;
    headers << tr( "Entities -> Components" );
    m_itemModel = new Gui::ItemModel( mainApp->getEngine(), this );
    m_entitiesTreeView->setModel( m_itemModel );
    m_materialEditor   = std::make_unique<MaterialEditor>();
    m_selectionManager = new Gui::SelectionManager( m_itemModel, this );
    m_entitiesTreeView->setSelectionModel( m_selectionManager );

    m_currentShaderBox->setEnabled( false );
    m_currentShaderBox->addItem( "" ); // add empty item

    createConnections();

    mainApp->framesCountForStatsChanged( uint( m_avgFramesCount->value() ) );

    // load default color from QSettings
    updateBackgroundColor();
}

MainWindow::~MainWindow() {
    // Child QObjects will automatically be deleted
}

void MainWindow::cleanup() {
    if ( auto signalManager = Ra::Engine::RadiumEngine::getInstance()->getSignalManager() )
    {
        {
            auto& obs = signalManager->getEntityCreatedNotifier();
            obs.detach( m_entityAddObserverId );
        }
        {
            auto& obs = signalManager->getEntityDestroyedNotifier();
            obs.detach( m_entityRemoveObserverId );
        }
        {
            auto& obs = signalManager->getComponentCreatedNotifier();
            obs.detach( m_componentAddObserverId );
        }
        {
            auto& obs = signalManager->getComponentDestroyedNotifier();
            obs.detach( m_componentRemoveObserverId );
        }
        {
            auto& obs = signalManager->getRenderObjectCreatedNotifier();
            obs.detach( m_roAddObserverId );
        }
        {
            auto& obs = signalManager->getRenderObjectDestroyedNotifier();
            obs.detach( m_roRemoveObserverId );
        }
    }
    m_timeline->detachFromEngine();
    m_viewer->getGizmoManager()->cleanup();
}

void MainWindow::activateTrackballManipulator() {
    // set trackball manipulator (default)
    m_viewer->setCameraManipulator(
        new Gui::TrackballCameraManipulator( *( m_viewer->getCameraManipulator() ) ) );
}

void MainWindow::activateFlightManipulator() {
    // set flightmode manipulator
    m_viewer->setCameraManipulator(
        new Gui::FlightCameraManipulator( *( m_viewer->getCameraManipulator() ) ) );
}

// Connection to gizmos must be done after GL is initialized
void MainWindow::createConnections() {
    connect( actionOpenMesh, &QAction::triggered, this, &MainWindow::loadFile );
    connect( actionReload_Shaders, &QAction::triggered, m_viewer, &Viewer::reloadShaders );
    connect(
        actionOpen_Material_Editor, &QAction::triggered, this, &MainWindow::openMaterialEditor );

    connect( actionFlight, &QAction::triggered, this, &MainWindow::activateFlightManipulator );
    connect(
        actionTrackball, &QAction::triggered, this, &MainWindow::activateTrackballManipulator );
    connect( actionAdd_plugin_path, &QAction::triggered, this, &MainWindow::addPluginPath );
    connect( actionClear_plugin_paths, &QAction::triggered, this, &MainWindow::clearPluginPaths );

    // Toolbox setup
    // to update display when mode is changed
    connect( actionToggle_Local_Global,
             &QAction::toggled,
             mainApp,
             &Ra::Gui::BaseApplication::askForUpdate );

    connect( actionGizmoOff, &QAction::triggered, this, &MainWindow::gizmoShowNone );
    connect( actionGizmoTranslate, &QAction::triggered, this, &MainWindow::gizmoShowTranslate );
    connect( actionGizmoRotate, &QAction::triggered, this, &MainWindow::gizmoShowRotate );
    connect( actionGizmoScale, &QAction::triggered, this, &MainWindow::gizmoShowScale );

    connect( actionSnapshot, &QAction::triggered, mainApp, &MainApplication::recordFrame );
    connect( actionRecord_Frames, &QAction::toggled, mainApp, &MainApplication::setRecordFrames );

    connect(
        actionReload_configuration, &QAction::triggered, this, &MainWindow::reloadConfiguration );
    connect(
        actionLoad_configuration_file, &QAction::triggered, this, &MainWindow::loadConfiguration );

    // Timeline setup
    connect( m_timeline, &Ra::Gui::Timeline::playClicked, this, &MainWindow::timelinePlay );
    connect( m_timeline, &Ra::Gui::Timeline::cursorChanged, this, &MainWindow::timelineGoTo );
    connect(
        m_timeline, &Ra::Gui::Timeline::startChanged, this, &MainWindow::timelineStartChanged );
    connect( m_timeline, &Ra::Gui::Timeline::endChanged, this, &MainWindow::timelineEndChanged );
    connect( m_timeline, &Ra::Gui::Timeline::setPingPong, this, &MainWindow::timelineSetPingPong );
    connect( m_timeline, &Ra::Gui::Timeline::keyFrameChanged, [=]( Scalar ) {
        mainApp->askForUpdate();
    } );

    // Loading setup.
    connect( this, &MainWindow::fileLoading, mainApp, &Ra::Gui::BaseApplication::loadFile );

    // Connect picking results (TODO Val : use events to dispatch picking directly)
    connect( m_viewer, &Viewer::toggleBrushPicking, this, &MainWindow::toggleCirclePicking );
    connect( m_viewer, &Viewer::rightClickPicking, this, &MainWindow::handlePicking );
    // leftClickPicking is obsolete with the new input manager

    connect( m_avgFramesCount,
             static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
             mainApp,
             &Ra::Gui::BaseApplication::framesCountForStatsChanged );
    connect( mainApp,
             &Ra::Gui::BaseApplication::updateFrameStats,
             this,
             &MainWindow::onUpdateFramestats );

    // Inform property editors of new selections
    connect( m_selectionManager,
             &Gui::SelectionManager::selectionChanged,
             this,
             &MainWindow::onSelectionChanged );
    // connect(this, &MainWindow::selectedItem, tab_edition, &TransformEditorWidget::setEditable);

    // Make selected item event visible to plugins
    connect( this, &MainWindow::selectedItem, mainApp, &MainApplication::onSelectedItem );

    // Enable changing shaders
    connect(
        m_currentShaderBox,
        static_cast<void ( QComboBox::* )( const QString& )>( &QComboBox::currentIndexChanged ),
        this,
        &MainWindow::changeRenderObjectShader );

    // RO Stuff
    connect( m_itemModel, &Gui::ItemModel::visibilityROChanged, this, &MainWindow::setROVisible );
    connect( m_editRenderObjectButton, &QPushButton::clicked, this, &MainWindow::editRO );
    connect( m_exportMeshButton, &QPushButton::clicked, this, &MainWindow::exportCurrentMesh );
    connect( m_removeEntityButton, &QPushButton::clicked, this, &MainWindow::deleteCurrentItem );
    connect( m_clearSceneButton, &QPushButton::clicked, this, &MainWindow::resetScene );
    connect( m_fitCameraButton, &QPushButton::clicked, this, &MainWindow::fitCamera );
    connect( m_showHideAllButton, &QPushButton::clicked, this, &MainWindow::showHideAllRO );

    // Renderer stuff
    connect(
        m_currentRendererCombo,
        static_cast<void ( QComboBox::* )( const QString& )>( &QComboBox::currentIndexChanged ),
        [=]( const QString& ) { this->onCurrentRenderChangedInUI(); } );

    connect(
        m_displayedTextureCombo,
        static_cast<void ( QComboBox::* )( const QString& )>( &QComboBox::currentIndexChanged ),
        m_viewer,
        &Viewer::displayTexture );

    connect( m_enablePostProcess, &QCheckBox::stateChanged, m_viewer, &Viewer::enablePostProcess );
    connect(
        m_enablePostProcess, &QCheckBox::stateChanged, mainApp, &MainApplication::askForUpdate );
    connect( m_enableDebugDraw, &QCheckBox::stateChanged, m_viewer, &Viewer::enableDebugDraw );
    connect( m_enableDebugDraw, &QCheckBox::stateChanged, mainApp, &MainApplication::askForUpdate );
    connect( m_realFrameRate,
             &QCheckBox::stateChanged,
             mainApp,
             &Ra::Gui::BaseApplication::setRealFrameRate );

    connect( m_printGraph,
             &QCheckBox::stateChanged,
             mainApp,
             &Ra::Gui::BaseApplication::setRecordGraph );
    connect( m_printTimings,
             &QCheckBox::stateChanged,
             mainApp,
             &Ra::Gui::BaseApplication::setRecordTimings );

    // Material editor
    connect( m_materialEditor.get(),
             &MaterialEditor::materialChanged,
             mainApp,
             &Ra::Gui::BaseApplication::askForUpdate );

    // Skeleton-based Animation
    connect( m_skelAnim,
             &Ra::Gui::SkeletonBasedAnimationUI::askForUpdate,
             mainApp,
             &Ra::Gui::BaseApplication::askForUpdate );

    // Connect engine signals to the appropriate callbacks
    auto signalManager = Ra::Engine::RadiumEngine::getInstance()->getSignalManager();

    auto& entityAddedObs = signalManager->getEntityCreatedNotifier();
    m_entityAddObserverId = entityAddedObs.attachMember( this, &MainWindow::onItemAdded);
    auto& entityRemovedObs = signalManager->getEntityDestroyedNotifier();
    m_entityRemoveObserverId = entityRemovedObs.attachMember( this, &MainWindow::onItemRemoved);

    auto& componentAddedObs = signalManager->getComponentCreatedNotifier();
    m_componentAddObserverId = componentAddedObs.attachMember( this, &MainWindow::onItemAdded);
    auto& componentRemovedObs = signalManager->getComponentDestroyedNotifier();
    m_componentRemoveObserverId = componentRemovedObs.attachMember( this, &MainWindow::onItemRemoved);

    auto& roAddedObs = signalManager->getRenderObjectCreatedNotifier();
    m_roAddObserverId = roAddedObs.attachMember( this, &MainWindow::onItemAdded);
    auto& roRemovedObs = signalManager->getRenderObjectDestroyedNotifier();
    m_roRemoveObserverId = roRemovedObs.attachMember( this, &MainWindow::onItemRemoved);
}

void MainWindow::loadFile() {

    QString filter;

    QString allexts;
    for ( const auto& loader : mainApp->m_engine->getFileLoaders() )
    {
        QString exts;
        for ( const auto& e : loader->getFileExtensions() )
        {
            exts.append( QString::fromStdString( e ) + tr( " " ) );
        }
        allexts.append( exts + tr( " " ) );
        filter.append( QString::fromStdString( loader->name() ) + tr( " (" ) + exts + tr( ");;" ) );
    }
    // add a filter concetenatting all the supported extensions
    filter.prepend( tr( "Supported files (" ) + allexts + tr( ");;" ) );

    // remove the last ";;" of the string
    filter.remove( filter.size() - 2, 2 );

    QSettings settings;
    QString path         = settings.value( "files/load", QDir::homePath() ).toString();
    QStringList pathList = QFileDialog::getOpenFileNames( this, "Open Files", path, filter );

    if ( !pathList.empty() )
    {
        settings.setValue( "files/load", pathList.front() );

        for ( const auto& file : pathList )
        {
            emit fileLoading( file );
        }
        auto sceneName = pathList.first().toStdString();
        activateCamera( sceneName );
        // If an animation is loaded, update the Animation related guiGui
        std::string loadedEntityName = Core::Utils::getBaseName( sceneName, false );
        auto rootEntity =
            Engine::RadiumEngine::getInstance()->getEntityManager()->getEntity( loadedEntityName );
        if ( rootEntity != nullptr ) { m_skelAnim->postLoadFile( rootEntity ); }
    }
}

void MainWindow::onUpdateFramestats( const std::vector<FrameTimerData>& stats ) {
    QString framesA2B = QString( "Frames #%1 to #%2 stats :" )
                            .arg( stats.front().numFrame )
                            .arg( stats.back().numFrame );
    m_frameA2BLabel->setText( framesA2B );

    auto romgr = mainApp->m_engine->getRenderObjectManager();

    auto polycount   = romgr->getNumFaces();
    auto vertexcount = romgr->getNumVertices();

    QString polyCountText =
        QString( "Rendering %1 faces and %2 vertices" ).arg( polycount ).arg( vertexcount );
    m_labelCount->setText( polyCountText );

    long sumRender     = 0;
    long sumTasks      = 0;
    long sumFrame      = 0;
    long sumInterFrame = 0;

    for ( uint i = 0; i < stats.size(); ++i )
    {
        sumRender += Core::Utils::getIntervalMicro( stats[i].renderData.renderStart,
                                                    stats[i].renderData.renderEnd );
        sumTasks += Core::Utils::getIntervalMicro( stats[i].tasksStart, stats[i].tasksEnd );
        sumFrame += Core::Utils::getIntervalMicro( stats[i].frameStart, stats[i].frameEnd );

        if ( i > 0 )
        {
            sumInterFrame +=
                Core::Utils::getIntervalMicro( stats[i - 1].frameEnd, stats[i].frameEnd );
        }
    }

    const uint N {uint( stats.size() )};
    const Scalar T( N * 1000000.f );
    m_renderTime->setNum( int( sumRender / N ) );
    m_renderUpdates->setNum( int( T / Scalar( sumRender ) ) );
    m_tasksTime->setNum( int( sumTasks / N ) );
    m_tasksUpdates->setNum( int( T / Scalar( sumTasks ) ) );
    m_frameTime->setNum( int( sumFrame / N ) );
    m_frameUpdates->setNum( int( T / Scalar( sumFrame ) ) );
    m_avgFramerate->setNum( int( ( N - 1 ) * Scalar( 1000000.0 / sumInterFrame ) ) );
}

Viewer* MainWindow::getViewer() {
    return m_viewer;
}

Gui::SelectionManager* MainWindow::getSelectionManager() {
    return m_selectionManager;
}

Gui::Timeline* MainWindow::getTimeline() {
    return m_timeline;
}

void Gui::MainWindow::toggleCirclePicking( bool on ) {
    centralWidget()->setMouseTracking( on );
}

void MainWindow::handlePicking( const Engine::Rendering::Renderer::PickingResult& pickingResult ) {
    Ra::Core::Utils::Index roIndex( pickingResult.getRoIdx() );
    Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
    if ( roIndex.isValid() )
    {
        auto ro = engine->getRenderObjectManager()->getRenderObject( roIndex );
        if ( ro->getType() != Ra::Engine::Rendering::RenderObjectType::UI )
        {
            Ra::Engine::Scene::Component* comp = ro->getComponent();
            Ra::Engine::Scene::Entity* ent     = comp->getEntity();

            // For now we don't enable group selection.
            m_selectionManager->setCurrentEntry( ItemEntry( ent, comp, roIndex ),
                                                 QItemSelectionModel::ClearAndSelect |
                                                     QItemSelectionModel::Current );
        }
    }
    else
    { m_selectionManager->clear(); }
}

void MainWindow::onSelectionChanged( const QItemSelection& /*selected*/,
                                     const QItemSelection& /*deselected*/ ) {

    if ( m_selectionManager->hasSelection() )
    {
        const ItemEntry& ent = m_selectionManager->currentItem();
        emit selectedItem( ent );
        m_selectedItemName->setText(
            QString::fromStdString( getEntryName( mainApp->getEngine(), ent ) ) );
        m_editRenderObjectButton->setEnabled( false );

        if ( ent.isRoNode() )
        {
            m_editRenderObjectButton->setEnabled( true );

            m_materialEditor->changeRenderObject( ent.m_roIndex );
            auto material = mainApp->m_engine->getRenderObjectManager()
                                ->getRenderObject( ent.m_roIndex )
                                ->getMaterial();
            const std::string& shaderName = material->getMaterialName();
            CORE_ASSERT( m_currentShaderBox->findText( shaderName.c_str() ) != -1,
                         "RO shaders must be already added to the list" );
            m_currentShaderBox->setCurrentText( shaderName.c_str() );
        }
        else
        { m_currentShaderBox->setCurrentText( "" ); }
        m_skelAnim->selectionChanged( ent );
        m_timeline->selectionChanged( ent );
    }
    else
    {
        m_currentShaderBox->setEnabled( false );
        m_currentShaderBox->setCurrentText( "" );
        emit selectedItem( ItemEntry() );
        m_selectedItemName->setText( "" );
        m_editRenderObjectButton->setEnabled( false );
        m_materialEditor->hide();
        m_skelAnim->selectionChanged( ItemEntry() );
        m_timeline->selectionChanged( ItemEntry() );
    }
}

void MainWindow::closeEvent( QCloseEvent* event ) {
    emit closed();
    event->accept();
}

void MainWindow::gizmoShowNone() {
    m_viewer->getGizmoManager()->changeGizmoType( GizmoManager::NONE );
    mainApp->askForUpdate();
}

void MainWindow::gizmoShowTranslate() {
    m_viewer->getGizmoManager()->changeGizmoType( GizmoManager::TRANSLATION );
    mainApp->askForUpdate();
}

void MainWindow::gizmoShowRotate() {
    m_viewer->getGizmoManager()->changeGizmoType( GizmoManager::ROTATION );
    mainApp->askForUpdate();
}

void MainWindow::gizmoShowScale() {
    m_viewer->getGizmoManager()->changeGizmoType( GizmoManager::SCALE );
    mainApp->askForUpdate();
}

void MainWindow::reloadConfiguration() {
    KeyMappingManager::getInstance()->reloadConfiguration();
}

void MainWindow::loadConfiguration() {
    QSettings settings;
    QString path = settings.value( "configs/load", QDir::homePath() ).toString();
    path         = QFileDialog::getOpenFileName(
        this, "Open Configuration File", path, "Configuration file (*.xml)" );

    if ( path.size() > 0 )
    {
        settings.setValue( "configs/load", path );
        KeyMappingManager::getInstance()->loadConfiguration( path.toStdString().c_str() );
    }
}

void MainWindow::onCurrentRenderChangedInUI() {
    // always restore displaytexture to 0 before switch to keep coherent renderer state
    m_displayedTextureCombo->setCurrentIndex( 0 );
    if ( m_viewer->changeRenderer( m_currentRendererCombo->currentIndex() ) )
    {
        updateDisplayedTexture();
        // in case the newly used renderer has not been set before and set another texture as its
        // default, set displayTexture to 0 again ;)
        m_displayedTextureCombo->setCurrentIndex( 0 );
    }
}

void MainWindow::updateDisplayedTexture() {
    QSignalBlocker blockTextures( m_displayedTextureCombo );

    m_displayedTextureCombo->clear();

    auto texs = m_viewer->getRenderer()->getAvailableTextures();
    for ( const auto& tex : texs )
    {
        m_displayedTextureCombo->addItem( tex.c_str() );
    }
}

void MainWindow::updateBackgroundColor( QColor c ) {
    // FIXME : sometime, settings does not define colors but Qt found one ....
    QSettings settings;
    // Get or set color from/to settings
    if ( !c.isValid() )
    {
        // get the default color or an already existing one
        auto defColor = Core::Utils::Color::linearRGBTosRGB( m_viewer->getBackgroundColor() );
        auto bgk      = QColor::fromRgb(
            defColor.rgb()[0] * 255, defColor.rgb()[1] * 255, defColor.rgb()[2] * 255 );
        c = settings.value( "colors/background", bgk ).value<QColor>();
    }
    else
    { settings.setValue( "colors/background", c ); }

    // update the color of the button
    QString qss = QString( "background-color: %1" ).arg( c.name() );
    m_currentColorButton->setStyleSheet( qss );

    // update the background coolor of the viewer
    auto bgk = Core::Utils::Color::sRGBToLinearRGB( Core::Utils::Color(
        Scalar( c.redF() ), Scalar( c.greenF() ), Scalar( c.blueF() ), Scalar( 0 ) ) );
    m_viewer->setBackgroundColor( bgk );
}

// Is this still a useful feature ?
void MainWindow::changeRenderObjectShader( const QString& shaderName ) {
    // FIXME : is this still a wanted feature. Commented out for now. if this feature is wanted,
    // need to find a
    //  way to change the render-technique.
    /*
        std::string name = shaderName.toStdString();
        if ( name.empty() ) { return; }

        const ItemEntry& item = m_selectionManager->currentItem();
        const Engine::Data::ShaderConfiguration config =
            Ra::Engine::Data::ShaderConfigurationFactory::getConfiguration( name );

        auto vector_of_ros = getItemROs( mainApp->m_engine.get(), item );
        for ( const auto& ro_index : vector_of_ros )
        {
            const auto& ro = mainApp->m_engine->getRenderObjectManager()->getRenderObject( ro_index
       ); if ( ro->getRenderTechnique()->getMaterial()->getMaterialName() != name )
            {
                // FIXME: this changes only the render technique, not the associated shader.

                auto builder = Ra::Engine::EngineRenderTechniques::getDefaultTechnique( name );
                builder.second( *ro->getRenderTechnique().get(), false );
            }
        }
    */
}

void Gui::MainWindow::setROVisible( Core::Utils::Index roIndex, bool visible ) {
    mainApp->m_engine->getRenderObjectManager()->getRenderObject( roIndex )->setVisible( visible );
    mainApp->askForUpdate();
}

void Gui::MainWindow::editRO() {
    ItemEntry item = m_selectionManager->currentItem();
    if ( item.isRoNode() )
    {
        m_materialEditor->changeRenderObject( item.m_roIndex );
        m_materialEditor->show();
    }
}

void Gui::MainWindow::showHideAllRO() {
    bool allEntityInvisible = true;

    const int j = 0;
    for ( int i = 0; i < m_itemModel->rowCount(); ++i )
    {
        auto idx  = m_itemModel->index( i, j );
        auto item = m_itemModel->getEntry( idx );
        if ( item.isValid() && item.isSelectable() )
        {
            bool isVisible = m_itemModel->data( idx, Qt::CheckStateRole ).toBool();
            if ( isVisible )
            {
                allEntityInvisible = false;
                break;
            }
        }
    }

    // if all entities are invisible : show all
    // if at least one entity is visible : hide all
    for ( int i = 0; i < m_itemModel->rowCount(); ++i )
    {
        auto idx  = m_itemModel->index( i, j );
        auto item = m_itemModel->getEntry( idx );
        if ( item.isValid() && item.isSelectable() )
        { m_itemModel->setData( idx, allEntityInvisible, Qt::CheckStateRole ); }
    }
    mainApp->askForUpdate();
}

void Gui::MainWindow::openMaterialEditor() {
    m_materialEditor->show();
}

void Gui::MainWindow::updateUi( Plugins::RadiumPluginInterface* plugin ) {
    QString tabName;

    // Add menu
    if ( plugin->doAddMenu() ) { QMainWindow::menuBar()->addMenu( plugin->getMenu() ); }

    // Add widget
    if ( plugin->doAddWidget( tabName ) ) { toolBox->addTab( plugin->getWidget(), tabName ); }

    // Add actions
    int nbActions;
    if ( plugin->doAddAction( nbActions ) )
    {
        for ( int i = 0; i < nbActions; ++i )
        {
            toolBar->insertAction( nullptr, plugin->getAction( i ) );
        }
        toolBar->addSeparator();
    }
}

void MainWindow::onRendererReady() {
    updateDisplayedTexture();
}

void MainWindow::onFrameComplete() {
    tab_edition->updateValues();
    // update timeline only if time changed, to allow manipulation of keyframed objects
    auto engine = Ra::Engine::RadiumEngine::getInstance();
    if ( !Ra::Core::Math::areApproxEqual( m_timeline->getTime(), engine->getTime() ) )
    {
        m_lockTimeSystem = true;
        m_timeline->onChangeCursor( engine->getTime() );
        m_lockTimeSystem = false;
    }
}

void MainWindow::addRenderer( const std::string& name,
                              std::shared_ptr<Engine::Rendering::Renderer> e ) {
    int id = m_viewer->addRenderer( e );
    CORE_UNUSED( id );
    CORE_ASSERT( id == m_currentRendererCombo->count(), "Inconsistent renderer state" );
    m_currentRendererCombo->addItem( QString::fromStdString( name ) );
}

void MainWindow::on_actionPlay_triggered( bool checked ) {
    Ra::Engine::RadiumEngine::getInstance()->play( checked );
    mainApp->setContinuousUpdate( checked );
}

void MainWindow::on_actionStop_triggered() {
    Ra::Engine::RadiumEngine::getInstance()->resetTime();
    mainApp->askForUpdate();
    actionPlay->setChecked( false );
}

void MainWindow::on_actionStep_triggered() {
    Ra::Engine::RadiumEngine::getInstance()->step();
    mainApp->askForUpdate();
}

void MainWindow::timelinePlay( bool play ) {
    actionPlay->setChecked( play );
    if ( !m_lockTimeSystem )
    {
        Ra::Engine::RadiumEngine::getInstance()->play( play );
        mainApp->setContinuousUpdate( play );
    }
}

void MainWindow::timelineGoTo( double t ) {
    if ( !m_lockTimeSystem )
    {
        Ra::Engine::RadiumEngine::getInstance()->setTime( Scalar( t ) );
        mainApp->askForUpdate();
    }
}

void MainWindow::timelineStartChanged( double t ) {
    if ( !m_lockTimeSystem )
    {
        Ra::Engine::RadiumEngine::getInstance()->setStartTime( Scalar( t ) );
        mainApp->askForUpdate();
    }
}

void MainWindow::timelineEndChanged( double t ) {
    if ( !m_lockTimeSystem )
    {
        Ra::Engine::RadiumEngine::getInstance()->setEndTime( Scalar( t ) );
        mainApp->askForUpdate();
    }
}

void MainWindow::timelineSetPingPong( bool status ) {
    if ( !m_lockTimeSystem )
    {
        Ra::Engine::RadiumEngine::getInstance()->setForwardBackward( status );
        mainApp->askForUpdate();
    }
}

void MainWindow::onItemAdded( const Engine::Scene::ItemEntry& ent ) {
    m_itemModel->addItem( ent );

    // update the m_currentShaderBox for added RO
    if ( ent.isRoNode() )
    {
        auto ro = Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
            ent.m_roIndex );
        if ( ro->getType() == Engine::Rendering::RenderObjectType::Geometry )
        {
            auto material                 = ro->getMaterial();
            const std::string& shaderName = material->getMaterialName();
            auto i                        = m_currentShaderBox->findText( shaderName.c_str() );
            if ( i == -1 )
            {
                QVariant refCount( 1 );
                m_currentShaderBox->addItem( QString( shaderName.c_str() ), refCount );
            }
            else
            {
                auto refCount = m_currentShaderBox->itemData( i );
                refCount      = refCount.toInt() + 1;
                m_currentShaderBox->setItemData( i, refCount );
            }
        }
    }
}

void MainWindow::onItemRemoved( const Engine::Scene::ItemEntry& ent ) {
    m_itemModel->removeItem( ent );
    // update the m_currentShaderBox for removed RO
    if ( ent.isRoNode() )
    {
        auto ro = Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
            ent.m_roIndex );
        if ( ro->getType() == Engine::Rendering::RenderObjectType::Geometry )
        {
            auto material                 = ro->getMaterial();
            const std::string& shaderName = material->getMaterialName();
            auto i                        = m_currentShaderBox->findText( shaderName.c_str() );
            if ( i == -1 )
            {
                // Should never be there ...
                LOG( logERROR ) << "MainWindow::onItemRemoved : trying to remove a non existing "
                                   "ShaderBox entry";
            }
            else
            {
                auto refCount = m_currentShaderBox->itemData( i );
                refCount      = refCount.toInt() - 1;
                if ( refCount.toInt() == 0 ) { m_currentShaderBox->removeItem( i ); }
                else
                { m_currentShaderBox->setItemData( i, refCount ); }
            }
        }
    }
}

void MainWindow::exportCurrentMesh() {
    std::stringstream filenameStream;
    filenameStream << mainApp->getExportFolderName() << "/radiummesh_" << std::setw( 6 )
                   << std::setfill( '0' ) << mainApp->getFrameCount();
    std::string filename = filenameStream.str();

    ItemEntry e = m_selectionManager->currentItem();

    // For now we only export a mesh if the selected entry is a render object.
    // There could be a virtual method to get a mesh representation for any object.
    if ( e.isRoNode() )
    {
        Ra::IO::OBJFileManager obj;
        auto ro = Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
            e.m_roIndex );
        const std::shared_ptr<Engine::Data::Displayable>& displ = ro->getMesh();
        const Engine::Data::Mesh* mesh = dynamic_cast<Engine::Data::Mesh*>( displ.get() );

        if ( mesh != nullptr && obj.save( filename, mesh->getCoreGeometry() ) )
        {
            LOG( logINFO ) << "Mesh from " << ro->getName() << " successfully exported to "
                           << filename;
        }
        else
        { LOG( logERROR ) << "Mesh from " << ro->getName() << "failed to export"; }
    }
    else
    { LOG( logWARNING ) << "Current entry was not a render object. No mesh was exported."; }
}

void MainWindow::deleteCurrentItem() {
    ItemEntry e = m_selectionManager->currentItem();

    // This call is very important to avoid a potential race condition
    // which happens if an object is selected while a gizmo is present.
    // If we do not do this, the removal of the object will call ItemModel::removeItem() which
    // will cause it to be unselected by the selection model. This in turn will cause
    // the gizmos ROs to disappear, but the RO mutex is already acquired by the call for
    // the object we want to delete, which causes a deadlock.
    // Clearing the selection before deleting the object will avoid this problem.
    m_selectionManager->clear();
    if ( e.isRoNode() ) { e.m_component->removeRenderObject( e.m_roIndex ); }
    else if ( e.isComponentNode() )
    { e.m_entity->removeComponent( e.m_component->getName() ); }
    else if ( e.isEntityNode() )
    {
        Engine::RadiumEngine::getInstance()->getEntityManager()->removeEntity(
            e.m_entity->getIndex() );
    }
    mainApp->askForUpdate();
}

void MainWindow::resetScene() {
    // Fix issue #378 : ask the viewer to switch back to the default camera
    m_viewer->resetToDefaultCamera();
    // To see why this call is important, please see deleteCurrentItem().
    m_selectionManager->clear();
    Engine::RadiumEngine::getInstance()->getEntityManager()->deleteEntities();
    fitCamera();
}

void MainWindow::fitCamera() {
    auto aabb = Engine::RadiumEngine::getInstance()->computeSceneAabb();
    if ( aabb.isEmpty() )
    {
        m_viewer->getCameraManipulator()->resetCamera();
        mainApp->askForUpdate();
    }
    else
    { m_viewer->fitCameraToScene( aabb ); }
}

void MainWindow::activateCamera( const std::string& sceneName ) {

    // If a camera is in the given scene, use it, else, use default
    std::string loadedEntityName = Core::Utils::getBaseName( sceneName, false );
    auto rootEntity =
        Engine::RadiumEngine::getInstance()->getEntityManager()->getEntity( loadedEntityName );
    if ( rootEntity != nullptr )
    {
        auto fc = std::find_if(
            rootEntity->getComponents().begin(),
            rootEntity->getComponents().end(),
            []( const auto& c ) { return ( c->getName().compare( 0, 7, "CAMERA_" ) == 0 ); } );
        if ( fc != rootEntity->getComponents().end() )
        {
            LOG( logINFO ) << "Activating camera " << ( *fc )->getName();

            auto cameraManager = static_cast<Ra::Engine::Scene::CameraManager*>(
                Engine::RadiumEngine::getInstance()->getSystem( "DefaultCameraManager" ) );

            auto comp = static_cast<Ra::Engine::Scene::CameraComponent*>( ( *fc ).get() );

            auto idx = cameraManager->getCameraIndex( comp );
            cameraManager->activate( idx );
            m_viewer->getCameraManipulator()->updateCamera();
        }
    }
}

void MainWindow::prepareDisplay() {
    m_selectionManager->clear();
    m_currentShaderBox->setEnabled( false );
    m_currentShaderBox->setCurrentText( "" );

    if ( m_viewer->prepareDisplay() ) { mainApp->askForUpdate(); }
}

void MainWindow::onGLInitialized() {
    // Connection to gizmos after their creation
    connect( actionToggle_Local_Global,
             &QAction::toggled,
             m_viewer->getGizmoManager(),
             &GizmoManager::setLocal );
    connect(
        this, &MainWindow::selectedItem, m_viewer->getGizmoManager(), &GizmoManager::setEditable );

    // set default renderer once OpenGL is configured
    std::shared_ptr<Engine::Rendering::Renderer> e( new Engine::Rendering::ForwardRenderer() );
    addRenderer( "Forward Renderer", e );
}

void MainWindow::addPluginPath() {
    QString dir = QFileDialog::getExistingDirectory( this,
                                                     tr( "Open Directory" ),
                                                     "",
                                                     QFileDialog::ShowDirsOnly |
                                                         QFileDialog::DontResolveSymlinks );
    LOG( logINFO ) << "Adding the directory " << dir.toStdString() << " to the plugin directories.";
    mainApp->addPluginDirectory( dir.toStdString() );
}

void MainWindow::clearPluginPaths() {
    mainApp->clearPluginDirectories();
}

} // namespace Gui
} // namespace Ra

void Ra::Gui::MainWindow::on_m_currentColorButton_clicked() {
    // get the default color or an already existing one
    auto defColor     = Core::Utils::Color::linearRGBTosRGB( m_viewer->getBackgroundColor() );
    auto currentColor = QColor::fromRgb(
        defColor.rgb()[0] * 255, defColor.rgb()[1] * 255, defColor.rgb()[2] * 255 );
    QColor c = QColorDialog::getColor( currentColor, this, "Renderer background color" );
    if ( c.isValid() ) { updateBackgroundColor( c ); }
}
