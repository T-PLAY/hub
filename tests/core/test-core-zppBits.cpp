#include "test-core-common.hpp"
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
#include <core/Any.hpp>
#include <core/Traits.hpp>

// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "zpp_bits.h"

struct Data {
    //    std::string_view name;
    //    std::string name;
    int age;
    double size;

        static struct {} packable;

    //    bool operator==( const Data& data ) const { return name == data.name && age == data.age; }
    bool operator==( const Data& data ) const { return age == data.age; }
    friend std::ostream& operator<<( std::ostream& os, const Data& data );
};
static_assert( sizeof( int ) == 4 );
static_assert( sizeof( double ) == 8 );
static_assert( sizeof( Data ) == 16 );

std::ostream& operator<<( std::ostream& os, const Data& data ) {
    os << data.age;
    return os;
}

using namespace zpp::bits::literals;

template <size_t Size>
class InputOutputBuff : public hub::Input, public hub::Output
{
  public:
//    using hub::Input::read;
//    using hub::Input::operator>>;
//    using hub::Output::write;
//    using hub::Output::operator<<;

    InputOutputBuff()
        :
        m_array { 0 }, m_datas( m_array.data() ) {}


    void read( hub::Data_t* data, hub::Size_t size ) override {
        memcpy( data, m_datas + m_iRead, size );
        m_iRead += size;
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        memcpy( m_datas + m_iWrite, data, size );
        m_iWrite += size;
    }

    void close() override {};

    bool isOpen() const override { return true; }

    bool isEmpty() const override {
        return m_iRead == m_iWrite;
    }

    void clear() override {
    }

  private:
    //    mutable std::mutex m_mtxDatas;
    size_t m_size;
    std::array<unsigned char, Size> m_array;
    unsigned char* m_datas = nullptr;
    size_t m_iRead         = 0;
    size_t m_iWrite        = 0;
    //    std::queue<std::vector<hub::Data_t>> m_datas;
};

//namespace std {
////static constexpr auto serialize(auto & archive, std::any & self)
////{
////    using archive_type = std::remove_cvref_t<decltype(archive)>;

////    if constexpr (archive_type::kind() == zpp::bits::kind::in) {
////        // Input archive
////        return archive();
////    } else if constexpr (archive_type::kind() == zpp::bits::kind::out) {
////        // Output archive
////        return archive();
////    } else {
////        // No such archive (no need to check for this)
////    }
////}
//constexpr auto serialize(auto & archive, std::any & any)
//{
//    return archive();
//}

//constexpr auto serialize(auto & archive, const std::any & any)
//{
//    return archive();
//}
//}

//auto serialize(const std::any & any) -> zpp::bits::members<2>;

