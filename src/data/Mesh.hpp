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

    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.px, self.py, self.pz, self.nx, self.ny, self.nz, self.tx, self.ty );
    }

    bool operator==( const Vertex& other ) const {
        return !std::memcmp( this, &other, sizeof( Vertex ) );
    }
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

    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.vertices, self.hasNormal, self.indices, self.name, self.material );
    }

    bool operator==( const Shape& other ) const {
        return vertices == other.vertices && hasNormal == other.hasNormal &&
               indices == other.indices && name == other.name && material == other.material;
    }
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
    bool operator==( const Material& other ) const {
        return name == other.name && !std::memcmp( Ka, other.Ka, sizeof( float ) * 21 ) &&
               illum == other.illum;
    }
};

class MeshImpl;

///
/// \brief The Mesh class
///
class SRC_API Mesh
{
  public:
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
    Mesh( std::initializer_list<std::string> filePaths );

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

    bool operator==( const Mesh& other ) const;

#if CPP_VERSION >= 20
    static constexpr auto serialize( auto& archive, auto& self ) { return archive(); }
#else
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
