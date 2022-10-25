#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Rendering/RenderObject.hpp>

#include <Gui/Viewer/Viewer.hpp>
#include <SensorComponent.h>
//#include <Core/Asset/Image.hpp>
#include <Engine/Data/BlinnPhongMaterial.hpp>
#include <Engine/Data/PlainMaterial.hpp>
#include <ScanMaterial/ScanMaterial.hpp>


//#define USE_BLINN_PHONG_MATERIAL
#ifdef USE_BLINN_PHONG_MATERIAL
using CurrentMaterial = Ra::Engine::Data::BlinnPhongMaterial;
#else
using CurrentMaterial = Ra::Engine::Data::ScanMaterial;
#endif

struct Scan {
    unsigned char* m_textureData                    = nullptr;
    std::string m_textureName                       = "";
    Ra::Engine::Data::Texture* m_textureScan        = nullptr;
    Ra::Engine::Rendering::RenderObject* m_quad     = nullptr;
    Ra::Engine::Rendering::RenderObject* m_scanLine = nullptr;
    std::shared_ptr<CurrentMaterial> m_material;

//    ~Scan();
};

/// This is a very basic component which holds a spinning cube.
struct ScanComponent : public SensorComponent {

    ScanComponent( const hub::InputSensor& inputSensor,
                   Ra::Engine::Scene::Entity* entity,
                   Ra::Engine::RadiumEngine& engine,
                   Ra::Gui::Viewer& viewer );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void update( const hub::Acquisition& acq ) override;

    void addScan();

    virtual void on_tune_valueChanged( double arg1 ) override;
    virtual void on_tune2_valueChanged( double arg1 ) override;
    virtual void on_tune3_valueChanged( double arg1 ) override;
    virtual void on_tune4_valueChanged( double arg1 ) override;
    virtual void on_palette_valueChanged( int palette ) override;
    virtual void on_setTransparency( bool isTransparent ) override;

  private:
    Ra::Engine::RadiumEngine& m_engine;
    Ra::Gui::Viewer& m_viewer;

    std::vector<Scan> m_scans;
    int m_iScan                = -1;
    int m_nScans               = 1;
    long long m_lastUpdateDate = 0;
    bool m_firstUpdate         = true;
    bool m_isLiveStream        = true;

    Eigen::Matrix4f m_localTransform = Eigen::Matrix4f::Identity();

    std::shared_ptr<Ra::Engine::Data::PlainMaterial> m_scanLineMaterial;
};
