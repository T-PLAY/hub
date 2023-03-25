
#include <catch2/catch_test_macros.hpp>

// #include <cmath>
#include <iostream>

#include <data/Mesh.hpp>

TEST_CASE( "Mesh test" ) {

    const std::string meshPath = HUB_DIR "tests/data/meshes/";

    hub::data::Mesh mesh( meshPath + "quad.obj" );

    const auto& attrib = mesh.getAttrib();
    const auto& shapes = mesh.getShapes();
    assert( shapes.size() == 1 );
    const auto& materials = mesh.getMaterials();

    CHECK( shapes.size() == 1 );
    CHECK( shapes[0].mesh.num_face_vertices.size() == 2 );
    CHECK( shapes[0].mesh.num_face_vertices[0] == 3 );
    CHECK( shapes[0].mesh.num_face_vertices[1] == 3 );

    // Loop over shapes
    for ( size_t s = 0; s < shapes.size(); s++ ) {
        //        std::cout << "[Mesh] shape " << s << std::endl;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for ( size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++ ) {
            //            std::cout << "[Mesh] face " << f << std::endl;
            size_t fv = size_t( shapes[s].mesh.num_face_vertices[f] );

            // Loop over vertices in the face.
            for ( size_t v = 0; v < fv; v++ ) {
                //                std::cout << "[Mesh] vertex " << v << std::endl;
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx   = attrib.vertices[3 * size_t( idx.vertex_index ) + 0];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex vx " << vx << std::endl;
                tinyobj::real_t vy = attrib.vertices[3 * size_t( idx.vertex_index ) + 1];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex vy " << vy << std::endl;
                tinyobj::real_t vz = attrib.vertices[3 * size_t( idx.vertex_index ) + 2];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex vz " << vz << std::endl;

                // Check if `normal_index` is zero or positive. negative = no normal data
                if ( idx.normal_index >= 0 ) {
                    tinyobj::real_t nx = attrib.normals[3 * size_t( idx.normal_index ) + 0];
                    std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                              << " vertex nx " << nx << std::endl;
                    tinyobj::real_t ny = attrib.normals[3 * size_t( idx.normal_index ) + 1];
                    std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                              << " vertex ny " << ny << std::endl;
                    tinyobj::real_t nz = attrib.normals[3 * size_t( idx.normal_index ) + 2];
                    std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                              << " vertex nz " << nz << std::endl;
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if ( idx.texcoord_index >= 0 ) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t( idx.texcoord_index ) + 0];
                    std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                              << " vertex tx " << tx << std::endl;
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t( idx.texcoord_index ) + 1];
                    std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                              << " vertex ty " << ty << std::endl;
                }

                // Optional: vertex colors
                tinyobj::real_t red = attrib.colors[3 * size_t( idx.vertex_index ) + 0];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex color red " << red << std::endl;
                tinyobj::real_t green = attrib.colors[3 * size_t( idx.vertex_index ) + 1];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex color green " << green << std::endl;
                tinyobj::real_t blue = attrib.colors[3 * size_t( idx.vertex_index ) + 2];
                std::cout << "[Mesh] shape " << s << " face " << f << " vertex " << v
                          << " vertex color blue " << blue << std::endl;
            }
            index_offset += fv;

            // per-face material
            std::cout << "[Mesh] shape " << s << " face " << f << " material "
                      << shapes[s].mesh.material_ids[f] << std::endl;
        }
    }

    for ( size_t m = 0; m < materials.size(); m++ ) {
        //        std::cout << "[Mesh] material " << m << std::endl;
        const auto& material = materials.at( m );

        std::cout << "[Mesh] material " << m << " diffuse r" << material.diffuse[0] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse g" << material.diffuse[1] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse b" << material.diffuse[2] << std::endl;
    }
}
