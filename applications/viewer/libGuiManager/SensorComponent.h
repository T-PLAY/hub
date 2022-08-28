
#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Data/Mesh.hpp>
#include <vector>

#include <InputSensor.hpp>

/// This is a very basic component which holds a spinning cube.
struct SensorComponent : public Ra::Engine::Scene::Component {

    //    Component( const std::string& name, Entity* entity );
    SensorComponent(const hub::InputSensor & inputSensor, Ra::Engine::Scene::Entity* entity);

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    virtual void update(const hub::Acquisition& acq) = 0;

    virtual void changeTransparency(double transparency) {};
    virtual void changeTransparency2(double transparency) {};

    // private:
protected:
    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
    Ra::Engine::Rendering::RenderObject* m_ro = nullptr;

    const hub::InputSensor & m_inputSensor;
    std::vector<std::shared_ptr<Ra::Engine::Data::Mesh>> m_meshAxis;
};
