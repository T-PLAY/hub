#include "Sensor.h"

//#include <QM
#include <Engine/Scene/EntityManager.hpp>
#include <QMdiSubWindow>
#include <WidgetStreamView.h>

#include <Core/Utils/Observable.hpp>
#include <Engine/Scene/Entity.hpp>

#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>
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

    auto& inputSensor = m_sensor.m_inputSensor;

    try {

        while ( !this->isInterruptionRequested() ) {

            //            Sensor::Acquisition acq;
            //            *mInputStream >> acq;
            const auto& acq = inputSensor->getAcquisition();
            //            std::cout << "[SensorThreaSensorThread] receive acq : " << acq <<
            //            std::endl;

            //            mAcqs.push(std::move(acq));
            //            mAcqs.push({});
            //            *mInputStream >> mAcqs.back();
            //            mAcqs.push(acq.clone());
            const auto& sensorSpec  = inputSensor->m_spec;
            const auto& resolutions = sensorSpec.m_resolutions;
            const auto& measures    = acq.getMeasures();

            // update 2D view
            {
                for ( int i = 0; i < resolutions.size(); ++i ) {
                    const auto& resolution = resolutions.at( i );
                    const auto& dims       = resolution.first;
                    const auto& format     = resolution.second;
                    const auto& measure    = measures.at( i );

                    //                assert( m_sensor.m_widgetStreamViews != nullptr );
                    m_sensor.m_widgetStreamViews[i]->setData(
                        (unsigned char*)measure.m_data, measure.m_size, dims, format );

                    if ( m_sensor.m_widgetStreamViewManipulator != nullptr ) {
                        if ( dims.size() == 2 ) {
                            m_sensor.m_widgetStreamViewManipulator->setData(
                                (unsigned char*)measure.m_data, measure.m_size, dims, format );
                        }
                    }
                }
            }

            // update 2D manipulator view
            //                        {
            //// assert(m_sensor.m_widgetStreamViewManipulator != nullptr);
            //                        const auto& measure = acq.getMeasures().at( 0 );
            //                if ( m_sensor.m_widgetStreamViewManipulator != nullptr ) {
            //                                m_sensor.m_widgetStreamViewManipulator->setData(
            //                                    (unsigned char*)measure.m_data,
            //                                    inputSensor->m_spec.m_acquisitionSize,
            //                                    dims,
            //                                    format );
            //                            }
            //                        }
            //                        const auto& measure = acq.getMeasures().at( 0 );
            //                        const auto& dims    = inputSensor->m_spec.m_resolutions.at( 0
            //                        ).first; const auto& format  =
            //                        inputSensor->m_spec.m_resolutions.at( 0 ).second;

            // update 3D component
            if ( !this->isInterruptionRequested() ) {
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

//        const QModelIndex & modelIndex = m_sensor.m_itemFps->index();
//        m_sensor.m_model.setData(modelIndex, QString(buff));
//        m_sensor.m_model.dataChanged(modelIndex, modelIndex);
//        m_sensor.m_model.itemChanged(m_sensor.m_itemFps);
//        m_sensor.m_view.update();
        m_sensor.m_view.viewport()->update();

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

Sensor::Sensor(std::unique_ptr<hub::InputSensor> inputSensor,
                QMdiArea& mdiArea,
                FormImageManipulator* imageManipulator,
                Ra::Engine::RadiumEngine* engine,
                Ra::Gui::Viewer* viewer,
                Ra::Engine::Scene::System* sys,
                Sensor* parentSensor,
                const std::string & streamName,
                QStandardItemModel &model,
                QTableView & view,
                QObject* parent) :
    QObject( parent ),
    m_inputSensor( std::move( inputSensor ) ),
    m_engine( engine ),
    m_viewer( viewer ),
    m_sys( sys ),
    m_mdiArea( mdiArea ),
    m_imageManipulator( imageManipulator )
    //    , m_widgetStreamViewManipulator(m_imageManipulator.getWidgetStreamView())
    //    , m_inputSensor(std::move(inputSensor))
    ,
    m_thread( *this, parent ),
    m_counterFpsThread( *this, parent ),
    m_model(model),
    m_view(view)

//    , m_parentEntity(parentEntity)
{
    const auto& sensorSpec  = m_inputSensor->m_spec;
    const auto& resolutions = sensorSpec.m_resolutions;

    // create 2D viewers
    // mdiArea window
    for ( const auto& resolution : resolutions ) {
        const auto& dims   = resolution.first;
        const auto& format = resolution.second;

        //        assert( m_widgetStreamViews == nullptr );
        WidgetStreamView* widgetStreamView = nullptr;

        //        const auto& dims = m_inputSensor->m_spec.m_dims;
        if ( dims.size() == 1 ) {
            widgetStreamView = new WidgetStreamView1D( &m_mdiArea );
            widgetStreamView->setMinimumSize( 400, 35 );
        }
        else if ( dims.size() == 2 ) {
            widgetStreamView = new WidgetStreamView2D( &m_mdiArea );
            widgetStreamView->setMinimumWidth( dims.at( 0 ) );
            widgetStreamView->setMinimumHeight( dims.at( 1 ) );
        }
        //        if (widgetStreamView != nullptr) {
        assert( widgetStreamView != nullptr );

        QMdiSubWindow* subWindow = m_mdiArea.addSubWindow( widgetStreamView );
        subWindow->setVisible( true );
        subWindow->setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint |
                                   Qt::WindowTitleHint );

        //        std::string dimText = "(dim: ";
        //        for ( int i = 0; i < dims.size(); ++i ) {
        //            dimText += std::to_string( dims.at( i ) );
        //            if ( i != dims.size() - 1 ) { dimText += " x "; }
        //        }
        //        dimText += ")";
        //        std::string formatText =
        //            std::string( "(format: " ) + hub::SensorSpec::format2string( format ) + ")";
        subWindow->setWindowTitle( ( m_inputSensor->m_spec.m_sensorName + "   " +
                                     hub::SensorSpec::dims2string( dims ) + "   " +
                                     hub::SensorSpec::format2string( format ) )
                                       .c_str() );
        m_subWindows.push_back( subWindow );

        m_widgetStreamViews.push_back( widgetStreamView );
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

        const int resolutionSize = resolutions.size();
        if ( resolutionSize == 1 ) {
            //            const auto& dims   = resolutions.at( 0 ).first;
            const auto& format = resolutions.at( 0 ).second;

            switch ( format ) {
            case hub::SensorSpec::Format::DOF6:
            case hub::SensorSpec::Format::MAT4:
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
        }
        else if ( resolutionSize == 2 ) {
            const auto& format  = resolutions.at( 0 ).second;
            const auto& format2 = resolutions.at( 1 ).second;

            assert( format == hub::SensorSpec::Format::DOF6 );
            //            assert( format2 == hub::SensorSpec::Format::Y16 );
            //            assert( format2 == hub::SensorSpec::Format::RGBA8 );

            m_component = new ScanComponent( *m_inputSensor, m_entity, *m_engine, *m_viewer );
        }

        assert( m_component != nullptr );
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

    m_items.append( new QStandardItem( streamName.c_str() ) );
    m_items.append( new QStandardItem( m_inputSensor->m_spec.m_sensorName.c_str() ) );
    m_items.append(
        new QStandardItem( hub::SensorSpec::resolutions2string( resolutions ).c_str() ) );
    //    m_items.append( new QStandardItem(
    //        hub::SensorSpec::format2string( format ).c_str() ) );
    //    m_items.append(
    //        new QStandardItem( hub::SensorSpec::dims2string( dims ).c_str() ) );
    m_items.append(
        new QStandardItem( std::to_string( m_inputSensor->m_spec.m_acquisitionSize ).c_str() ) );
    m_itemFps = new QStandardItem( "0" );
    m_items.append( m_itemFps );


    //    m_thread.run();
    m_counterFpsThread.start();
    m_thread.start();
}

Sensor::~Sensor() {
    std::cout << "[Sensor] ~Sensor(" << m_inputSensor->m_spec.m_sensorName << ") start"
              << std::endl;

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
    //    assert( m_widgetStreamViews != nullptr );
    //    m_mdiArea.removeSubWindow(m_widgetStreamViews->parentWidget());
    assert( m_widgetStreamViews.size() == m_subWindows.size() );
    for ( int i = 0; i < m_widgetStreamViews.size(); ++i ) {

        auto* widgetStreamView = m_widgetStreamViews.at( i );
        delete widgetStreamView;

        auto* subWindow = m_subWindows.at( i );
        m_mdiArea.removeSubWindow( subWindow );
        delete subWindow;
        //        delete m_widgetStreamViews;
        //        m_widgetStreamViews = nullptr;
        //        m_subWindows = nullptr;
    }

    if ( m_parent != nullptr ) {
        auto& sons = m_parent->m_sons;
        sons.remove( this );
        //        auto * it = sons.find(this);
        //        assert(it != sons.end());
        //        sons.erase(it);
    }
    for ( auto* sensor : m_sons ) {
        sensor->m_entity->setTransform( Ra::Core::Transform::Identity() );
        sensor->m_parent = nullptr;
    }

    detachFromImageManipulator();
    std::cout << "[Sensor] ~Sensor(" << m_inputSensor->m_spec.m_sensorName << ") end" << std::endl;
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

    //    return;
    //    //    RA_DISPLAY_AABB( aabb, Ra::Core::Utils::Color::Blue() );
    assert( m_widgetStreamViewManipulator == nullptr );

    const auto& resolutions = m_inputSensor->m_spec.m_resolutions;
    const auto& dims        = resolutions.at( resolutions.size() - 1 ).first;
    if (dims.size() != 2)
        return;
    const auto& metaData    = m_inputSensor->m_spec.m_metaData;
    double scanWidth        = 1.0;
    if ( metaData.find( "scanWidth" ) != metaData.end() ) {
        scanWidth = std::any_cast<double>( metaData.at( "scanWidth" ) );
    }

    double scanDepth = 1.0;
    if ( metaData.find( "scanDepth" ) != metaData.end() ) {
        scanDepth = std::any_cast<double>( metaData.at( "scanDepth" ) );
    }

    assert( dims.size() == 2 );
    if ( m_imageManipulator != nullptr ) {
        m_widgetStreamViewManipulator = &m_imageManipulator->getWidgetStreamView();
        m_widgetStreamViewManipulator->init( dims.at( 0 ), dims.at( 1 ), scanWidth, scanDepth );
        //        m_imageManipulator.
        //        m_imageManipulator.update();
    }
}

void Sensor::fitView()
{
    auto& viewer = *m_viewer;
    //    auto aabb = Ra::Engine::RadiumEngine::getInstance()->computeSceneAabb();
    //    auto aabb = m_app->m_engine->computeSceneAabb();

    //        const auto& traces = m_comp->getRoTraces();
    //    auto aabb = traces[0].compu
    //        auto aabb = traces[0]->computeAabb();
    //        for (int i = 1; i < g_nTraces; ++i) {
    //            aabb.extend(traces[i]->computeAabb());
    //        }
    //        auto aabb = m_enti
    auto aabb = m_entity->computeAabb();
    RA_CLEAR_DEBUG_DISPLAY();
    RA_DISPLAY_AABB( aabb, Ra::Core::Utils::Color::Green() );

    if ( aabb.isEmpty() ) { viewer.getCameraManipulator()->resetCamera(); }
    else { viewer.fitCameraToScene( aabb ); }


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
            //            std::cout << "[Sensor] update transform from observable" << std::endl;
            m_entity->setTransform( entity->getTransform() );
        };
        m_observerId = transformObservers.attach( m_observer );

        m_parent->m_sons.push_back( this );
        std::cout << "[Sensor] ----> link '" << m_inputSensor->m_spec.m_sensorName
                  << "' to parent '" << m_parent->m_inputSensor->m_spec.m_sensorName << "'"
                  << std::endl;
    }
}

void Sensor::on_tune_valueChanged( double arg1 ) {
    m_component->on_tune_valueChanged( arg1 );
}

void Sensor::on_tune2_valueChanged( double arg1 ) {
    m_component->on_tune2_valueChanged( arg1 );
}

void Sensor::on_tune3_valueChanged( double arg1 ) {
    m_component->on_tune3_valueChanged( arg1 );
}

void Sensor::on_tune4_valueChanged( double arg1 ) {
    m_component->on_tune4_valueChanged( arg1 );
}

void Sensor::on_palette_valueChanged( int palette ) {
    m_component->on_palette_valueChanged( palette );
}

void Sensor::on_setTransparency( bool isTransparent ) {
    m_component->on_setTransparency( isTransparent );
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
