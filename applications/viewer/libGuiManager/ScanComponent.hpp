#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>

#include <SensorComponent.h>
#include <Gui/Viewer/Viewer.hpp>
//#include <Core/Asset/Image.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <ScanMaterial/ScanMaterial.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

struct Scan {
    unsigned char * m_textureData = nullptr;
    std::string m_textureName = "";
    Ra::Engine::Data::Texture* m_textureScan = nullptr;
    Ra::Engine::Rendering::RenderObject * m_quad = nullptr;
    Ra::Engine::Rendering::RenderObject * m_scanLine = nullptr;
    std::shared_ptr<Ra::Engine::Data::ScanMaterial> m_material;
};

/// This is a very basic component which holds a spinning cube.
//struct ScanComponent : public Ra::Engine::Scene::Component {
struct ScanComponent : public SensorComponent {

    ScanComponent(const hub::InputSensor & inputSensor, Ra::Engine::Scene::Entity* entity, Ra::Engine::RadiumEngine & engine, Ra::Gui::Viewer & viewer);

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void update(const hub::Acquisition & acq) override;

    void addScan();

    void changeTransparency(double transparency) override;
    void changeTransparency2(double transparency) override;

private:
//    Ra::Engine::Rendering::RenderObject* m_roGrid = nullptr;
//    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
//    Ra::Engine::Rendering::RenderObject* m_ro = nullptr;

    Ra::Engine::RadiumEngine & m_engine;
    Ra::Gui::Viewer & m_viewer;

    std::vector<Scan> m_scans;
    int m_iScan = -1;
    long long m_lastUpdateDate = 0;
    bool m_firstUpdate = true;

    std::shared_ptr<Ra::Engine::Data::PlainMaterial> m_scanLineMaterial;
//    std::unique_ptr<Ra::Engine::Data::ScanMaterial> m_scanMaterial;

//    unsigned char * m_data = nullptr;
////    const hub::InputSensor & m_inputStream;
//    std::string m_textureName;
//    Ra::Engine::Data::Texture* m_textureScan = nullptr;
////    int m_width;
////    int m_height;
////    std::shared_ptr<Ra::Core::Asset::Image> m_image;
//    Ra::Engine::Rendering::RenderObject * m_scanLine = nullptr;

};
