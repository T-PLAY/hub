#include "test_common.hpp"


#include <array>
#include <queue>
#include <string>
#include <thread>
//#include <mutex>

//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include <core/Input.hpp>
#include <core/Output.hpp>
#include <core/Vector.hpp>


class InputOutput : public hub::Input, public hub::Output
{
  public:
    using hub::Input::read;
    using hub::Input::operator>>;
    using hub::Output::write;
    using hub::Output::operator<<;

    InputOutput() = default;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        m_mtxDatas.lock();
        assert( !m_datas.empty() );
        auto vector = m_datas.front();
        m_datas.pop();
        assert( vector.size() == size );
        //        std::cout << HEADER_INPUT_MSG "read(Data_t*, Size_t) : data = " << vector <<
        //        std::endl;

        memcpy( data, vector.data(), size );
        m_mtxDatas.unlock();
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        m_mtxDatas.lock();
        std::vector<hub::Data_t> vector( data, data + size );
        //        std::cout << HEADER_OUTPUT_MSG "write(Data_t*, Size_t) : data = " << vector <<
        //        std::endl;
        m_datas.push( vector );
//        std::cout << "write datas size = " << m_datas.size() << std::endl;
        m_mtxDatas.unlock();
    }

    void close() override {};

    bool isOpen() const override { return true; };

    bool isEmpty() const override { return m_datas.empty(); }

    void clear() override {
        while ( !m_datas.empty() )
            m_datas.pop();
    }

    std::mutex m_mtxDatas;
    std::queue<std::vector<hub::Data_t>> m_datas;
};

//int main() {
TEST_CASE( "Any test" ) {

    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };

    InputOutput inputOutput;
    hub::Input& input   = inputOutput;
    hub::Output& output = inputOutput;

    inputOutput.write( 5 );
    inputOutput.put( 5 );
    inputOutput << 5;

    int v;
    inputOutput.read( v );
    assert( v == 5 );
    v = inputOutput.get<int>();
    assert( v == 5 );
    v = 0;
    inputOutput >> v;

    const std::vector<int> vints { 1, 2, 3, 4, 5, 6 };
    inputOutput.writeAll( vints );

    std::vector<int> vints_read;
    inputOutput.readAll( vints_read );
    assert( vints == vints_read );

    assert( inputOutput.isEmpty() );

    inputOutput.writeAll( vints );

    int a = 0;
    input >> a;
    std::cout << "a = " << a << std::endl;

    input.clear();
    assert( input.isEmpty() );

    std::cout << "----------------------------------------" << std::endl;

    bool exit = false;
    bool start = false;


    const int data = 5;


//    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto waitingStart = []() {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    };

    size_t nRead = 0;
    std::thread threadInput( [&]() {
        while (! start) {
            waitingStart();
        }
        while ( !exit ) {
            int data_read;
            input >> data_read;
            CHECK( data_read == data );
            ++nRead;
        }
    } );

    size_t nRead2 = 0;
    std::thread threadInput2( [&]() {
        while (! start) {
            waitingStart();
        }
        while ( !exit ) {
            int data_read2;
            input >> data_read2;
            CHECK( data_read2 == data );
            ++nRead2;
        }
    } );

    size_t nRead3 = 0;
    std::thread threadInput3( [&]() {
        while (! start) {
            waitingStart();
        }
        while ( !exit ) {
            int data_read3;
            input >> data_read3;
            CHECK( data_read3 == data );
            ++nRead3;
        }
    } );

    auto waitingOutput = []() {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    };

    size_t nWrite = 0;
    std::thread threadOutput( [&]() {
        while (! start) {
            waitingStart();
        }
        while ( !exit ) {
            output << data;
            ++nWrite;
            waitingOutput();
        }
    } );

    size_t nWrite2 = 0;
    std::thread threadOutput2( [&]() {
        while (! start) {
            waitingStart();
        }
        while ( !exit ) {
            output << data;
            ++nWrite2;
            waitingOutput();
        }
    } );

    const auto startClock = std::chrono::high_resolution_clock::now();
    start = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    exit = true;
    const auto endClock = std::chrono::high_resolution_clock::now();
    const auto& duration =
        std::chrono::duration_cast<std::chrono::milliseconds>( endClock - startClock ).count();

    threadOutput.join();
    threadOutput2.join();
    threadInput.join();
    threadInput2.join();
    threadInput3.join();

    const auto writePerSecond = 1000.0 * (nWrite + nWrite2) / duration;
    const auto readPerSecond = 1000.0 * (nRead + nRead2 + nRead3) / duration;

//    const auto nTotalWrite = nWrite;
//    const auto nTotalRead = nRead + nRead2;

    std::cout << "nb write: " << std::to_string(nWrite) << std::endl;
    std::cout << "nb write2: " << std::to_string(nWrite2) << std::endl;
    std::cout << "nb read: " << std::to_string(nRead) << std::endl;
    std::cout << "nb read2: " << std::to_string(nRead2) << std::endl;
    std::cout << "nb read3: " << std::to_string(nRead3) << std::endl;
    std::cout << std::to_string((int)writePerSecond) << " write/s" << std::endl;
    std::cout  << std::to_string((int)readPerSecond) << " read/s" << std::endl;

}
