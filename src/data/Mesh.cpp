#include "Mesh.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>

#include <cgltf.h>
#include <gltf/gltfpack.h>
#include <meshoptimizer/meshoptimizer.h>

#include "io/Memory.hpp"

constexpr int s_materialSize = 4 * 3 * 6 + 4 * 3 + 4;

namespace hub {
namespace data {

class MeshImpl
{
  public:
    ~MeshImpl();

  private:
    uint64_t m_nVertice  = 0;
    uint64_t m_nTriangle = 0;
    uint64_t m_nDraw     = 0;

    uint64_t m_nMesh           = 0;
    uint64_t m_mesh_triangles  = 0;
    uint64_t m_mesh_vertices   = 0;
    uint64_t m_total_triangles = 0;
    uint64_t m_total_instances = 0;
    uint64_t m_total_draws     = 0;

    std::string m_name = "";

    std::vector<Shape> m_shapes;
    std::vector<Material> m_materials;

    void write( Output& output ) {
        output.write( m_nVertice );
        output.write( m_nTriangle );
        output.write( m_nDraw );

        output.write( m_nMesh );
        output.write( m_mesh_triangles );
        output.write( m_mesh_vertices );
        output.write( m_total_triangles );
        output.write( m_total_instances );
        output.write( m_total_draws );

        output.write( m_name );
        output.write( (uint64_t)m_shapes.size() );
        for ( const auto& shape : m_shapes ) {
            uint64_t size = shape.vertices.size();
            output.write( size );
            output.write( reinterpret_cast<const unsigned char*>(shape.vertices.data()),
                          shape.vertices.size() * sizeof( Vertex ) );

            output.write( shape.hasNormal );
            size = shape.indices.size();
            output.write( size );
            output.write( reinterpret_cast<const unsigned char*>(shape.indices.data()),
                          shape.indices.size() * sizeof( unsigned int ) );

            output.write( shape.name );
            output.write( shape.material );
        }
        output.write( (uint64_t)m_materials.size() );
        for ( const auto& material : m_materials ) {
            output.write( material.name );
            assert( sizeof( float ) == 4 );
            assert( sizeof( int ) == 4 );
            output.write( reinterpret_cast<const unsigned char*>(&material.Ka), s_materialSize );
        }
    }

