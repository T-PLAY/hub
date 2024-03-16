
#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <filesystem>

#include <data/Mesh.hpp>
#include <core/io/Archive.hpp>

TEST_CASE( "Mesh test" ) {

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // hub::data::Mesh quadMesh()
    CHECK(std::filesystem::exists(meshPath));
    CHECK(std::filesystem::exists(meshPath + "quad.gltf"));

    const hub::data::Mesh quadMesh(meshPath + "quad");
//    const hub::data::Mesh quadMesh(meshPath + "sensor");
    const auto quadShapes = quadMesh.getShapes();
    CHECK(quadShapes.size() == 1);
    const auto quadShape = quadShapes.at(0);
    CHECK(quadShape.indices.size() == 6);
    CHECK(quadShape.vertices.size() == 4);
    CHECK(quadShape.material == 0);
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

    CHECK(quadMesh == quadMesh_read);

    std::cout << "typename mesh : " << TYPE_NAME(hub::data::Mesh()) << std::endl;


}
