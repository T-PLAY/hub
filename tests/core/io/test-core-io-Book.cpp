// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "core/io/test_core_io_common.hpp"
#include "core/test_core_common.hpp"
#include "test_common.hpp"

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
#include <set>
// #include <core/Vector.hpp>

#include "zpp_bits.h"

#include <core/Any.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
#include <core/io/Book.hpp>

using namespace zpp::bits::literals;

TEST_CASE( "Book test" ) {
    // int main() {
    using namespace testCoreIoCommon;

    //    enum class IOImpls { ZppBits = 0, BookZppBits, BookImpl, ArchiveImpl, ArchiveZppBits,
    //    Count }; static constexpr auto s_nIOImpl = (int)IOImpls::Count; static constexpr
    //    std::string_view ioImplNames[s_nIOImpl] {
    //        "ZppBits", "BookImpl", "BookZppBits", "ArchiveImpl", "ArchiveZppBits" };

    static constexpr size_t s_maxDataSize   = 1'000'000;
#ifdef HUB_DEBUG_INPUT
    static constexpr size_t s_nReadWrite = 1;
#else
    static constexpr size_t s_nReadWrite = s_maxDataSize / s_dataSize;
#endif
    static constexpr size_t s_bookSize = s_nReadWrite * s_dataSize;

    static constexpr size_t s_nReadWriteDataPtr = 1;
    static constexpr size_t s_DataSizePtr       = 1'00'000'000;

    std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;
    std::cout << "read/write size: "
              << std::to_string( s_dataSize * s_nReadWrite / 1'000'000.0 ) << " Mo"
              << std::endl;
    std::cout << "nb read/write ptr: " << s_nReadWriteDataPtr << std::endl;
    std::cout << "read/write ptr size: "
              << std::to_string( s_nReadWriteDataPtr * s_DataSizePtr / 1'000'000.0 ) << " Mo"
              << std::endl;
    std::cout << std::endl;

    UserData data( 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    //    std::vector<BenchStat> dataBenchStats;
    std::set<BenchStat> dataBenchStats;
    //    std::vector<ReadWriteStat> dataPtrBenchStats;
    {
        BenchStat benchStat { "ZppBits" };
        {
            std::array<unsigned char, s_bookSize> characters;

            auto read = [&]( UserData& data ) {
                zpp::bits::in(characters)( data ).or_throw();
            };
            auto write = [&]( const UserData& data ) {
                zpp::bits::out( characters )( data ).or_throw();
            };
            benchStat.readWriteDataStat = readWriteData( read, write, s_nReadWrite, data );
        }
        {
            static std::array<unsigned char, s_DataSizePtr + 100> characters;
            auto read = [&]( hub::Data_t* data, hub::Size_t size ) {
                std::span span( data, size );
                zpp::bits::in(characters)( span ).or_throw();
            };
            auto write = [&]( const hub::Data_t* data, hub::Size_t size ) {
                std::span span( data, size );
                zpp::bits::out( characters )( span ).or_throw();
            };
            benchStat.readWriteDataPtrStat =
                readWriteDataPtr( read, write, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        }
        //        dataBenchStats.push_back( std::move( benchStat ) );
        dataBenchStats.insert( std::move( benchStat ) );
    }

    ///////////////////////////////////////////////////

    BenchStat benchStatBookImpl { "BookImpl" };
    {
        {
            hub::io::book::BookImpl<s_bookSize> bookImpl;
            benchStatBookImpl.readWriteDataStat =
                readWriteData( bookImpl, s_nReadWrite, data );
            assert( bookImpl.isEnd() );
        }
        {
            static hub::io::book::BookImpl<s_DataSizePtr> bookImpl;
            benchStatBookImpl.readWriteDataPtrStat =
                readWriteDataPtr( bookImpl, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
            assert( bookImpl.isEnd() );
        }
        //        dataBenchStats.push_back( std::move( benchStat ) );
        dataBenchStats.insert( benchStatBookImpl );
    }

    ///////////////////////////////////////////////////

    BenchStat benchStatBookZppBits { "BookZppBits" };
    {
        {
            hub::io::book::BookZppBits<s_bookSize> book;
            benchStatBookZppBits.readWriteDataStat = readWriteData( book, s_nReadWrite, data );
            assert( book.isEnd() );
        }
        {
            //        static hub::io::book::BookZppBits<s_DataSizePtr + 100> book;
            static std::array<unsigned char, s_DataSizePtr + 100> characters;
            //        std::vector<unsigned char> characters;
            hub::io::book::BookZppBits book( characters );
            assert( book.isEnd() );
            auto read  = [&]( hub::Data_t* data, hub::Size_t size ) { book.read( data, size ); };
            auto write = [&]( hub::Data_t* data, hub::Size_t size ) { book.write( data, size ); };
            benchStatBookZppBits.readWriteDataPtrStat =
                readWriteDataPtr( read, write, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
            //            benchStat.readWriteDataPtrStat =
            //                readWriteDataPtr( book, s_nReadWriteDataPtr, data_write, s_DataSizePtr
            //                );
            assert( book.isEnd() );
        }
        //        dataBenchStats.push_back( std::move( benchStat ) );
        dataBenchStats.insert( benchStatBookZppBits );
    }

    ///////////////////////////////////////////////////

    {
        BenchStat benchStat { "ArchiveImpl" };
        hub::io::Archive<hub::serializer::SerializerImpl> archive;

        benchStat.readWriteDataStat = readWriteData( archive, s_nReadWrite, data );
        assert( archive.isEnd() );

        benchStat.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( std::move( benchStat ) );
    }

//    {
//        BenchStat benchStat { "ArchiveZppBits" };
//        hub::io::Archive<hub::io::InputOutputZppBits> archive;

//        benchStat.readWriteDataStat = readWriteData( archive, s_nReadWrite, data );
//        assert( archive.isEnd() );

//        benchStat.readWriteDataPtrStat =
//            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//        assert( archive.isEnd() );
//        dataBenchStats.insert( std::move( benchStat ) );
//    }

    delete[] data_write;

#ifndef DEBUG
    CHECK( benchStatBookZppBits < benchStatBookImpl );
#endif
    static_assert(std::is_same_v<hub::io::Book, hub::io::book::BookZppBits<>> );

    printStats( dataBenchStats );

    //    return 0;
}
