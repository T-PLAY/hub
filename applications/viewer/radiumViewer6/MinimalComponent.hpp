#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Scene/SystemDisplay.hpp>


/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
struct MinimalComponent : public Ra::Engine::Scene::Component {

    MinimalComponent(Ra::Engine::Scene::Entity* entity, Ra::Engine::RadiumEngine & e);

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

private:
    Ra::Engine::RadiumEngine & m_engine;
};
