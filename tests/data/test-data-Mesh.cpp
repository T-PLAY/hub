
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <filesystem>

#include <data/Mesh.hpp>
#include <core/io/Archive.hpp>

TEST_CASE( "Mesh test" ) {

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // hub::data::Mesh quadMesh()
    assert(std::filesystem::exists(meshPath));
    assert(std::filesystem::exists(meshPath + "quad.gltf"));

    const hub::data::Mesh quadMesh(meshPath + "quad");
    const auto quadShapes = quadMesh.getShapes();
    assert(quadShapes.size() == 1);
    const auto quadShape = quadShapes.at(0);
    assert(quadShape.indices.size() == 6);
    assert(quadShape.vertices.size() == 4);
    assert(quadShape.material == 0);
    const auto quadMaterials = quadMesh.getMaterials();

    // hub::data::Mesh quadMesh(meshPath + "sensor");
    std::cout << "quadMesh : " << quadMesh << std::endl;
    std::cout << std::endl;
    std::cout << "quadMesh info : " << std::endl;
    quadMesh.printInfo();
    std::cout << std::endl;
    std::cout << "quadMesh stats : " << std::endl;
    quadMesh.printStats();

    hub::io::Archive archive;
    archive.write(quadMesh);

    hub::data::Mesh quadMesh_read;
    archive.read(quadMesh_read);
    std::cout << "quadMesh_read : " << quadMesh_read << std::endl;

    assert(quadMesh == quadMesh_read);

    std::cout << "typename mesh : " << TYPE_NAME(hub::data::Mesh()) << std::endl;


}
