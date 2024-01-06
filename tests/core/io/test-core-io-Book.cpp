// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

// #include "core/io/test_core_io_common.hpp"
// //#include "core/test_core_common.hpp"
// #include "test_common.hpp"

// // #include <iostream>
// // #include <cstdlib>
// // #include <cstring>

// // #include <array>
// // #include <string>
// // #include <thread>
// //  #include <mutex>
// // #include <atomic>
// #include <any>
// #include <array>
// #include <set>
// // #include <core/Vector.hpp>

// #ifdef HUB_USE_ZPP_BITS
// #include "zpp_bits.h"
// using namespace zpp::bits::literals;
// #endif

// #include <core/Any.hpp>
// #include <core/Traits.hpp>
// #include <core/io/Archive.hpp>
// #include <core/io/Book.hpp>
// #include <core/io/book/BookImpl.hpp>


// TEST_CASE( "Book test" ) {
    // TEST_BEGIN()

//     // int main() {
//     using namespace testCoreIoCommon;

//     //    enum class IOImpls { ZppBits = 0, BookZppBits, BookImpl, ArchiveImpl, ArchiveZppBits,
//     //    Count }; static constexpr auto s_nIOImpl = (int)IOImpls::Count; static constexpr
//     //    std::string_view ioImplNames[s_nIOImpl] {
//     //        "ZppBits", "BookImpl", "BookZppBits", "ArchiveImpl", "ArchiveZppBits" };

// #ifdef WIN32
// static constexpr size_t s_maxDataSize   = 100'000;
// #else
// static constexpr size_t s_maxDataSize   = 1'000'000;
// #endif
// #ifdef HUB_DEBUG_INPUT
//     static constexpr size_t s_nReadWrite = 1;
// #else
//     static constexpr size_t s_nReadWrite = s_maxDataSize / s_dataSize;
// #endif
//     static constexpr size_t s_bookSize = s_maxDataSize;

//     static constexpr size_t s_nReadWriteDataPtr = 1;
//     static constexpr size_t s_DataSizePtr       = 500'000'000;

//     std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;
//     std::cout << "read/write size: "
//               << std::to_string( s_dataSize * s_nReadWrite / 1'000'000.0 ) << " Mo"
//               << std::endl;
//     std::cout << "nb read/write ptr: " << s_nReadWriteDataPtr << std::endl;
//     std::cout << "read/write ptr size: "
//               << std::to_string( s_nReadWriteDataPtr * s_DataSizePtr / 1'000'000.0 ) << " Mo"
//               << std::endl;

//     UserData data{ 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } };

//     unsigned char* data_write = new unsigned char[s_DataSizePtr];
//     memset( data_write, 'a', s_DataSizePtr );

//     //    std::vector<BenchStat> dataBenchStats;
//     std::set<BenchStat> dataBenchStats;
//     //    std::vector<ReadWriteStat> dataPtrBenchStats;
// #ifdef HUB_USE_ZPP_BITS
//     {
//         BenchStat benchStat { "ZppBits" };
//         {
//             std::array<unsigned char, s_bookSize> characters;

//             auto read = [&]( UserData& data ) {
//                 zpp::bits::in in(characters);
//                 in( data ).or_throw();
//             };
//             auto write = [&]( const UserData& data ) {
//                 zpp::bits::out out( characters );
//                 out( data ).or_throw();
//             };
//             benchStat.readWriteDataStat = readWriteData( read, write, s_nReadWrite, data );
//         }
//         {
//             static std::array<unsigned char, s_DataSizePtr + 100> characters;
//             auto read = [&]( hub::Data_t* data, hub::Size_t size ) {
//                 std::span span( data, size );
//                 zpp::bits::in in(characters);
//                 in( span ).or_throw();
//             };
//             auto write = [&]( const hub::Data_t* data, hub::Size_t size ) {
//                 std::span span( data, size );
//                 zpp::bits::out out( characters );
//                 out( span ).or_throw();
//             };
//             benchStat.readWriteDataPtrStat =
//                 readWriteDataPtr( read, write, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//         }
//         //        dataBenchStats.push_back( std::move( benchStat ) );
//         dataBenchStats.insert( std::move( benchStat ) );
//     }
// #endif

