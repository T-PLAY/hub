#include "Mesh.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <thread>

//#include <cgltf.h>
#include <cgltf/cgltf.h>
#include <meshoptimizer/gltf/gltfpack.h>
#include <meshoptimizer/meshoptimizer.h>

constexpr int s_materialSize = 4 * 3 * 6 + 4 * 3 + 4;

namespace hub {
namespace data {

class MeshImpl
{
  public:
    ~MeshImpl();

  private:
    friend class Mesh;
};

MeshImpl::~MeshImpl() {}

// Mesh::Mesh( const Mesh& mesh ) :
//     m_pimpl( mesh.m_pimpl )

// {}

Mesh::Mesh( const std::string& filePath ) : Mesh( { filePath } ) {}

// Mesh::~Mesh() = default;

Mesh::Mesh( std::initializer_list<std::string> filePaths ) : m_pimpl( new MeshImpl ) {

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

        std::string filePathWithExtension = parentPath + "/" + filename + ".gltf";
        gltfFilePaths.emplace_back( filePathWithExtension );

        std::string compressedFilePath = parentPath + "/" + filename + ".glb";
        compressedFilePaths.emplace_back( compressedFilePath );

        if ( !std::ifstream { compressedFilePath } ) {

            meshopt_encodeIndexVersion( 1 );

            Settings settings      = {};
            settings.pos_bits      = 14;
            settings.tex_bits      = 12;
            settings.nrm_bits      = 8;
            settings.col_bits      = 8;
            settings.trn_bits      = 16;
            settings.rot_bits      = 12;
            settings.scl_bits      = 16;
            settings.anim_freq     = 30;
            settings.texture_scale = 1.f;
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
        }

    } // for ( const auto& filePath : filePaths )

    for ( int i = 0; i < filenames.size(); ++i ) {
        const auto& filename = filenames.at( i );
        m_name += filename;
        if ( i != filenames.size() - 1 ) m_name += " + ";
    }

    std::vector<cgltf_data*> glbCgltfData;
    std::vector<_Mesh> glbMeshes;
    std::vector<Animation> glbAnimations;
    std::vector<std::string> glbNodeNames;
    const char* error = 0;

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

    int iMesh = 0;
    for ( const auto& mesh : glbMeshes ) {

        assert( mesh.nodes.size() == 1 );
        for ( int i = 0; i < mesh.streams.size(); ++i ) {
            const auto& stream = mesh.streams[i];

            if ( stream.type == cgltf_attribute_type_position ) {
                m_nVertice += mesh.streams[i].data.size();
                m_nDraw += std::max( size_t( 1 ), mesh.nodes.size() );
            }
        }
        m_nTriangle += mesh.indices.size() / 3;

        ++iMesh;
    }

    for ( int g = 0; g < glbCgltfData.size(); ++g ) {
        const auto* gltfData = glbCgltfData.at( g );

        for ( int i = 0; i < gltfData->materials_count; ++i ) {
            const auto& gltfMaterial = gltfData->materials[i];
            m_materials.push_back( Material() );
            auto& material = m_materials.back();
            material.name  = gltfMaterial.name;
            for ( int j = 0; j < 3; ++j ) {
                material.Kd[j] = gltfMaterial.pbr_metallic_roughness.base_color_factor[j];
            }
            material.d = 1.0 - gltfMaterial.transmission.transmission_factor;
        }
    }

    std::set<std::string> dejaVu;
    iMesh = 0;
    for ( const auto& mesh : glbMeshes ) {

        m_shapes.push_back( Shape() );
        auto& shape          = m_shapes.back();
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
        for (int i = 0; i <shapeIndices.size(); ++i) {
            assert(shapeIndices.at(i) < shapeVertices.size());
        }
        shape.name   = shapeName;

        shape.material = -1;
        if ( mesh.material != nullptr ) {
            for ( int iMaterial = 0; iMaterial < m_materials.size(); ++iMaterial ) {
                const auto& material = m_materials.at( iMaterial );
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

        m_mesh_triangles += mesh.indices.size() / 3;
        m_mesh_vertices += mesh.streams.empty() ? 0 : mesh.streams[0].data.size();

        size_t instances = std::max( size_t( 1 ), mesh.nodes.size() + mesh.instances.size() );

        m_total_triangles += mesh.indices.size() / 3 * instances;
        m_total_instances += instances;
        m_total_draws += std::max( size_t( 1 ), mesh.nodes.size() );
    }
    m_nMesh = glbMeshes.size();

    // std::vector<unsigned char> buff;
    // m_shapes.clear();
    // m_materials.clear();
}

// void Mesh::unpack( bool headerOnly ) const {

//     const auto start = std::chrono::high_resolution_clock::now();

//     std::vector<unsigned char> buff;
// }

void Mesh::printStats() const {
    // if ( m_name == "" ) unpack();
    // assert( m_name != "" );

    std::cout << "mesh statistics:" << std::endl;
    printf( "%s: %d mesh primitives (%d triangles, %d vertices); %d draw calls (%d instances, %lld "
            "triangles)\n",
            m_name.c_str(),
            int( m_nMesh ),
            int( m_mesh_triangles ),
            int( m_mesh_vertices ),
            int( m_total_draws ),
            int( m_total_instances ),
            (long long)m_total_triangles );
    std::cout << "-----------------" << std::endl;
}

void Mesh::printInfo() const {
    // if ( m_shapes.empty() ) unpack( false );
    // assert( !m_shapes.empty() );

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



std::string Vertex::toString() const
{
    std::string str;
    str += "x:" + std::to_string(px) + " y:" + std::to_string(py) + " z:" + std::to_string(pz);
    str += "nx:" + std::to_string(nx) + " ny:" + std::to_string(ny) + " nz:" + std::to_string(nz);
    str += "tx:" + std::to_string(tx) + " ty:" + std::to_string(ty);
    return str;
}

} // namespace data
} // namespace hub
