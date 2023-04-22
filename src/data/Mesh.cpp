// #define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.hpp"

// #include <filesystem>
#include <iostream>

// #include "io/Memory.hpp"

#include <cgltf.h>
#include <gltf/gltfpack.h>
#include <meshoptimizer/meshoptimizer.h>

// #include <filesystem>
// #include <experimental/filesystem>
#include <filesystem>
#include <fstream>
#include <sstream>
// #include <stdio.h>
// #include <stdlib.h>

#ifdef __unix
#    define fopen_s( pFile, filename, mode ) \
        ( ( *( pFile ) ) = fopen( ( filename ), ( mode ) ) ) == NULL
#endif

int getFileSize( std::string path ) {
    // #include <fstream>
    FILE* pFile = NULL;

    // get the file stream
    fopen_s( &pFile, path.c_str(), "rb" );

    // set the file pointer to end of file
    fseek( pFile, 0, SEEK_END );

    // get the file size
    int Size = ftell( pFile );

    // return the file pointer to begin of file if you want to read it
    // rewind( pFile );

    // close stream and release buffer
    fclose( pFile );

    return Size;
}

namespace hub {
namespace data {

static cgltf_data* s_data = nullptr;
static std::vector<::Mesh> s_meshes;
static std::vector<Animation> s_animations;
static std::vector<std::string> s_nodeNames;
static size_t s_nVertice  = 0;
static size_t s_nTriangle = 0;
static size_t s_nDraw     = 0;

Mesh::Mesh( const Measure& measure ) :
    Measure( measure.getData(), measure.getSize(), Resolution { { 1 }, Format::MESH } ) {
    //    assert( measure.getSize() == 64 );
    //    memcpy( (unsigned char*)&m_x, m_data, m_size );
    assert( m_data != nullptr );
    //    memcpy( m_data, measure.getData(), m_size );

    unpack();
}

Mesh::Mesh( const Mesh& mesh ) :
    Measure( new unsigned char[mesh.m_size],
             mesh.m_size,
             Resolution { { 1 }, Format::MESH },
             true ) {
    memcpy( m_data, mesh.m_data, m_size );
    //    Measure( mesh.getData(), mesh.getSize(), Resolution { { 1 }, Format::MESH } ) {
    //    assert( m_data != nullptr );
    unpack();
}

// Mesh::Mesh( const std::string& name, const std::vector<std::string>& filePaths ) :
Mesh::Mesh( const std::string& name, std::initializer_list<std::string> filePaths ) :
    Measure( (unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH } ), m_name( name ) {

    //}

    std::vector<std::string> compressedFilePaths;
    std::vector<uint64_t> compressedFilePathSizes;
    size_t size = 0;
    //    std::vector<uns
    for ( const auto& filePath : filePaths ) {
// void Mesh::load( const std::string& filePath ) {
//         std::filesystem::path path(filePath);
#ifdef WIN32
        size_t slash = filePath.find_last_of( "\\" );
#else
        size_t slash = filePath.find_last_of( "/" );
#endif
        std::string parentPath =
            ( slash != std::string::npos ) ? filePath.substr( 0, slash ) : filePath;

        size_t lastindex               = filePath.find_last_of( "." );
        std::string filePathWithoutExt = filePath.substr( 0, lastindex );
        //        const std::string fileMtlPath = filename + ".mtl";

        size_t lastindex2    = filePath.find_last_of( "/" );
        std::string filename = filePath.substr( lastindex2 + 1, lastindex - lastindex2 - 1 );

        //        std::cout << "[Mesh] file path = '" << filePath << "'" << std::endl;

        //        m_name = rawname;
        //        std::cout << "[Mesh] parentPath = " << parentPath << std::endl;
        //        std::cout << "[Mesh] filename = " << filename << std::endl;
        //        std::cout << "[Mesh] filename = " << filePathWithoutExt << std::endl;

        std::string iext = getExtension( filePath.c_str() );
        //        std::cout << "[Mesh] extension : " << iext << std::endl;

        //    std::string reportFilePath = parentPath + "/" + filename + ".txt";
        std::string compressedFilePath = parentPath + "/" + filename + ".glb";
        compressedFilePaths.emplace_back( compressedFilePath );
        //        std::cout << "[Mesh] compressed file path = '" << compressedFilePath << "'" <<
        //        std::endl;

        //    return;

        //        if ( iext == ".gltf" && !
        //        std::experimental::filesystem::exists(compressedFilePath) ) {
        if ( !std::ifstream { compressedFilePath } ) {
            if ( iext == ".gltf" ) {

                meshopt_encodeIndexVersion( 1 );

                Settings settings  = {};
                settings.quantize  = true;
                settings.pos_bits  = 14;
                settings.tex_bits  = 12;
                settings.nrm_bits  = 8;
                settings.col_bits  = 8;
                settings.trn_bits  = 16;
                settings.rot_bits  = 12;
                settings.scl_bits  = 16;
                settings.anim_freq = 30;
                //            settings.simplify_threshold = 1.f;
                settings.texture_scale = 1.f;
                for ( int kind = 0; kind < TextureKind__Count; ++kind )
                    settings.texture_quality[kind] = 8;

                settings.compress              = true;
                settings.compressmore          = true;
                settings.verbose               = 2;
                settings.simplify_aggressive   = true;
                settings.simplify_lock_borders = true;
                settings.simplify_threshold    = 0.1;
                settings.keep_nodes            = true;
                //            settings.keep_extras           = true;
                //            settings.keep_materials        = true;

                const char* input  = filePath.c_str();
                const char* output = compressedFilePath.c_str();
                const char* report = 0;
                //            const char* report = reportFilePath.c_str();

                int ret = gltfpack( input, output, report, settings );
                assert( ret == 0 );
            }
            else {
                std::cerr << "[Mesh] unable to open " << iext << " extension" << std::endl;
                assert( false );
            }
        }

        const auto fileSize = getFileSize( compressedFilePath );
        compressedFilePathSizes.emplace_back( fileSize );
        size += fileSize;
    }
    //        cgltf_data* s_data = 0;
    //    std::vector<::Mesh> meshes;
    //    std::vector<Animation> animations;

    //    data              = parseObj( filePath.c_str(), meshes, &error );
    //    s_data = parseGlb( filePath.c_str(), s_meshes, s_animations, &error );
    //    s_data = parseGlb( compressedFilePath.c_str(), s_meshes, s_animations, &error );

    //        std::vector<char> buff;

    //        io::Memory<decltype( buff )> memory( buff );
    //        memory.write( rawname );
    //        memory.write( fileObjTxt );
    //        memory.write( fileMtlTxt );

    //        //    assert(buff.size() == fileObjTxt.size() + fileMtlTxt.size() + 8 );
    //        assert( buff.size() == fileObjTxt.size() + fileMtlTxt.size() + 8 + rawname.size() + 4
    //        ); m_data = new unsigned char[buff.size()]; m_size = buff.size(); memcpy( m_data,
    //        buff.data(), m_size );

    //    void* file_data = NULL;
    //    cgltf_size file_size = 0;
    //    std::ifstream iFile(compressedFilePath.c_str(), std::ios::binary);
    //    void * file_data = iFile.rdbuf();

    //    size_t cur = 0;
    m_size = sizeof( uint64_t ) + m_name.size() + size +
             sizeof( uint64_t ) * ( 1 + compressedFilePaths.size() ) + size;
    //    std::cout << "total size : " << m_size << std::endl;
    m_data = new unsigned char[m_size];

    size_t nData = compressedFilePaths.size();
    //    std::cout << "nData : " << nData << std::endl;
    size_t cur         = 0;
    uint64_t* dataLong = (uint64_t*)m_data;
    dataLong[cur++]    = nData;
    //    std::vector<uint64_t> dataSizes(nData);
    //    std::cout << "dataSizes : ";
    for ( int i = 0; i < nData; ++i ) {
        //        dataSizes[i] = dataLong[cur++];
        dataLong[cur++] = compressedFilePathSizes[i];
        //        std::cout << compressedFilePathSizes[i] << ", ";
    }
    dataLong[cur++] = m_name.size();
    //    std::cout << std::endl;
    cur = sizeof( uint64_t ) * ( nData + 2 );

    memcpy( m_data + cur, m_name.c_str(), m_name.size() );
    cur += m_name.size();

    for ( const auto& compressedFilePath : compressedFilePaths ) {
        std::ostringstream buf;
        std::ifstream input( compressedFilePath.c_str() );
        buf << input.rdbuf();
        input.close();
        const auto& fileData = buf.str();
        //        std::cout << "write " << fileData.size() << " bytes to " << cur << " position" <<
        //        std::endl;
        memcpy( m_data + cur, fileData.data(), fileData.size() );
        cur += fileData.size();
    }

    unpack();

    //    result = cgltf_parse(options, file_data, file_size, out_data);

    //    std::cout << "nScene : " << s_data->scenes_count << std::endl;
    //    //    std::cout << "scene name : " << data->scene->name << std::endl;
    //    //    std::cout << "scene : '" << data->scenes[0].name << "'" << std::endl;

    //    std::cout << "nNodes : " << s_data->nodes_count << std::endl;
    //    //    std::vector<std::string> nodeNames;
    //    s_nodeNames.reserve( s_data->nodes_count );
    //    for ( int i = 0; i < s_data->nodes_count; ++i ) {
    //        const auto& node = s_data->nodes[i];
    //        if ( node.name != nullptr ) {
    //            std::cout << "node " << i << ", name : " << node.name << std::endl;
    //            s_nodeNames.emplace_back( node.name );
    //            //            assert(node.has_mesh_gpu_instancing);
    //            //            assert(node.mesh != nullptr);
    //            if ( node.mesh != nullptr ) {
    //                const auto& mesh = *node.mesh;
    //                std::cout << "node " << i << ", nPrimitive : " << mesh.primitives_count
    //                          << std::endl;
    //                //				data->memory.free_func(data->memory.user_data,
    //                // data->meshes[i].primitives[j].attributes[k].name);
    //                //        std::cout << i << " nTriangle : " << mesh.indices.size() / 3 <<
    //                std::endl;
    //                //        std::cout << i << " nVertice : " << mesh.streams[0].data.size() <<
    //                //        std::endl;
    //            }
    //        }
    //    }

    //    std::cout << "nMaterial : " << s_data->materials_count << std::endl;
    //    for ( int i = 0; i < s_data->materials_count; ++i ) {
    //        const auto& material = s_data->materials[i];
    //        if ( material.name != nullptr )
    //            std::cout << "material " << i << ", name : " << material.name << std::endl;
    //    }

    //    std::cout << "nMesh : " << s_meshes.size() << std::endl;
    //    std::cout << s_meshes.size() << std::endl;
    //}
    // else {
    //    assert( false );
    //}

    //    loadMesh(m_internal, fileObjPath.c_str());
    //    process(m_internal, fileObjPath.c_str());

    //    size_t lastindex              = fileObjPath.find_last_of( "." );
    //    std::string filename          = fileObjPath.substr( 0, lastindex );
    //    const std::string fileMtlPath = filename + ".mtl";

    //    size_t lastindex2   = fileObjPath.find_last_of( "/" );
    //    std::string rawname = fileObjPath.substr( lastindex2 + 1, lastindex - lastindex2 - 1 );

    //    m_name = rawname;
    //    std::cout << "[Mesh] dirPath = " << dirPath << std::endl;

    //    //    std::string inputfile = "cornell_box.obj";
    //        tinyobj::ObjReaderConfig reader_config;
    //    //    reader_config.mtl_search_path = "./"; // Path to material files
    //        reader_config.mtl_search_path = dirPath; // Path to material files

    //    tinyobj::ObjReader reader;

    //        if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //            if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error();
    //            } exit( 1 );
    //        }

    //    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    //    m_attrib = reader.GetAttrib();
    //    m_shapes = reader.GetShapes();
    //    m_materials = reader.GetMaterials();

    //    tinyobj::attrib_t m_attrib = reader.GetAttrib();
    //    std::vector<tinyobj::shape_t> m_shapes = reader.GetShapes();
    //    std::vector<tinyobj::material_t> m_materials = reader.GetMaterials();

    //    std::ifstream inFileObj;
    //    inFileObj.open( fileObjPath );
    //    assert( inFileObj.is_open() );
    //    std::stringstream strStreamObj;
    //    strStreamObj << inFileObj.rdbuf();
    //    const std::string fileObjTxt = strStreamObj.str();

    //    std::ifstream inFileMtl;
    //    inFileMtl.open( fileMtlPath );
    //    assert( inFileMtl.is_open() );
    //    std::stringstream strStreamMtl;
    //    strStreamMtl << inFileMtl.rdbuf();
    //    const std::string fileMtlTxt = strStreamMtl.str();

    //    std::vector<char> buff;

    //    io::Memory<decltype( buff )> memory( buff );
    //    memory.write( rawname );
    //    memory.write( fileObjTxt );
    //    memory.write( fileMtlTxt );

    //    //    assert(buff.size() == fileObjTxt.size() + fileMtlTxt.size() + 8 );
    //    assert( buff.size() == fileObjTxt.size() + fileMtlTxt.size() + 8 + rawname.size() + 4 );
    //    m_data = new unsigned char[buff.size()];
    //    m_size = buff.size();
    //    memcpy( m_data, buff.data(), m_size );

    //    unpack();
}

Mesh::Mesh( const std::string& name, std::string filePath ) : Mesh( name, { filePath } ) {}

void Mesh::unpack() {

    const char* error = 0;
    //    s_data = readGlb(fileData.c_str(), fileData.size(), s_meshes, s_animations, &error);
    size_t cur               = 0;
    const uint64_t* dataLong = (uint64_t*)m_data;
    const auto nData         = dataLong[cur++];
    //    std::cout << "nData : " << nData << std::endl;
    std::vector<uint64_t> dataSizes( nData );
    //    std::cout << "dataSizes : ";
    for ( int i = 0; i < nData; ++i ) {
        dataSizes[i] = dataLong[cur++];
        //        std::cout << dataSizes[i] << ", ";
    }

    uint64_t strLen = dataLong[cur++];
    //    std::cout << std::endl;
    cur = sizeof( uint64_t ) * ( nData + 2 );

    unsigned char strBuff[80] = { 0 };

    memcpy( strBuff, m_data + cur, strLen );
    m_name = std::string( strBuff, strBuff + strLen );
    cur += m_name.size();

    for ( int i = 0; i < nData; ++i ) {
        //        dataSizes[i] = dataLong[cur++];
        static cgltf_data* data = nullptr;
        data = readGlb( (const char*)m_data + cur, dataSizes[i], s_meshes, s_animations, &error );
        //        std::cout << "read " << dataSizes[i] << " bytes from " << cur << " position" <<
        //        std::endl; assert(s_data != nullptr);
        cur += dataSizes[i];

        //        std::cout << "nNodes : " << data->nodes_count << std::endl;
        //    std::vector<std::string> nodeNames;
        s_nodeNames.reserve( data->nodes_count );
        for ( int i = 0; i < data->nodes_count; ++i ) {
            const auto& node = data->nodes[i];
            if ( node.name != nullptr ) {
                //                std::cout << "node " << i << ", name : " << node.name <<
                //                std::endl;
                s_nodeNames.emplace_back( node.name );
                //            assert(node.has_mesh_gpu_instancing);
                //            assert(node.mesh != nullptr);
                if ( node.mesh != nullptr ) {
                    const auto& mesh = *node.mesh;
                    std::cout << "node " << i << ", nPrimitive : " << mesh.primitives_count
                              << std::endl;
                    //				data->memory.free_func(data->memory.user_data,
                    // data->meshes[i].primitives[j].attributes[k].name);
                    //        std::cout << i << " nTriangle : " << mesh.indices.size() / 3 <<
                    //        std::endl; std::cout << i << " nVertice : " <<
                    //        mesh.streams[0].data.size() << std::endl;
                }
            }
        }
        s_data = data;
    }

    if ( error ) {
        fprintf( stderr, "Error loading: %s\n", error );
        return;
    }

    printMeshStats( s_meshes, m_name.c_str() );

    //    std::vector<char> buff;
    //    buff.insert( buff.begin(), m_data, m_data + m_size );
    //    io::Memory<decltype( buff )> memory( buff );

    int iMesh = 0;
    for ( const auto& mesh : s_meshes ) {

        //        const std::string header = "[Mesh:" + std::to_string( iMesh ) + "] ";
        assert( mesh.nodes.size() == 1 );
        //        if ( mesh.nodes[0]->name != nullptr )
        //            std::cout << header << "name : '" << mesh.nodes[0]->name << "'" << std::endl;
        //        std::cout << header << "name : '" << s_nodeNames.at( iMesh ) << "'" << std::endl;
        for ( int i = 0; i < mesh.streams.size(); ++i ) {
            const auto& stream = mesh.streams[i];
            //            std::cout << header << "stream attr : " << stream.type << std::endl;
            if ( stream.type == cgltf_attribute_type_position ) {
                //                std::cout << header << "nVertice : " << stream.data.size() <<
                //                std::endl;
                //            assert( mesh.streams.size() == 1 );
                s_nVertice += mesh.streams[i].data.size();
                s_nDraw += std::max( size_t( 1 ), mesh.nodes.size() );
            }
        }
        //        std::cout << header << "nTriangle : " << mesh.indices.size() / 3 << std::endl;
        //        assert( mesh.indices.size() == 1 );
        s_nTriangle += mesh.indices.size() / 3;
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

    //    memory.read( m_name );
    //    std::string fileObjTxtReaded;
    //    memory.read( fileObjTxtReaded );
    //    std::string fileMtlTxtReaded;
    //    memory.read( fileMtlTxtReaded );

    //    assert( memory.isEnd() );

    //    //    std::string inputfile = "cornell_box.obj";
    //    tinyobj::ObjReaderConfig reader_config;
    //    //    reader_config.mtl_search_path = "./"; // Path to material files
    //    reader_config.mtl_search_path = dirPath; // Path to material files

    //    tinyobj::ObjReader reader;

    //    if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //        exit( 1 );
    //    }
    //    if ( !reader.ParseFromString( fileObjTxtReaded, fileMtlTxtReaded ) ) {
    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //        exit( 1 );
    //    }

    //    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    //    m_attrib = reader.GetAttrib();
    //    m_shapes = reader.GetShapes();

    //    m_materials = reader.GetMaterials();
}

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

std::string Mesh::to_string() const {
    std::string str = "";
    str += "'" + m_name + "'";
    str += ", nDraw:" + std::to_string( s_nDraw );
    str += ", nVertice:" + std::to_string( s_nVertice );
    str += ", nTriangle:" + std::to_string( s_nTriangle );
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

std::ostream& operator<<( std::ostream& os, const Mesh& mesh ) {
    os << mesh.to_string();
    return os;
}

} // namespace data
} // namespace hub
