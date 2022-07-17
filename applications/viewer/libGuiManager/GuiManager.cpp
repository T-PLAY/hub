#include "GuiManager.h"

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

#include <FormSensorViews.h>

#include <Core/Asset/FileLoaderInterface.hpp>
//#include <DicomLoader/DicomLoader.hpp>
#include <Engine/Scene/GeometrySystem.hpp>

#include <FormInputStreamViews.h>
#include <FormWidgetLoader.h>

#include <Gui/TreeModel/EntityTreeModel.hpp>

#include <Engine/Rendering/RenderObjectManager.hpp>>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Scene/Component.hpp>

GuiManager::GuiManager(QObject* parent)
    : QObject { parent }
{
}

// GuiManager::GuiManager()
//{

//}

GuiManager::~GuiManager()
{
    std::cout << "[GuiManager] ~GuiManager() start" << std::endl;
    //    delete m_toolBar;
    //    m_toolBar = nullptr;
    //    delete m_action3D;
    //    m_action3D = nullptr;
    //    delete m_action2D;
    //    m_action2D = nullptr;
    //    delete m_comp;
    //    m_comp = nullptr;
    //    delete m_formWidgetLoader;
    //    m_formWidgetLoader = nullptr;
    //    delete m_formInputStreamViews;
    //    m_formInputStreamViews = nullptr;
    //    delete m_formSensorViews;
    //    m_formSensorViews = nullptr;

    std::cout << "[GuiManager] ~GuiManager() end" << std::endl;
}

#include <QAction>
#include <QDockWidget>
#include <QToolBar>

void GuiManager::init()
{
    assert(!m_initialized);
    assert(m_mainWindow != nullptr);
    m_dockLeft = new QDockWidget(m_mainWindow);
    m_dockTop = new QDockWidget(m_mainWindow);
    m_dockRight = new QDockWidget(m_mainWindow);
    m_dockBottom = new QDockWidget(m_mainWindow);
    m_mainWindow->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_dockLeft);
    m_mainWindow->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, m_dockTop);
    m_mainWindow->addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, m_dockRight);
    m_mainWindow->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, m_dockBottom);

    assert(m_dockLeft != nullptr);
    assert(m_dockTop != nullptr);
    assert(m_dockRight != nullptr);
    assert(m_dockBottom != nullptr);

    assert(m_mdiArea != nullptr);

    assert(m_engine != nullptr);
    assert(m_viewer != nullptr);
    assert(m_system != nullptr);

    assert(m_stackedWidget != nullptr);
    assert(m_layout3DView != nullptr);

    //////////////////////////////////////// TOOL BAR

    m_actionAddSources = new QAction("Add Source", m_mainWindow);
    m_toolBarTopLeft = new QToolBar(m_mainWindow);
    m_toolBarTopLeft->setMinimumHeight(40);
    m_toolBarTopLeft->setMovable(false);
    m_toolBarTopLeft->addAction(m_actionAddSources);
    m_mainWindow->addToolBar(m_toolBarTopLeft);

    m_toolBarTopRight = new QToolBar(m_mainWindow);
    m_toolBarTopRight->setMovable(false);
    m_toolBarTopRight->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    m_actionSettings = new QAction("Settings", m_mainWindow);
    m_toolBarTopRight->addAction(m_actionSettings);
    m_action3D = new QAction("3D", m_mainWindow);
    QObject::connect(m_action3D,
        &QAction::triggered,
        this,
        &GuiManager::on_action3D_triggered);
    m_toolBarTopRight->addAction(m_action3D);
    m_toolBarTopRight->addSeparator();
    m_action2D = new QAction("2D", m_mainWindow);
    QObject::connect(m_action2D,
        &QAction::triggered,
        this,
        &GuiManager::on_action2D_triggered);
    m_toolBarTopRight->addAction(m_action2D);
    m_mainWindow->addToolBar(m_toolBarTopRight);

    //////////////////////////////////////// 3D VIEW

    m_3DToolBox = new Form3DToolBox(m_mainWindow);
    QObject::connect(m_3DToolBox,
        &Form3DToolBox::pushButton_fitScene_clicked,
        this,
        &GuiManager::on_toolButton_fitScene_clicked);
    QObject::connect(m_3DToolBox,
        &Form3DToolBox::pushButton_fitTrace_clicked,
        this,
        &GuiManager::on_toolButton_fitTrace_clicked);
    QObject::connect(m_3DToolBox,
        &Form3DToolBox::checkBox_showTrace_toggled,
        this,
        &GuiManager::on_checkBox_trace_toggled);
    QObject::connect(m_3DToolBox,
        &Form3DToolBox::checkBox_showGrid_toggled,
        this,
        &GuiManager::on_checkBox_grid_toggled);

    //    m_layout3DView->addWidget(m_3DToolBox);
    m_layout3DView->insertWidget(0, m_3DToolBox);

    //////////////////////////////////////// RIGHT
    //     dockWidgetContents_right->setMinimumWidth(500);

    //    m_imageManipulator = new FormImageManipulator(this);

    m_dockRight->setMinimumWidth(200);
