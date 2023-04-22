#pragma once

#include <string>

#include "Measure.hpp"
#include <initializer_list>

// #define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//  Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
// #include "tiny_obj_loader.h"

namespace hub {
namespace data {

//struct Vertex {
//    float px, py, pz;
//    float nx, ny, nz;
//    float tx, ty;
//};

//struct Mesh_internal {
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//};

// class SRC_API Mesh : public Measure
class SRC_API Mesh : public Measure
{
  public:
    explicit Mesh( const Measure& measure );

    explicit Mesh( const Mesh& mesh );

//    explicit Mesh(const std::string & name, const std::vector<std::string> &filePaths);
    Mesh(const std::string & name, std::initializer_list<std::string> filePaths);
    Mesh(const std::string & name, std::string filePath);

//    void load( const std::string& filePath );

    //    Mesh(const std::string & objTxt, const std::string & mtlTxt);

    std::string to_string() const;

    //    const tinyobj::attrib_t & getAttrib() const;
    //    const std::vector<tinyobj::shape_t> & getShapes() const;
    //    const std::vector<tinyobj::material_t> & getMaterials() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

  private:
    void unpack();
//    void pack();

    std::string m_name;
//    Mesh_internal m_internal;

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
