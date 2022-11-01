#include "SceneManager.h"

#include <Engine/Scene/EntityManager.hpp>
#include <SceneComponent.hpp>

#include <QList>
#include <ScanMaterial/ScanMaterial.hpp>

SceneManager::SceneManager( QObject* parent ) : QObject { parent } {}

SceneManager::~SceneManager() {
    std::cout << "[SceneManager] ~SceneManager() start" << std::endl;

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
}

void SceneManager::delSensor( const std::string& streamName ) {

    if ( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() ) {
        m_streamName2sensor.erase( streamName );

        QList<QStandardItem*> lst =
            m_sensorModel.findItems( streamName.c_str(), Qt::MatchExactly, 0 );
        assert( lst.size() == 1 );
        m_sensorModel.removeRow( lst.front()->index().row() );
    }
}

void SceneManager::clear() {
    m_streamName2sensor.clear();
}

// const std::list<Sensor>& SceneManager::getSensors() const {
// }

// Sensor& SceneManager::getSensor( int iSensor ) {
// }

void SceneManager::fitView( const std::string& streamName ) {
    // void SceneManager::fitView(int iSensor)
    //{
    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = *m_streamName2sensor.at( streamName );
    sensor.fitView();
}

void SceneManager::enableTrace( bool enable ) {
    assert( enable != m_enableTrace );
    m_enableTrace = enable;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
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
        auto& sensor = *pair.second;
        sensor.detachFromImageManipulator();
    }

    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = m_streamName2sensor.at( streamName );
    sensor->attachFromImageManipulator();
}

void SceneManager::detachSensorFromImageManipulator( const std::string& streamName ) {
    if ( m_streamName2sensor.find( streamName ) == m_streamName2sensor.end() ) return;
    assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
    auto& sensor = m_streamName2sensor.at( streamName );
    sensor->detachFromImageManipulator();
}
#endif

void SceneManager::on_tune_valueChanged( double arg1 ) {
    m_tune0 = arg1;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune_valueChanged( m_tune0 );
    }
}

void SceneManager::on_tune2_valueChanged( double arg1 ) {
    m_tune1 = arg1;
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune2_valueChanged( m_tune1 );
    }
}

void SceneManager::on_tune3_valueChanged( double arg1 ) {

    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune3_valueChanged( arg1 );
    }
}

void SceneManager::on_tune4_valueChanged( double arg1 ) {

    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_tune4_valueChanged( arg1 );
    }
}

void SceneManager::on_palette_valueChanged( int palette ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_palette_valueChanged( palette );
    }
}

void SceneManager::on_setTransparency( bool isTransparent ) {
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        sensor.on_setTransparency( isTransparent );
    }
}
