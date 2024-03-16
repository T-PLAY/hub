#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <io/Header.hpp>

TEST_CASE( "Test io header" ) {
    TEST_BEGIN()

    hub::io::Archive archive;

    const hub::Datas_t datas { 1, 2, 3, 4, 5 };

    const hub::io::Header header( 10, datas );

    std::cout << "-------------------------------- writing header -------------------------- "
              << std::endl;

    archive.write( header );

    std::cout << std::endl;
    std::cout << "-------------------------------- reading header -------------------------- "
              << std::endl;

    hub::io::Header header_read;

    archive.read( header_read );

    CHECK( header == header_read );

    TEST_END()
}
