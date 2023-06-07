// #define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.hpp"

// #include <filesystem>
#include <chrono>
#include <iostream>
#include <set>

#include "io/Memory.hpp"

#include <cgltf.h>
#include <gltf/gltfpack.h>
#include <meshoptimizer/meshoptimizer.h>

// #include <filesystem>
// #include <experimental/filesystem>
// #include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
// #include <stdio.h>
// #include <stdlib.h>
// #include <filesystem>

constexpr int s_materialSize = 4 * 3 * 6 + 4 * 3 + 4;

// #ifdef __unix
// #    def ine fopen_s( pFile, filename, mode ) \
//        ( ( *( pFile ) ) = fopen( ( filename ), ( mode ) ) ) == NULL
// #endif

// int getFileSize( std::string path ) {
//     // #include <fstream>
//     FILE* pFile = NULL;

//    // get the file stream
//    fopen_s( &pFile, path.c_str(), "rb" );

//    // set the file pointer to end of file
//    fseek( pFile, 0, SEEK_END );

//    // get the file size
//    int Size = ftell( pFile );

//    // return the file pointer to begin of file if you want to read it
//    // rewind( pFile );

//    // close stream and release buffer
//    fclose( pFile );

//    return Size;
//}

namespace hub {
namespace data {

    class MeshImpl {
    public:
        ~MeshImpl();

    private:
        //    std::vector<cgltf_data*> m_cgltfData;
        //    std::vector<::Mesh> m_meshes;
        //    std::vector<Animation> m_animations;
        //    std::vector<std::string> m_nodeNames;
        uint64_t m_nVertice = 0;
        //    size_t m_nVertice = 0;
        uint64_t m_nTriangle = 0;
        uint64_t m_nDraw = 0;

        uint64_t m_nMesh = 0;
        uint64_t m_mesh_triangles = 0;
        uint64_t m_mesh_vertices = 0;
        uint64_t m_total_triangles = 0;
        uint64_t m_total_instances = 0;
        uint64_t m_total_draws = 0;

        std::string m_name = "";

        std::vector<Shape> m_shapes;
        std::vector<Material> m_materials;

        void write(hub::io::Output& output)
        {
            output.write(m_nVertice);
            output.write(m_nTriangle);
            output.write(m_nDraw);

            output.write(m_nMesh);
            output.write(m_mesh_triangles);
            output.write(m_mesh_vertices);
            output.write(m_total_triangles);
            output.write(m_total_instances);
            output.write(m_total_draws);

            output.write(m_name);
            //        uint64_t shapeSize = m_shapes.size();
            output.write((uint64_t)m_shapes.size());
            for (const auto& shape : m_shapes) {
                uint64_t size = shape.vertices.size();
                output.write(size);
                output.write((const unsigned char*)shape.vertices.data(),
                    shape.vertices.size() * sizeof(Vertex));
                //                        output.write( shape.vertices );

                output.write(shape.hasNormal);
                size = shape.indices.size();
                output.write(size);
                output.write((const unsigned char*)shape.indices.data(),
                    shape.indices.size() * sizeof(unsigned int));
                //                        output.write( shape.indices );

                output.write(shape.name);
                output.write(shape.material);
            }
            output.write((uint64_t)m_materials.size());
            for (const auto& material : m_materials) {
                //            output.write( material );
                output.write(material.name);
                assert(sizeof(float) == 4);
                assert(sizeof(int) == 4);
//                assert(sizeof(Material) == 8 + 4 * 3 * 6 + 3 * 4 + 4 );
//                memory.read((unsigned char*)&material.Ka, 4 * 3 * 6 + 4 * 3 + 4 );
                output.write((unsigned char*)&material.Ka, s_materialSize);
            }
        }

        //    void read( hub::io::Input& input ) {
        //        input.read( m_nVertice );
        //        input.read( m_nTriangle );
        //        input.read( m_nDraw );

        //        input.read( m_name );

        //        m_shapes.clear();
        //        decltype( m_shapes.size() ) nShape;
        //        input.read( nShape );
        //        for ( int iShape = 0; iShape < nShape; ++iShape ) {
        //            m_shapes.push_back( Shape() );
        //            auto& shape = m_shapes.back();
        //            //        for ( const auto& shape : m_shapes ) {
        //            input.read( shape.vertices );
        //            input.read( shape.hasNormal );
        //            input.read( shape.indices );
        //            input.read( shape.name );
        //            input.read( shape.material );
        //        }

        //        m_materials.clear();
        //        decltype( m_materials.size() ) nMaterial;
        //        input.read( nMaterial );
        //        for ( int iMaterial = 0; iMaterial < nMaterial; ++iMaterial ) {
        //            //        for ( const auto& material : m_materials ) {
        //            m_materials.push_back( Material() );
        //            auto& material = m_materials.back();
        //            input.read( material );
        //        }

        //        input.read( m_nMesh );
        //        input.read( m_mesh_triangles );
        //        input.read( m_mesh_vertices );
        //        input.read( m_total_triangles );
        //        input.read( m_total_instances );
        //        input.read( m_total_draws );
        //    }

