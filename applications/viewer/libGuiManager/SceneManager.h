#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/EntityManager.hpp>
#include <Gui/Viewer/Viewer.hpp>
#include <QObject>

#include <IOStream.h>
#include <Sensor.h>
#include <list>
//#include <QAbstractTableModel>
//#include <QTableView>
#include <Engine/Scene/Component.hpp>
#include <QStandardItemModel>
#include <stream.h>

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

    QStandardItemModel m_sensorModel;

private:
    std::list<Sensor> m_sensors;
    //    std::map<std::string, Sensor> m_sensorName2sensor;
    //    QStringListModel m_sensorModel;
    //    QTableModel m_sensorModel;

    bool m_initialized = false;
};

template <class IOStreamT>
void SceneManager::addSensor(IOStreamT&& iostream)
{
    //    m_sensors.push_back(std::make_unique<InputStream>(std::move(iostream)));
    //    QList<QStandardItem*> items;

    auto&& inputStream = std::make_unique<InputStream>(std::move(iostream));
    const auto& sensorName = inputStream->getSensorName();
    const auto& metaData = inputStream->getMetaData();

    const char* parentName = nullptr;
    if (metaData.find("parent") != metaData.end()) {
        parentName = std::any_cast<const char*>(metaData.at("parent"));
    }

    Ra::Engine::Scene::Entity* parentEntity = nullptr;

    if (parentName != nullptr) {
        std::cout << "[SceneManager] searching parent in 3D scene = '" << parentName << "'" << std::endl;
        for (const auto& sensor : m_sensors) {
            if (sensor.m_inputStream->getSensorName() == parentName) {
                parentEntity = sensor.getEntity();
                break;
            }
        }

        if (parentEntity == nullptr) {
            QMessageBox msgBox;
            msgBox.setText((std::string("Could not find '") + parentName + "' sensor.\nUnable to attach the sensor '" + sensorName + "' with his parent.").c_str());
            msgBox.exec();
        }
    }

    m_sensors.emplace_back(std::move(inputStream), *m_mdiArea, m_engine, m_viewer, m_sys, parentEntity, this);
    //    m_sensors.emplace_back(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, m_engine, m_viewer, m_sys, this);

    //    items.append(new QStandardItem(inputStream.getSensorName().c_str()));
    //    items.append(new QStandardItem(Stream::format2string[(int)inputStream.getFormat()]));
    //    items.append(new QStandardItem(Stream::dims2string(inputStream.getDims()).c_str()));
    //    items.append(new QStandardItem(std::to_string(inputStream.getAcquisitionSize()).c_str()));
    //    items.append(new QStandardItem("0"));
    //    m_sensorModel.appendRow(items);
    m_sensorModel.appendRow(m_sensors.back().getItems());
    //    m_sensorModel.setItem(0, 0, new QStandardItem("root"));

    //    const auto & inputStream = *m_sensors.back().m_inputStream;
    //    m_sensorName2sensor[sensorName] = Sensor(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, this);
}

#endif // SCENEMANAGER_H
