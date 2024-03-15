/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/03/25

#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

#include <core/Macros.hpp>
#include <core/Traits.hpp>

namespace hub {
namespace data {

///
/// \brief The Vertex class
/// represents textured point in cartezian space with 3D normal
///
struct Vertex {
    /// \brief px
    float px;
    /// \brief py
    float py;
    /// \brief pz
    float pz;
    /// \brief nx
    float nx;
    /// \brief ny
    float ny;
    /// \brief nz
    float nz;
    /// \brief tx
    float tx;
    /// \brief ty
    float ty;

//    auto toString() const {
//        std::string str;
//        str += std::to_string(px) + ":" + std::to_string(py) + ":" + std::to_string(pz);
//        return str;
//    }

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.px, self.py, self.pz, self.nx, self.ny, self.nz, self.tx, self.ty );
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Vertex& other ) const {
        return !std::memcmp( this, &other, sizeof( Vertex ) );
    }
};

///
/// \brief The Shape class
/// represents single manifold object
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

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.vertices, self.hasNormal, self.indices, self.name, self.material );
    }

    auto toString() const {
        std::string str;
        //str += hub::to_string(vertices) + " " +  std::to_string(hasNormal) + " " + hub::to_string(indices)  + " " + name + " " + std::to_string(material);
        str += std::to_string(vertices.size()) + " " +  std::to_string(hasNormal) + " " + std::to_string(indices.size())  + " " + name + " " + std::to_string(material);
        return str;
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Shape& other ) const;
};

///
/// \brief The Material class
/// represents render information
///
struct Material {
    ///
    /// \brief name
    ///
    std::string name;

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

    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.name,
                 self.Ka,
                 self.Kd,
                 self.Ks,
                 self.Ke,
                 self.Kt,
                 self.Ns,
                 self.Ni,
                 self.Tf,
                 self.d,
                 self.illum );
    }

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Material& other ) const {
        return name == other.name && !std::memcmp( &Ka, &other.Ka, sizeof( float ) * 21 ) &&
               illum == other.illum;
    }
};

class MeshImpl;

///
/// \brief The Mesh class
/// represent a set of Shape with different material
///
class SRC_API Mesh
{
  public:
    ///
    /// \brief name
    ///
    static constexpr auto name() { return "Mesh"; };

    ///
    /// \brief Mesh
    /// \param measure
    ///

    Mesh() = default;

    ///
    /// \brief Mesh
    /// \param mesh
    ///
    // explicit Mesh( const Mesh& mesh );

    ///
    /// \brief Mesh
    /// \param filePaths
    ///
    explicit Mesh( std::initializer_list<std::string> filePaths );

    ///
    /// \brief Mesh
    /// \param filePath
    ///
    explicit Mesh( const std::string& filePath );

    // ~Mesh();

    ///
    /// \brief to_string
    /// \return
    ///
    std::string toString() const {
        // if ( m_name == "" ) unpack();
        // assert( m_name != "" );

        std::string str = "";
        str += "'" + m_name + "'";
        str += ", draw:" + std::to_string( m_nDraw );
        str += ", vx:" + std::to_string( m_nVertice );
        str += ", tri:" + std::to_string( m_nTriangle );

        return str;
    }

    ///
    /// \brief getShapes
    /// \return
    ///
    const std::vector<Shape>& getShapes() const {
        // if ( m_shapes.empty() ) unpack( false );
        // assert( !m_shapes.empty() );
        return m_shapes;
    }

    ///
    /// \brief getMaterials
    /// \return
    ///
    const std::vector<Material>& getMaterials() const {
        // if (m_shapes.empty() ) unpack( false );
        // assert( !m_shapes.empty() );
        return m_materials;
    }

    ///
    /// \brief operator <<
    /// \param os
    /// \param mesh
    /// \return
    ///
    // SRC_API friend std::ostream& operator<<( std::ostream& os, const Mesh& mesh );

    ///
    /// \brief printStats
    ///
    void printStats() const;

    ///
    /// \brief printInfo
    ///
    void printInfo() const;

    ///
    /// \brief operator ==
    /// \param other
    /// \return
    ///
    bool operator==( const Mesh& other ) const;

#if CPP_VERSION >= 20
    static constexpr auto serialize( const auto& archive, auto& self ) { return archive(); }
#else
    ///
    /// \brief serialize
    /// \param archive
    /// \param self
    ///
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.m_name,
                 self.m_shapes,
                 self.m_materials,
                 self.m_nVertice,
                 self.m_nTriangle,
                 self.m_nDraw,
                 self.m_nMesh,
                 self.m_mesh_triangles,
                 self.m_mesh_vertices,
                 self.m_total_triangles,
                 self.m_total_instances,
                 self.m_total_draws );
    }
#endif

  private:
    // void unpack( bool headerOnly = true ) const;

    std::string m_name = "";
    std::vector<Shape> m_shapes;
    std::vector<Material> m_materials;
    uint64_t m_nVertice        = 0;
    uint64_t m_nTriangle       = 0;
    uint64_t m_nDraw           = 0;
    uint64_t m_nMesh           = 0;
    uint64_t m_mesh_triangles  = 0;
    uint64_t m_mesh_vertices   = 0;
    uint64_t m_total_triangles = 0;
    uint64_t m_total_instances = 0;
    uint64_t m_total_draws     = 0;

    std::shared_ptr<MeshImpl> m_pimpl;
};

} // namespace data
} // namespace hub