//     ///////////////////////////////////////////////////

//     BenchStat benchStatBookImpl { "BookImpl" };
//     {
//         {
//             hub::io::book::BookImpl<s_bookSize> bookImpl;
//             benchStatBookImpl.readWriteDataStat =
//                 readWriteData( bookImpl, s_nReadWrite, data );
//             assert( bookImpl.isEnd() );
//         }
//         {
//             static hub::io::book::BookImpl<s_DataSizePtr> bookImpl;
//             benchStatBookImpl.readWriteDataPtrStat =
//                 readWriteDataPtr( bookImpl, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//             assert( bookImpl.isEnd() );
//         }
//         //        dataBenchStats.push_back( std::move( benchStat ) );
//         dataBenchStats.insert( benchStatBookImpl );
//     }

//     ///////////////////////////////////////////////////

// #ifdef HUB_USE_ZPP_BITS
//     BenchStat benchStatBookZppBits { "BookZppBits" };
//     {
//         {
//             hub::io::book::BookZppBits<s_bookSize> book;
//             benchStatBookZppBits.readWriteDataStat = readWriteData( book, s_nReadWrite, data );
//             assert( book.isEnd() );
//         }
//         {
//             //        static hub::io::book::BookZppBits<s_DataSizePtr + 100> book;
//             static std::array<unsigned char, s_DataSizePtr + 100> characters;
//             //        std::vector<unsigned char> characters;
//             hub::io::book::BookZppBits book( characters );
//             assert( book.isEnd() );
//             auto read  = [&]( hub::Data_t* data, hub::Size_t size ) { book.read( data, size ); };
//             auto write = [&]( hub::Data_t* data, hub::Size_t size ) { book.write( data, size ); };
//             benchStatBookZppBits.readWriteDataPtrStat =
//                 readWriteDataPtr( read, write, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//             //            benchStat.readWriteDataPtrStat =
//             //                readWriteDataPtr( book, s_nReadWriteDataPtr, data_write, s_DataSizePtr
//             //                );
//             assert( book.isEnd() );
//         }
//         //        dataBenchStats.push_back( std::move( benchStat ) );
//         dataBenchStats.insert( benchStatBookZppBits );
//     }
// #endif

//     ///////////////////////////////////////////////////

//     {
//         BenchStat benchStat { "ArchiveImpl" };
//         hub::io::ArchiveT<hub::serializer::SerializerImpl> archive;

//         benchStat.readWriteDataStat = readWriteData( archive, s_nReadWrite, data );
//         assert( archive.isEnd() );

//         benchStat.readWriteDataPtrStat =
//             readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//         assert( archive.isEnd() );
//         dataBenchStats.insert( std::move( benchStat ) );
//     }

// #ifdef HUB_USE_ZPP_BITS
//     {
//         BenchStat benchStat { "ArchiveZppBits" };
//         hub::io::ArchiveT<hub::serializer::SerializerZppBits> archive;

//         benchStat.readWriteDataStat = readWriteData( archive, s_nReadWrite, data );
//         assert( archive.isEnd() );

//         benchStat.readWriteDataPtrStat =
//             readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//         assert( archive.isEnd() );
//         dataBenchStats.insert( std::move( benchStat ) );
//     }
// #endif

//     delete[] data_write;

// //#if ! defined(DEBUG) && defined(HUB_USE_ZPP_BITS)
// //    CHECK( benchStatBookZppBits < benchStatBookImpl );
// //#endif
// // #ifdef HUB_USE_ZPP_BITS
// //     static_assert(std::is_same_v<hub::io::Book, hub::io::book::BookZppBits<>> );
// // #endif

//     printStats( dataBenchStats );

//     //    return 0;
    // TEST_END()
// }
