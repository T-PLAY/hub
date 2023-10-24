
#include "test_common.hpp"

#include "core/test-core-common.hpp"

#include <iostream>

#include <sensor/resolution/Format.hpp>

TEST_CASE( "sensor/Format test" ) {


    InputOutput inputOutput;

    static_assert( !hub::serializable_v<hub::sensor::resolution::Format> );
    static_assert( !hub::Output::writable_v<hub::sensor::resolution::Format> );
    static_assert( !hub::Input::readable_v<hub::sensor::resolution::Format> );


    static_assert(hub::sensor::resolution::format::RGB8.nByte == 3);
//    static_assert(hub::sensor::resolution::format::RGB8.name == "RGB8");
    static_assert(! hub::sensor::resolution::format::RGB8.interpolable);

//    auto & format = hub::sensor::resolution::format::RGB8;

    constexpr auto format = hub::sensor::resolution::Format{};
    static_assert(format.interpolable == false);
    static_assert(format.nByte == 0);
//    assert(! std::memcmp(format.name, "");

    inputOutput.write(format);
    hub::sensor::resolution::Format format_read;
    inputOutput.read(format_read);
    CHECK(format == format_read);

    constexpr auto format2 = hub::sensor::resolution::Format{5, "hello", true};
    static_assert(format2.interpolable == true);
    static_assert(format2.nByte == 5);

    inputOutput.write(format2);
    auto format2_read = inputOutput.get<hub::sensor::resolution::Format>();
    CHECK(format2 == format2_read);
}
