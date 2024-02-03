#include "test_common.hpp"

#include <io/input/InputFile.hpp>
#include <io/output/OutputFile.hpp>

#if CPP_VERSION >= 17
#    include <filesystem>
#endif

#include <thread>

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

TEST_CASE( "File test" ) {
    TEST_BEGIN()

    const std::string filename = FILE_NAME + ".txt";

#if CPP_VERSION >= 17
    std::filesystem::remove( filename );
#endif

    const UserDefined userDefined { 1, true, 2.0 };
    const auto header_ref = hub::io::make_header( userDefined );

    const int a_ref = 5;
    {
        hub::output::OutputFile outputFile( header_ref, filename );
        outputFile.write( a_ref );
    }

    int a_read;
    {
        hub::input::InputFile inputFile( filename );
        assert( inputFile.getHeader() == header_ref );
        inputFile.read( a_read );

        assert( a_ref == a_read );

        hub::io::Memory memory( inputFile.getHeader().getUserDefined() );
        UserDefined userDefined_read;
        memory.read( userDefined_read );
        assert( userDefined == userDefined_read );

        assert( inputFile.isEnd() );
    }

    ////        hub::output::OutputFile outputFile(filename);

    ////            hub::input::InputFile(
    ////                std::fstream( filename, std::ios::out | std::ios::binary | std::ios::trunc )
    ///) );

    ////        hub::input::InputFile inputFile(filename);
    ////        hub::sensor::InputSensor inputSensor(
    ////            hub::input::InputFile(filename));
    ////            hub::input::InputFile( std::fstream( filename, std::ios::in | std::ios::binary )
    ///) );

    TEST_END()
}