        //    friend void write(hub::io::Output & output, MeshImpl& meshImpl) {

        //    }

        friend class Mesh;
    };

    // void write( hub::io::Output& output, MeshImpl& meshImpl ) {
    //     output.write(meshImpl.)
    // }

    MeshImpl::~MeshImpl()
    {
        //    for ( cgltf_data* cgltfData : m_cgltfData ) {
        //        delete cgltfData;
        //    }
    }

    Mesh::Mesh(const Measure& measure)
        : Measure(measure.getData(), measure.getSize(), Resolution { { 1 }, Format::MESH })
        , m_pimpl(new MeshImpl)
    {
        //    std::cout << "[Mesh] Mesh(const Measure&)" << std::endl;
        //    assert( measure.getSize() == 64 );
        //    memcpy( (unsigned char*)&m_x, m_data, m_size );
        assert(m_data != nullptr);
        //    memcpy( m_data, measure.getData(), m_size );

        //    unpack();
    }

    Mesh::Mesh(const Mesh& mesh)
        : Measure(new unsigned char[mesh.m_size],
            mesh.m_size,
            Resolution { { 1 }, Format::MESH },
            true)
        ,
        //    m_pimpl( new MeshImpl )
        m_pimpl(mesh.m_pimpl)

    {
        //    std::cout << "[Mesh] Mesh(const Mesh&)" << std::endl;
        memcpy(m_data, mesh.m_data, m_size);
        //    Measure( mesh.getData(), mesh.getSize(), Resolution { { 1 }, Format::MESH } ) {
        //    assert( m_data != nullptr );
        //    unpack();
    }

    Mesh::Mesh(const std::string& filePath)
        : Mesh({ filePath })
    {
    }

    Mesh::~Mesh() = default;
    // Mesh::~Mesh() {
    // }

