#pragma once

#include <Engine/Data/SimpleMaterial.hpp>

namespace Ra {
namespace Engine {
namespace Data {
/**
 * Implementation of the Scan Material BSDF.
 * This material implements a flat color rendering.
 * This material could not be loaded from a file and must be defined and associated to
 * renderobjects programatically.
 *
 * This Material is parameterized either globally, by setting the m_color base color or locally,
 * by setting a vertex color attribute that define the per vertex color and setting m_perVertexColor
 * to true.
 *
 */
//class RA_ENGINE_API ScanMaterial final : public SimpleMaterial
class ScanMaterial final : public SimpleMaterial
{
  public:
    /**
     * Construct a named Scan material
     * @param name The name of the material
     */
    explicit ScanMaterial( const std::string& instanceName );

    /**
     * Destructor.
     * @note The material does not have ownership on its texture nor its shaders.
     * This destructor do not delete the associated textures and the corresponding shaders.
     */
    ~ScanMaterial() override;

    /**
     * Register the material in the material library.
     * After registration, the material could be instantiated by any Radium system, renderer,
     * plugin, ...
     */
    static void registerMaterial();

    /**
     * Remove the material from the material library.
     * After removal, the material is no more available, ...
     */
    static void unregisterMaterial();

    bool isTransparent() const override;
};

} // namespace Data
} // namespace Engine
} // namespace Ra
