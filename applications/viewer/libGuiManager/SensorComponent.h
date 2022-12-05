
#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>

#include <Engine/Data/Mesh.hpp>
#include <Engine/Rendering/RenderObject.hpp>
#include <vector>

#include <InputSensor.hpp>

//#include <Core/RaCore.hpp>
#include <Core/Types.hpp>

/// This is a very basic component which holds a spinning cube.
struct SensorComponent : public Ra::Engine::Scene::Component {

//    SensorComponent( const hub::InputSensor& inputSensor, Ra::Engine::Scene::Entity* entity );
    SensorComponent( const hub::SensorSpec& sensorSpec, Ra::Engine::Scene::Entity* entity );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    virtual void update( const hub::Acquisition& acq ) = 0;
    virtual Ra::Core::Aabb getAabb() const             = 0;
    virtual void enableTrace( bool enable )            = 0;
    virtual void enableLive( bool enable )             = 0;

    virtual void on_tune_valueChanged( double arg1 ) {};
    virtual void on_tune2_valueChanged( double arg1 ) {};
    virtual void on_tune3_valueChanged( double arg1 ) {};
    virtual void on_tune4_valueChanged( double arg1 ) {};
    virtual void on_palette_valueChanged( int palette ) {};
    virtual void on_setTransparency( bool isTransparent ) {};

    // private:
  protected:
    Ra::Engine::Rendering::RenderObject* m_roAxes[3] = { nullptr };
    Ra::Engine::Rendering::RenderObject* m_ro        = nullptr;

//    const hub::InputSensor& m_inputSensor;
    const hub::SensorSpec & m_sensorSpec;
    std::vector<std::shared_ptr<Ra::Engine::Data::Mesh>> m_meshAxis;
};
