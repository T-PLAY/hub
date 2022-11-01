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

    //    const std::list<Sensor>& getSensors() const;
    //    Sensor& getSensor( int iSensor );

    void fitView( const std::string& streamName );
    void enableTrace( bool enable );
    void enableLive( bool enable );

#ifdef ENABLE_IMAGE_VIEWER
    void attachSensorFromImageManipulator( const std::string& streamName );
    void detachSensorFromImageManipulator( const std::string& streamName );
#endif
    //        void attachSensorFromImageManipulator( int iSensor );
    //        void detachSensorFromImageManipulator( int iSensor );

    //    void detachAllSensorsFromImageManipulator();
    //    void onTransparencyChanged(double transparency);
    //    void onTransparency2Changed(double transparency);
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
    //    std::list<Sensor> m_sensors;
    std::map<std::string, std::unique_ptr<Sensor>> m_streamName2sensor;

    //    std::map<std::string, std::unique_ptr<Sensor>> m_streamName2Sensor;
    //    std::vector<Sensor> m_sensors;

    //    std::map<std::string, Sensor> m_sensorName2sensor;
    //    QStringListModel m_sensorModel;
    //    QTableModel m_sensorModel;

    bool m_initialized = false;
};

template <class InterfaceT>
void SceneManager::addSensor( InterfaceT&& interfaceT, const std::string streamName ) {
    //    m_sensors.push_back(std::make_unique<hub::InputSensor>(std::move(interfaceT)));
    //    QList<QStandardItem*> items;

    auto&& inputSensor     = std::make_unique<hub::InputSensor>( std::move( interfaceT ) );
    const auto& sensorName = inputSensor->m_spec.m_sensorName;
    const auto& metaData   = inputSensor->m_spec.m_metaData;

    const char* parentName = nullptr;
    if ( metaData.find( "parent" ) != metaData.end() ) {
        parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
    }

    //    Ra::Engine::Scene::Entity* parentEntity = nullptr;
    Sensor* parentSensor = nullptr;

    // if parent exist in scene, link to it
    if ( parentName != nullptr ) {
        std::cout << "[SceneManager] searching parent in 3D scene = '" << parentName << "'"
                  << std::endl;
        for ( auto& pair : m_streamName2sensor ) {
            //        for ( auto& sensor : m_sensors ) {
            //            auto & streamName = pair.first;
            auto& sensor = *pair.second;
            if ( sensor.m_inputSensor->m_spec.m_sensorName == parentName ) {
                parentSensor = &sensor;
                //                parentEntity = sensor.getEntity();
                break;
            }
        }

        if ( parentSensor == nullptr ) {
            std::cout << "[SceneManager] the parent is not alive" << std::endl;
            //                    QMessageBox msgBox;
            //                    msgBox.setText((std::string("Could not find '") + parentName + "'
            //                    sensor.\nUnable to attach the sensor '" + sensorName + "' with his
            //                    parent.").c_str()); msgBox.exec();
        }
    }

    assert( m_streamName2sensor.find( streamName ) == m_streamName2sensor.end() );
    //    m_sensors.emplace_back( std::move( inputSensor ),
    //                            *m_mdiArea,
    //                            m_imageManipulator,
    //                            m_engine,
    //                            m_viewer,
    //                            m_sys,
    //                            parentSensor,
    //                            this );
    assert( m_sensorsView != nullptr );
    m_streamName2sensor[streamName] = std::make_unique<Sensor>( std::move( inputSensor ),
                                                                *m_mdiArea,
                                                                //        m_imageManipulator,
                                                                m_engine,
                                                                m_viewer,
                                                                m_sys,
                                                                parentSensor,
                                                                streamName,
                                                                m_sensorModel,
                                                                *m_sensorsView,
                                                                this );

    //    Sensor sensor( std::move( inputSensor ),
    //                   *m_mdiArea,
    //                   m_imageManipulator,
    //                   m_engine,
    //                   m_viewer,
    //                   m_sys,
    //                   parentSensor,
    //                   this );
    //    m_streamName2sensor[streamName] = std::move(sensor);
    //    m_streamName2sensor.emplace( std::make_pair<std::string, Sensor>( streamName,
    //    m_streamName2sensor.emplace(  streamName, std::move(sensor));
    //                                                                      Sensor { std::move(
    //                                                                      inputSensor ),
    //                                                                        *m_mdiArea,
    //                                                                        m_imageManipulator,
    //                                                                        m_engine,
    //                                                                        m_viewer,
    //                                                                        m_sys,
    //                                                                        parentSensor,
    //                                                                        this }  );
    //    m_sensors.emplace_back(std::make_unique<hub::InputSensor>(std::move(interfaceT)),
    //    *m_mdiArea, m_engine, m_viewer, m_sys, this);

    //    items.append(new QStandardItem(inputSensor.getSensorName().c_str()));
    //    items.append(new QStandardItem(Interface::format2string[(int)inputSensor.getFormat()]));
    //    items.append(new QStandardItem(Interface::dims2string(inputSensor.getDims()).c_str()));
    //    items.append(new QStandardItem(std::to_string(inputSensor.getAcquisitionSize()).c_str()));
    //    items.append(new QStandardItem("0"));
    //    m_sensorModel.appendRow(items);

    //    auto& newSensor = m_sensors.back();
    auto& newSensor = *m_streamName2sensor.at( streamName );
    newSensor.getSensorComponent()->enableTrace( m_enableTrace );
    newSensor.getSensorComponent()->enableLive( m_enableLive );
    newSensor.on_tune_valueChanged( m_tune0 );
    newSensor.on_tune2_valueChanged( m_tune1 );
#ifdef ENABLE_IMAGE_VIEWER
    newSensor.m_imageManipulator = m_imageManipulator;
#endif
    m_sensorModel.appendRow( newSensor.getItems() );

    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));
    //    const auto & inputSensor = *m_sensors.back().m_inputSensor;
    //    m_sensorName2sensor[streamName] =
    //    Sensor(std::make_unique<hub::InputSensor>(std::move(interfaceT)), *m_mdiArea, this);

    // prevent all father's sons, the father is coming
    for ( auto& pair : m_streamName2sensor ) {
        //    for ( auto& sensor : m_sensors ) {
        auto& sensor            = *pair.second;
        const auto& inputSensor = sensor.m_inputSensor;
        const auto& metaData    = inputSensor->m_spec.m_metaData;

        const char* parentName = nullptr;
        if ( metaData.find( "parent" ) != metaData.end() ) {
            parentName = std::any_cast<const char*>( metaData.at( "parent" ) );
            if ( sensorName == parentName ) {
                //                sensor.setParentEntity(newSensor.getEntity());
                sensor.setParent( &newSensor );
            }
        }

        //            if (sensor.m_inputSensor->getSensorName() == parentName) {
        //                parentEntity = sensor.getEntity();
        //                break;
        //            }
    }
}

#endif // SCENEMANAGER_H
