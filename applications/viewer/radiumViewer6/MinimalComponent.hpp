#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>
#include <Gui/Viewer/Viewer.hpp>

#include <stream.h>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
struct MinimalComponent : public Ra::Engine::Scene::Component {

    MinimalComponent( Ra::Engine::Scene::Entity* entity, Ra::Engine::RadiumEngine& e, Ra::Gui::Viewer & viewer );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void updateShader();

    void updateProbe(const Stream::Acquisition & acq);
    void initProbe();
    void updateScan(const Stream::Acquisition & acq);
    void initScan();

  public:
    Ra::Engine::Rendering::RenderObject* m_scan          = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe         = nullptr;
    Ra::Engine::Rendering::RenderObject* m_probe_axis[3] = { nullptr, nullptr, nullptr };

  private:
    Ra::Engine::Data::Texture * m_textureScan = nullptr;
    Ra::Engine::RadiumEngine& m_engine;
    Ra::Gui::Viewer & m_viewer;
};