//    m_dockRight->setWidget(&m_imageManipulator);
//    m_dockRight->setWidget();
//    QVBoxLayout * vLayout = new QVBoxLayout(m_dockRight);
//    m_dockRight->setLayout(vLayout);
//    QTreeView * treeView = new QTreeView(m_dockRight);
//    treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    treeView->setMinimumSize(QSize(200, 200));
//    vLayout->addWidget(treeView);
//    Ra::Gui::ItemModel * itemModel = new Ra::Gui::ItemModel(m_engine, this);
//    treeView->setModel(itemModel);

    //    auto& streamView = ui->dockWidgetContents_right->getWidgetStreamView();
//    auto& streamView = m_imageManipulator.getWidgetStreamView();
//    streamView.init(512, 192, 35.0, 50.0);
//    streamView.init(256, 256, 250, 250);

    //////////////////////////////////////// TOP
    assert(m_mdiArea != nullptr);
    m_formInputStreamViews = new FormInputStreamViews(*m_mdiArea, m_dockTop);
    //    ui->dockWidget_top->setWidget(m_formInputStreamViews);
    m_dockTop->setWidget(m_formInputStreamViews);

    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::initPose, this,
    //    &GuiManager::onInitPose); QObject::connect(m_formInputStreamViews,
    //    &FormInputStreamViews::initScan, this, &GuiManager::onInitScan);
    QObject::connect(m_formInputStreamViews,
        &FormInputStreamViews::newAcquisition,
        this,
        &GuiManager::onNewAcquisition);
    QObject::connect(
        m_formInputStreamViews, &FormInputStreamViews::init, this, &GuiManager::onInit);
    QObject::connect(m_formInputStreamViews,
        &FormInputStreamViews::selectedSourceChanged,
        this,
        &GuiManager::onSelectedSourceChanged);
    //    QObject::connect(m_formInputStreamViews, &FormInputStreamViews::newAcquisitionScan, this,
    //    &GuiManager::onUpdateScan);

    //////////////////////////////////////// BOTTOM
    m_formWidgetLoader = new FormWidgetLoader(m_dockBottom);
    //    ui->dockWidget_bottom->setWidget(m_formWidgetLoader);
    m_dockBottom->setWidget(m_formWidgetLoader);
    QObject::connect(m_formWidgetLoader,
        &FormWidgetLoader::recordPathLoaded,
        this,
        &GuiManager::onRecordLoaderPathLoaded);
    QObject::connect(m_formWidgetLoader,
        &FormWidgetLoader::snapshotPathLoaded,
        this,
        &GuiManager::onSnapshotLoaderPathLoaded);

    //////////////////////////////////////// LEFT
    m_formSensorViews = new FormSensorViews(m_dockLeft);

    //    if (m_formSensorViews->isServerConnected()) {
    //    ui->dockWidget_left->setWidget(m_formSensorViews);
    m_dockLeft->setWidget(m_formSensorViews);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::streamingStarted,
        this,
        &GuiManager::onServerStreamStarted);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::streamingStopped,
        this,
        &GuiManager::onServerStreamStopped);
    QObject::connect(m_formSensorViews,
        &FormSensorViews::serverDisconnected,
        this,
        &GuiManager::onServerDisconnected);
    //        m_formSensorViews->startStreaming();
    //    } else {
    //        delete m_formSensorViews;
    //        ui->dockWidget_left->close();
    //    }
    //    ui->dockWidgetContents_left = new FormSensorViews(this);

    //////////////////////////////////////// INIT 3D ENVIRONMENT
    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* e = m_engine->getEntityManager()->createEntity("Cube");
    m_comp = new MinimalComponent(e, *m_engine, *m_viewer);
    m_system->addComponent(e, m_comp);
    m_comp->initialize();
    m_comp->updateShader();

    Ra::Core::Transform TLocal = Ra::Core::Transform::Identity();
    TLocal.translate(Ra::Core::Vector3(10_ra, 10_ra, 10_ra));
//    e->setTransform(TLocal);
//    auto transform = Ra::Core::Transform::translation()
//    e->setTransform(
//    e->transformationObservers();

//    auto * entityManager = m_engine->getEntityManager();
//    auto * entity1 = entityManager->createEntity("entity1");

//    auto * component1 = new Ra::Engine::Scene::Component("component1", entity1);

