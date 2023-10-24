#include "test_common.hpp"
#include "test-core-common.hpp"

#include <array>
#include <string>
#include <thread>
// #include <mutex>
#include <atomic>

// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT



// int main() {
TEST_CASE( "InputOutput test" ) {

    constexpr std::array<unsigned char, 6> data1 { 1, 2, 3, 4, 5, 6 };
    constexpr std::array<unsigned char, 3> data2 { 1, 2, 3 };

    InputOutput inputOutput;
    hub::Input& input   = inputOutput;
    hub::Output& output = inputOutput;

    inputOutput.write( 5 );
//    inputOutput.put( 5 );
    inputOutput << 5;

    int v;
    inputOutput.read( v );
    assert( v == 5 );
//    v = inputOutput.get<int>();
//    assert( v == 5 );
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

    bool exit  = false;
    std::atomic<bool> start;
//    bool start;
    start = false;

    const int data = 5;

    constexpr auto nData = 100'000;

    //    std::mutex mtxInput;

    //    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    //    auto waitingStart = []() { std::this_thread::sleep_for( std::chrono::microseconds( 1 ) );
    //    };
    auto waitingStart = []() {};
    //    auto waitingRead  = []() { std::this_thread::sleep_for( std::chrono::microseconds( 1 ) );
    //    };

    size_t nRead = 0;
    std::thread threadInput( [&]() {
        while ( !start ) {
            waitingStart();
//            std::cout << "\t>> waiting to start read" << std::endl;
        }
        std::cout << "\t>> start reading" << std::endl;

        //        while ( !exit ) {
        int data_read;
        //            while ( input.isEmpty() ) {
        //                waitingRead();
        //            }
        //            mtxInput.lock();
        for ( int i = 0; i < nData; ++i ) {
            while ( input.isEmpty() ) {
                std::cout << "\t>> waiting for data " << std::endl;
                std::this_thread::sleep_for( std::chrono::microseconds( 1 ) );
            };
//            std::cout << "\t>> reading data " << nRead << std::endl;
            input >> data_read;
            CHECK( data_read == data );
            ++nRead;
        }
        std::cout << "\t>> end reading" << std::endl;
        //            mtxInput.unlock();
        //        }
    } );

#ifdef MULTITHREAD
    size_t nRead2 = 0;
    std::thread threadInput2( [&]() {
        while ( !start ) {
            waitingStart();
        }
        while ( !exit ) {
            int data_read2;
            //            while ( input.isEmpty() ) {
            //                waitingRead();
            //            }
            //            mtxInput.lock();
            //            if ( !input.isEmpty() ) {
            input >> data_read2;
            CHECK( data_read2 == data );
            ++nRead2;
            //            }
            //            mtxInput.unlock();
        }
    } );

    size_t nRead3 = 0;
    std::thread threadInput3( [&]() {
        while ( !start ) {
            waitingStart();
        }
        while ( !exit ) {
            int data_read3;
            //            while ( input.isEmpty() ) {
            //                waitingRead();
            //            }
            //            mtxInput.lock();
            //            if ( !input.isEmpty() ) {
            input >> data_read3;
            CHECK( data_read3 == data );
            ++nRead3;
            //            }
            //            mtxInput.unlock();
        }
    } );
#endif

    //    auto waitingOutput = []() { std::this_thread::sleep_for( std::chrono::microseconds( 100000
    //    ) ); };
    auto waitingOutput = []() {};

    size_t nWrite = 0;
    std::thread threadOutput( [&]() {
        while ( !start ) {
            waitingStart();
//            std::cout << "<< waiting to start write" << std::endl;
        }
        std::cout << "<< start writing" << std::endl;

        for ( int i = 0; i < nData; ++i ) {
//            std::cout << "<< writing data " << nWrite << std::endl;
            output << data;
            ++nWrite;
            waitingOutput();
        }
        std::cout << "<< end writing" << std::endl;
    } );

#ifdef MULTITHREAD
    size_t nWrite2 = 0;
    std::thread threadOutput2( [&]() {
        while ( !start ) {
            waitingStart();
        }
        while ( !exit ) {
            output << data;
            ++nWrite2;
            waitingOutput();
        }
    } );
#endif

    const auto startClock = std::chrono::high_resolution_clock::now();
    std::cout << "----------------- start -----------------------" << std::endl;
    start = true;
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
    //    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //    exit                = true;

    threadOutput.join();
    const auto endOutputClock = std::chrono::high_resolution_clock::now();
    threadInput.join();
    const auto endInputClock = std::chrono::high_resolution_clock::now();

    std::cout << "---------------------- end ----------------------------" << std::endl;
//    const auto endClock = std::chrono::high_resolution_clock::now();
    const auto& durationOutput =
        std::chrono::duration_cast<std::chrono::microseconds>( endOutputClock - startClock ).count();
    const auto& durationInput =
        std::chrono::duration_cast<std::chrono::microseconds>( endInputClock - startClock ).count();

#ifdef MULTITHREAD
    threadOutput2.join();
    threadInput2.join();
    threadInput3.join();
#endif

#ifdef MULTITHREAD
    const auto writePerSecond = 1000.0 * ( nWrite + nWrite2 ) / duration;
    const auto readPerSecond  = 1000.0 * ( nRead + nRead2 + nRead3 ) / duration;
#else
    const auto megaWritePerSecond = (double)nWrite / durationOutput;
    const auto megaReadPerSecond  = (double)nRead / durationInput;
#endif
    //    const auto readPerSecond  = 1000.0 * ( nRead ) / duration;

    //    const auto nTotalWrite = nWrite;
    //    const auto nTotalRead = nRead + nRead2;

    std::cout << "nb write: " << std::to_string( nWrite ) << std::endl;
    std::cout << "nb read: " << std::to_string( nRead ) << std::endl;
#ifdef MULTITHREAD
    std::cout << "nb write2: " << std::to_string( nWrite2 ) << std::endl;
    std::cout << "nb read2: " << std::to_string( nRead2 ) << std::endl;
    std::cout << "nb read3: " << std::to_string( nRead3 ) << std::endl;
#endif
    std::cout << "writing rate: " << std::to_string( megaWritePerSecond ) << " MegaWrite/s" << std::endl;
    std::cout << "reading rate: " << std::to_string( megaReadPerSecond ) << " MegaRead/s" << std::endl;
    const auto latency = (durationInput - durationOutput) / 1000.0;
    std::cout << "latency: " << latency << " ms" << std::endl;

    std::cout << "----------------------------------------" << std::endl;

    CHECK_VALUE( megaWritePerSecond, 5, 5, "Output: MegaWritePerSecond" );
    CHECK_VALUE( megaReadPerSecond, 5, 5, "Input: MegaReadPerSecond" );
    CHECK_VALUE( latency, 150, 150, "Latency: MilliSeconds" );
}
