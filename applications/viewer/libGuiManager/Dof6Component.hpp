#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>
#include <SensorComponent.h>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
//struct Dof6Component : public Ra::Engine::Scene::Component {
struct Dof6Component : SensorComponent {

    Dof6Component(const hub::InputSensor & inputStream, Ra::Engine::Scene::Entity* entity);

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void update(const hub::Acquisition & acq) override;
    Ra::Core::Aabb getAabb() const override;
    void enableTrace(bool enable) override;
    void enableLive(bool enable) override;

private:
    hub::SensorSpec::Format m_format;
//    Ra::Engine::Rendering::RenderObject* m_roGrid = nullptr;
//    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };

//    Ra::Engine::Rendering::RenderObject* m_ro = nullptr;

};
