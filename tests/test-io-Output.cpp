#include <catch2/catch_test_macros.hpp>

#include <io/Output.hpp>

#include <io/Memory.hpp>

TEST_CASE( "Output test" ) {

    std::string ref_string    = "hello";
    const auto ref_resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    unsigned char data[3] { 1, 2, 3 };
    hub::Acquisition ref_acq =
        std::move( hub::Acquisition { 0, 1 } << hub::data::Measure { data, 3, ref_resolution } );
    const char ref_str[] = "hello";
    hub::Any ref_any_int( 5 );
    hub::Any ref_any_double( 5.0 );
    hub::Any ref_any_string( std::string( "hello" ) );
    hub::Any ref_any_constCharPtr( "hello" );
    hub::Any ref_any_mat4( hub::data::Mat4( 0.0 ) );
        const std::string meshPath = HUB_DIR "tests/data/mesh/";
        hub::data::Mesh ref_mesh( meshPath + "quad.gltf" );
        hub::Any ref_any_mesh( ref_mesh );

    std::vector<char> buff;

    hub::io::Memory<decltype( buff )> memory( buff );

    memory.write( ref_string );
    memory.write( ref_sensorSpec );
    memory.write( ref_acq );
    memory.write( ref_str );

    memory.write( ref_any_int );
    memory.write( ref_any_double );
    memory.write( ref_any_string );
    memory.write( ref_any_constCharPtr );
    memory.write( ref_any_mat4 );
    memory.write(ref_any_mesh);

    std::string string;
    memory.read( string );
    CHECK( ref_string == string );

    hub::SensorSpec sensorSpec;
    memory.read( sensorSpec );
    CHECK( ref_sensorSpec == sensorSpec );

    hub::Acquisition acq;
    memory.read(acq);
    CHECK( ref_acq == acq );

    char str[80];
    memory.read( str );
    CHECK( str != ref_str );
    CHECK( !strcmp( str, ref_str ) );

    hub::Any any_int;
    memory.read( any_int );
    CHECK( ref_any_int == any_int );

    hub::Any any_double;
    memory.read( any_double );
    CHECK( ref_any_double == any_double );

    hub::Any any_string;
    memory.read( any_string );
    CHECK( ref_any_string == any_string );

    hub::Any any_constCharPtr;
    memory.read( any_constCharPtr );
    CHECK( ref_any_constCharPtr == any_constCharPtr );

    hub::Any any_mat4;
    memory.read( any_mat4 );
    std::cout << "ref_mat4: " << ref_any_mat4 << std::endl;
    std::cout << "mat4: " << any_mat4 << std::endl;
    assert( ref_any_mat4 == any_mat4 );
    CHECK( ref_any_mat4 == any_mat4 );

        hub::Any any_mesh;
        memory.read( any_mesh );
        CHECK( ref_any_mesh == any_mesh );

    CHECK( memory.isEnd() );
}
