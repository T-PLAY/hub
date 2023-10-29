
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

class Random {
  public:
    int a;
    bool b;
    std::vector<int> c;

    unsigned char d[6]; // zppBits unable to visit

//    std::any any;
    bool operator==(const Random & random) const {
        return a == random.a && b == random.b && c == random.c
               && std::memcmp(d, random.d, 5) == 0;
    }

//    template <class Input>
//    void read(Input & input) {
////        input.readAll(a, b);
////        input.read(c);
//        input(a, b, d);
//        input(c);
//    }

//    template <class Output>
//    void write(Output & output) const {
//        output(a, b, d);
//        output(c);
////        output.writeAll(a, b);
////        output.write(c);
//    }

    template <class Serial>
    void serialize(Serial & serial) {
        serial(a, b, d);
        serial(c);
    }

//    template <class Serial>
//    void serialize(Serial & serial) const {
//        serial(a, b, c);
//    }

    friend std::ostream & operator<<(std::ostream & os, const Random & random) {
        os << random.a << " " << random.b << " " << random.c
           << " " << random.d;
        return os;
    }
};

//auto serialize(const Random &) -> zpp::bits::pb_protocol;

TEST_CASE( "InputOutput test" ) {

//    static_assert(hub::io::serializable_v<Random>);
//    static_assert(hub::io::serializable_v<const Random>);
//    static_assert(hub::io::serializable_v<const Random &>);
//    static_assert(hub::io::serializable_v<Random &&>);

//    static_assert(hub::io::input::readable_v<Random>);
//    static_assert(hub::io::input::readable_v<const Random>);
//    static_assert(hub::io::input::readable_v<const Random &>);
//    static_assert(hub::io::input::readable_v<Random &&>);

//    static_assert(hub::io::output::writable_v<Random >);
//    static_assert(hub::io::output::writable_v<const Random>);
//    static_assert(hub::io::output::writable_v<const Random &>);
//    static_assert(hub::io::output::writable_v<Random &&>);

//    static_assert(hub::io::input::InputZppBits::serializable_v<Random>);
//    static_assert(hub::io::output::OutputZppBits::serializable_v<Random>);
//    static_assert(hub::io::input::readable_v<Random>);
//    static_assert(hub::io::output::writable_v<Random>);

//    hub::Any any;
    Random random{5, true, {1, 2, 3}
                    , "abcde"};

//    hub::io::Book book;
    hub::io::Archive archive;
    archive.write(random);

//    hub::Any any_read;
    Random random_read;
    archive.read(random_read);
    assert(random == random_read);
    assert(archive.isEnd());
    std::cout << std::endl;

    //////////////////////////////////

    hub::Any any;
    archive.write(any);
    hub::Any any_read;
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;

    any = 1;
    archive.write(any);
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;

    any = random;
    archive.write(any);
    archive.read(any_read);
    assert(any == any_read);
    assert(archive.isEnd());
    std::cout << std::endl;


    return;

    /////////////////////////////////////////////////////////////


    using namespace testCoreIoCommon;

#ifdef DEBUG
    static constexpr size_t s_nReadWrite = 10'000;
#else
    static constexpr size_t s_nReadWrite = 1'000'000;
#endif

#ifdef DEBUG
    static constexpr size_t s_nReadWriteDataPtr = 5;
    static constexpr size_t s_DataSizePtr       = 10'000'000;
#else
    static constexpr size_t s_nReadWriteDataPtr = 1;
    static constexpr size_t s_DataSizePtr       = 1'000'000'000;
#endif

    std::cout << "nb read/write: " << s_nReadWrite / 1000000.0 << " M" << std::endl;
    std::cout << "read/write size: "
              << s_dataSize * s_nReadWrite / 1'000'000.0 << " Mo"
              << std::endl;
    std::cout << "nb read/write ptr: " << s_nReadWriteDataPtr  << std::endl;
    std::cout << "read/write ptr size: "
              << s_nReadWriteDataPtr * s_DataSizePtr / 1'000'000.0 << " Mo"
              << std::endl;
    std::cout << std::endl;

    Data data( 'a', 5, "gauthier", { 0, 1, 2, 3, 4 } );

    unsigned char* data_write = new unsigned char[s_DataSizePtr];
    memset( data_write, 'a', s_DataSizePtr );

    std::set<BenchStat> dataBenchStats;

//    BenchStat benchStatInputOutput { "InputOutput" };
//    {
//        hub::io::Archive archive;

//        benchStatInputOutput.readWriteDataStat =
//            readWriteData( archive, s_nReadWrite, data );
//        assert( archive.isEnd() );

//        benchStatInputOutput.readWriteDataPtrStat =
//            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
//        assert( archive.isEnd() );
//        dataBenchStats.insert( benchStatInputOutput );
//    }

    BenchStat benchStatInputOutputImpl { "InputOutputImpl" };
    {
        hub::io::Archive<hub::io::InputOutputImpl> archive;

        benchStatInputOutputImpl.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, data );
        assert( archive.isEnd() );

        benchStatInputOutputImpl.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputImpl );
    }

    BenchStat benchStatInputOutputZppBits { "InputOutputZppBits" };
    {
        hub::io::Archive<hub::io::InputOutputZppBits> archive;

        benchStatInputOutputZppBits.readWriteDataStat =
            readWriteData( archive, s_nReadWrite, data );
        assert( archive.isEnd() );

        benchStatInputOutputZppBits.readWriteDataPtrStat =
            readWriteDataPtr( archive, s_nReadWriteDataPtr, data_write, s_DataSizePtr );
        assert( archive.isEnd() );
        dataBenchStats.insert( benchStatInputOutputZppBits );
    }

    delete[] data_write;

    CHECK( benchStatInputOutputZppBits < benchStatInputOutputImpl );
    static_assert(std::is_same_v<hub::io::Archive<>, hub::io::Archive<hub::io::InputOutput>> );
    static_assert(std::is_same_v<hub::io::InputOutput, hub::io::InputOutputZppBits> );
    static_assert(std::is_same_v<hub::Input, hub::io::input::InputZppBits> );
    static_assert(std::is_same_v<hub::Output, hub::io::output::OutputZppBits> );

//    auto inputOutputRank = std::distance(dataBenchStats.begin(), std::find(dataBenchStats.begin(), dataBenchStats.end(), benchStatInputOutput));
//    const auto inputOutputRank = std::distance(dataBenchStats.begin(), dataBenchStats.find(benchStatInputOutput));
//    CHECK(inputOutputRank < 2);

    printStats( dataBenchStats );
}
