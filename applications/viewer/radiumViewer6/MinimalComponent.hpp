#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>
#include <Gui/Viewer/Viewer.hpp>

//#include <Engine/Data/BlinnPhongMaterial.hpp>
//#include <Engine/Data/PlainMaterial.hpp>
//#include <Engine/Data/LambertianMaterial.hpp>

#include <stream.h>
#include <FormInputStreamView.h>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */


/// This is a very basic component which holds a spinning cube.
struct MinimalComponent : public Ra::Engine::Scene::Component {

    MinimalComponent( Ra::Engine::Scene::Entity* entity,
                      Ra::Engine::RadiumEngine& e,
                      Ra::Gui::Viewer& viewer );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void addProbe();
    void updateShader();

    void updatePose( const Stream::Acquisition& acq );
//    void updatePose( Acquisitions & acqs );
//    void updatePose( Stream::Acquisition && acq );
    void initPose(int iProbe = 0);
    void updateScan( const Stream::Acquisition& acq );
//    void updateScan( Acquisitions& acqs );
//    void updateScan( Stream::Acquisition && acq );
    void initScan(int iProbe = 0);

  public:
  private:
    Ra::Engine::RadiumEngine& m_engine;
    Ra::Gui::Viewer& m_viewer;

};