    friend class Mesh;
};

MeshImpl::~MeshImpl() {}

Mesh::Mesh( const Measure& measure ) :
    Measure( measure.getData(), measure.getSize(), sensor::Resolution { { 1 }, sensor::Format::MESH } ),
    m_pimpl( new MeshImpl ) {
    assert( m_data != nullptr );
}

Mesh::Mesh( const Mesh& mesh ) :
    Measure( new unsigned char[mesh.m_size],
             mesh.m_size,
             sensor::Resolution { { 1 }, sensor::Format::MESH },
             true ),
    m_pimpl( mesh.m_pimpl )

{
    memcpy( m_data, mesh.m_data, m_size );
}

Mesh::Mesh( const std::string& filePath ) : Mesh( { filePath } ) {}

Mesh::~Mesh() = default;
// Mesh::~Mesh() {
// }

// Mesh::Mesh( const std::string& name, const std::vector<std::string>& filePaths ) :
Mesh::Mesh( std::initializer_list<std::string> filePaths ) :
    Measure( (unsigned char*)nullptr, 0, sensor::Resolution { { 1 }, sensor::Format::MESH } ),
    m_pimpl( new MeshImpl ) {

    std::vector<std::string> filenames;
    std::vector<std::string> compressedFilePaths;
    std::vector<std::string> gltfFilePaths;

    for ( const auto& filePath : filePaths ) {
        if ( std::ifstream { filePath } ) {
            std::cerr << "[Mesh] please refer filepath without extension" << std::endl;
            assert( false );
        }

        size_t slash = filePath.find_last_of( "/" );
        std::string parentPath =
            ( slash != std::string::npos ) ? filePath.substr( 0, slash ) : filePath;
        std::cout << "parentPath : " << parentPath << std::endl;

        size_t lastindex2    = filePath.find_last_of( "/" );
        std::string filename = filePath.substr( lastindex2 + 1, filePath.size() - lastindex2 - 1 );
        filenames.emplace_back( filename );

        std::cout << "filename : " << filename << std::endl;

//        std::string iext                  = ".gltf";
        std::string filePathWithExtension = parentPath + "/" + filename + ".gltf";
        gltfFilePaths.emplace_back( filePathWithExtension );

        std::string compressedFilePath = parentPath + "/" + filename + ".glb";
        compressedFilePaths.emplace_back( compressedFilePath );

        if ( !std::ifstream { compressedFilePath } ) {
//            if ( iext == ".gltf" ) {

                meshopt_encodeIndexVersion( 1 );

                Settings settings           = {};
                settings.pos_bits           = 14;
                settings.tex_bits           = 12;
                settings.nrm_bits           = 8;
                settings.col_bits           = 8;
                settings.trn_bits           = 16;
                settings.rot_bits           = 12;
                settings.scl_bits           = 16;
                settings.anim_freq          = 30;
//                settings.simplify_threshold = 1.f;
                settings.texture_scale      = 1.f;
                for ( int kind = 0; kind < TextureKind__Count; ++kind )
                    settings.texture_quality[kind] = 8;

                settings.compressmore          = true;
                settings.verbose               = 2;
                settings.simplify_aggressive   = true;
                settings.simplify_lock_borders = true;
                settings.simplify_threshold    = 0.5f;
                settings.keep_nodes            = true;
                settings.keep_materials        = true;

                const char* input  = filePathWithExtension.c_str();
                const char* output = compressedFilePath.c_str();
                const char* report = 0;

                int ret = gltfpack( input, output, report, settings );
                assert( ret == 0 );
//            }
//            else {
//                std::cerr << "[Mesh] unable to open " << iext << " extension, please use glb."
//                          << std::endl;
//                assert( false );
//            }
        }

    } // for ( const auto& filePath : filePaths )

    for ( int i = 0; i < filenames.size(); ++i ) {
        const auto& filename = filenames.at( i );
        m_pimpl->m_name += filename;
        if ( i != filenames.size() - 1 ) m_pimpl->m_name += " + ";
    }

    std::vector<cgltf_data*> glbCgltfData;
    std::vector<_Mesh> glbMeshes;
    std::vector<Animation> glbAnimations;
    std::vector<std::string> glbNodeNames;
    const char* error = 0;

    // parse glb data

    for ( const auto& filePath : compressedFilePaths ) {

        std::cout << "[Mesh] parse " << filePath << " file" << std::endl;

        static cgltf_data* gltfData = nullptr;

        gltfData = parseGltf( filePath.c_str(), glbMeshes, glbAnimations, &error );

        if ( error ) {
            fprintf( stderr, "Error readGlb: %s\n", error );
            return;
        }

        glbNodeNames.reserve( gltfData->nodes_count );
        for ( int i = 0; i < gltfData->nodes_count; ++i ) {
            const auto& node = gltfData->nodes[i];
            if ( node.name != nullptr ) { glbNodeNames.emplace_back( node.name ); }
        }
        glbCgltfData.push_back( gltfData );
    }

    if ( error ) {
        fprintf( stderr, "Error loading: %s\n", error );
        return;
    }

    // get mesh info
    int iMesh = 0;
    for ( const auto& mesh : glbMeshes ) {

        assert( mesh.nodes.size() == 1 );
        for ( int i = 0; i < mesh.streams.size(); ++i ) {
            const auto& stream = mesh.streams[i];

            if ( stream.type == cgltf_attribute_type_position ) {
                m_pimpl->m_nVertice += mesh.streams[i].data.size();
                m_pimpl->m_nDraw += std::max( size_t( 1 ), mesh.nodes.size() );
            }
        }
        m_pimpl->m_nTriangle += mesh.indices.size() / 3;

        ++iMesh;
    }

    // unpack to internal material representation
    for ( int g = 0; g < glbCgltfData.size(); ++g ) {
        const auto* gltfData = glbCgltfData.at( g );

        for ( int i = 0; i < gltfData->materials_count; ++i ) {
            const auto& gltfMaterial = gltfData->materials[i];
            m_pimpl->m_materials.push_back( Material() );
            auto& material = m_pimpl->m_materials.back();
            material.name  = gltfMaterial.name;
            for ( int j = 0; j < 3; ++j ) {
                material.Kd[j] = gltfMaterial.pbr_metallic_roughness.base_color_factor[j];
            }
            material.d = 1.0 - gltfMaterial.transmission.transmission_factor;
        }
    }

    // unpack to internal shape representation
    std::set<std::string> dejaVu;
    iMesh = 0;
    for ( const auto& mesh : glbMeshes ) {

        m_pimpl->m_shapes.push_back( Shape() );
        auto& shape          = m_pimpl->m_shapes.back();
        shape.hasNormal      = false;
        auto& shapeVertices  = shape.vertices;
        auto& shapeIndices   = shape.indices;
        const auto* nodeName = mesh.nodes.at( 0 )->name;
        assert( nodeName != nullptr );
        std::string shapeName = nodeName;

        if ( dejaVu.find( nodeName ) != dejaVu.end() ) {
            int iShapeDejaVu = 2;
            while ( dejaVu.find( shapeName + std::to_string( iShapeDejaVu ) ) != dejaVu.end() ) {
                ++iShapeDejaVu;
            }
            shapeName = shapeName + std::to_string( iShapeDejaVu );
        }

        dejaVu.insert( shapeName );

        bool hasVertex = false;
        assert( mesh.nodes.size() == 1 );
        for ( int i = 0; i < mesh.streams.size(); ++i ) {
            const auto& stream = mesh.streams[i];
            assert( shapeVertices.empty() || shapeVertices.size() == stream.data.size() );
            if ( shapeVertices.empty() ) shapeVertices.resize( stream.data.size() );

            if ( stream.type == cgltf_attribute_type_position ) {
                hasVertex = true;
                for ( int iVertice = 0; iVertice < stream.data.size(); ++iVertice ) {
                    auto& vertice    = shapeVertices.at( iVertice );
                    const auto& attr = stream.data.at( iVertice );
                    vertice.px       = attr.f[0];
                    vertice.py       = attr.f[1];
                    vertice.pz       = attr.f[2];
                }
            }
            else if ( stream.type == cgltf_attribute_type_normal ) {
                shape.hasNormal = true;
                for ( int iVertice = 0; iVertice < stream.data.size(); ++iVertice ) {
                    auto& vertice    = shapeVertices.at( iVertice );
                    const auto& attr = stream.data.at( iVertice );
                    vertice.nx       = attr.f[0];
                    vertice.ny       = attr.f[1];
                    vertice.nz       = attr.f[2];
                }
            }
        }
        assert( hasVertex );

        shapeIndices = mesh.indices;
        shape.name   = shapeName;

        shape.material = -1;
        if ( mesh.material != nullptr ) {
            for ( int iMaterial = 0; iMaterial < m_pimpl->m_materials.size(); ++iMaterial ) {
                const auto& material = m_pimpl->m_materials.at( iMaterial );
                if ( material.name == mesh.material->name ) {
                    shape.material = iMaterial;
                    break;
                }
            }
            assert( shape.material != -1 );
        }
        ++iMesh;
    }

    for ( size_t i = 0; i < glbMeshes.size(); ++i ) {
        const _Mesh& mesh = glbMeshes[i];

        m_pimpl->m_mesh_triangles += mesh.indices.size() / 3;
        m_pimpl->m_mesh_vertices += mesh.streams.empty() ? 0 : mesh.streams[0].data.size();

        size_t instances = std::max( size_t( 1 ), mesh.nodes.size() + mesh.instances.size() );

        m_pimpl->m_total_triangles += mesh.indices.size() / 3 * instances;
        m_pimpl->m_total_instances += instances;
        m_pimpl->m_total_draws += std::max( size_t( 1 ), mesh.nodes.size() );
    }
    m_pimpl->m_nMesh = glbMeshes.size();

    std::vector<unsigned char> buff;

//    io::Memory<decltype( buff )> memory( buff );
    output::OutputMemory<decltype( buff )> memory( buff );
    m_pimpl->write( memory );

    m_data    = new unsigned char[buff.size()];
    m_ownData = true;
    m_size    = buff.size();
    memcpy( m_data, buff.data(), m_size );

    m_pimpl->m_shapes.clear();
    m_pimpl->m_materials.clear();
}

void Mesh::unpack( bool headerOnly ) const {

    const auto start = std::chrono::high_resolution_clock::now();

    std::vector<unsigned char> buff;
    buff.insert( buff.begin(), m_data, m_data + m_size );
//    io::Memory<decltype( buff )> memory( buff );
    input::InputMemory<decltype( buff )> memory( buff );

    memory.read( m_pimpl->m_nVertice );
    memory.read( m_pimpl->m_nTriangle );
    memory.read( m_pimpl->m_nDraw );

    memory.read( m_pimpl->m_nMesh );
    memory.read( m_pimpl->m_mesh_triangles );
    memory.read( m_pimpl->m_mesh_vertices );
    memory.read( m_pimpl->m_total_triangles );
    memory.read( m_pimpl->m_total_instances );
    memory.read( m_pimpl->m_total_draws );

    memory.read( m_pimpl->m_name );

    if ( !headerOnly ) {
        m_pimpl->m_shapes.clear();
        uint64_t nShape;
        memory.read( nShape );
        for ( int iShape = 0; iShape < nShape; ++iShape ) {
            m_pimpl->m_shapes.push_back( Shape() );
            auto& shape = m_pimpl->m_shapes.back();

            uint64_t size;
            memory.read( size );
            shape.vertices.resize( size );
            memory.read( reinterpret_cast<unsigned char*>(shape.vertices.data()),
                         shape.vertices.size() * sizeof( Vertex ) );

            memory.read( shape.hasNormal );

            memory.read( size );
            shape.indices.resize( size );
            memory.read( reinterpret_cast<unsigned char*>(shape.indices.data()),
                         shape.indices.size() * sizeof( unsigned int ) );

            memory.read( shape.name );
            memory.read( shape.material );
        }

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        m_pimpl->m_materials.clear();
        uint64_t nMaterial;
        memory.read( nMaterial );
        for ( int iMaterial = 0; iMaterial < nMaterial; ++iMaterial ) {
            m_pimpl->m_materials.push_back( Material() );
            auto& material = m_pimpl->m_materials.back();
            memory.read( material.name );
            std::cout << "[Mesh] read material name: " << material.name << std::endl;
            memory.read( reinterpret_cast<unsigned char*>(&material.Ka), s_materialSize );
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
        const double speed = (double)m_size / duration;
        std::cout << "[Mesh] unpack scene " << int( m_size / 1'000.0 ) << " Ko in "
                  << duration / 1000.0 << " ms (" << speed << " Mo/s)" << std::endl;
        assert( memory.isEnd() );
    }
}

std::string Mesh::to_string() const {
    if ( m_pimpl->m_name == "" ) unpack();
    assert( m_pimpl->m_name != "" );

    std::string str = "";
    str += "'" + m_pimpl->m_name + "'";
    str += ", draw:" + std::to_string( m_pimpl->m_nDraw );
    str += ", vx:" + std::to_string( m_pimpl->m_nVertice );
    str += ", tri:" + std::to_string( m_pimpl->m_nTriangle );

    ////        str += shape.name + ": nb points = " + std::to_string(shape.points.indices.size());
    ////        str += shape.name + ": nTriangle = " + std::to_string(nFace);
    ////        if (s != m_shapes.size() - 1) {
    ////            str += ", ";
    ////        }
    return str;
}

const std::vector<Shape>& Mesh::getShapes() const {
    if ( m_pimpl->m_shapes.empty() ) unpack( false );
    assert( !m_pimpl->m_shapes.empty() );

    return m_pimpl->m_shapes;
}

const std::vector<Material>& Mesh::getMaterials() const {
    if ( m_pimpl->m_shapes.empty() ) unpack( false );
    assert( !m_pimpl->m_shapes.empty() );

    return m_pimpl->m_materials;
}

void Mesh::printStats() const {
    if ( m_pimpl->m_name == "" ) unpack();
    assert( m_pimpl->m_name != "" );

    std::cout << "mesh statistics:" << std::endl;
    printf( "%s: %d mesh primitives (%d triangles, %d vertices); %d draw calls (%d instances, %lld "
            "triangles)\n",
            m_pimpl->m_name.c_str(),
            int( m_pimpl->m_nMesh ),
            int( m_pimpl->m_mesh_triangles ),
            int( m_pimpl->m_mesh_vertices ),
            int( m_pimpl->m_total_draws ),
            int( m_pimpl->m_total_instances ),
            (long long)m_pimpl->m_total_triangles );
    std::cout << "-----------------" << std::endl;
}

void Mesh::printInfo() const {
    if ( m_pimpl->m_shapes.empty() ) unpack( false );
    assert( !m_pimpl->m_shapes.empty() );

    const auto& shapes    = getShapes();
    const auto& materials = getMaterials();

    for ( size_t m = 0; m < materials.size(); m++ ) {
        const auto& material = materials.at( m );
        std::cout << "[Mesh] material " << m << " name " << material.name << std::endl;

        std::cout << "[Mesh] material " << m << " diffuse r " << material.Kd[0] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse g " << material.Kd[1] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse b " << material.Kd[2] << std::endl;
    }

    for ( size_t s = 0; s < shapes.size(); s++ ) {
        const auto& shape = shapes.at( s );

        std::cout << "shape name " << shape.name << std::endl;
        std::cout << "vertices size " << shape.vertices.size() << std::endl;
        std::cout << "indices size " << shape.indices.size() << std::endl;
        std::cout << "material id " << shape.material << std::endl;
        std::cout << "has normal " << shape.hasNormal << std::endl;
    }
}

std::ostream& operator<<( std::ostream& os, const Mesh& mesh ) {
    os << mesh.to_string();
    return os;
}

} // namespace data
} // namespace hub
