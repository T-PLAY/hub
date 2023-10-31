
 #define HUB_DEBUG_INPUT
 #define HUB_DEBUG_OUTPUT

#include <set>

#include "core/io/test_core_io_common.hpp"
#include "core/test_core_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>

#include <core/io/InputOutput.hpp>
#include <core/io/InputOutputImpl.hpp>
#include <core/io/InputOutputZppBits.hpp>
//#include <core/InputOutput.hpp>
#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/Any.hpp>
#include <core/io/Book.hpp>


struct Random {
    double a;
    int b;
    std::string c;
    std::vector<int> d;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, c, d );
    }

    bool operator==( const Random& random ) const {
        return a == random.a && b == random.b && c == random.c &&
               d == random.d;
    }
    friend std::ostream& operator<<( std::ostream& os, const Random& random );
};

std::ostream& operator<<( std::ostream& os, const Random& data ) {
    os << data.a << " " << data.b << " " << data.c << " ";
    ::operator<<(os, data.d);
    return os;
}


TEST_CASE( "InputOutput test" ) {
//    using namespace testCoreIoCommon;

//    hub::Any any;

//    hub::io::Book book;
    hub::io::Archive archive;
//    hub::io::Archive<hub::serializer::SerializerZppBits> archive;
//    hub::io::Archive<hub::serializer::SerializerImpl> archive;

    //////////////////////////////////

    std::cout << "------------------ void ---------------------" << std::endl;
    hub::Any any;
    archive.write(any);
    hub::Any any_read;
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;

    std::cout << "------------------ int ---------------------" << std::endl;
    any = 1;
    archive.write(any);
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;

    std::cout << "------------------ Random ---------------------" << std::endl;
    any = Random{5, true, "hello", {1, 2, 3}};
    archive.write(any);
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;

#ifdef DEBUG
    std::cout << "io call: " << archive.getNCall() << std::endl;
#endif
}
