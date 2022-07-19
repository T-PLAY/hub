#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <Engine/RadiumEngine.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <list>
#include <Sensor.h>
#include <IOStream.h>
//#include <QAbstractTableModel>
//#include <QTableView>
#include <QStandardItemModel>

class SceneManager : public QObject
{
    Q_OBJECT
public:
    explicit SceneManager(QObject *parent = nullptr);

    void init();

    template <class IOStreamT>
    void addSensor(IOStreamT&& iostream);

    void delSensor(const std::string & sensorName);

signals:


public:
    Ra::Engine::RadiumEngine * m_engine = nullptr;
    Ra::Engine::Scene::System * m_sys = nullptr;
    Ra::Gui::Viewer * m_viewer = nullptr;

    QMdiArea * m_mdiArea = nullptr;

    QStandardItemModel m_sensorModel;

private:
    std::list<Sensor> m_sensors;
//    std::map<std::string, Sensor> m_sensorName2sensor;
//    QStringListModel m_sensorModel;
//    QTableModel m_sensorModel;

    bool m_initialized = false;
};


template<class IOStreamT>
void SceneManager::addSensor(IOStreamT &&iostream)
{
//    m_sensors.push_back(std::make_unique<InputStream>(std::move(iostream)));
//    QList<QStandardItem*> items;

    m_sensors.emplace_back(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, this);

//    const auto & inputStream = *m_sensors.back().m_inputStream;
//    items.append(new QStandardItem(inputStream.getSensorName().c_str()));
//    items.append(new QStandardItem(Stream::format2string[(int)inputStream.getFormat()]));
//    items.append(new QStandardItem(Stream::dims2string(inputStream.getDims()).c_str()));
//    items.append(new QStandardItem(std::to_string(inputStream.getAcquisitionSize()).c_str()));
//    items.append(new QStandardItem("0"));
//    m_sensorModel.appendRow(items);
    m_sensorModel.appendRow(m_sensors.back().getItems());
//    m_sensorModel.setItem(0, 0, new QStandardItem("root"));

//    m_sensorName2sensor[sensorName] = Sensor(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, this);
}

#endif // SCENEMANAGER_H
