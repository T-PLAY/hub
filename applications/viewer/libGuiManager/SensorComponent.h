
#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>

#include <stream.h>

/// This is a very basic component which holds a spinning cube.
struct SensorComponent : public Ra::Engine::Scene::Component {

    //    Component( const std::string& name, Entity* entity );
    SensorComponent(const std::string& name, Ra::Engine::Scene::Entity* entity)
        : Ra::Engine::Scene::Component(name, entity) {};

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    virtual void update(const Stream::Acquisition& acq) = 0;

    // private:
protected:
    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
    Ra::Engine::Rendering::RenderObject* m_ro = nullptr;
};
