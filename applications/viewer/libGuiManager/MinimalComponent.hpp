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

#include <FormInputStreamView.h>
#include <stream.h>

//#include <MinimalSystem.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

struct Scanner {
    double width;
    double depth;
    Ra::Core::Vector3 pos;
};

constexpr int g_nTraces = 3;
using Traces = std::array<Ra::Engine::Rendering::RenderObject*, g_nTraces>;

/// This is a very basic component which holds a spinning cube.
struct MinimalComponent : public Ra::Engine::Scene::Component {

    MinimalComponent( Ra::Engine::Scene::Entity* entity,
                      Ra::Engine::RadiumEngine& e,
                      Ra::Gui::Viewer& viewer
                      //                      MinimalSystem & minimalSystem
    );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    void addProbe();
    void updateShader();

    void updatePose( const Stream::Acquisition& acq );
    //    void updatePose( Acquisitions & acqs );
    //    void updatePose( Stream::Acquisition && acq );
    void initPose( int iProbe = 0 );
    void updateScan( const Stream::Acquisition& acq );
    //    void updateScan( Acquisitions& acqs );
    //    void updateScan( Stream::Acquisition && acq );
    void initScan( int iProbe = 0 );

    void setupScanner(double scanWidth, double scanDepth, double x, double y, double z);


    void initPoseTraces( const std::vector<Stream::Acquisition>& poseAcqs );

    void incIter();


  private:
    Ra::Engine::RadiumEngine& m_engine;
    Ra::Gui::Viewer& m_viewer;
    //    MinimalSystem & m_system;
    Scanner m_scanner;

    Ra::Engine::Rendering::RenderObject* m_roGrid = nullptr;
//    Ra::Engine::Rendering::RenderObject* m_roTraces[3] = {nullptr, nullptr, nullptr};
    Traces m_roTraces;

  public:
    void traceSetVisible(bool visible);
    Ra::Engine::Rendering::RenderObject & getRoGrid();
    Traces & getRoTraces();
};
