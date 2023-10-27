// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

// #include "core/test-core-common.hpp"
// #include "test_common.hpp"

// #include <iostream>
// #include <cstdlib>
// #include <cstring>

// #include <array>
// #include <string>
// #include <thread>
//  #include <mutex>
// #include <atomic>
#include <any>
#include <array>
#include <core/Any.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
// #include <core/Vector.hpp>

#include "zpp_bits.h"

#include <core/io/Book.hpp>

namespace testCoreIoBook {
struct Character {
    //    char c;
    double a;
    int b;
    //    int c;
    std::string name;
    std::vector<int> vints;
    //    std::any any;
    //        std::map<std::string, int> map;

    //    static struct {
    //    } packable;

    //    auto serialize() const -> decltype(std::make_tuple(a)) {
    //        return std::make_tuple(a);
    //    }

    constexpr auto serialize() {
        //        return std::make_tuple( a, b );
        //        return std::make_tuple(a, b);
        //        return m_serial;
        //        return std::tuple<double&, int&, std::string&>{a, b, name};
        return std::tuple<double&, int&, std::string&, std::vector<int>&> { a, b, name, vints };
    }

    //    std::tuple<double&, int&, std::string&, std::vector<int>&, std::map<std::string, int>&>
    //    m_serial {a, b, name, vints, map};

    //    bool operator==( const Character& character ) const { return name == character.name && age
    //    == character.age; }
    bool operator==( const Character& character ) const {
        //                return true;
        return a == character.a && b == character.b && name == character.name &&
               vints == character.vints;
        //               && map ==
        //                               character.map;
    }
    friend std::ostream& operator<<( std::ostream& os, const Character& character );

    //  private:
    //    std::tuple<double*, int*> m_serial {a, b};
    //        std::tuple<double&, int&, std::string&, std::vector<int>&, std::map<std::string,
    //        int>&> m_serial {a, b, name, vints, map};
};
static_assert( hub::serializable_v<Character> );
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );
// static_assert( sizeof( Character ) == 1 );
// static_assert( sizeof( Character ) == 16 );

std::ostream& operator<<( std::ostream& os, const Character& character ) {
    os << character.a << " " << character.b << " " << character.name;
    //           << " " << character.vints;
    return os;
}

static constexpr size_t s_characterSize = sizeof( Character );

static constexpr size_t s_maxDataSize = 1'000'000;

#ifdef HUB_DEBUG_INPUT
static constexpr size_t s_nReadWrite = 1;
#else
static constexpr size_t s_nReadWrite = s_maxDataSize / s_characterSize;
#endif

static constexpr size_t s_bookSize = s_nReadWrite * s_characterSize;
// static constexpr size_t s_nReadWrite = 100;
// static constexpr int s_nIteration                = 10;
// static constexpr size_t s_nReadWrite             = s_nReadWrite * s_nIteration;

// enum class IOImpls { Archive = 0, Book, Count };
enum class IOImpls { ZppBits = 0, BookZppBits, BookImpl, ArchiveImpl, ArchiveZppBits, Count };
static constexpr auto s_nIOImpl = (int)IOImpls::Count;
static constexpr std::string_view ioImplNames[s_nIOImpl] { "ZppBits",
                                                           "BookZppBits",
                                                           "BookImpl",
                                                           "ArchiveImpl",
                                                           "ArchiveZppBits" };

static double s_megaReadWritePerSeconds[s_nIOImpl];
static double s_gigaBytePerSeconds[s_nIOImpl];
static long long s_durations[s_nIOImpl];

static constexpr size_t s_nReadWriteDataPtr = 1;
static constexpr size_t s_DataSizePtr       = 1'00'000'000;
static double s_ReadWritePerSecondsDataPtr[s_nIOImpl];
static double s_gigaBytePerSecondsDataPtr[s_nIOImpl];
static long long s_durationsDataPtr[s_nIOImpl];

