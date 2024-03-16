#include "test_common.hpp"

#include <algorithm>
#include <array>

#include <core/Utils.hpp>
#include <net/ServerSocket.hpp>

#ifdef HUB_USE_BOOST
#    include <boost/asio.hpp>
#    include <boost/asio/ip/tcp.hpp>
#endif

TEST_CASE( "Net test : ClientSocket speed test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;
    const auto port2    = port + 1;

    constexpr auto bigDataSize = 500'000'000; // 500 Mo
    std::cout << "[test] data size: " << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;

    unsigned char* bigData = new unsigned char[bigDataSize];

    memset( bigData, 65, bigDataSize );

    ///////////////////////////////////////////////////////////////////

    constexpr auto nWrite = 2;

#ifdef HUB_USE_BOOST
    double boostBytesPerSecond;
    double boostGigaBytesPerSecond;
    // Boost::asio::ip::tcp
    {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::acceptor acceptor_(
            io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) );

        boost::asio::io_service io_service2;
        boost::asio::ip::tcp::socket clientSocket( io_service2 );
        clientSocket.connect( boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string( "127.0.0.1" ), port ) );

        boost::asio::ip::tcp::socket clientServerSocket( io_service );
        acceptor_.accept( clientServerSocket );

        unsigned char* dataIn = new unsigned char[bigDataSize];
        memset( dataIn, 0, bigDataSize );

        size_t dataWrote  = 0;
        const auto& start = std::chrono::high_resolution_clock::now();
        {
            auto thread = std::thread( [&]() {
                try {
                    for ( int i = 0; i < nWrite; ++i ) {
                        const auto& buffer = boost::asio::buffer( bigData, bigDataSize );
                        auto uploadBytes   = boost::asio::write( clientSocket, buffer );
#    ifdef DEBUG
                        CHECK( uploadBytes == bigDataSize );
#    endif
                        dataWrote += bigDataSize;
                    }
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test][ClientSocket] catch exception : " << ex.what()
                              << std::endl;
                }
            } );

            try {
                for ( int i = 0; i < nWrite; ++i ) {
                    const auto& buffer = boost::asio::buffer( dataIn, bigDataSize );
                    auto downloadBytes = boost::asio::read( clientServerSocket, buffer );
#    ifdef DEBUG
                    CHECK( downloadBytes == bigDataSize );
#    endif
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test][Boost::asio::ip::tcp] catch exception : " << ex.what()
                          << std::endl;
            }

            CHECK( thread.joinable() );
            thread.join();
        }
        const auto& end = std::chrono::high_resolution_clock::now();

        CHECK( memcmp( bigData, dataIn, bigDataSize ) == 0 );
        delete[] dataIn;

        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        boostBytesPerSecond     = 1000.0 * dataWrote / duration;
        boostGigaBytesPerSecond = boostBytesPerSecond / 1'000'000'000.0;

        std::cout << "[test][Boost::asio::ip::tcp] Giga byte wrote : "
                  << dataWrote / 1'000'000'000.0 << "Go" << std::endl;
        std::cout << "[test][Boost::asio::ip::tcp] Giga byte per second : "
                  << boostGigaBytesPerSecond << " Go/s" << std::endl;
    }
#endif

    double sysBytesPerSecond;
    double sysGigaBytesPerSecond;
    // ClientSocket
    {
        const std::string ipv4 = "127.0.0.1";

        hub::net::ServerSocket serverSocket( port2 );

        hub::net::ClientSocket clientSocket( ipv4, port2 );

        CHECK( clientSocket.isConnected() );
        auto clientServerSocket = serverSocket.waitNewClient();
        CHECK( clientServerSocket.isConnected() );

        unsigned char* dataIn = new unsigned char[bigDataSize];
        memset( dataIn, 0, bigDataSize );

        size_t dataWrote  = 0;
        const auto& start = std::chrono::high_resolution_clock::now();
        {
            auto thread = std::thread( [&]() {
                try {
                    for ( int i = 0; i < nWrite; ++i ) {
                        clientSocket.write( bigData, bigDataSize );
                        dataWrote += bigDataSize;
                    }
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test][ClientSocket] catch exception : " << ex.what()
                              << std::endl;
                }
            } );

            try {
                for ( int i = 0; i < nWrite; ++i ) {
                    clientServerSocket.read( dataIn, bigDataSize );
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test][ClientSocket] catch exception : " << ex.what() << std::endl;
            }

            CHECK( thread.joinable() );
            thread.join();
        }
        const auto& end = std::chrono::high_resolution_clock::now();

        CHECK( memcmp( bigData, dataIn, bigDataSize ) == 0 );
        delete[] dataIn;

        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        sysBytesPerSecond     = 1000.0 * dataWrote / duration;
        sysGigaBytesPerSecond = sysBytesPerSecond / 1'000'000'000.0;

        std::cout << "[test][ClientSocket] Giga byte wrote : " << dataWrote / 1'000'000'000.0
                  << "Go" << std::endl;
        std::cout << "[test][ClientSocket] Giga byte per second : " << sysGigaBytesPerSecond
                  << " Go/s" << std::endl;
    }

#ifdef HUB_USE_BOOST
    const double ratioSysBoost = sysBytesPerSecond / boostBytesPerSecond;
    std::cout << "[test] sys socket impl is " << ratioSysBoost
              << " more efficient than boost::asio::ip::tcp" << std::endl;

    CHECK_DECLINE( ratioSysBoost, "SysSocketImpl/BoostAsioIpTcp", "/" );
#endif

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] bigData;

    TEST_END()
}
