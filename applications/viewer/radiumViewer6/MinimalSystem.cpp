
#include "MinimalSystem.hpp"

/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
void MinimalSystem::generateTasks( Ra::Core::TaskQueue*, const Ra::Engine::FrameInfo& ) {}

void MinimalSystem::addComponent( Ra::Engine::Scene::Entity* ent, MinimalComponent* comp ) {
    registerComponent( ent, comp );
}
