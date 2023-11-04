
#include "test_common.hpp"

#include "core/test_core_common.hpp"

#include <iostream>

#include <sensor/Format.hpp>
#include <core/io/Archive.hpp>

TEST_CASE( "sensor/Format test" ) {


//    InputOutput inputOutput;
    hub::io::Archive archive;

//    static_assert( !hub::serializable_v<hub::sensor::Format> );
//    static_assert( !hub::writable_v<hub::sensor::Format> );
//    static_assert( !hub::readable_v<hub::sensor::Format> );


//    static_assert(hub::sensor::format::RGB8.nByte == 3);
//    static_assert(hub::sensor::format::RGB8.name == "RGB8");
//    static_assert(! hub::sensor::format::RGB8.interpolable);

//    auto & format = hub::sensor::format::RGB8;

    const auto format = hub::sensor::Format{};
    assert(format.interpolable == false);
    assert(format.nByte == 0);
//    assert(! std::memcmp(format.name, "");

//    archive.write(format);
//    hub::sensor::Format format_read;
//    archive.read(format_read);
//    CHECK(format == format_read);

//    constexpr auto format2 = hub::sensor::Format{5, "hello", true};
//    static_assert(format2.interpolable == true);
//    static_assert(format2.nByte == 5);

//    archive.write(format2);
//    auto format2_read = archive.get<hub::sensor::Format>();
//    CHECK(format2 == format2_read);
}
