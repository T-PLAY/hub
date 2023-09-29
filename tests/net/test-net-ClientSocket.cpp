#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

// #include <server/Server.hpp>
#include <net/ServerSocket.hpp>

#include <utils/Utils.hpp>

#ifdef HUB_TESTS_MQTT_FOUND
#    include <mqtt/client.h>
#endif

#include <Version.h>

void dataIsOk( const unsigned char* const data, size_t len ) {
    for ( int i = 0; i < std::min( (size_t)1'000'000, len ); ++i ) {
        assert( data[i] == 65 );
    }
}

TEST_CASE( "Net test : ClientSocket" ) {
    const auto hostname = hub::utils::getHostname();

    // initing datum
    constexpr auto bigDataSize = 1'000'000'000; // 1 Go
                                                //    assert(sizeof(size_t) == 8);

    unsigned char* bigData = new unsigned char[bigDataSize * 2u];

    //    srand( (unsigned)time( NULL ) );
    memset( bigData, 65, bigDataSize * 2u );
    //    for ( int j = 0; j < bigDataSize * 2; ++j ) {
    //        bigData[j] = rand() % 256;
    //        bigData[j] = 65;
    //    }
    //    dataIsOk( bigData, bigDataSize );

    const int port = GET_RANDOM_PORT;
    hub::net::ServerSocket serverSocket( port );

    //    std::shuffle(bigData, bigData + bigDataSize, std::default_random_engine(11));
    // datum inited

    // raw socket
    {
        const std::string ipv4 = "127.0.0.1";
        //        constexpr int packetSize = MAX_NET_BUFFER_SIZE;
        //        constexpr int nPart      = bigDataSize / packetSize;
        std::cout << "[test][ClientSocket] sending big data size: " << bigDataSize / 1'000'000'000.0
                  << " Go" << std::endl;
        std::cout << "[test][ClientSocket] start streaming" << std::endl;

        hub::net::ClientSocket clientSocket( ipv4, port );
        assert( clientSocket.isOpen() );
        auto clientServerSocket = serverSocket.waitNewClient();
        assert( clientServerSocket.isOpen() );

        unsigned char* dataIn = new unsigned char[bigDataSize * 2u];
        size_t dataWrote      = 0;
        const auto& start     = std::chrono::high_resolution_clock::now();
        {
            //            for ( int i = 0; i < 10; ++i ) {
            for ( int64_t iSize = bigDataSize; iSize < bigDataSize * 2u; iSize += 1'000'000'000 ) {
                //                int64_t iSize = bigDataSize + i * 10'00;
                //                int64_t iSize = bigDataSize;
                //                const unsigned char* data = &datas[iSize * bigDataSize];
                //                int uploadSize            = 0;
                std::cout << "[test][ClientSocket] sending data size: " << iSize / 1'000'000'000.0
                          << " Go" << std::endl;

                assert( iSize > 0 );

                //                for ( int i = 0; i < nPart - 1; ++i ) {
                // #ifdef OS_MACOS
                // #endif
                //                auto* thread = new std::thread( [&]() {
                auto thread = std::thread( [&]() {
                    //                    dataIsOk( bigData, iSize );
                    try {
                        clientSocket.write( bigData, iSize );
                    }
                    catch ( std::exception& ex ) {
                        std::cout << "[test][ClientSocket] catch exception : " << ex.what()
                                  << std::endl;
                    }

                    //                    dataIsOk( bigData, iSize );
                } );
                //                thread.detach();
                // std::cout << "[test][ClientSocket] part sended " << i << std::endl;
                //                dataIsOk( bigData, iSize );

                // #ifdef OS_MACOS
                //         std::cout << "[test][ClientSocket] reading part " << i << std::endl;
                // #endif
                try {
                    clientServerSocket.read( dataIn, iSize );
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test][ClientSocket] catch exception : " << ex.what()
                              << std::endl;
                }

                //                dataIsOk( bigData, iSize );

                //                if ( thread.joinable() )
                //                assert( thread->joinable() );
                //                thread->join();
                //                delete thread;

                assert( thread.joinable() );
                thread.join();

                // std::cout << "[test][ClientSocket] part readed " << i << std::endl;

                //                    uploadSize += packetSize;
                //                }

                //                clientSocket.write( bigData + uploadSize, bigDataSize -
                //                uploadSize
                //                ); clientServerSocket.read( dataIn + uploadSize, bigDataSize -
                //                uploadSize );

#ifdef DEBUG
                //                dataIsOk( bigData, bigDataSize );
                assert( memcmp( bigData, dataIn, iSize ) == 0 );
#endif
                //                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                //                                break;
                //                break;
                //                CHECK( memcmp( data, dataIn, bigDataSize ) == 0 );
                dataWrote += iSize;
            }
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        delete[] dataIn;

        //        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        //                const auto& bytes               = bigDataSize;
        const auto bytesPerSecond     = 1000.0 * dataWrote / duration;
        const auto gigaBytesPerSecond = bytesPerSecond / 1'000'000'000.0;

        REPORT( "[test][ClientSocket] Giga byte wrote : " << dataWrote / 1'000'000'000.0 << "Go" );
        //                  << std::endl;
        REPORT( "[test][ClientSocket] Giga byte per second : " << gigaBytesPerSecond << " Go/s" );
    }

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] bigData;
}
