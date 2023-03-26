#pragma once

#include <string>

#include "Measure.hpp"

//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"


namespace hub {
namespace data {

class SRC_API Mesh : public Measure
{
  public:
    explicit Mesh(const Measure & measure);

    explicit Mesh(const Mesh& mesh);

    explicit Mesh(const std::string & fileObjPath);

//    Mesh(const std::string & objTxt, const std::string & mtlTxt);

    std::string to_string() const;

    const tinyobj::attrib_t & getAttrib() const;
    const std::vector<tinyobj::shape_t> & getShapes() const;
    const std::vector<tinyobj::material_t> & getMaterials() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

  private:
    void unpack();
    void pack();

    tinyobj::attrib_t m_attrib;
    std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const tinyobj::attrib_t & Mesh::getAttrib() const
{
    return m_attrib;
}

inline const std::vector<tinyobj::shape_t> & Mesh::getShapes() const
{
    return m_shapes;
}

inline const std::vector<tinyobj::material_t> & Mesh::getMaterials() const
{
    return m_materials;
}


} // namespace data
} // namespace hub
