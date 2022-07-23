#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>

#include <SensorComponent.h>
#include <Gui/Viewer/Viewer.hpp>
#include <Core/Asset/Image.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.
//struct ScanComponent : public Ra::Engine::Scene::Component {
struct ScanComponent : public SensorComponent {

    ScanComponent(const InputStream & inputStream, Ra::Engine::Scene::Entity* entity, Ra::Engine::RadiumEngine & engine, Ra::Gui::Viewer & viewer);

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void update(const Stream::Acquisition & acq) override;

private:
//    Ra::Engine::Rendering::RenderObject* m_roGrid = nullptr;
//    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
//    Ra::Engine::Rendering::RenderObject* m_ro = nullptr;

    Ra::Engine::RadiumEngine & m_engine;
    Ra::Gui::Viewer & m_viewer;

    unsigned char * m_data = nullptr;
//    const InputStream & m_inputStream;
    std::string m_textureName;
    Ra::Engine::Data::Texture* m_textureScan = nullptr;
//    int m_width;
//    int m_height;
    std::shared_ptr<Ra::Core::Asset::Image> m_image;

};
