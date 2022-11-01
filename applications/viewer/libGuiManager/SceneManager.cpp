#include "SceneManager.h"

#include <Engine/Scene/EntityManager.hpp>
#include <SceneComponent.hpp>

#include <QList>
#include <ScanMaterial/ScanMaterial.hpp>

SceneManager::SceneManager( QObject* parent ) : QObject { parent } {}

SceneManager::~SceneManager() {
    std::cout << "[SceneManager] ~SceneManager() start" << std::endl;

    //    m_sensors.clear();
    m_streamName2sensor.clear();

    std::cout << "[SceneManager] ~SceneManager() end" << std::endl;
}

void SceneManager::init() {
    assert( m_engine != nullptr );
    assert( m_viewer != nullptr );
    assert( m_sys != nullptr );
    assert( m_mdiArea != nullptr );
#ifdef ENABLE_IMAGE_VIEWER
    assert( m_imageManipulator != nullptr );
#endif

    Ra::Engine::Data::ScanMaterial::registerMaterial();

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* e = m_engine->getEntityManager()->createEntity( "Scene entity" );
    m_sceneComponent             = new SceneComponent( e );
    //    Dof6Component* c = new Dof6Component(e);
    m_sys->addComponent( e, m_sceneComponent );
    m_sceneComponent->initialize();

    m_initialized = true;

    m_viewer->prepareDisplay();

    m_sensorModel.setColumnCount( 5 );
    QStringList header;
    header << "Stream name"
           << "Sensor name"
           << "Resolutions"
           << "Size"
           << "Frequency";
    m_sensorModel.setHorizontalHeaderLabels( header );
    //    QList<QStandardItem*> items;
    //    items.append(QStandardItem("hello"));
    //    items << "ouou";
    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));

    //    m_sensorModel.appendRow(&items);
    //    m_sensorModel.app

    //    m_sensorModel.se
    //    m_sensorModel.row
}

void SceneManager::delSensor( const std::string& streamName ) {

    if ( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() ) {
        //        assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
        //    m_streamName2sensor.erase(m_streamName2sensor.find(streamName));
        m_streamName2sensor.erase( streamName );

        QList<QStandardItem*> lst =
            m_sensorModel.findItems( streamName.c_str(), Qt::MatchExactly, 0 );
        assert( lst.size() == 1 );
        m_sensorModel.removeRow( lst.front()->index().row() );

        //    auto it = m_sensors.begin();
        //    int i   = 0;
        //    while ( it != m_sensors.end() ) {
        //        auto& sensor = *it;
        //        if ( sensor.m_inputSensor->m_spec.m_sensorName == streamName ) {
        //            std::cout << "[SceneManager] delSensor( " << streamName << " )" << std::endl;

        //            it = m_sensors.erase( it );
        //            m_sensorModel.removeRow( i );
        //            continue;
        //        }
        //        ++it;
        //        ++i;
        //    }
    }
}

void SceneManager::clear() {
    //    m_sensors.clear();
    m_streamName2sensor.clear();
}

// const std::list<Sensor>& SceneManager::getSensors() const {
//     return m_sensors;
// }

// Sensor& SceneManager::getSensor( int iSensor ) {
//     assert( iSensor < m_sensors.size() );
//     int i = 0;
//     for ( auto& sensor : m_sensors ) {
//         if ( i == iSensor ) { return sensor; }
//         ++i;
//     }
//     assert( false );
//     return m_sensors.front();
// }

void SceneManager::fitView( const std::string& streamName ) {
    // void SceneManager::fitView(int iSensor)
    //{
    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = *m_streamName2sensor.at( streamName );
    sensor.fitView();

    //    int i = 0;
    //    for ( auto& sensor : m_sensors ) {
    //        if ( i == iSensor ) { sensor.fitView(); }
    //        ++i;
    //    }
}

void SceneManager::enableTrace( bool enable ) {
    assert( enable != m_enableTrace );
    m_enableTrace = enable;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //        sensor.
        sensor.getSensorComponent()->enableTrace( m_enableTrace );
    }
}

void SceneManager::enableLive( bool enable ) {
    assert( enable != m_enableLive );
    m_enableLive = enable;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.getSensorComponent()->enableLive( m_enableLive );
    }
}

#ifdef ENABLE_IMAGE_VIEWER
void SceneManager::attachSensorFromImageManipulator( const std::string& streamName ) {
    for ( auto& pair : m_streamName2sensor ) {
        //    for ( auto& sensor : m_sensors ) {
        auto& sensor = *pair.second;
        sensor.detachFromImageManipulator();
    }

    //    const auto & streamName = modelIndex.
    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = m_streamName2sensor.at( streamName );
    sensor->attachFromImageManipulator();
    //    int i = 0;
    //    for ( auto& sensor : m_sensors ) {
    //        if ( i == iSensor ) { sensor.attachFromImageManipulator(); }
    //        ++i;
    //    }
}

void SceneManager::detachSensorFromImageManipulator( const std::string& streamName ) {
    if ( m_streamName2sensor.find( streamName ) == m_streamName2sensor.end() ) return;
    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = m_streamName2sensor.at( streamName );
    sensor->detachFromImageManipulator();
    //    int i = 0;
    //    for ( auto& sensor : m_sensors ) {
    //        if ( i == iSensor ) { sensor.detachFromImageManipulator(); }
    //        ++i;
    //    }
}
#endif

void SceneManager::on_tune_valueChanged( double arg1 ) {
    m_tune0 = arg1;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //    for ( auto& sensor : m_sensors ) {
        sensor.on_tune_valueChanged( m_tune0 );
    }
}

void SceneManager::on_tune2_valueChanged( double arg1 ) {
    m_tune1 = arg1;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //    for ( auto& sensor : m_sensors ) {
        sensor.on_tune2_valueChanged( m_tune1 );
    }
}

void SceneManager::on_tune3_valueChanged( double arg1 ) {

    //    for ( auto& sensor : m_sensors ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune3_valueChanged( arg1 );
    }
}

void SceneManager::on_tune4_valueChanged( double arg1 ) {

    //    for ( auto& sensor : m_sensors ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune4_valueChanged( arg1 );
    }
}

void SceneManager::on_palette_valueChanged( int palette ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //    for ( auto& sensor : m_sensors ) {
        sensor.on_palette_valueChanged( palette );
    }
}

void SceneManager::on_setTransparency( bool isTransparent ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //    for ( auto& sensor : m_sensors ) {
        sensor.on_setTransparency( isTransparent );
    }
}