TEST_CASE( "InputOutput test" ) {
    static_assert(hub::isPackable<Data>());

    std::vector<std::byte> dataV;
    auto [in, out] = zpp::bits::in_out(dataV);
    int a;
    out(5).or_throw();
    in(a).or_throw();
    assert(a == 5);

//    std::cout << in.allocation_limit << " " << in.endian_aware << " " << in.resizable << std::endl;
//    std::cout << "in: " << in.data().size() << " " << in.processed_data().size() << " " << in.remaining_data().size() << " " << in.position() << std::endl;
//    std::cout << "out: " << out.data().size() << " " << out.processed_data().size() << " " << out.remaining_data().size() << " " << out.position() << std::endl;
//    std::any any = 1;

//    std::cout << "<< any: " << any.type().name() << std::endl;

////    auto serialize(std::any) -> zpp::bits::members<1>;
////    hub::Any any;
//    std::cout << "in: " << in.data().size() << " " << in.processed_data().size() << " " << in.remaining_data().size() << " " << in.position() << std::endl;
//    std::cout << "out: " << out.data().size() << " " << out.processed_data().size() << " " << out.remaining_data().size() << " " << out.position() << std::endl;
//    std::any any_read;
////    hub::Any any_read;
//    std::cout << "<< any_read: " << any_read.type().name() << std::endl;
//    assert(any == any_read);
//    std::cout << "in: " << in.data().size() << " " << in.processed_data().size() << " " << in.remaining_data().size() << " " << in.position() << std::endl;
//    std::cout << "out: " << out.data().size() << " " << out.processed_data().size() << " " << out.remaining_data().size() << " " << out.position() << std::endl;
//    std::cout << in << std::endl;

    return;

    constexpr size_t nReadWrite = 100'000;
    const auto data             = Data { 30, 2.0 };
    constexpr size_t datasSize  = nReadWrite * sizeof( Data );
    constexpr int nIteration    = 10;
    std::cout << "nb read/write: " << std::to_string( nReadWrite * nIteration / 1'000'000.0 )
              << " G" << std::endl;
    std::cout << "read/write size: " << std::to_string( datasSize * nIteration / 1'000'000.0 )
              << " Go" << std::endl;
    std::cout << std::endl;

    double megaWritePerSecondZppBits;
    double megaWriteGigabytePerSecondZppBits;
    double megaReadPerSecondZppBits;
    double megaReadGigabytePerSecondZppBits;

    long long durationOutputZppBits = 0;
    long long durationInputZppBits  = 0;
    long long durationZppBits       = 0;

    {

        for ( int i = 0; i < nIteration; ++i ) {
            std::array<unsigned char, datasSize> datas;

            zpp::bits::in input( datas );
            zpp::bits::out output( datas );

            auto startClock = std::chrono::high_resolution_clock::now();

            size_t nWrite         = 0;
            auto startOutputClock = std::chrono::high_resolution_clock::now();
            while ( nWrite < nReadWrite ) {
                try {
                    output( data ).or_throw();
                }
                catch ( const std::exception& ex ) {
                    std::cout << "[zppBits] catch exception : " << ex.what() << std::endl;
                    continue;
                }
                ++nWrite;
            }
            auto endOutputClock = std::chrono::high_resolution_clock::now();

            size_t nRead = 0;
            Data data_read;
            auto startInputClock = std::chrono::high_resolution_clock::now();
            while ( nRead < nReadWrite ) {
                try {
                    input( data_read ).or_throw();
                }
                catch ( const std::exception& ex ) {
                    std::cout << "[zppBits] catch exception : " << ex.what() << std::endl;
                    continue;
                }

                if ( data_read != data ) {
                    std::cerr << "[zppBits] " << data_read << " != " << data << std::endl;
                }

                ++nRead;
            }
            auto endInputClock = std::chrono::high_resolution_clock::now();
            auto endClock      = std::chrono::high_resolution_clock::now();

            assert( nWrite == nReadWrite );
            assert( nRead == nReadWrite );

            durationOutputZppBits += std::chrono::duration_cast<std::chrono::microseconds>(
                                         endOutputClock - startOutputClock )
                                         .count();
            durationInputZppBits += std::chrono::duration_cast<std::chrono::microseconds>(
                                        endInputClock - startInputClock )
                                        .count();
            durationZppBits +=
                std::chrono::duration_cast<std::chrono::microseconds>( endClock - startClock )
                    .count();
        }

        megaWritePerSecondZppBits         = (double)nReadWrite * nIteration / durationOutputZppBits;
        megaWriteGigabytePerSecondZppBits = megaWritePerSecondZppBits * sizeof( Data ) / 1000.0;
        megaReadPerSecondZppBits          = (double)nReadWrite * nIteration / durationInputZppBits;
        megaReadGigabytePerSecondZppBits  = megaReadPerSecondZppBits * sizeof( Data ) / 1000.0;

        std::cout << "[zppBits] writing rate: " << std::to_string( megaWritePerSecondZppBits )
                  << " MegaWrite/s, " << std::to_string( megaWriteGigabytePerSecondZppBits )
                  << " Go/s" << std::endl;
        std::cout << "[zppBits] reading rate: " << std::to_string( megaReadPerSecondZppBits )
                  << " MegaRead/s, " << std::to_string( megaReadGigabytePerSecondZppBits )
                  << " Go/s" << std::endl;
        //        latencyZppBits = ( durationInputZppBits - durationOutputZppBits ) / 1000.0;
        std::cout << "[zppBits] write time: " << durationOutputZppBits / 1000.0 << " ms"
                  << std::endl;
        std::cout << "[zppBits] read time: " << durationInputZppBits / 1000.0 << " ms" << std::endl;
        std::cout << "[zppBits] total time: " << durationZppBits / 1000.0 << " ms" << std::endl;
    }

    std::cout << "#################################################################################"
                 "###################"
              << std::endl;

    {
        long long durationOutput = 0;
        long long durationInput  = 0;
        long long duration       = 0;

        for ( int i = 0; i < nIteration; ++i ) {

            InputOutputBuff<datasSize> inputOutput;
            hub::Input& input   = inputOutput;
            hub::Output& output = inputOutput;

            auto startClock = std::chrono::high_resolution_clock::now();

            size_t nWrite         = 0;
            auto startOutputClock = std::chrono::high_resolution_clock::now();
            while ( nWrite < nReadWrite ) {
                output.write( data );
                ++nWrite;
            }
            auto endOutputClock = std::chrono::high_resolution_clock::now();

            size_t nRead = 0;
            Data data_read;
            auto startInputClock = std::chrono::high_resolution_clock::now();
            while ( nRead < nReadWrite ) {
                input.read( data_read );
                if ( data_read != data ) {
                    std::cerr << data_read << " != " << data << std::endl;
                    continue;
                }
                ++nRead;
            }
            auto endInputClock = std::chrono::high_resolution_clock::now();
            auto endClock      = std::chrono::high_resolution_clock::now();

            assert( nWrite == nReadWrite );
            assert( nRead == nReadWrite );

            durationOutput += std::chrono::duration_cast<std::chrono::microseconds>(
                                  endOutputClock - startOutputClock )
                                  .count();
            durationInput += std::chrono::duration_cast<std::chrono::microseconds>(
                                 endInputClock - startInputClock )
                                 .count();
            duration +=
                std::chrono::duration_cast<std::chrono::microseconds>( endClock - startClock )
                    .count();
        }

        const auto megaWritePerSecond         = (double)nReadWrite * nIteration / durationOutput;
        const auto megaWriteGigabytePerSecond = megaWritePerSecond * sizeof( Data ) / 1000.0;

        const auto megaReadPerSecond         = (double)nReadWrite * nIteration / durationInput;
        const auto megaReadGigabytePerSecond = megaReadPerSecond * sizeof( Data ) / 1000.0;

        std::cout << "writing rate: " << std::to_string( megaWritePerSecond ) << " MegaWrite/s, "
                  << std::to_string( megaWriteGigabytePerSecond ) << " Go/s ("
                  << 100 * megaWritePerSecond / megaWritePerSecondZppBits << "%)" << std::endl;
        std::cout << "reading rate: " << std::to_string( megaReadPerSecond ) << " MegaRead/s, "
                  << std::to_string( megaReadGigabytePerSecond ) << " Go/s ("
                  << 100 * megaReadPerSecond / megaReadPerSecondZppBits << "%)" << std::endl;
        std::cout << "write time: " << durationOutput / 1000.0 << " ms ("
                  << 100 * (double)durationOutputZppBits / durationOutput << "%)" << std::endl;
        std::cout << "read time: " << durationInput / 1000.0 << " ms ("
                  << 100 * (double)durationInputZppBits / durationInput << "%)" << std::endl;
        std::cout << "total time: " << duration / 1000.0 << " ms ("
                  << 100 * (double)durationZppBits / duration << "%)" << std::endl;
    }
}
