#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/Viewer.hpp>
#include <QObject>

#include <Sensor.h>
#include <list>
//#include <QAbstractTableModel>
//#include <QTableView>
#include <Engine/Scene/Component.hpp>
#include <QStandardItemModel>

#include <SceneComponent.hpp>

class SceneManager : public QObject
{
    Q_OBJECT
  public:
    explicit SceneManager( QObject* parent = nullptr );
    ~SceneManager();

    void init();

    template <class InterfaceT>
    void addSensor( InterfaceT&& interfaceT, const std::string streamName );

    void delSensor( const std::string& streamName );

    void clear();

  signals:

  public:
    Ra::Engine::RadiumEngine* m_engine = nullptr;
    Ra::Gui::Viewer* m_viewer          = nullptr;
    Ra::Engine::Scene::System* m_sys   = nullptr;

    QMdiArea* m_mdiArea = nullptr;
#ifdef ENABLE_IMAGE_VIEWER
    FormImageManipulator* m_imageManipulator = nullptr;
#endif

    QStandardItemModel m_sensorModel;
    QTableView* m_sensorsView = nullptr;
    bool m_enableTrace        = true;
    bool m_enableLive         = true;
    double m_tune0            = 0.25;
    double m_tune1            = 1.0;

    void fitView( const std::string& streamName );
    void enableTrace( bool enable );
    void enableLive( bool enable );

#ifdef ENABLE_IMAGE_VIEWER
    void attachSensorFromImageManipulator( const std::string& streamName );
    void detachSensorFromImageManipulator( const std::string& streamName );
#endif

  public slots:
    void on_tune_valueChanged( double arg1 );
    void on_tune2_valueChanged( double arg1 );
    void on_tune3_valueChanged( double arg1 );
    void on_tune4_valueChanged( double arg1 );
    void on_palette_valueChanged( int arg1 );
    void on_setTransparency( bool isTransparent );

  public:
    SceneComponent* m_sceneComponent = nullptr;

  private:
    std::map<std::string, std::unique_ptr<Sensor>> m_streamName2sensor;

    bool m_initialized = false;
};

template <class InterfaceT>
void SceneManager::addSensor( InterfaceT&& interfaceT, const std::string streamName ) {

    auto&& inputSensor     = std::make_unique<hub::InputSensor>( std::move( interfaceT ) );
    const auto& sensorName = inputSensor->m_spec.m_sensorName;
    const auto& metaData   = inputSensor->m_spec.m_metaData;

    const char* parentName = nullptr;
    if ( metaData.find( "parent" ) != metaData.end() ) {
        parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
    }

    Sensor* parentSensor = nullptr;

    // if parent exist in scene, link to it
    if ( parentName != nullptr ) {
        std::cout << "[SceneManager] searching parent in 3D scene = '" << parentName << "'"
                  << std::endl;
        for ( auto& pair : m_streamName2sensor ) {
            auto& sensor = *pair.second;
            if ( sensor.m_inputSensor->m_spec.m_sensorName == parentName ) {
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
    m_streamName2sensor[streamName] = std::make_unique<Sensor>( std::move( inputSensor ),
                                                                *m_mdiArea,
                                                                m_engine,
                                                                m_viewer,
                                                                m_sys,
                                                                parentSensor,
                                                                streamName,
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
        auto& sensor            = *pair.second;
        const auto& inputSensor = sensor.m_inputSensor;
        const auto& metaData    = inputSensor->m_spec.m_metaData;

        const char* parentName = nullptr;
        if ( metaData.find( "parent" ) != metaData.end() ) {
            parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
            if ( sensorName == parentName ) { sensor.setParent( &newSensor ); }
        }
    }
}

#endif // SCENEMANAGER_H
