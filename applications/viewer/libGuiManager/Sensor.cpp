#include "Sensor.h"


#include <QMdiSubWindow>
#include <WidgetStreamView.h>

#include <Engine/Scene/EntityManager.hpp>
#include <Core/Utils/Observable.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Gui/Viewer/RotateAroundCameraManipulator.hpp>

#include <IO/Stream.hpp>
//#include <InputSensor.hpp>
#include <SensorSpec.hpp>

#include <FormStreamViews.h>

SensorThread::SensorThread( Sensor& sensor, QObject* parent ) :
    QThread { parent }, m_sensor( sensor ) {}

SensorThread::~SensorThread() {
    std::cout << "[SensorThread] ~SensorThread(" << m_sensor.m_sensorSpec.m_sensorName << ")"
              << std::endl;
}

void SensorThread::run() {
    std::cout << "[SensorThread] run()" << std::endl;

    assert( FormStreamViews::s_ipv4 != nullptr );
    assert( FormStreamViews::s_port != nullptr );

    hub::InputSensor inputSensor( hub::io::InputStream(
        m_sensor.m_streamName,
        m_sensor.m_syncStreamName,
        //                              hub::net::ClientSocket( m_sensor.m_ipv4, m_sensor.m_port ) )
        //                              );
        hub::net::ClientSocket( *FormStreamViews::s_ipv4, *FormStreamViews::s_port ) ) );
    //    auto& inputSensor = m_sensor.m_inputSensor;
    try {
        while ( !this->isInterruptionRequested() ) {
            const auto acq = inputSensor.getAcquisition();
//            std::cout << "[Sensor] receive acq : " << acq << std::endl;
            m_sensor.update( acq );
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[SensorThread] catch exception : " << e.what() << std::endl;
        return;
    }

    std::cout << "[SensorThread] run() end" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////////

SensorCounterFpsThread::SensorCounterFpsThread( Sensor& sensor, QObject* parent ) :
    QThread( parent ), m_sensor( sensor ) {}

SensorCounterFpsThread::~SensorCounterFpsThread() {}

void SensorCounterFpsThread::run() {

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
        m_sensor.m_itemFps->setText( QString( buff ) );
        m_sensor.m_counterFrame = 0;

        m_sensor.m_view.viewport()->update();
    }
}

/////////////////////////////////////////////////////////////////////

Sensor::Sensor( const std::string& streamName,
                const hub::SensorSpec& sensorSpec,
                const std::string& syncStreamName,
                QMdiArea& mdiArea,
                Ra::Engine::RadiumEngine* engine,
                Ra::Gui::Viewer* viewer,
                Ra::Engine::Scene::System* sys,
                Sensor* parentSensor,
                QStandardItemModel& model,
                QTableView& view,
                QObject* parent ) :
    QObject( parent ),
    //    m_inputSensor( std::move( inputSensor ) ),
    m_streamName( streamName ),
    m_sensorSpec( sensorSpec ),
    m_syncStreamName( syncStreamName ),
    m_engine( engine ),
    m_viewer( viewer ),
    m_sys( sys ),
    m_mdiArea( mdiArea ),
    m_sensorThread( *this, parent ),
    m_counterFpsThread( *this, parent ),
    m_model( model ),
    m_view( view )

{
    //    const auto& sensorSpec  = sensorSpec;
    const auto& resolutions = sensorSpec.m_resolutions;

    // create 2D viewers
    // mdiArea window
    for ( const auto& resolution : resolutions ) {
        const auto& dims   = resolution.first;
        const auto& format = resolution.second;

        WidgetStreamView* widgetStreamView = nullptr;

        if ( dims.size() == 1 ) {
            widgetStreamView = new WidgetStreamView1D( &m_mdiArea );
            widgetStreamView->setMinimumSize( 400, 35 );
        }
        else if ( dims.size() == 2 ) {
            widgetStreamView = new WidgetStreamView2D( &m_mdiArea );
            widgetStreamView->setMinimumWidth( dims.at( 0 ) );
            widgetStreamView->setMinimumHeight( dims.at( 1 ) );
        }
        assert( widgetStreamView != nullptr );

        QMdiSubWindow* subWindow = m_mdiArea.addSubWindow( widgetStreamView );
        subWindow->setVisible( true );
        subWindow->setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint |
                                   Qt::WindowTitleHint );

        subWindow->setWindowTitle( ( m_sensorSpec.m_sensorName + "   " +
                                     hub::SensorSpec::dims2string( dims ) + "   " +
                                     hub::SensorSpec::format2string( format ) )
                                       .c_str() );
        m_subWindows.push_back( subWindow );

        m_widgetStreamViews.push_back( widgetStreamView );
    }

    // create 3D scene object
    {
        assert( m_component == nullptr );
        m_entity =
            m_engine->getEntityManager()->createEntity( m_sensorSpec.m_sensorName + " entity" );

        setParent( parentSensor );

        const int resolutionSize = resolutions.size();
        if ( resolutionSize == 1 ) {
            const auto& format = resolutions.at( 0 ).second;

            switch ( format ) {
            case hub::SensorSpec::Format::DOF6:
            case hub::SensorSpec::Format::MAT4:
                //                m_component = new Dof6Component( *m_inputSensor, m_entity );
                m_component = new Dof6Component( m_sensorSpec, m_entity );
                break;

            case hub::SensorSpec::Format::Y8:
            case hub::SensorSpec::Format::Y16:
            case hub::SensorSpec::Format::BGR8:
                //                m_component = new ScanComponent( *m_inputSensor, m_entity,
                //                *m_engine, *m_viewer );
                m_component = new ScanComponent( m_sensorSpec, m_entity, *m_engine, *m_viewer );
                break;

            default:
                assert( false );
            }
        }
        else if ( resolutionSize == 2 ) {
            const auto& format  = resolutions.at( 0 ).second;
            const auto& format2 = resolutions.at( 1 ).second;

            assert( format == hub::SensorSpec::Format::DOF6 || format2 == hub::SensorSpec::Format::DOF6 );

            //            m_component = new ScanComponent( *m_inputSensor, m_entity, *m_engine,
            //            *m_viewer );
            m_component = new ScanComponent( m_sensorSpec, m_entity, *m_engine, *m_viewer );
        }

        assert( m_component != nullptr );
        m_sys->addComponent( m_entity, m_component );
        m_component->initialize();

        auto renderer = m_viewer->getRenderer();
        assert( renderer != nullptr );
        m_viewer->makeCurrent();
        renderer->buildAllRenderTechniques();
        auto aabb = Ra::Engine::RadiumEngine::getInstance()->computeSceneAabb();
        m_viewer->doneCurrent();
    }

    m_items.append( new QStandardItem( streamName.c_str() ) );
    m_items.append( new QStandardItem( m_sensorSpec.m_sensorName.c_str() ) );
    m_items.append(
        new QStandardItem( hub::SensorSpec::resolutions2string( resolutions ).c_str() ) );
    m_items.append( new QStandardItem( std::to_string( m_sensorSpec.m_acquisitionSize ).c_str() ) );
    m_itemFps = new QStandardItem( "0" );
    m_items.append( m_itemFps );

    m_counterFpsThread.start();

    //    if (m_ipv4 != "") {
    if ( !FormStreamViews::s_autoSync ) { m_sensorThread.start(); }
}

