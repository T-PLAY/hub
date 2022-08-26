#include "Sensor.h"

//#include <QM
#include <Engine/Scene/EntityManager.hpp>
#include <QMdiSubWindow>
#include <WidgetStreamView.h>

#include <Core/Utils/Observable.hpp>
#include <Engine/Scene/Entity.hpp>

#include <SensorSpec.hpp>

SensorThread::SensorThread( Sensor& sensor, QObject* parent ) :
    QThread { parent }, m_sensor( sensor ) {}

SensorThread::~SensorThread() {
    std::cout << "[SensorThread] ~SensorThread(" << m_sensor.m_inputSensor->m_spec.m_sensorName
              << ")" << std::endl;

    //    assert(! isRunning());
    //    if (this->isRunning()) {
    //        this->requestInterruption();
    //        this->wait();
    //    }
}

void SensorThread::run() {
    std::cout << "[SensorThread] run()" << std::endl;

    auto& inputStream = m_sensor.m_inputSensor;

    try {

        while ( !this->isInterruptionRequested() ) {

            //            Sensor::Acquisition acq;
            //            *mInputStream >> acq;
            const auto& acq = inputStream->getAcquisition();
            //            std::cout << "[SensorThreaSensorThread] receive acq : " << acq << std::endl;

            //            mAcqs.push(std::move(acq));
            //            mAcqs.push({});
            //            *mInputStream >> mAcqs.back();
            //            mAcqs.push(acq.clone());

            // update 2D view
            {
                assert( m_sensor.m_widgetStreamView != nullptr );
                m_sensor.m_widgetStreamView->setData( (unsigned char*)acq.m_data,
                                                      inputStream->m_spec.m_acquisitionSize,
                                                      inputStream->m_spec.m_dims,
                                                      inputStream->m_spec.m_format );
            }

            // update 2D manipulator view
            {
                //                assert(m_sensor.m_widgetStreamViewManipulator != nullptr);
                if ( m_sensor.m_widgetStreamViewManipulator != nullptr ) {
                    m_sensor.m_widgetStreamViewManipulator->setData(
                        (unsigned char*)acq.m_data,
                        inputStream->m_spec.m_acquisitionSize,
                        inputStream->m_spec.m_dims,
                        inputStream->m_spec.m_format );
                }
            }

            // update 3D component
            {
                assert( m_sensor.m_component != nullptr );
                m_sensor.m_component->update( acq );
            }

            ++m_sensor.m_counterFrame;

            //            mAcqs.emplace()
            //            std::cout << "[SensorThread] acqs back : " << mAcqs.front() <<
            //            std::endl; mAcqs.emplace(acq); mAcqs.push(std::move(acq));

            //            emit newAcquisition(mSourceType);
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[SensorThread] catch exception : " << e.what() << std::endl;
        //        std::cout << "[SensorThread] streamingStopped : " << mSourceType <<
        //        std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        //        emit streamingStopped(mSourceType);
        //        mFormInputStreamViews.onKillInputStreamPrivate(mSensorName);
        return;
    }

    std::cout << "[SensorThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

SensorCounterFpsThread::SensorCounterFpsThread( Sensor& sensor, QObject* parent ) :
    QThread( parent ), m_sensor( sensor ) {}

SensorCounterFpsThread::~SensorCounterFpsThread() {
    //    if (this->isRunning()) {
    //        this->requestInterruption();
    //        this->wait();
    //    }
}

void SensorCounterFpsThread::run() {

    //    double fps = 0.0;
    //    m_labelFps.setText("0 Hz");

    while ( !this->isInterruptionRequested() ) {

        int i = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> start =
            std::chrono::high_resolution_clock::now();
        while ( !this->isInterruptionRequested() && i < 10 ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++i;
        }
        std::chrono::time_point<std::chrono::high_resolution_clock> end =
            std::chrono::high_resolution_clock::now();
        double duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();

        char buff[32];
        double fps = 1000.0 * m_sensor.m_counterFrame / duration;
        sprintf( buff, "%.2f Hz", fps );
        //        m_labelFps.setText((std::string(buff) + " Hz").c_str());
        m_sensor.m_itemFps->setText( QString( buff ) );
        //        m_counterFrame = 0;
        m_sensor.m_counterFrame = 0;

        //        std::cout << "[SensorCounterFpsThread:" << m_sensorName << "] update" <<
        //        std::endl; double fps = m_counterFrame * 10.0;
    }
    //    assert(false);
}

/////////////////////////////////////////////////////////////////////

// Sensor::Sensor(IOStream&& iostream, QObject* parent)
//     : QObject { parent }
//     , m_inputSensor(std::move(iostream))
//{
// }

// Sensor::Sensor(IOStream &&iostream, QObject *parent)
//     :QObject{parent}
//     , m_inputSensor(new hub::InputSensor(std::move(iostream)))
//{

//}

// void update(Ra::Engine::Scene::Entity *entity)
//{
////    m_entity.setTransform(entity.);
//    m_entity->setTransform(entity->getTransform());
//}

Sensor::Sensor( std::unique_ptr<hub::InputSensor> inputStream,
                QMdiArea& mdiArea,
                FormImageManipulator* imageManipulator,
                Ra::Engine::RadiumEngine* engine,
                Ra::Gui::Viewer* viewer,
                Ra::Engine::Scene::System* sys,
                Sensor* parentSensor,
                QObject* parent ) :
    QObject( parent ),
    m_inputSensor( std::move( inputStream ) ),
    m_engine( engine ),
    m_viewer( viewer ),
    m_sys( sys ),
    m_mdiArea( mdiArea ),
    m_imageManipulator( imageManipulator )
    //    , m_widgetStreamViewManipulator(m_imageManipulator.getWidgetStreamView())
    //    , m_inputSensor(std::move(inputStream))
    ,
    m_thread( *this, parent ),
    m_counterFpsThread( *this, parent )
//    , m_parentEntity(parentEntity)
{

    // mdiArea window
    {
        assert( m_widgetStreamView == nullptr );

        const auto& dims = m_inputSensor->m_spec.m_dims;
        if ( dims.size() == 1 ) {
            m_widgetStreamView = new WidgetStreamView1D( &m_mdiArea );
            m_widgetStreamView->setMinimumSize( 400, 35 );
        }
        else if ( dims.size() == 2 ) {
            m_widgetStreamView = new WidgetStreamView2D( &m_mdiArea );
            m_widgetStreamView->setMinimumWidth( dims.at( 0 ) );
            m_widgetStreamView->setMinimumHeight( dims.at( 1 ) );
        }
        //        if (m_widgetStreamView != nullptr) {
        assert( m_widgetStreamView != nullptr );

        m_subWindow = m_mdiArea.addSubWindow( m_widgetStreamView );
        m_subWindow->setVisible( true );
        m_subWindow->setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint |
                                     Qt::WindowTitleHint );

        std::string dimText = "(dim: ";
        for ( int i = 0; i < dims.size(); ++i ) {
            dimText += std::to_string( dims.at( i ) );
            if ( i != dims.size() - 1 ) { dimText += " x "; }
        }
        dimText += ")";

        std::string formatText = std::string( "(format: " ) +
                                 hub::SensorSpec::format2string( m_inputSensor->m_spec.m_format ) +
                                 ")";
        m_subWindow->setWindowTitle(
            ( m_inputSensor->m_spec.m_sensorName + "   " + dimText + "   " + formatText ).c_str() );
        //        }
    }

    //     init image manipulator
    //    {
    //        if (m_inputSensor->getDims().size() == 2) {
    //            m_widgetStreamViewManipulator = &imageManipulator.getWidgetStreamView();
    //            m_widgetStreamViewManipulator->init(512, 192, 35.0, 50.0);
    //        }
    //    }

    // create 3D scene object
    {
        assert( m_component == nullptr );
        m_entity = m_engine->getEntityManager()->createEntity( m_inputSensor->m_spec.m_sensorName +
                                                               " entity" );

        //        auto & transformObservers = m_entity->transformationObservers();
        setParent( parentSensor );
        //        if (m_parentEntity != nullptr) {
        //            m_entity->setTransform(m_parentEntity->getTransform());

        //            auto& transformObservers = m_parentEntity->transformationObservers();

        //            //            Ra::Core::Utils::Observable<Ra::Engine::Scene::Entity*> observer
        //            = &SensoSensor::update;
        //            //            std::function<void(const Ra::Engine::Scene::Entity * entity)>
        //            observer = &Stream::updateTransform;
        //            //            std::function<void(const Ra::Engine::Scene::Entity * entity)>
        //            observer = this->updateTransform; std::function<void(const
        //            Ra::Engine::Scene::Entity* entity)> observer = [this](const
        //            Ra::Engine::Scene::Entity* entity) {
        //                assert(m_entity != nullptr);
        //                std::cout << "[Sensor] update transform from observable" << std::endl;
        //                m_entity->setTransform(entity->getTransform());
        //            };
        //            transformObservers.attach(observer);
        //        }

        switch ( m_inputSensor->m_spec.m_format ) {
        case hub::SensorSpec::Format::DOF6:
            //            assert(m_dof6Component == nullptr);
            //            m_dof6Component = new Dof6Component(m_entity);
            m_component = new Dof6Component( *m_inputSensor, m_entity );
            break;

        case hub::SensorSpec::Format::Y8:
        case hub::SensorSpec::Format::Y16:
            m_component = new ScanComponent( *m_inputSensor, m_entity, *m_engine, *m_viewer );
            break;

        default:
            assert( false );
        }
        //    Ra::Engine::Scene::Component* c = new Dof6Component(e);
        m_sys->addComponent( m_entity, m_component );
        m_component->initialize();
        //        m_viewer->glInitialized();
        //        m_viewer->prepareDisplay();

        auto renderer = m_viewer->getRenderer();
        assert( renderer != nullptr );
        m_viewer->makeCurrent();
        renderer->buildAllRenderTechniques();
        auto aabb = Ra::Engine::RadiumEngine::getInstance()->computeSceneAabb();
        //            if ( aabb.isEmpty() ) { getCameraManipulator()->resetCamera(); }
        //            else {
        //                fitCameraToScene( aabb );
        //            }
        m_viewer->doneCurrent();
    }

    m_items.append( new QStandardItem( m_inputSensor->m_spec.m_sensorName.c_str() ) );
    m_items.append( new QStandardItem(
        hub::SensorSpec::format2string( m_inputSensor->m_spec.m_format ).c_str() ) );
    m_items.append(
        new QStandardItem( hub::SensorSpec::dims2string( m_inputSensor->m_spec.m_dims ).c_str() ) );
    m_items.append(
        new QStandardItem( std::to_string( m_inputSensor->m_spec.m_acquisitionSize ).c_str() ) );
    m_itemFps = new QStandardItem( "0" );
    m_items.append( m_itemFps );

    //    m_thread.run();
    m_counterFpsThread.start();
    m_thread.start();
}

Sensor::~Sensor() {
    std::cout << "[Sensor] ~Sensor(" << m_inputSensor->m_spec.m_sensorName << ")" << std::endl;

    if ( m_thread.isRunning() ) {
        m_thread.requestInterruption();
        m_thread.wait();
    }
    if ( m_counterFpsThread.isRunning() ) {
        m_counterFpsThread.requestInterruption();
        m_counterFpsThread.wait();
    }

    // delete observer
    {
        if ( m_parent != nullptr ) {
            auto* parentEntity       = m_parent->m_entity;
            auto& transformObservers = parentEntity->transformationObservers();
            transformObservers.detach( m_observerId );
        }
    }

    // 3D
    m_engine->getEntityManager()->removeEntity( m_entity );
    //    delete m_component;
    m_component = nullptr;
    //    delete m_entity;
    m_entity = nullptr;

    // 2D
    assert( m_widgetStreamView != nullptr );
    //    m_mdiArea.removeSubWindow(m_widgetStreamView->parentWidget());
    m_mdiArea.removeSubWindow( m_subWindow );
    delete m_widgetStreamView;
    m_widgetStreamView = nullptr;
    delete m_subWindow;
    m_subWindow = nullptr;

    if ( m_parent != nullptr ) {
        auto& sons = m_parent->m_sons;
        sons.remove( this );
        //        auto * it = sons.find(this);
        //        assert(it != sons.end());
        //        sons.erase(it);
    }
    for ( auto* sensor : m_sons ) {
        sensor->m_parent = nullptr;
    }
}

void Sensor::updateTransform( const Ra::Engine::Scene::Entity* entity ) {
    //    m_entity.setTransform(entity.);
    m_entity->setTransform( entity->getTransform() );
}

void Sensor::detachFromImageManipulator() {
    if ( m_imageManipulator != nullptr ) {
        if ( m_widgetStreamViewManipulator != nullptr ) {
            m_widgetStreamViewManipulator->clear();
            m_widgetStreamViewManipulator = nullptr;
        }
    }
}

void Sensor::attachFromImageManipulator() {
    assert( m_widgetStreamViewManipulator == nullptr );
    if ( m_inputSensor->m_spec.m_dims.size() == 2 ) {
        if ( m_imageManipulator != nullptr ) {
            m_widgetStreamViewManipulator = &m_imageManipulator->getWidgetStreamView();
            m_widgetStreamViewManipulator->init( 512, 192, 35.0, 50.0 );
            //        m_imageManipulator.
            //        m_imageManipulator.update();
        }
    }
}

void Sensor::setParent( Sensor* parent ) {
    assert( m_parent == nullptr );
    m_parent = parent;

    //    assert(m_parentEntity == nullptr);
    //    m_parentEntity = entity;
    if ( m_parent != nullptr ) {
        auto* parentEntity = m_parent->m_entity;
        m_entity->setTransform( parentEntity->getTransform() );

        auto& transformObservers = parentEntity->transformationObservers();

        m_observer = [this]( const Ra::Engine::Scene::Entity* entity ) {
            assert( m_entity != nullptr );
            std::cout << "[Sensor] update transform from observable" << std::endl;
            m_entity->setTransform( entity->getTransform() );
        };
        m_observerId = transformObservers.attach( m_observer );

        m_parent->m_sons.push_back( this );
    }
}

Ra::Engine::Scene::Entity* Sensor::getEntity() const {
    assert( m_entity != nullptr );
    return m_entity;
}

///////////////////////////////////////////////////////

const QList<QStandardItem*>& Sensor::getItems() const {
    return m_items;
}

Ra::Engine::Scene::Component* Sensor::getComponent() {
    assert( m_component != nullptr );
    return m_component;
    //    switch (m_inputSensor->getFormat()) {
    //    case Sensor::Format::DOF6:
    //        assert(m_dof6Component != nullptr);
    //        return m_dof6Component;
    //        break;

    //    default:
    //        assert(false);
    //    }
}