template <class ReadInputFunc, class WriteOutputFunc, class Data>
auto writeAndReadBook( ReadInputFunc& readInputFunc,
                       WriteOutputFunc& writeOutputFunc,
                       const Data& data,
                       int i ) {
    const auto& implName = ioImplNames[i];
#ifdef HUB_DEBUG_INPUT
    std::cout << "------------------------------ " << implName
              << " --------------------------------" << std::endl;
#endif

    const auto startClock = std::chrono::high_resolution_clock::now();

    Data data_write = data;

    size_t iReadWrite = 0;
    while ( iReadWrite < s_nReadWrite ) {

        data_write.b = iReadWrite + i;

        writeOutputFunc( data_write );
        Data data_read;
        readInputFunc( data_read );

        if ( data_read != data_write ) {
            std::cerr << "writeRead data_write != data_read for impl : " << implName << std::endl;
            std::cerr << data_read << " != " << data_write << std::endl;
            exit( 1 );
        }

        ++iReadWrite;
    }
    if ( iReadWrite != s_nReadWrite ) { exit( 2 ); }
    const auto endClock = std::chrono::high_resolution_clock::now();
    s_durations[i] =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
    assert( s_durations[i] > 0 );

    s_megaReadWritePerSeconds[i] = 1000.0 * (double)s_nReadWrite / s_durations[i];
    assert( s_megaReadWritePerSeconds[i] > 0 );
    s_gigaBytePerSeconds[i] = s_megaReadWritePerSeconds[i] * s_characterSize / 1000.0;
    assert( s_gigaBytePerSeconds[i] > 0 );
};

template <class ReadInputFunc, class WriteOutputFunc>
auto writeReadCharPtr( ReadInputFunc& readInputFunc,
                       WriteOutputFunc& writeOutputFunc,
                       unsigned char* data_write,
                       //                      size_t size,
                       int i ) {
    const auto& implName = ioImplNames[i];

    unsigned char* data_read = new unsigned char[s_DataSizePtr];

    const auto startClock = std::chrono::high_resolution_clock::now();
    size_t iReadWrite     = 0;
    while ( iReadWrite < s_nReadWriteDataPtr ) {

        data_write[0] = i + iReadWrite;

        writeOutputFunc( data_write, s_DataSizePtr );
        data_read[0] = i + iReadWrite;
        readInputFunc( data_read, s_DataSizePtr );

        if ( memcmp( data_read, data_write, s_DataSizePtr ) != 0 ) {
            std::cerr << "writeReadCharPtr data_write != data_read for impl : " << implName
                      << std::endl;
            exit( 1 );
        }

        ++iReadWrite;
    }
    const auto endClock = std::chrono::high_resolution_clock::now();
    s_durationsDataPtr[i] =
        std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
    assert( s_durations[i] > 0 );

    s_ReadWritePerSecondsDataPtr[i] =
        1000'000'000.0 * (double)s_nReadWriteDataPtr / s_durationsDataPtr[i];
    s_gigaBytePerSecondsDataPtr[i] =
        s_ReadWritePerSecondsDataPtr[i] * s_DataSizePtr / 1'000'000'000.0;

    delete[] data_read;
};

} // namespace testCoreIoBook

using namespace zpp::bits::literals;

