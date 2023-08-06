#pragma once

#include <initializer_list>
#include <memory>
#include <string>

#include "Measure.hpp"

namespace hub {
namespace data {

///
/// \brief The Vertex class
///
struct Vertex {
    ///
    /// \brief px
    ///
    float px;
    ///
    /// \brief py
    ///
    float py;
    ///
    /// \brief pz
    ///
    float pz;
    ///
    /// \brief nx
    ///
    float nx;
    ///
    /// \brief ny
    ///
    float ny;
    ///
    /// \brief nz
    ///
    float nz;
    ///
    /// \brief tx
    ///
    float tx;
    ///
    /// \brief ty
    ///
    float ty;
};

///
/// \brief The Shape class
///
struct Shape {
    ///
    /// \brief vertices
    ///
    std::vector<Vertex> vertices;
    ///
    /// \brief hasNormal
    ///
    bool hasNormal;
    ///
    /// \brief indices
    ///
    std::vector<unsigned int> indices;
    ///
    /// \brief name
    ///
    std::string name;
    ///
    /// \brief material
    ///
    int material;
};

///
/// \brief The Material class
///
struct Material {
    /* Material name */
    ///
    /// \brief name
    ///
    std::string name;
    /* Parameters */
    ///
    /// \brief Ka
    ///
    float Ka[3]; /* Ambient */
    ///
    /// \brief Kd
    ///
    float Kd[3]; /* Diffuse */
    ///
    /// \brief Ks
    ///
    float Ks[3]; /* Specular */
    ///
    /// \brief Ke
    ///
    float Ke[3]; /* Emission */
    ///
    /// \brief Kt
    ///
    float Kt[3]; /* Transmittance */
    ///
    /// \brief Ns
    ///
    float Ns;    /* Shininess */
    ///
    /// \brief Ni
    ///
    float Ni;    /* Index of refraction */
    ///
    /// \brief Tf
    ///
    float Tf[3]; /* Transmission filter */
    ///
    /// \brief d
    ///
    float d;     /* Disolve (alpha) */
    ///
    /// \brief illum
    ///
    int illum;   /* Illumination model */
};

class MeshImpl;

///
/// \brief The Mesh class
///
class SRC_API Mesh : public Measure
{
  public:
    ///
    /// \brief Mesh
    /// \param measure
    ///
    explicit Mesh( const Measure& measure );

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

  private:
    void unpack( bool headerOnly = true ) const;

    std::shared_ptr<MeshImpl> m_pimpl;
};

} // namespace data
} // namespace hub