    // Mesh::Mesh( const std::string& name, const std::vector<std::string>& filePaths ) :
    Mesh::Mesh(std::initializer_list<std::string> filePaths)
        : Measure((unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH })
        , m_pimpl(new MeshImpl)
    //    m_name( name )
    {

        //}

        //    std::vector<std::string> compressedFilePaths;
        //    std::vector<uint64_t> compressedFilePathSizes;
        //    size_t size = 0;
        //    std::vector<uns

        std::vector<std::string> filenames;
        std::vector<std::string> compressedFilePaths;
        std::vector<std::string> gltfFilePaths;

        //  pack gltf files to glb (simplify and bake meshes with gltfPack)
        for (const auto& filePath : filePaths) {
            // void Mesh::load( const std::string& filePath ) {
            //         std::filesystem::path path(filePath);
            //        assert(! std::experimental::filesystem::exists(filePath));
            if (std::ifstream { filePath }) {
                std::cerr << "[Mesh] please refer filepath without extension" << std::endl;
                assert(false);
            }
            //        assert ( !std::ifstream { filePath } );

            // #ifdef WIN32
            //         size_t slash = filePath.find_last_of( "\\" );
            // #else
            //         size_t slash = filePath.find_last_of( "/" );
            // #endif
            size_t slash = filePath.find_last_of("/");
            std::string parentPath = (slash != std::string::npos) ? filePath.substr(0, slash) : filePath;
            std::cout << "parentPath : " << parentPath << std::endl;

            //        size_t lastindex               = filePath.find_last_of( "." );
            //        std::string filePathWithoutExt = filePath.substr( 0, lastindex );

            //        const std::string fileMtlPath = filename + ".mtl";

            size_t lastindex2 = filePath.find_last_of("/");
            //        std::string filename = filePath.substr( lastindex2 + 1, lastindex - lastindex2 - 1
            //        );
            std::string filename = filePath.substr(lastindex2 + 1, filePath.size() - lastindex2 - 1);
            //        std::string filename = std::filesystem::path(filePath)
            filenames.emplace_back(filename);

            std::cout << "filename : " << filename << std::endl;

            //        std::string iext = getExtension( filePath.c_str() );
            std::string iext = ".gltf";
            //        std::cout << "[Mesh] extension : " << iext << std::endl;
            std::string filePathWithExtension = parentPath + "/" + filename + ".gltf";
            //        m_gltfPaths.emplace_back( filePathWithExtension );
            gltfFilePaths.emplace_back(filePathWithExtension);

            //    std::string reportFilePath = parentPath + "/" + filename + ".txt";
            std::string compressedFilePath = parentPath + "/" + filename + ".glb";
            compressedFilePaths.emplace_back(compressedFilePath);

            //        if ( iext == ".gltf" && !
            //        std::experimental::filesystem::exists(compressedFilePath) ) {

            //        if ( true || !std::ifstream { compressedFilePath } ) {
            if (!std::ifstream { compressedFilePath }) {
                //        if (false) {
                if (iext == ".gltf") {

                    meshopt_encodeIndexVersion(1);

                    Settings settings = {};
                    //                                settings.quantize  = true;
                    settings.pos_bits = 14;
                    settings.tex_bits = 12;
                    settings.nrm_bits = 8;
                    settings.col_bits = 8;
                    settings.trn_bits = 16;
                    settings.rot_bits = 12;
                    settings.scl_bits = 16;
                    settings.anim_freq = 30;
                    settings.simplify_threshold = 1.f;
                    settings.texture_scale = 1.f;
                    for (int kind = 0; kind < TextureKind__Count; ++kind)
                        settings.texture_quality[kind] = 8;

                    //                                settings.compress              = true;
                    settings.compressmore = true;
                    settings.verbose = 2;
                    settings.simplify_aggressive = true;
                    settings.simplify_lock_borders = true;
                    settings.simplify_threshold = 0.5f;
                    settings.keep_nodes = true;
                    //                                settings.pos_float = true;
                    //                            settings.keep_extras           = true;
                    settings.keep_materials = true;

                    const char* input = filePathWithExtension.c_str();
                    const char* output = compressedFilePath.c_str();
                    const char* report = 0;
                    //            const char* report = reportFilePath.c_str();

                    int ret = gltfpack(input, output, report, settings);
                    assert(ret == 0);
                } else {
                    std::cerr << "[Mesh] unable to open " << iext << " extension, please use glb."
                              << std::endl;
                    assert(false);
                }
            }

            //        const auto fileSize = getFileSize( compressedFilePath );
            //        compressedFilePathSizes.emplace_back( fileSize );
            //        size += fileSize;

        } // for ( const auto& filePath : filePaths )

        for (int i = 0; i < filenames.size(); ++i) {
            const auto& filename = filenames.at(i);
            m_pimpl->m_name += filename;
            if (i != filenames.size() - 1)
                m_pimpl->m_name += " + ";
        }

        //    for ( const auto& compressedFilePath : compressedFilePaths ) {
        //        std::ostringstream buf;
        //        std::ifstream input( compressedFilePath.c_str() );
        //        buf << input.rdbuf();
        //        input.close();
        //        const auto& fileData = buf.str();

        //        m_rawData = fileData;
        //        std::cout << "write " << fileData.size() << " bytes to " << cur << " position" <<
        //        const char * data = fileData.data();

        //        m_fileData.resize(fileData.size());
        //        for (int i = 0; i < fileData.size(); ++i) {
        //            m_fileData[i] = fileData.data()[i];
        //        }

        //        m_fileData = fileData;
        //        std::endl;
        //        uint64_t dataSize = fileData.size();
        //        memory.write( dataSize );
        //        memory.write( (unsigned char*)fileData.data(), dataSize );
        //        memcpy( m_data + cur, fileData.data(), fileData.size() );
        //        cur += fileData.size();
        //    }

        std::vector<cgltf_data*> glbCgltfData;
        std::vector<_Mesh> glbMeshes;
        std::vector<Animation> glbAnimations;
        std::vector<std::string> glbNodeNames;
        const char* error = 0;

        // parse glb data
        //    for ( int i = 0; i < nData; ++i ) {

        for (const auto& filePath : compressedFilePaths) {
            //        for (const auto & filePath : gltfFilePaths) {

            //        dataSizes[i] = dataLong[cur++];
            //        uint64_t dataSize;
            //        memory.read( dataSize );
            //        char* data = new char[dataSize];
            //        memory.read( (unsigned char*)data, dataSize );

            //        std::cout << "[Mesh] unpack " << dataSize << " bytes from data " << i <<
            //        std::endl;
            std::cout << "[Mesh] parse " << filePath << " file" << std::endl;

            //        std::vector<int> fileData;
            //        fileData.resize(dataSize);
            //        for (int j = 0; j < fileData.size(); ++j) {
            //            fileData[j] = data[j];
            //        }
            //        for (int j = 0; j < dataSize; ++j) {
            //            char c = data[j];
            //            assert(data[j] == m_fileData[j]);
            //        }
            //        std::string fileData((char*)data);
            //        assert(fileData == m_fileData);
            //        char * dataChar = (char*)data;
            //        char * dataChar = new char[dataSize];
            //        for (int j = 0; j < 100; ++j) {
            //            dataChar[j] = (char)data[j];
            //        }
            //        int dataInt[100] = {0};
            //        for (int j = 0; j < 100; ++j) {
            //            dataInt[j] = data[j];
            //        }

            static cgltf_data* gltfData = nullptr;
            //        gltfData = readGlb( (const char*)m_data + cur, dataSizes[i], s_meshes,
            //        s_animations, &error );
            //        gltfData = readGlb(
            //            (const char*)m_rawData.data(), dataSize, m_pimpl->m_meshes,
            //            m_pimpl->m_animations, &error );

            //        std::cout << "readGlb size = " << dataSize << std::endl;
            //        gltfData = readGlb(
            //            (const char*)data, dataSize, m_pimpl->m_meshes, m_pimpl->m_animations, &error
            //            );
            //        assert( m_compressedFilePaths.size() == 1 );
            //        std::cout << "parseGlb started" << std::endl;

            //        gltfData = parseGlb( filePath.c_str(), glbMeshes, glbAnimations, &error );
            gltfData = parseGltf(filePath.c_str(), glbMeshes, glbAnimations, &error);

            //        std::cout << "parseGlb ended" << std::endl;
            //        gltfData = parseGltf(
            //            m_gltfPaths.at(0).c_str(), m_pimpl->m_meshes, m_pimpl->m_animations, &error );

            //        assert(gltfData->buffers[0].data != nullptr);

            //        std::cout << "read " << dataSizes[i] << " bytes from " << cur << " position" <<
            //        std::endl; assert(s_data != nullptr);
            //        cur += dataSizes[i];
            if (error) {
                fprintf(stderr, "Error readGlb: %s\n", error);
                return;
            }

            //        gltfData->buffer_views;
            //        const auto & buffer = gltfData->buffers;
            //        assert(gltfData->buffer_views->name != nullptr);

            //        std::cout << m_pimpl->m_cgltfData.size() << std::endl;

            //        assert(buffer->name != nullptr);
            //        std::cout << "buffer " << " size = " << buffer->size << std::endl;
            //        assert(buffer->data != nullptr);
            //        float * bufferFloat = (float*)buffer->data;

            //        std::cout << "buffer view count : " << gltfData->buffer_views_count << std::endl;
            //        for ( int i = 0; i < gltfData->buffer_views_count; ++i ) {
            //            const auto& buffer_view = gltfData->buffer_views[i];
            //            std::cout << "buffer view " << i << "size = " << buffer_view.size <<
            //            std::endl;

            //            assert(buffer_view.meshopt_compression.buffer != nullptr);
            //            assert(buffer_view.data != nullptr);
            //            for (int j = 0; j < buffer_view.size; ++j) {
            //                std::cout << ((float*)buffer_view.data)[j] << " ";
            //            }
            //            std::cout << std::endl;
            //        }
            //        std::cout << "buffer count : " << gltfData->buffers_count << std::endl;
            //        for ( int i = 0; i < gltfData->buffers_count; ++i ) {
            //            std::cout << "buffer " << i << "size = " << gltfData->buffers[i].size <<
            //            std::endl; const auto& buffer = gltfData->buffers[i];

            //            assert(buffer.data != nullptr);
            //        }

            //        std::cout << "nNodes : " << gltfData->nodes_count << std::endl;
            //    std::vector<std::string> nodeNames;
            glbNodeNames.reserve(gltfData->nodes_count);
            for (int i = 0; i < gltfData->nodes_count; ++i) {
                const auto& node = gltfData->nodes[i];
                if (node.name != nullptr) {
                    //                std::cout << "node " << i << ", name : " << node.name <<
                    //                std::endl;
                    glbNodeNames.emplace_back(node.name);
                }
            }
            glbCgltfData.push_back(gltfData);
            //        m_pimpl->m_cgltfData = gltfData;
            //        std::cout << "end loop" << std::endl;

            //        delete[] data;
        }

        //    assert( memory.isEnd() );

        if (error) {
            fprintf(stderr, "Error loading: %s\n", error);
            return;
        }

        //    printMeshStats( s_meshes, m_name.c_str() );

        //    std::cout << "mesh" << std::endl;
        // get mesh info
        int iMesh = 0;
        for (const auto& mesh : glbMeshes) {

            //        const std::string header = "[Mesh:" + std::to_string( iMesh ) + "] ";
            assert(mesh.nodes.size() == 1);
            //        if ( mesh.nodes[0]->name != nullptr )
            //            std::cout << header << "name : '" << mesh.nodes[0]->name << "'" << std::endl;
            //        std::cout << header << "name : '" << s_nodeNames.at( iMesh ) << "'" << std::endl;
            for (int i = 0; i < mesh.streams.size(); ++i) {
                const auto& stream = mesh.streams[i];

                //            std::cout << header << "stream attr : " << stream.type << std::endl;
                if (stream.type == cgltf_attribute_type_position) {
                    //                std::cout << header << "nVertice : " << stream.data.size() <<
                    //                std::endl;
                    //            assert( mesh.streams.size() == 1 );
                    m_pimpl->m_nVertice += mesh.streams[i].data.size();
                    m_pimpl->m_nDraw += std::max(size_t(1), mesh.nodes.size());
                }
            }
            //        std::cout << header << "nTriangle : " << mesh.indices.size() / 3 << std::endl;
            //        assert( mesh.indices.size() == 1 );
            m_pimpl->m_nTriangle += mesh.indices.size() / 3;
            //        std::cout << header << "material : '" << mesh.material->name << "'" << std::endl;
            //        std::cout << header << "nNode : " << mesh.nodes.size() << std::endl;
            //        std::cout << header << "nTargetName : " << mesh.target_names.size() << std::endl;
            //                std::cout << iMesh << ", node : '" << mesh.nodes[0]->name << "'" <<
            //                std::endl;

            //        for ( const auto& name : mesh.target_names ) {
            //            std::cout << iMesh << name << std::endl;
            //        }
            ++iMesh;
        }

        //    std::cout << "material" << std::endl;
        // unpack to internal material representation
        //    std::vector<Material> materials;
        //    std::cout << "nMaterial : " << s_data->materials_count << std::endl;
        for (int g = 0; g < glbCgltfData.size(); ++g) {
            const auto* gltfData = glbCgltfData.at(g);

            //    std::cout << "nMaterial : " << gltfData->materials_count << std::endl;
            for (int i = 0; i < gltfData->materials_count; ++i) {
                const auto& gltfMaterial = gltfData->materials[i];
                m_pimpl->m_materials.push_back(Material());
                //            auto& material           = materials.at( i );
                auto& material = m_pimpl->m_materials.back();
                material.name = gltfMaterial.name;
                //            std::cout << "[Mesh] gltfMaterial " << i << " = " << material.name <<
                //            std::endl;
                for (int j = 0; j < 3; ++j) {
                    material.Kd[j] = gltfMaterial.pbr_metallic_roughness.base_color_factor[j];
                    //                gltfMaterial.clearcoat;
                    //                material.Kd[j] = gltfMaterial.
                }
                //            if ( material.name != nullptr )
                //                std::cout << "material " << i << ", name : " << material.name <<
                //                std::endl;
            }
        }

        //        for ( int i = 0; i < m_pimpl->m_materials.size(); ++i ) {
        //            const auto& material = m_pimpl->m_materials.at( i );
        //            std::cout << "[Mesh] material " << i << " = " << material.name << std::endl;
        //        }

        //    std::vector<Shape> shapes( m_pimpl->m_meshes.size() );

        //    std::cout << "shape total : " << m_pimpl->m_meshes.size() << std::endl;
        // unpack to internal shape representation
        std::set<std::string> dejaVu;
        iMesh = 0;
        for (const auto& mesh : glbMeshes) {
            //        std::cout << "mesh " << iMesh << std::endl;

            m_pimpl->m_shapes.push_back(Shape());
            //        auto& shape          = shapes.at( iMesh );
            auto& shape = m_pimpl->m_shapes.back();
            shape.hasNormal = false;
            auto& shapeVertices = shape.vertices;
            auto& shapeIndices = shape.indices;
            //        assert( iMesh < glbNodeNames.size() );
            //        const auto& nodeName = glbNodeNames.at( iMesh );
            const auto* nodeName = mesh.nodes.at(0)->name;
            assert(nodeName != nullptr);
            std::string shapeName = nodeName;

            if (dejaVu.find(nodeName) != dejaVu.end()) {
                int iShapeDejaVu = 2;
                while (dejaVu.find(shapeName + std::to_string(iShapeDejaVu)) != dejaVu.end()) {
                    ++iShapeDejaVu;
                }
                shapeName = shapeName + std::to_string(iShapeDejaVu);
            }

            dejaVu.insert(shapeName);

            bool hasVertex = false;
            assert(mesh.nodes.size() == 1);
            for (int i = 0; i < mesh.streams.size(); ++i) {
                //            std::cout << "stream " << i << std::endl;
                //            assert(mesh.streams.size() == 1);
                const auto& stream = mesh.streams[i];
                assert(shapeVertices.empty() || shapeVertices.size() == stream.data.size());
                if (shapeVertices.empty())
                    shapeVertices.resize(stream.data.size());

                if (stream.type == cgltf_attribute_type_position) {
                    hasVertex = true;
                    for (int iVertice = 0; iVertice < stream.data.size(); ++iVertice) {
                        auto& vertice = shapeVertices.at(iVertice);
                        const auto& attr = stream.data.at(iVertice);
                        //                    std::cout << iVertice << "attr: " << attr.f[0] <<
                        //                    attr.f[1] << attr.f[2]
                        //                              << std::endl;
                        vertice.px = attr.f[0];
                        vertice.py = attr.f[1];
                        vertice.pz = attr.f[2];
                    }
                } else if (stream.type == cgltf_attribute_type_normal) {
                    shape.hasNormal = true;
                    for (int iVertice = 0; iVertice < stream.data.size(); ++iVertice) {
                        auto& vertice = shapeVertices.at(iVertice);
                        const auto& attr = stream.data.at(iVertice);
                        //                    std::cout << iVertice << "attr: " << attr.f[0] <<
                        //                    attr.f[1] << attr.f[2]
                        //                              << std::endl;
                        vertice.nx = attr.f[0];
                        vertice.ny = attr.f[1];
                        vertice.nz = attr.f[2];
                    }
                }
            }
            assert(hasVertex);

            shapeIndices = mesh.indices;
            shape.name = shapeName;
            //    std::cout << "nMaterial : " << s_data->materials_count << std::endl;
            //    for ( int i = 0; i < s_data->materials_count; ++i ) {
            //        const auto& material = s_data->materials[i];
            //        if ( material.name != nullptr )
            //            std::cout << "material " << i << ", name : " << material.name << std::endl;
            //    }

            //        shape.material = std::distance( m_pimpl->m_cgltfData->materials, mesh.material );
            //        std::cout << "[Mesh] mesh " << iMesh << " material name = " << mesh.material->name
            //        << std::endl;
            shape.material = -1;
            if (mesh.material != nullptr) {
                for (int iMaterial = 0; iMaterial < m_pimpl->m_materials.size(); ++iMaterial) {
                    const auto& material = m_pimpl->m_materials.at(iMaterial);
                    //            if ( material.name == mesh.material->name ) {
//                    if (!strcmp(material.name, mesh.material->name)) {
                        if (material.name == mesh.material->name) {
                        //                std::cout << "[Mesh] found match '" << material.name << "'" <<
                        //                std::endl;
                        shape.material = iMaterial;
                        break;
                    }
                }
                assert(shape.material != -1);
            }
            //        shape.material = mesh.material->name;
            //        m_pimpl->m_nTriangle += mesh.indices.size() / 3;
            //        std::cout << "[Mesh] mesh " << iMesh << " material id = " << shape.material  <<
            //        std::endl;
            ++iMesh;
        }

        for (size_t i = 0; i < glbMeshes.size(); ++i) {
            const _Mesh& mesh = glbMeshes[i];

            m_pimpl->m_mesh_triangles += mesh.indices.size() / 3;
            m_pimpl->m_mesh_vertices += mesh.streams.empty() ? 0 : mesh.streams[0].data.size();

            size_t instances = std::max(size_t(1), mesh.nodes.size() + mesh.instances.size());

            m_pimpl->m_total_triangles += mesh.indices.size() / 3 * instances;
            m_pimpl->m_total_instances += instances;
            m_pimpl->m_total_draws += std::max(size_t(1), mesh.nodes.size());
        }
        m_pimpl->m_nMesh = glbMeshes.size();

        std::vector<unsigned char> buff;

        io::Memory<decltype(buff)> memory(buff);
        //    memory.write( m_pimpl->m_name );
        //    memory.write(m_pimpl);
        //    write( memory, *m_pimpl );
        //    memory.write(*m_pimpl);
        m_pimpl->write(memory);

        //    //    uint64_t nData = compressedFilePaths.size();
        //    //    memory.write( nData );

        m_data = new unsigned char[buff.size()];
        m_ownData = true;
        m_size = buff.size();
        memcpy(m_data, buff.data(), m_size);

        //    unpack();
        //    m_unpacked = true;
        m_pimpl->m_shapes.clear();
        m_pimpl->m_materials.clear();
    }

