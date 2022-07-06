//#include <Engine/Data/ScanMaterial.hpp>
#include "ScanMaterial.hpp"

#include <Engine/Data/ShaderConfigFactory.hpp>
#include <Engine/Data/ShaderProgramManager.hpp>
#include <Engine/Data/TextureManager.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Engine/Rendering/RenderTechnique.hpp>

namespace Ra {
namespace Engine {
namespace Data {

static const std::string materialName { "Scan" };

ScanMaterial::ScanMaterial( const std::string& instanceName ) :
    SimpleMaterial( instanceName, materialName, Material::MaterialAspect::MAT_OPAQUE ) {}

ScanMaterial::~ScanMaterial() = default;

void ScanMaterial::registerMaterial() {
    // Get the Radium Resource location on the filesystem
//    auto resourcesRootDir { RadiumEngine::getInstance()->getResourcesDir() };
    const std::string shadersDir   = PROJECT_DIR "applications/viewer/libGuiManager/ScanMaterial/";

    auto shaderProgramManager = RadiumEngine::getInstance()->getShaderProgramManager();

    shaderProgramManager->addNamedString( "/Scan.glsl",
                                          shadersDir + "Scan.glsl" );
    // registering re-usable shaders
    Data::ShaderConfiguration lpconfig(
        "Scan",
        shadersDir + "Scan.vert.glsl",
        shadersDir + "Scan.frag.glsl" );

    Data::ShaderConfigurationFactory::addConfiguration( lpconfig );

    Data::ShaderConfiguration zprepassconfig(
        "ZprepassScan",
        shadersDir + "Scan.vert.glsl",
        shadersDir + "Scan.frag.glsl" );
    Data::ShaderConfigurationFactory::addConfiguration( zprepassconfig );

    // Registering technique
    Rendering::EngineRenderTechniques::registerDefaultTechnique(
        materialName, []( Rendering::RenderTechnique& rt, bool ) {
            // Lighting pass
            auto lightpass = Data::ShaderConfigurationFactory::getConfiguration( "Scan" );
            rt.setConfiguration( *lightpass, Rendering::DefaultRenderingPasses::LIGHTING_OPAQUE );
            // Z prepass
            auto zprepass = Data::ShaderConfigurationFactory::getConfiguration( "ZprepassScan" );
            rt.setConfiguration( *zprepass, Rendering::DefaultRenderingPasses::Z_PREPASS );
        } );
}

void ScanMaterial::unregisterMaterial() {
    Rendering::EngineRenderTechniques::removeDefaultTechnique( "Scan" );
}

} // namespace Data
} // namespace Engine
} // namespace Ra
