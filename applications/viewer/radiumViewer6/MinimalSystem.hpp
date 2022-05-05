#pragma once

#include "MinimalComponent.hpp"

//#include <Engine/RadiumEngine.hpp>
//#include <Engine/Scene/Entity.hpp>
#include <Engine/Scene/System.hpp>
#include <Engine/Scene/SystemDisplay.hpp>



/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
class MinimalSystem : public Ra::Engine::Scene::System
{
  public:
    virtual void generateTasks( Ra::Core::TaskQueue* q,
                                const Ra::Engine::FrameInfo& info ) override;
    void addComponent( Ra::Engine::Scene::Entity* ent, MinimalComponent* comp );
};
