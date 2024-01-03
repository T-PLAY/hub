#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include <core/Macros.hpp>

// #include "Measure.hpp"

namespace hub {
namespace data {

///
/// \brief The Vertex class
///
struct Vertex {
    float px;
    float py;
    float pz;
    float nx;
    float ny;
    float nz;
    float tx;
    float ty;
};

///
/// \brief The Shape class
///
struct Shape {
    std::vector<Vertex> vertices;
    bool hasNormal;
    std::vector<unsigned int> indices;
    std::string name;
    int material;
};

///
/// \brief The Material class
///
struct Material {
    /* Material name */
    std::string name;
    /* Parameters */
    float Ka[3]; /* Ambient */
    float Kd[3]; /* Diffuse */
    float Ks[3]; /* Specular */
    float Ke[3]; /* Emission */
    float Kt[3]; /* Transmittance */
    float Ns;    /* Shininess */
    float Ni;    /* Index of refraction */
    float Tf[3]; /* Transmission filter */
    float d;     /* Disolve (alpha) */
    int illum;   /* Illumination model */
};

class MeshImpl;

///
/// \brief The Mesh class
///
// class SRC_API Mesh : public Measure
class SRC_API Mesh
{
  public:
    ///
    /// \brief Mesh
    /// \param measure
    ///
    // explicit Mesh( const Measure& measure );

    Mesh() = default;

    ///
    /// \brief Mesh
    /// \param mesh
    ///
    explicit Mesh( const Mesh& mesh );

    ///
    /// \brief Mesh
    /// \param filePaths
    ///
    Mesh( std::initializer_list<std::string> filePaths );

    ///
    /// \brief Mesh
    /// \param filePath
    ///
    explicit Mesh( const std::string& filePath );

    ~Mesh();

    ///
    /// \brief to_string
    /// \return
    ///
    std::string to_string() const;

    ///
    /// \brief getShapes
    /// \return
    ///
    const std::vector<Shape>& getShapes() const;

    ///
    /// \brief getMaterials
    /// \return
    ///
    const std::vector<Material>& getMaterials() const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param mesh
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

    ///
    /// \brief printStats
    ///
    void printStats() const;

    ///
    /// \brief printInfo
    ///
    void printInfo() const;

    bool operator==( const Mesh& other ) const;

    static constexpr auto serialize( auto& archive, auto& self ) {
        // todo
        return archive();
    }

  private:
    void unpack( bool headerOnly = true ) const;

    std::shared_ptr<MeshImpl> m_pimpl;
};

} // namespace data
} // namespace hub
