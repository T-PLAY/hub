#include "SceneManager.h"

#include <Engine/Scene/EntityManager.hpp>
#include <SceneComponent.hpp>

SceneManager::SceneManager(QObject* parent)
    : QObject { parent }
{
}

void SceneManager::init()
{
    assert(m_engine != nullptr);
    assert(m_sys != nullptr);
    assert(m_viewer != nullptr);
    assert(m_mdiArea != nullptr);

    // Create and initialize entity and component
    Ra::Engine::Scene::Entity* e = m_engine->getEntityManager()->createEntity("Scene entity");
    SceneComponent* c = new SceneComponent(e);
    m_sys->addComponent(e, c);
    c->initialize();

    m_initialized = true;

    m_viewer->prepareDisplay();
}

void SceneManager::delSensor(const std::string& sensorName)
{
    auto it = m_sensors.begin();
    while (it != m_sensors.end()) {
        auto& sensor = *it;
        if (sensor.m_inputStream->getSensorName() == sensorName) {
            std::cout << "[SceneManager] delSensor( " << sensorName << " )" << std::endl;
            it = m_sensors.erase(it);
            continue;
        }
        ++it;
    }
}
