#define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.hpp"

//#include <filesystem>
#include <iostream>

#include "io/Memory.hpp"

namespace hub {
namespace data {

Mesh::Mesh(const Measure &measure)
    : Measure( measure.getData(), measure.getSize(), Resolution { { 1 }, Format::MESH } ) {
//    assert( measure.getSize() == 64 );
//    memcpy( (unsigned char*)&m_x, m_data, m_size );
    assert( m_data != nullptr );

}

Mesh::Mesh( const Mesh& mesh ) :
    Measure( new unsigned char[mesh.m_size],
             mesh.m_size,
             Resolution { { 1 }, Format::MESH },
             true
              ) {
    memcpy( m_data, mesh.m_data, m_size );
}



Mesh::Mesh( const std::string& fileObjPath ) :
    Measure( (unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH } ) {

    //    std::filesystem::path path(fileObjPath);
    // #ifdef WIN32
    //    size_t slash = fileObjPath.find_last_of( "\\" );
    // #else
    //    size_t slash = fileObjPath.find_last_of( "/" );
    // #endif
    //    std::string dirPath = ( slash != std::string::npos ) ? fileObjPath.substr( 0, slash ) :
    //    fileObjPath;
    size_t lastindex = fileObjPath.find_last_of(".");
    std::string rawname = fileObjPath.substr(0, lastindex);
    const std::string fileMtlPath = rawname + ".mtl";


    //    std::cout << "[Mesh] dirPath = " << dirPath << std::endl;

    std::ifstream inFileObj;
    inFileObj.open( fileObjPath );
    assert(inFileObj.is_open());
    std::stringstream strStreamObj;
    strStreamObj << inFileObj.rdbuf();
    const std::string fileObjTxt = strStreamObj.str();

    std::ifstream inFileMtl;
    inFileMtl.open( fileMtlPath );
    assert(inFileMtl.is_open());
    std::stringstream strStreamMtl;
    strStreamMtl << inFileMtl.rdbuf();
    const std::string fileMtlTxt = strStreamMtl.str();

    std::vector<char> buff;
    m_data = new unsigned char[buff.size()];
    m_size = buff.size();
    memcpy(m_data, buff.data(), m_size);

    io::Memory<decltype( buff )> memory( buff );
    memory.write(fileObjTxt);
    memory.write(fileMtlTxt);

    assert(buff.size() == fileObjTxt.size() + fileMtlTxt.size() + 8);

    std::string fileObjTxtReaded;
    memory.read(fileObjTxtReaded);
    std::string fileMtlTxtReaded;
    memory.read(fileMtlTxtReaded);

    assert(memory.isEnd());

    //    //    std::string inputfile = "cornell_box.obj";
    //    tinyobj::ObjReaderConfig reader_config;
    //    //    reader_config.mtl_search_path = "./"; // Path to material files
    //    reader_config.mtl_search_path = dirPath; // Path to material files

    tinyobj::ObjReader reader;

    //    if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //        exit( 1 );
    //    }
    if ( !reader.ParseFromString( fileObjTxtReaded, fileMtlTxtReaded ) ) {
        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
        exit( 1 );
    }

    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    m_attrib = reader.GetAttrib();
    m_shapes = reader.GetShapes();

    m_materials = reader.GetMaterials();
}

Mesh::Mesh(const std::string &objTxt, const std::string &mtlTxt)
    : Measure( (unsigned char*)nullptr, 0, Resolution { { 1 }, Format::MESH } )
{
    tinyobj::ObjReader reader;

    //    if ( !reader.ParseFromFile( fileObjPath, reader_config ) ) {
    //        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
    //        exit( 1 );
    //    }
    if ( !reader.ParseFromString( objTxt, mtlTxt ) ) {
        if ( !reader.Error().empty() ) { std::cerr << "TinyObjReader: " << reader.Error(); }
        exit( 1 );
    }

    if ( !reader.Warning().empty() ) { std::cout << "TinyObjReader: " << reader.Warning(); }

    m_attrib = reader.GetAttrib();
    m_shapes = reader.GetShapes();

    m_materials = reader.GetMaterials();

}

std::string Mesh::to_string() const
{
    std::string str = "[";
//    return
    for ( size_t s = 0; s < m_shapes.size(); s++ ) {
        const auto & shape = m_shapes.at(s);
        str += shape.name + ": nb points = " + std::to_string(shape.points.indices.size());
        if (s != m_shapes.size() - 1) {
            str += ", ";
        }
    }
    str += "]";
    return str;
}


} // namespace data
} // namespace hub
