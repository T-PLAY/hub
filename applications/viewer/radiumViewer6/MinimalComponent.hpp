#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
struct MinimalComponent : public Ra::Engine::Scene::Component {

    MinimalComponent( Ra::Engine::Scene::Entity* entity, Ra::Engine::RadiumEngine& e );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void updateShader();

    void updateProbe(Ra::Core::Vector3 pos, Ra::Core::Quaternion orientation = Ra::Core::Quaternion());

  public:
    Ra::Engine::Rendering::RenderObject* m_scan          = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe         = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe_axis[3] = { nullptr, nullptr, nullptr };

  private:
    Ra::Engine::RadiumEngine& m_engine;
};