// TEST_CASE( "Book test" ) {
int main() {
    using namespace testCoreIoBook;

    std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;

    std::cout << "read/write size: "
              << std::to_string( s_characterSize * s_nReadWrite / 1'000'000.0 ) << " Mo"
              << std::endl;
    std::cout << std::endl;

    Character character( 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    {
        std::array<unsigned char, s_bookSize> characters;

        auto readZppBits = [&]( Character& character ) {
            zpp::bits::in(characters)( character ).or_throw();
        };
        auto writeZppBits = [&]( const Character& character ) {
            zpp::bits::out( characters )( character ).or_throw();
        };
        writeAndReadBook( readZppBits, writeZppBits, character, 0 );
    }

    {
        static std::array<unsigned char, s_DataSizePtr + 100> characters;
        auto readZppBitsDataPtr = [&]( hub::Data_t* data, hub::Size_t size ) {
            std::span span( data, size );
            zpp::bits::in(characters)( span ).or_throw();
        };
        auto writeZppBitsDataPtr = [&]( const hub::Data_t* data, hub::Size_t size ) {
            std::span span( data, size );
            zpp::bits::out( characters )( span ).or_throw();
        };
        writeReadCharPtr( readZppBitsDataPtr, writeZppBitsDataPtr, data_write, 0 );
    }

    ///////////////////////////////////////////////////

    {
        hub::io::book::BookZppBits<s_bookSize> bookZppBits;
        auto readBookZppBits  = [&]( Character& character ) { bookZppBits.read( character ); };
        auto writeBookZppBits = [&]( const Character& character ) {
            bookZppBits.write( character );
        };
        writeAndReadBook( readBookZppBits, writeBookZppBits, character, 1 );
        assert( bookZppBits.isEnd() );
    }
    {
//        static hub::io::book::BookZppBits<s_DataSizePtr + 100> bookZppBits;
        static std::array<unsigned char, s_DataSizePtr + 100> characters;
//        std::vector<unsigned char> characters;
        hub::io::book::BookZppBits bookZppBits(characters);
        assert( bookZppBits.isEnd() );
        auto readZppBitsDataPtr = [&]( hub::Data_t* data, hub::Size_t size ) {
            bookZppBits.read( data, size );
        };
        auto writeZppBitsDataPtr = [&]( const hub::Data_t* data, hub::Size_t size ) {
            bookZppBits.write( data, size );
        };
        writeReadCharPtr( readZppBitsDataPtr, writeZppBitsDataPtr, data_write, 1 );
        assert( bookZppBits.isEnd() );
    }

    ///////////////////////////////////////////////////

    {
        hub::io::book::BookImpl<s_bookSize> bookImpl;
        auto readBookImpl  = [&]( Character& character ) { bookImpl.read( character ); };
        auto writeBookImpl = [&]( const Character& character ) { bookImpl.write( character ); };
        writeAndReadBook( readBookImpl, writeBookImpl, character, 2 );
        assert( bookImpl.isEnd() );
    }
    {
        static hub::io::book::BookImpl<s_DataSizePtr> bookImpl;
        assert( bookImpl.isEnd() );
        auto readZppBitsDataPtr = [&]( hub::Data_t* data, hub::Size_t size ) {
            bookImpl.read( data, size );
        };
        auto writeZppBitsDataPtr = [&]( const hub::Data_t* data, hub::Size_t size ) {
            bookImpl.write( data, size );
        };
        writeReadCharPtr( readZppBitsDataPtr, writeZppBitsDataPtr, data_write, 2 );
        assert( bookImpl.isEnd() );
    }

    ///////////////////////////////////////////////////

    {
        hub::io::Archive<hub::io::InputOutputImpl> archive;

        auto readBookImpl  = [&]( Character& character ) { archive.read( character ); };
        auto writeBookImpl = [&]( const Character& character ) { archive.write( character ); };
        writeAndReadBook( readBookImpl, writeBookImpl, character, 3 );
        assert( archive.isEnd() );

        auto readZppBitsDataPtr = [&]( hub::Data_t* data, hub::Size_t size ) {
            archive.read( data, size );
        };
        auto writeZppBitsDataPtr = [&]( const hub::Data_t* data, hub::Size_t size ) {
            archive.write( data, size );
        };
        writeReadCharPtr( readZppBitsDataPtr, writeZppBitsDataPtr, data_write, 3 );
        assert( archive.isEnd() );
    }

    {
        hub::io::Archive<hub::io::InputOutputZppBits> archive;

        auto readBookImpl  = [&]( Character& character ) { archive.read( character ); };
        auto writeBookImpl = [&]( const Character& character ) { archive.write( character ); };
        writeAndReadBook( readBookImpl, writeBookImpl, character, 4 );
        assert( archive.isEnd() );

        auto readZppBitsDataPtr = [&]( hub::Data_t* data, hub::Size_t size ) {
            archive.read( data, size );
        };
        auto writeZppBitsDataPtr = [&]( const hub::Data_t* data, hub::Size_t size ) {
            archive.write( data, size );
        };
        writeReadCharPtr( readZppBitsDataPtr, writeZppBitsDataPtr, data_write, 4 );
        assert( archive.isEnd() );
    }

    delete[] data_write;

    for ( int iImpl = 0; iImpl < s_nIOImpl; ++iImpl ) {

        const auto& implName = ioImplNames[iImpl];
        std::cout << "------------------------------------" << std::endl;
        std::cout << "[" << implName << "] writing/reading rate: "
                  << std::to_string( s_megaReadWritePerSeconds[iImpl] / 1000.0 )
                  << " GigaReadWrite/s, " << std::to_string( s_gigaBytePerSeconds[iImpl] )
                  << " Go/s" << std::endl;
        std::cout << "[" << implName << "] total time: " << s_durations[iImpl] / 1000'000.0 << " ms"
                  << std::endl;

        if ( s_durationsDataPtr[iImpl] != 0 ) {
            std::cout << "[" << implName << "] writing/reading data ptr rate: "
                      << std::to_string( s_gigaBytePerSecondsDataPtr[iImpl] ) << " Go/s"
                      << std::endl;
            std::cout << "[" << implName
                      << "] total time: " << s_durationsDataPtr[iImpl] / 1000'000.0 << " ms"
                      << std::endl;
        }
    }

    return 0;
}
