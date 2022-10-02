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
#define ENABLE_IMAGE_VIEWER

class SceneManager : public QObject
{
    Q_OBJECT
  public:
    explicit SceneManager( QObject* parent = nullptr );
    ~SceneManager();

    void init();

    template <class InterfaceT>
    void addSensor( InterfaceT&& interfaceT );

    void delSensor( const std::string& sensorName );

    void clear();

  signals:

  public:
    Ra::Engine::RadiumEngine* m_engine = nullptr;
    Ra::Gui::Viewer* m_viewer          = nullptr;
    Ra::Engine::Scene::System* m_sys   = nullptr;

    QMdiArea* m_mdiArea                      = nullptr;
    FormImageManipulator* m_imageManipulator = nullptr;

    QStandardItemModel m_sensorModel;

    const std::list<Sensor>& getSensors() const;
    Sensor& getSensor( int iSensor );

    void fitView(int iSensor);
    void attachSensorFromImageManipulator( int iSensor );
    void detachSensorFromImageManipulator( int iSensor );
    //    void detachAllSensorsFromImageManipulator();
    //    void onTransparencyChanged(double transparency);
    //    void onTransparency2Changed(double transparency);
  public slots:
    void on_tune_valueChanged( double arg1 );
    void on_tune2_valueChanged( double arg1 );
    void on_tune3_valueChanged( double arg1 );
    void on_tune4_valueChanged( double arg1 );
    void on_palette_valueChanged( int arg1 );
    void on_setTransparency(bool isTransparent);

  public:
    SceneComponent* m_sceneComponent = nullptr;

  private:
    std::list<Sensor> m_sensors;
    //    std::map<std::string, std::unique_ptr<Sensor>> m_streamName2Sensor;
    //    std::vector<Sensor> m_sensors;

    //    std::map<std::string, Sensor> m_sensorName2sensor;
    //    QStringListModel m_sensorModel;
    //    QTableModel m_sensorModel;

    bool m_initialized = false;
};

template <class InterfaceT>
void SceneManager::addSensor( InterfaceT&& interfaceT ) {
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
        for ( auto& sensor : m_sensors ) {
            if ( sensor.m_inputSensor->m_spec.m_sensorName == parentName ) {
                parentSensor = &sensor;
                //                parentEntity = sensor.getEntity();
                break;
            }
        }

        if ( parentSensor == nullptr ) {
            std::cout << "[SceneManager] the parent is not alive"
                      << std::endl;
            //                    QMessageBox msgBox;
            //                    msgBox.setText((std::string("Could not find '") + parentName + "'
            //                    sensor.\nUnable to attach the sensor '" + sensorName + "' with his
            //                    parent.").c_str()); msgBox.exec();
        }
    }

    m_sensors.emplace_back( std::move( inputSensor ),
                            *m_mdiArea,
                            m_imageManipulator,
                            m_engine,
                            m_viewer,
                            m_sys,
                            parentSensor,
                            this );
    //    m_sensors.emplace_back(std::make_unique<hub::InputSensor>(std::move(interfaceT)),
    //    *m_mdiArea, m_engine, m_viewer, m_sys, this);

    //    items.append(new QStandardItem(inputSensor.getSensorName().c_str()));
    //    items.append(new QStandardItem(Interface::format2string[(int)inputSensor.getFormat()]));
    //    items.append(new QStandardItem(Interface::dims2string(inputSensor.getDims()).c_str()));
    //    items.append(new QStandardItem(std::to_string(inputSensor.getAcquisitionSize()).c_str()));
    //    items.append(new QStandardItem("0"));
    //    m_sensorModel.appendRow(items);

    auto& newSensor = m_sensors.back();
    m_sensorModel.appendRow( newSensor.getItems() );

    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));
    //    const auto & inputSensor = *m_sensors.back().m_inputSensor;
    //    m_sensorName2sensor[sensorName] =
    //    Sensor(std::make_unique<hub::InputSensor>(std::move(interfaceT)), *m_mdiArea, this);

    // prevent all father's sons, the father is coming
    for ( auto& sensor : m_sensors ) {
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