Sensor::~Sensor() {

    std::cout << "[Sensor] ~Sensor(" << m_sensorSpec.m_sensorName << ") start" << std::endl;

    m_mtxUpdating.lock();
    m_lost = true;
    m_mtxUpdating.unlock();

    if ( m_sensorThread.isRunning() ) {
        m_sensorThread.requestInterruption();
        m_sensorThread.wait();
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
    m_component = nullptr;
    m_entity    = nullptr;

    // 2D
    assert( m_widgetStreamViews.size() == m_subWindows.size() );
    for ( int i = 0; i < m_widgetStreamViews.size(); ++i ) {

        auto* widgetStreamView = m_widgetStreamViews.at( i );
        delete widgetStreamView;

        auto* subWindow = m_subWindows.at( i );
        m_mdiArea.removeSubWindow( subWindow );
        delete subWindow;
    }

    if ( m_parent != nullptr ) {
        auto& sons = m_parent->m_sons;
        sons.remove( this );
    }
    for ( auto* sensor : m_sons ) {
        sensor->m_entity->setTransform( Ra::Core::Transform::Identity() );
        sensor->m_parent = nullptr;
    }

#ifdef ENABLE_IMAGE_VIEWER
    detachFromImageManipulator();
#endif
    std::cout << "[Sensor] ~Sensor(" << m_sensorSpec.m_sensorName << ") end" << std::endl;
}

void Sensor::updateTransform( const Ra::Engine::Scene::Entity* entity ) {
    m_entity->setTransform( entity->getTransform() );
}

void Sensor::update( const hub::Acquisition& acq ) {
    //    m_mtxUpdating.lock();
    if ( m_lost ) {
        //        m_mtxUpdating.unlock();
        return;
    }
    m_mtxUpdating.lock();
    assert( !m_lost );
    //            const auto& sensorSpec  = inputSensor->m_spec;
    const auto& resolutions = m_sensorSpec.m_resolutions;
    const auto& measures    = acq.getMeasures();

    // update 2D view
    {
        for ( int i = 0; i < resolutions.size(); ++i ) {
            const auto& resolution = resolutions.at( i );
            const auto& dims       = resolution.first;
            const auto& format     = resolution.second;
            const auto& measure    = measures.at( i );

            assert( !m_lost );
            assert( measure.m_data != nullptr );

            m_widgetStreamViews[i]->setData(
                (unsigned char*)measure.m_data, measure.m_size, dims, format );

            if ( m_widgetStreamViewManipulator != nullptr ) {
                if ( dims.size() == 2 ) {
                    m_widgetStreamViewManipulator->setData(
                        (unsigned char*)measure.m_data, measure.m_size, dims, format );
                }
            }
        }
    }

    // update 2D manipulator view
    //// assert(m_sensor.m_widgetStreamViewManipulator != nullptr);

    // update 3D component
    //            if ( !this->isInterruptionRequested() ) {
    assert( m_component != nullptr );
    m_component->update( acq );
    //            }

    ++m_counterFrame;

    m_mtxUpdating.unlock();
}

#ifdef ENABLE_IMAGE_VIEWER
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

    const auto& resolutions = m_sensorSpec.m_resolutions;
    const auto& dims        = resolutions.at( resolutions.size() - 1 ).first;
    if ( dims.size() != 2 ) return;
    const auto& metaData = m_sensorSpec.m_metaData;
    double scanWidth     = 1.0;
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
    }
}
#endif