    //{
    //}

    void Mesh::unpack(bool headerOnly) const
    {
        //    memcpy(m_pimpl.get(), m_data, sizeof(size_t) * 9);

        const auto start = std::chrono::high_resolution_clock::now();

        std::vector<unsigned char> buff;
        buff.insert(buff.begin(), m_data, m_data + m_size);
        io::Memory<decltype(buff)> memory(buff);

        //    void read( hub::io::Input& input ) {
        memory.read(m_pimpl->m_nVertice);
        memory.read(m_pimpl->m_nTriangle);
        memory.read(m_pimpl->m_nDraw);

        memory.read(m_pimpl->m_nMesh);
        memory.read(m_pimpl->m_mesh_triangles);
        memory.read(m_pimpl->m_mesh_vertices);
        memory.read(m_pimpl->m_total_triangles);
        memory.read(m_pimpl->m_total_instances);
        memory.read(m_pimpl->m_total_draws);

        memory.read(m_pimpl->m_name);

        if (!headerOnly) {
            m_pimpl->m_shapes.clear();
            //        decltype( m_pimpl->m_shapes.size() ) nShape;
            uint64_t nShape;
            memory.read(nShape);
            for (int iShape = 0; iShape < nShape; ++iShape) {
                m_pimpl->m_shapes.push_back(Shape());
                auto& shape = m_pimpl->m_shapes.back();
                //        for ( const auto& shape : m_shapes ) {

                uint64_t size;
                memory.read(size);
                shape.vertices.resize(size);
                memory.read((unsigned char*)shape.vertices.data(),
                    shape.vertices.size() * sizeof(Vertex));
                //            memory.read( shape.vertices );

                memory.read(shape.hasNormal);

                memory.read(size);
                shape.indices.resize(size);
                memory.read((unsigned char*)shape.indices.data(),
                    shape.indices.size() * sizeof(unsigned int));
                //            memory.read( shape.indices );

                memory.read(shape.name);
                memory.read(shape.material);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            m_pimpl->m_materials.clear();
            //        decltype( m_pimpl->m_materials.size() ) nMaterial;
            uint64_t nMaterial;
            memory.read(nMaterial);
            for (int iMaterial = 0; iMaterial < nMaterial; ++iMaterial) {
                //        for ( const auto& material : m_materials ) {
                m_pimpl->m_materials.push_back(Material());
                auto& material = m_pimpl->m_materials.back();
//                memory.read(material);
                memory.read(material.name);
                std::cout << "[Mesh] read material name: " << material.name << std::endl;
//                output.write(material.name);
//                assert(sizeof(float) == 4);
//                assert(sizeof(Material) == 8 + 4 * 3 * 6 + 3 * 4 + 4 );
//                output.write((unsigned char*)&material.Ka, sizeof(Material) - sizeof(char*));
                memory.read((unsigned char*)&material.Ka, s_materialSize);
            }
            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            //        const auto speed = 1'000'000 * m_size / duration; // Byte/s
            const double speed = (double)m_size / duration;
            std::cout << "[Mesh] unpack scene " << int(m_size / 1'000.0) << " Ko in " << duration / 1000.0 << " ms (" << speed << " Mo/s)" << std::endl;
            assert(memory.isEnd());
        }
        //    }
        //        memory.read( *m_pimpl );
        //    m_pimpl->read( memory );
        //    m_pimpl->readFull( memory );
        //    assert( memory.isEnd() );

        //    uint64_t nData;
        //    memory.read( nData );

        //    std::cout << "[Mesh] unpack " << m_size << " bytes" << std::endl;
        //    m_unpacked = true;
    }

    // void Mesh::unpackScene() const {
    //     std::vector<unsigned char> buff;
    //     buff.insert( buff.begin(), m_data, m_data + m_size );
    //     io::Memory<decltype( buff )> memory( buff );

    //    //        memory.read( *m_pimpl );
    //    m_pimpl->read( memory );
    //    //    m_pimpl->readFull( memory );
    //    assert( memory.isEnd() );

    //    //    uint64_t nData;
    //    //    memory.read( nData );

    //    std::cout << "[Mesh] unpack " << m_size << " bytes" << std::endl;
    //    m_unpacked = true;
    //}

    // void Mesh::unpack() const {
    //     //    std::cout << "[Mesh] unpack" << std::endl;

    //    //    const char* error = 0;
    //    //    s_data = readGlb(fileData.c_str(), fileData.size(), s_meshes, s_animations, &error);
    //    //    size_t cur               = 0;
    //    //    const uint64_t* dataLong = (uint64_t*)m_data;
    //    //    const auto nData         = dataLong[cur++];
    //    //    //    std::cout << "nData : " << nData << std::endl;
    //    //    std::vector<uint64_t> dataSizes( nData );
    //    //    //    std::cout << "dataSizes : ";
    //    //    for ( int i = 0; i < nData; ++i ) {
    //    //        dataSizes[i] = dataLong[cur++];
    //    //        //        std::cout << dataSizes[i] << ", ";
    //    //    }

    //    std::vector<unsigned char> buff;
    //    buff.insert( buff.begin(), m_data, m_data + m_size );
    //    io::Memory<decltype( buff )> memory( buff );

    //    //        memory.read( *m_pimpl );
    //    m_pimpl->read( memory );
    ////    m_pimpl->readFull( memory );
    //    assert( memory.isEnd() );

    //    //    uint64_t nData;
    //    //    memory.read( nData );

    //    std::cout << "[Mesh] unpack " << m_size << " bytes" << std::endl;
    //    m_unpacked = true;

    //    //    uint64_t strLen = dataLong[cur++];
    //    //    //    std::cout << std::endl;
    //    //    cur = sizeof( uint64_t ) * ( nData + 2 );

    //    //    unsigned char strBuff[80] = { 0 };

    //    //    memcpy( strBuff, m_data + cur, strLen );
    //    //    m_name = std::string( strBuff, strBuff + strLen );
    //    //    cur += m_name.size();

    //    //    std::cout << "end" << std::endl;

    //    //    memory.read( m_name );
    //    //    std::string fileObjTxtReaded;
    //    //    memory.read( fileObjTxtReaded );
    //    //    std::string fileMtlTxtReaded;
    //    //    memory.read( fileMtlTxtReaded );

    //    //    assert( memory.isEnd() );

    //    //    //    std::string inputfile = "cornell_box.obj";
    //    //    tinyobj::ObjReaderConfig reader_config;
    //    //    //    reader_config.mtl_search_path = "./"; // Path to material files
    //    //    reader_config.mtl_search_path = dirPath; // Path to material files

    //    //    tinyobj::ObjReader reader;

    //    //    if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //    //        exit( 1 );
    //    //    }
    //    //    if ( !reader.ParseFromString( fileObjTxtReaded, fileMtlTxtReaded ) ) {
    //    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //    //        exit( 1 );
    //    //    }

    //    //    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    //    //    m_attrib = reader.GetAttrib();
    //    //    m_shapes = reader.GetShapes();

    //    //    m_materials = reader.GetMaterials();
    //}

    // void Mesh::pack() {}

    // Mesh::Mesh(const std::string &objTxt, const std::string &mtlTxt)
    //     : Measure( (unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH } )
    //{
    //     tinyobj::ObjReader reader;

    //    //    if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //    //        exit( 1 );
    //    //    }
    //    if ( !reader.ParseFromString( objTxt, mtlTxt ) ) {
    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //        exit( 1 );
    //    }

    //    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    //    m_attrib = reader.GetAttrib();
    //    m_shapes = reader.GetShapes();

    //    m_materials = reader.GetMaterials();

    //    auto size = objTxt.size() + mtlTxt.size() + 8;
    //    m_data = new unsigned char[size];
    //    m_size = size;
    //    memcpy(m_data, buff.data(), m_size);
    //}

    std::string Mesh::to_string() const
    {
        //    if ( !m_unpacked ) unpack();
        if (m_pimpl->m_name == "")
            unpack();
        assert(m_pimpl->m_name != "");

        std::string str = "";
        str += "'" + m_pimpl->m_name + "'";
        str += ", draw:" + std::to_string(m_pimpl->m_nDraw);
        str += ", vx:" + std::to_string(m_pimpl->m_nVertice);
        str += ", tri:" + std::to_string(m_pimpl->m_nTriangle);
        //    printMeshStats(s_meshes, m_name.c_str());

        //    return
        //    size_t nTriangle = 0;
        //    size_t nVertice = m_attrib.vertices.size() / 3;
        //    for ( size_t s = 0; s < m_shapes.size(); s++ ) {
        //        const auto & shape = m_shapes.at(s);
        ////        str += shape.name + ": nb points = " + std::to_string(shape.points.indices.size());
        //        const int nFace = shape.mesh.num_face_vertices.size();
        //        nTriangle += nFace;
        ////        str += shape.name + ": nTriangle = " + std::to_string(nFace);
        ////        if (s != m_shapes.size() - 1) {
        ////            str += ", ";
        ////        }
        //    }
        //    std::string str = "[nShape = " + std::to_string(m_shapes.size()) + ", nTriangle = " +
        //    std::to_string(nTriangle) + "]";
        //    std::string str = "['" + m_name + "', nShape = " + std::to_string( m_shapes.size() ) +
        //                      ", nVertice = " + std::to_string( nVertice ) + "]";
        //    str += "]";
        return str;
    }

    const std::vector<Shape>& Mesh::getShapes() const
    {
        //    if ( !m_unpacked ) unpack();
        if (m_pimpl->m_shapes.empty())
            unpack(false);
        assert(!m_pimpl->m_shapes.empty());

        return m_pimpl->m_shapes;
    }

    const std::vector<Material>& Mesh::getMaterials() const
    {
        if (m_pimpl->m_shapes.empty())
            unpack(false);
        assert(!m_pimpl->m_shapes.empty());

        return m_pimpl->m_materials;
    }

    void Mesh::printStats() const
    {
        if (m_pimpl->m_name == "")
            unpack();
        assert(m_pimpl->m_name != "");

        std::cout << "mesh statistics:" << std::endl;
        //    printMeshStats( m_pimpl->m_meshes, m_pimpl->m_name.c_str() );
        printf("%s: %d mesh primitives (%d triangles, %d vertices); %d draw calls (%d instances, %lld "
               "triangles)\n",
            m_pimpl->m_name.c_str(),
            int(m_pimpl->m_nMesh),
            int(m_pimpl->m_mesh_triangles),
            int(m_pimpl->m_mesh_vertices),
            int(m_pimpl->m_total_draws),
            int(m_pimpl->m_total_instances),
            (long long)m_pimpl->m_total_triangles);
        std::cout << "-----------------" << std::endl;
    }

    void Mesh::printInfo() const
    {
        if (m_pimpl->m_shapes.empty())
            unpack(false);
        assert(!m_pimpl->m_shapes.empty());

        const auto& shapes = getShapes();
        const auto& materials = getMaterials();

        for (size_t m = 0; m < materials.size(); m++) {
            const auto& material = materials.at(m);
            std::cout << "[Mesh] material " << m << " name " << material.name << std::endl;

            std::cout << "[Mesh] material " << m << " diffuse r " << material.Kd[0] << std::endl;
            std::cout << "[Mesh] material " << m << " diffuse g " << material.Kd[1] << std::endl;
            std::cout << "[Mesh] material " << m << " diffuse b " << material.Kd[2] << std::endl;
        }

        for (size_t s = 0; s < shapes.size(); s++) {
            const auto& shape = shapes.at(s);

            std::cout << "shape name " << shape.name << std::endl;
            std::cout << "vertices size " << shape.vertices.size() << std::endl;
            std::cout << "indices size " << shape.indices.size() << std::endl;
            std::cout << "material id " << shape.material << std::endl;
            std::cout << "has normal " << shape.hasNormal << std::endl;

            //        for ( int i = 0; i < shape.vertices.size(); ++i ) {
            //            const auto& vertex = shape.vertices.at( i );
            //            std::cout << "v: " << vertex.px << " " << vertex.py << " " << vertex.pz <<
            //            std::endl;
            //        }
        }
    }

    std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
    {
        os << mesh.to_string();
        return os;
    }

} // namespace data
} // namespace hub