//    auto * renderObjectManager = m_engine->getRenderObjectManager();
//    auto * ro = Ra::Engine::Rendering::RenderObject::createRenderObject("ro1",
//    renderObjectManager->addRenderObject();

    m_viewer->prepareDisplay();

    m_initialized = true;
}

void GuiManager::incIter()
{

    m_comp->incIter();
}

void GuiManager::onRecordLoaderPathLoaded()
{
    std::cout << "[GuiManager] onRecordLoaderPathLoaded()" << std::endl;
    const auto& recordLoader = m_formWidgetLoader->getRecordLoader();
    const auto& outputStreamBuffs = recordLoader.getOutputStreamBuffs();

    for (const auto& pair : outputStreamBuffs) {
        const auto& streamName = pair.first;
        auto& cyclicBuff = const_cast<CyclicBuff&>(*pair.second);

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream(streamName, RamIO(cyclicBuff));
        //    m_formInputStreamViews->connect(recordOutputStreams);

        if (streamName.substr(0, g_probePoseSensorName.size()) == g_probePoseSensorName) {
            const auto& poseAcqs = recordLoader.getAcquisitions(g_probePoseSensorName);
            m_comp->initPoseTraces(poseAcqs);
        }
    }
}

void GuiManager::onSnapshotLoaderPathLoaded()
{
    std::cout << "[GuiManager] onRecordLoaderPathLoaded()" << std::endl;
    const auto& outputStreamBuffs = m_formWidgetLoader->getSnapshotLoader().getOutputStreamBuffs();

    for (const auto& pair : outputStreamBuffs) {
        const auto& streamName = pair.first;
        auto& cyclicBuff = const_cast<CyclicBuff&>(*pair.second);

        //        IOStream & ioStream = cyclicBuff.getIOStream();
        //        RamIO & ramIO = dynamic_cast<RamIO&>(ioStream);
        //        InputStream inputStream(ramIO);
        std::cout << "connected " << streamName << std::endl;

        m_formInputStreamViews->addInputStream(streamName, RamIO(cyclicBuff));
        //    m_formInputStreamViews->connect(recordOutputStreams);
    }
}

void GuiManager::on_action2D_triggered()
{
    //    ui->stackedWidget->setCurrentIndex(0);
    m_stackedWidget->setCurrentIndex(0);
}

void GuiManager::on_action3D_triggered()
{
    //    ui->stackedWidget->setCurrentIndex(1);
    m_stackedWidget->setCurrentIndex(1);
}

void GuiManager::onServerStreamStarted(const std::string& sensorName,
    const std::string& syncSensorName)
{
    std::cout << "[GuiManager] onServerStreamStarted()" << std::endl;

    //    ClientSocket * socket = nullptr;
    //    if (sensorName == g_probePoseSensorName) {
    //        socket = new ClientSocket(sensorName, syncSensorName);
    //    }

    //    m_formInputStreamViews->addInputStream(sensorName, std::move(*socket));

    m_formInputStreamViews->addInputStream(sensorName,
        ClientSocket(sensorName, syncSensorName));
}

void GuiManager::onServerStreamStopped(const std::string& sensorName)
{
    std::cout << "[GuiManager] onServerStreamStopped()" << std::endl;

    m_formInputStreamViews->deleteInputStream(sensorName);
    //        m_formInputStreamViews->onKillInputStream(sensorName);
}

void GuiManager::onServerDisconnected()
{
    delete m_formSensorViews;
    m_formSensorViews = nullptr;
    //    ui->dockWidgetContents_left.c
    //    ui->dockWidget_left->close();
    m_dockLeft->close();
}

void GuiManager::onInit(const std::string& sensorName)
{
    std::cout << "[GuiManager] onInit '" << sensorName << "'" << std::endl;
    if (sensorName == g_probeScanSensorName) {
        m_comp->initScan();
        //        ui->dockWidgetContents_right->init();
        m_imageManipulator.init();

    } else if (sensorName == g_probePoseSensorName) {
        m_comp->initPose();
    }
}

// void GuiManager::onInitPose()
//{
//    std::cout << "[GuiManager] onInitPose" << std::endl;
//    m_comp->initPose();
//}

// void GuiManager::onInitScan()
//{
//    std::cout << "[GuiManager] onInitScan" << std::endl;
//    m_comp->initScan();
//}

