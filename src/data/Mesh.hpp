#pragma once

#include <memory>
#include <string>

#include "Measure.hpp"
#include <initializer_list>

// #define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
// #include "tiny_obj_loader.h"

namespace hub {
namespace data {

struct Vertex {
    float px, py, pz;
    float nx, ny, nz;
    float tx, ty;
};

struct Shape {
    std::vector<Vertex> vertices;
    bool hasNormal;
    std::vector<unsigned int> indices;
    std::string name;
    int material;
};

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

// class SRC_API Mesh : public Measure
class SRC_API Mesh : public Measure
{
  public:
    explicit Mesh( const Measure& measure );

    explicit Mesh( const Mesh& mesh );

    Mesh( std::initializer_list<std::string> filePaths );
    Mesh( const std::string& filePath );

    ~Mesh();

    std::string to_string() const;

    const std::vector<Shape>& getShapes() const;
    const std::vector<Material>& getMaterials() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

    void printStats() const;
    void printInfo() const;

  private:
    void unpack( bool headerOnly = true ) const;

    std::shared_ptr<MeshImpl> m_pimpl;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// inline const tinyobj::attrib_t & Mesh::getAttrib() const
// }

// inline const std::vector<tinyobj::shape_t> & Mesh::getShapes() const
// }

// inline const std::vector<tinyobj::material_t> & Mesh::getMaterials() const
// }

} // namespace data
} // namespace hub
