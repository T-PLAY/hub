#pragma once

#include <string>

#include "Measure.hpp"

//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"


namespace hub {
namespace data {

//class SRC_API Mesh : public Measure
class SRC_API Mesh_ : public Measure
{
  public:
    explicit Mesh_(const Measure & measure);

    explicit Mesh_(const Mesh_& mesh);

    explicit Mesh_(const std::string & fileObjPath);

//    Mesh_(const std::string & objTxt, const std::string & mtlTxt);

    std::string to_string() const;

    const tinyobj::attrib_t & getAttrib() const;
    const std::vector<tinyobj::shape_t> & getShapes() const;
    const std::vector<tinyobj::material_t> & getMaterials() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh_& mesh );

  private:
    void unpack();
    void pack();

    std::string m_rawName;
    tinyobj::attrib_t m_attrib;
    std::vector<tinyobj::shape_t> m_shapes;
    std::vector<tinyobj::material_t> m_materials;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const tinyobj::attrib_t & Mesh_::getAttrib() const
{
    return m_attrib;
}

inline const std::vector<tinyobj::shape_t> & Mesh_::getShapes() const
{
    return m_shapes;
}

inline const std::vector<tinyobj::material_t> & Mesh_::getMaterials() const
{
    return m_materials;
}


} // namespace data
} // namespace hub
