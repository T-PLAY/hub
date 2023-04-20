// #define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.hpp"

// #include <filesystem>
#include <iostream>

#include "io/Memory.hpp"

#include <gltf/gltfpack.h>

namespace hub {
namespace data {


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

Mesh::Mesh( const std::string& name ) :
    Measure( (unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH } ), m_name( name ) {}

void Mesh::load( const std::string& fileObjPath ) {
    //        std::filesystem::path path(fileObjPath);
#ifdef WIN32
    size_t slash = fileObjPath.find_last_of( "\\" );
#else
    size_t slash = fileObjPath.find_last_of( "/" );
#endif
    std::string dirPath =
        ( slash != std::string::npos ) ? fileObjPath.substr( 0, slash ) : fileObjPath;


    cgltf_data* data = 0;
    std::vector<::Mesh> meshes;
//    std::vector<Animation> animations;

    const char* error = 0;
    data = parseObj(fileObjPath.c_str(), meshes, &error);

    std::cout << meshes.size() << std::endl;
    for (const auto & mesh : meshes) {
        std::cout << mesh.target_names.size() << std::endl;
        for (const auto & name : mesh.target_names) {
            std::cout << name << std::endl;
        }
    }




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

void Mesh::unpack() {
    std::vector<char> buff;
    buff.insert( buff.begin(), m_data, m_data + m_size );
    io::Memory<decltype( buff )> memory( buff );

    memory.read( m_name );
    std::string fileObjTxtReaded;
    memory.read( fileObjTxtReaded );
    std::string fileMtlTxtReaded;
    memory.read( fileMtlTxtReaded );

    assert( memory.isEnd() );

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

void Mesh::pack() {}

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
    str += ", nVertice:" + std::to_string(m_internal.vertices.size());
    str += ", nTriangle:" + std::to_string(m_internal.indices.size() / 3);

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
