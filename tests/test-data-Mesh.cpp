
#include <catch2/catch_test_macros.hpp>

// #include <cmath>
#include <iostream>

#include <data/Mesh.hpp>

TEST_CASE( "Mesh test" ) {

    const std::string meshPath = HUB_DIR "tests/data/mesh/";

    hub::data::Mesh mesh( meshPath + "quad" );

    std::cout << mesh << std::endl;

    mesh.printStats();

    const auto& shapes    = mesh.getShapes();
    const auto& materials = mesh.getMaterials();

    for ( size_t m = 0; m < materials.size(); m++ ) {
        const auto& material = materials.at( m );
        std::cout << "[Mesh] material " << m << " name " << material.name << std::endl;

        std::cout << "[Mesh] material " << m << " diffuse r" << material.Kd[0] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse g" << material.Kd[1] << std::endl;
        std::cout << "[Mesh] material " << m << " diffuse b" << material.Kd[2] << std::endl;
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