void Sensor::fitView() {

    auto& viewer = *m_viewer;

    auto aabb = m_component->getAabb();

    RA_CLEAR_DEBUG_DISPLAY();
    RA_DISPLAY_AABB( aabb, Ra::Core::Utils::Color::Green() );

    if ( aabb.isEmpty() ) { viewer.getCameraManipulator()->resetCamera(); }
    else { viewer.fitCameraToScene( aabb ); }
}

void Sensor::setParent( Sensor* parent ) {
    assert( m_parent == nullptr );
    m_parent = parent;

    if ( m_parent != nullptr ) {
        auto* parentEntity = m_parent->m_entity;
        m_entity->setTransform( parentEntity->getTransform() );

        auto& transformObservers = parentEntity->transformationObservers();

        m_observer = [this]( const Ra::Engine::Scene::Entity* entity ) {
            assert( m_entity != nullptr );
            m_entity->setTransform( entity->getTransform() );
        };
        m_observerId = transformObservers.attach( m_observer );

        m_parent->m_sons.push_back( this );
        std::cout << "[Sensor] ----> link '" << m_sensorSpec.m_sensorName << "' to parent '"
                  << m_parent->m_sensorSpec.m_sensorName << "'" << std::endl;
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
}

SensorComponent* Sensor::getSensorComponent() {
    assert( m_component != nullptr );
    return m_component;
}