void GuiManager::onNewAcquisition(const std::string& sensorName, const std::string& sourceType)
{
    //    std::cout << "[GuiManager] onNewAcquisition(" << sensorName << ", " << sourceType << ")"
    //    << std::endl;
    //    const auto& acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    //    auto && acq = m_formInputStreamViews->getAcquisition(sensorName, sourceType);
    auto& acqs = m_formInputStreamViews->getAcquisitions(sensorName, sourceType);

    if (acqs.empty()) {
        std::cout << "[GuiManager] void signal, empty acqs --------------" << std::endl;
        return;
    }
    assert(!acqs.empty());

    if (sensorName == g_probeScanSensorName) {
        //        for ( const auto& acq : acqs ) {

        //        m_comp->updateScan(acqs);
        //        while (! acqs.empty()) {
        const auto& acq = acqs.front();

        m_comp->updateScan(acq);
        m_imageManipulator.update(acq);
        //        ui->dockWidgetContents_right->update(acq);

        //            acqs.pop();
        //        }
        //        }
    } else if (sensorName == g_probePoseSensorName) {
        //        for ( const auto& acq : acqs ) {
        //        m_comp->updatePose( acq );
        //        }
        //        m_comp->updatePose(acqs);
        //        while (! acqs.empty()) {
        ////            const auto & acq = acqs.front();
        ////            std::cout << "[GuiManager] update pose : " << acq << std::endl;
        m_comp->updatePose(acqs.front());
        //            acqs.pop();
        //        }
    }
    acqs.pop();
    //    else {
    // do n
    //        assert( false );
    //    }
}

void GuiManager::onSelectedSourceChanged(const std::string& sensorName, const std::string& sourceType)
{
    std::cout << "[GuiManager] onSelectedSourceChanged(" << sensorName << ", " << sourceType << ")" << std::endl;

    const auto& inputStream = m_formInputStreamViews->getInputStream(sensorName, sourceType);

    std::cout << inputStream << std::endl;

    if (sensorName == g_probePoseSensorName) {
        const auto& metadata = inputStream.getMetaData();
        double scanWidth = 5.0;
        if (metadata.find("scanWidth") != metadata.end()) {
            scanWidth = std::any_cast<double>(metadata.at("scanWidth"));
        }

        double scanDepth = 3.5;
        if (metadata.find("scanDepth") != metadata.end()) {
            scanDepth = std::any_cast<double>(metadata.at("scanDepth"));
        }

        double x = 3.1;
        if (metadata.find("x") != metadata.end()) {
            x = std::any_cast<double>(metadata.at("x"));
        }
        double y = 0.0;
        if (metadata.find("y") != metadata.end()) {
            y = std::any_cast<double>(metadata.at("y"));
        }
        double z = 16.0;
        if (metadata.find("z") != metadata.end()) {
            z = std::any_cast<double>(metadata.at("z"));
        }

        m_comp->setupScanner(scanWidth, scanDepth, x, y, z);
    }

    //    m_engine->loadFile(MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm");
    //    m_app->m_viewer->prepareDisplay();
}

// void GuiManager::onUpdatePose()
//{
//     //    std::cout << "[GuiManager] onUpdatePose()" << std::endl;
////    m_comp->updatePose(m_formInputStreamViews->getPoseAcquisition());
//}

// void GuiManager::onUpdateScan()
//{
//     //    std::cout << "[GuiManager] onUpdateScan()" << std::endl;
////    m_comp->updateScan(m_formInputStreamViews->getScanAcquisition());
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

void GuiManager::on_checkBox_grid_toggled(bool checked)
{
    m_comp->getRoGrid().setVisible(checked);
}

void GuiManager::on_checkBox_trace_toggled(bool checked)
{
    m_comp->traceSetVisible(checked);
}

void GuiManager::on_toolButton_fitScene_clicked()
{
    m_viewer->fitCamera();
}

#include <Core/Utils/Color.hpp>
void GuiManager::on_toolButton_fitTrace_clicked()
{
    auto& viewer = *m_viewer;
    //    auto aabb = Ra::Engine::RadiumEngine::getInstance()->computeSceneAabb();
    //    auto aabb = m_app->m_engine->computeSceneAabb();
    const auto& traces = m_comp->getRoTraces();
    //    auto aabb = traces[0].compu
    auto aabb = traces[0]->computeAabb();
    for (int i = 1; i < g_nTraces; ++i) {
        aabb.extend(traces[i]->computeAabb());
    }

    if (aabb.isEmpty()) {
        viewer.getCameraManipulator()->resetCamera();
    } else {
        viewer.fitCameraToScene(aabb);
    }

    //    RA_DISPLAY_AABB( aabb, Ra::Core::Utils::Color::Blue() );
}

void GuiManager::loadFile(QString path)
{
    std::string filename = path.toLocal8Bit().data();
    //    LOG( logINFO ) << "Loading file " << filename << "...";
    bool res = m_engine->loadFile(filename);

    if (!res) {
        std::cout << "Aborting file loading !";
        //        return false;
    }

    m_engine->releaseFile();

    //    m_mainWindow->prepareDisplay();
    //    m_app->m_viewer->update(0);

    //    emit loadComplete();
    //    return true;
}
