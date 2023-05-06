#pragma once

#include <string>
#include <memory>

#include "Measure.hpp"
#include <initializer_list>

// #define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//  Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
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

struct Material
{
    /* Material name */
    char*                       name;

    /* Parameters */
    float                       Ka[3];  /* Ambient */
    float                       Kd[3];  /* Diffuse */
    float                       Ks[3];  /* Specular */
    float                       Ke[3];  /* Emission */
    float                       Kt[3];  /* Transmittance */
    float                       Ns;     /* Shininess */
    float                       Ni;     /* Index of refraction */
    float                       Tf[3];  /* Transmission filter */
    float                       d;      /* Disolve (alpha) */
    int                         illum;  /* Illumination model */
};

class MeshImpl;

// class SRC_API Mesh : public Measure
class SRC_API Mesh : public Measure
{
  public:
    explicit Mesh( const Measure& measure );

    explicit Mesh( const Mesh& mesh );

//    explicit Mesh(const std::string & name, const std::vector<std::string> &filePaths);
    Mesh(std::initializer_list<std::string> filePaths);
    Mesh(const std::string & filePath);

//    void load( const std::string& filePath );
    ~Mesh();

    //    Mesh(const std::string & objTxt, const std::string & mtlTxt);

    std::string to_string() const;

    //    const tinyobj::attrib_t & getAttrib() const;
    //    const std::vector<tinyobj::shape_t> & getShapes() const;
    const std::vector<Shape> & getShapes() const;
//        const std::vector<tinyobj::material_t> & getMaterials() const;
    const std::vector<Material> &  getMaterials() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

    void printStats() const;
    void printInfo() const;

  private:
    void unpack();
//    void pack();

//    std::string m_name;
//    Mesh_internal m_internal;

//    std::unique_ptr<MeshImpl> m_pimpl;
    std::shared_ptr<MeshImpl> m_pimpl;
//    MeshImpl * m_pimpl = nullptr;



//    std::vector<std::string> m_compressedFilePaths;
//    std::vector<std::string> m_gltfPaths;

//    std::string m_fileData;
//    std::string m_rawData;
//    std::vector<int> m_fileData;
    //    std::vector<Vertex> vertices;
    //    std::vector<unsigned int> indices;
    //    tinyobj::attrib_t m_attrib;
    //    std::vector<tinyobj::shape_t> m_shapes;
    //    std::vector<tinyobj::material_t> m_materials;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// inline const tinyobj::attrib_t & Mesh::getAttrib() const
//{
//     return m_attrib;
// }

//inline const std::vector<Shape> &Mesh::getShapes() const {
//    return m_shapes;
//    //    return shapes;
//}

//inline const std::vector<Material> &Mesh::getMaterials() const {
//    return m_materials;

//           //    return materials;
//}

// inline const std::vector<tinyobj::shape_t> & Mesh::getShapes() const
//{
//     return m_shapes;
// }

// inline const std::vector<tinyobj::material_t> & Mesh::getMaterials() const
//{
//     return m_materials;
// }

} // namespace data
} // namespace hub
