#include "SceneManager.h"

#include <Engine/Scene/EntityManager.hpp>
#include <SceneComponent.hpp>

#include <QList>
#include <ScanMaterial/ScanMaterial.hpp>

SceneManager::SceneManager( QObject* parent ) : QObject { parent } {}

SceneManager::~SceneManager() {
    std::cout << "[SceneManager] ~SceneManager() start" << std::endl;

    m_mtxSensors.lock();
    m_streamName2sensor.clear();
    m_mtxSensors.unlock();

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

void SceneManager::addSensor(const std::string &streamName, const hub::SensorSpec &sensorSpec, const std::string &syncStreamName) {

    //    auto&& inputSensor     = std::make_unique<hub::InputSensor>( std::move( interfaceT ) );
    const auto& sensorName = sensorSpec.m_sensorName;
    const auto& metaData   = sensorSpec.m_metaData;

    const char* parentName = nullptr;
    if ( metaData.find( "parent" ) != metaData.end() ) {
        parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
    }

//    m_mtxSensors.lock();
    Sensor* parentSensor = nullptr;

    // if parent exist in scene, link to it
    if ( parentName != nullptr ) {
        std::cout << "[SceneManager] searching parent in 3D scene = '" << parentName << "'"
                  << std::endl;
        for ( auto& pair : m_streamName2sensor ) {
            auto& sensor = *pair.second;
            if ( sensor.m_sensorSpec.m_sensorName == parentName ) {
                parentSensor = &sensor;
                break;
            }
        }

        if ( parentSensor == nullptr ) {
            std::cout << "[SceneManager] the parent is not alive" << std::endl;
        }
    }

    assert( m_streamName2sensor.find( streamName ) == m_streamName2sensor.end() );
    assert( m_sensorsView != nullptr );
    m_streamName2sensor[streamName] = std::make_unique<Sensor>(
                                                                streamName,
                sensorSpec,
                syncStreamName,
                                                                *m_mdiArea,
                                                                m_engine,
                                                                m_viewer,
                                                                m_sys,
                                                                parentSensor,
                                                                m_sensorModel,
                                                                *m_sensorsView,
                                                                this );

    auto& newSensor = *m_streamName2sensor.at( streamName );
    newSensor.getSensorComponent()->enableTrace( m_enableTrace );
    newSensor.getSensorComponent()->enableLive( m_enableLive );
    newSensor.on_tune_valueChanged( m_tune0 );
    newSensor.on_tune2_valueChanged( m_tune1 );
#ifdef ENABLE_IMAGE_VIEWER
    newSensor.m_imageManipulator = m_imageManipulator;
#endif
    m_sensorModel.appendRow( newSensor.getItems() );

    // prevent all father's sons, the father is coming
    for ( auto& pair : m_streamName2sensor ) {
        auto& sensor = *pair.second;
        //        const auto& inputSensor = sensor.m_inputSensor;
        const auto& metaData = sensor.m_sensorSpec.m_metaData;

        const char* parentName = nullptr;
        if ( metaData.find( "parent" ) != metaData.end() ) {
            parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
            if ( sensorName == parentName ) { sensor.setParent( &newSensor ); }
        }
    }
//    m_mtxSensors.unlock();
}

void SceneManager::delSensor(const std::string &streamName, const hub::SensorSpec &sensorSpec, const std::string &syncStreamName) {

    m_mtxSensors.lock();
    if ( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() ) {
        m_streamName2sensor.erase( streamName );

        QList<QStandardItem*> lst =
            m_sensorModel.findItems( streamName.c_str(), Qt::MatchExactly, 0 );
        assert( lst.size() == 1 );
        m_sensorModel.removeRow( lst.front()->index().row() );
    }
    m_mtxSensors.unlock();
}

const Sensor *SceneManager::getSensor(const std::string &streamName) const
{
//    m_mtxSensors.lock();
    while ( m_streamName2sensor.find( streamName ) == m_streamName2sensor.end() ) {
//    m_mtxSensors.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "[SceneManager] getSensor() waiting for stream '" << streamName << "' inited" << std::endl;
//    m_mtxSensors.lock();
    }
//    if ( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() ) {
        assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
//    m_mtxSensors.unlock();
        return m_streamName2sensor.at(streamName).get();
//        m_streamName2sensor.at( streamName )->update( acq );
//    }
}

void SceneManager::newAcquisition( const std::string& streamName, const hub::Acquisition& acq ) {
    m_mtxSensors.lock();
    if ( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() ) {
        assert( m_streamName2sensor.find( streamName ) != m_streamName2sensor.end() );
        m_streamName2sensor.at( streamName )->update( acq );
    }
    m_mtxSensors.unlock();
}

void SceneManager::clear() {
    m_mtxSensors.lock();
    m_streamName2sensor.clear();
    m_mtxSensors.unlock();
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
