#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
#include "test_io_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

struct UserDefined {
    int a;
    bool b;
    double c;
    template <typename Archive, typename Self>
    static void serialize( Archive& archive, Self& self ) {
        archive( self.a, self.b, self.c );
    }
    hub::Size_t dataSize() const { return sizeof( int ); }
    auto toString() const {
        return std::to_string( a ) + ":" + std::to_string( b ) + ":" + std::to_string( c );
    }
    bool operator==( const UserDefined& other ) const {
        return a == other.a && b == other.b && c == other.c;
    }
};

TEST_CASE( "InputOutputStream local header test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        const UserDefined userDefined { 1, true, 2.0 };
        const auto header_ref = hub::io::make_header( userDefined );

        hub::output::OutputStream outputStream( header_ref, port );

        hub::input::InputStream inputStream( port );
        assert( inputStream.getHeader() == header_ref );

        const int a_ref = 5;
        static_assert( sizeof( int ) == 4 );
        outputStream.write( 5 );
        int a_read;
        inputStream.read( a_read );
        assert( a_ref == a_read );

        hub::io::Memory memory( inputStream.getHeader().getUserDefined() );
        UserDefined userDefined_read;
        memory.read( userDefined_read );
        assert( userDefined == userDefined_read );
    }

    TEST_END()
}
