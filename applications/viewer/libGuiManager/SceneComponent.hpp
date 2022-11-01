#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
struct SceneComponent : public Ra::Engine::Scene::Component {

    explicit SceneComponent( Ra::Engine::Scene::Entity* entity );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void setVisible( bool visible );

  private:
    Ra::Engine::Rendering::RenderObject* m_roGrid    = nullptr;
    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
};
