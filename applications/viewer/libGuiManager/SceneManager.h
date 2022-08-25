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

class SceneManager : public QObject {
    Q_OBJECT
public:
    explicit SceneManager(QObject* parent = nullptr);

    void init();

    template <class IOStreamT>
    void addSensor(IOStreamT&& iostream);

    void delSensor(const std::string& sensorName);

signals:

public:
    Ra::Engine::RadiumEngine* m_engine = nullptr;
    Ra::Gui::Viewer* m_viewer = nullptr;
    Ra::Engine::Scene::System* m_sys = nullptr;

    QMdiArea* m_mdiArea = nullptr;
    FormImageManipulator* m_imageManipulator = nullptr;

    QStandardItemModel m_sensorModel;

    const std::list<Sensor>& getSensors() const;
    Sensor& getSensor(int iSensor);

    void attachSensorFromImageManipulator(int iSensor);
    void detachSensorFromImageManipulator(int iSensor);
    //    void detachAllSensorsFromImageManipulator();

private:
    std::list<Sensor> m_sensors;
    //    std::vector<Sensor> m_sensors;

    //    std::map<std::string, Sensor> m_sensorName2sensor;
    //    QStringListModel m_sensorModel;
    //    QTableModel m_sensorModel;

    bool m_initialized = false;
};

template <class IOStreamT>
void SceneManager::addSensor(IOStreamT&& iostream)
{
    //    m_sensors.push_back(std::make_unique<hub::InputSensor>(std::move(iostream)));
    //    QList<QStandardItem*> items;

    auto&& inputStream = std::make_unique<hub::InputSensor>(std::move(iostream));
    const auto& sensorName = inputStream->m_spec.m_sensorName;
    const auto& metaData = inputStream->m_spec.m_metaData;

    const char* parentName = nullptr;
    if (metaData.find("parent") != metaData.end()) {
        parentName = std::any_cast<const char*>(metaData.at("parent"));
    }

//    Ra::Engine::Scene::Entity* parentEntity = nullptr;
    Sensor * parentSensor = nullptr;

    // if parent exist in scene, link to it
    if (parentName != nullptr) {
        std::cout << "[SceneManager] searching parent in 3D scene = '" << parentName << "'" << std::endl;
        for (auto& sensor : m_sensors) {
            if (sensor.m_inputStream->m_spec.m_sensorName == parentName) {
                parentSensor = &sensor;
//                parentEntity = sensor.getEntity();
                break;
            }
        }

//                if (parentSensor == nullptr) {
//                    QMessageBox msgBox;
//                    msgBox.setText((std::string("Could not find '") + parentName + "' sensor.\nUnable to attach the sensor '" + sensorName + "' with his parent.").c_str());
//                    msgBox.exec();
//                }
    }

    m_sensors.emplace_back(std::move(inputStream), *m_mdiArea, *m_imageManipulator, m_engine, m_viewer, m_sys, parentSensor, this);
    //    m_sensors.emplace_back(std::make_unique<hub::InputSensor>(std::move(iostream)), *m_mdiArea, m_engine, m_viewer, m_sys, this);

    //    items.append(new QStandardItem(inputStream.getSensorName().c_str()));
    //    items.append(new QStandardItem(Stream::format2string[(int)inputStream.getFormat()]));
    //    items.append(new QStandardItem(Stream::dims2string(inputStream.getDims()).c_str()));
    //    items.append(new QStandardItem(std::to_string(inputStream.getAcquisitionSize()).c_str()));
    //    items.append(new QStandardItem("0"));
    //    m_sensorModel.appendRow(items);

    auto& newSensor = m_sensors.back();
    m_sensorModel.appendRow(newSensor.getItems());

    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));
    //    const auto & inputStream = *m_sensors.back().m_inputStream;
    //    m_sensorName2sensor[sensorName] = Sensor(std::make_unique<hub::InputSensor>(std::move(iostream)), *m_mdiArea, this);

    // prevent all father's sons, the father is comming
    for (auto& sensor : m_sensors) {
        const auto& inputStream = sensor.m_inputStream;
        const auto& metaData = inputStream->m_spec.m_metaData;

        const char* parentName = nullptr;
        if (metaData.find("parent") != metaData.end()) {
            parentName = std::any_cast<const char*>(metaData.at("parent"));
            if (sensorName == parentName) {
//                sensor.setParentEntity(newSensor.getEntity());
                sensor.setParent(&newSensor);
            }
        }

        //            if (sensor.m_inputStream->getSensorName() == parentName) {
        //                parentEntity = sensor.getEntity();
        //                break;
        //            }
    }
}

#endif // SCENEMANAGER_H
