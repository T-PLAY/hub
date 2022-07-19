#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <Engine/RadiumEngine.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <list>
#include <Sensor.h>
#include <IOStream.h>

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


private:
    std::list<Sensor> m_sensors;
//    std::map<std::string, Sensor> m_sensorName2sensor;

    bool m_initialized = false;
};


template<class IOStreamT>
void SceneManager::addSensor(IOStreamT &&iostream)
{
//    m_sensors.push_back(std::make_unique<InputStream>(std::move(iostream)));
    m_sensors.emplace_back(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, this);

//    m_sensorName2sensor[sensorName] = Sensor(std::make_unique<InputStream>(std::move(iostream)), *m_mdiArea, this);
}

#endif // SCENEMANAGER_H
