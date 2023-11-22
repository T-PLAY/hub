#include "test_common.hpp"

#include <algorithm>
#include <array>

// #define HUB_DEBUG_SOCKET

#include <core/Utils.hpp>
#include <net/ServerSocket.hpp>

#ifdef HUB_USE_BOOST
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#endif

// std::string read_( boost::asio::ip::tcp::socket& socket ) {
//     boost::asio::streambuf buf;
//     boost::asio::read_until( socket, buf, "\n" );
//     std::string data = boost::asio::buffer_cast<const char*>( buf.data() );
//     return data;
// }
// void send_( boost::asio::ip::tcp::socket& socket, const std::string& message ) {
//     const std::string msg = message + "\n";
//     boost::asio::write( socket, boost::asio::buffer( message ) );
// }

TEST_CASE( "Net test : ClientSocket" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;
    const auto port2    = port + 1;

    // initing datum
    constexpr auto bigDataSize = 2'000'000'000; // 1 Go
                                                //    constexpr auto bigDataSize = 1000; // 1 Go
                                                //    assert(sizeof(size_t) == 8);
    std::cout << "[test] data size: " << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
    //    std::cout << "[test] start streaming" << std::endl;

    //    unsigned char* bigData = new unsigned char[bigDataSize * 2u];
    unsigned char* bigData = new unsigned char[bigDataSize];
    //    std::array<unsigned char, bigDataSize> bigData;

    //    srand( (unsigned)time( NULL ) );
    memset( bigData, 65, bigDataSize );
    //        memset( bigData, 65, bigDataSize * 2u );
    //        for ( int i = 0; i < bigDataSize; ++i ) {
    //            bigData[i] = rand() % 256;
    //            bigData[j] = 65;
    //        }
    //    dataIsOk( bigData, bigDataSize );

    ///////////////////////////////////////////////////////////////////

    //    const int port = GET_RANDOM_PORT;

    constexpr auto nWrite = 2;

    //    std::shuffle(bigData, bigData + bigDataSize, std::default_random_engine(11));
    // datum inited

#ifdef HUB_USE_BOOST
    double boostBytesPerSecond;
    double boostGigaBytesPerSecond;
    // Boost::asio::ip::tcp
    {
        // server side
        //        std::thread thread( []() {
        boost::asio::io_service io_service;
        // listen for new connection
        boost::asio::ip::tcp::acceptor acceptor_(
            io_service, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port ) );
        // socket creation
        // read operation
        //            std::string message = read_( readSocket );
        //            std::cout << message << std::endl;
        // write operation
        //            send_( readSocket, "Hello From Server!" );
        //            std::cout << "Servent sent Hello message to Client!" << std::endl;
        //        } );

        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        // client side
        //        {
        boost::asio::io_service io_service2;
        // socket creation
        boost::asio::ip::tcp::socket clientSocket( io_service2 );
        // connection
        clientSocket.connect( boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string( "127.0.0.1" ), port ) );
        // request/message from client

        boost::asio::ip::tcp::socket clientServerSocket( io_service );
        // waiting for connection
        acceptor_.accept( clientServerSocket );

        //            const std::string msg = "Hello from Client!\n";
        //            boost::system::error_code error;
        //            boost::asio::write( clientSocket, boost::asio::buffer( msg ), error );
        //            if ( !error ) { std::cout << "Client sent hello message!" << std::endl; }
        //            else { std::cout << "send failed: " << error.message() << std::endl; }
        // getting response from server
        //            boost::asio::streambuf receive_buffer;
        //            boost::asio::read( clientSocket, receive_buffer, boost::asio::transfer_all(),
        //            error ); if ( error && error != boost::asio::error::eof ) {
        //                std::cout << "receive failed: " << error.message() << std::endl;
        //            }
        //            else {
        //                const char* data = boost::asio::buffer_cast<const char*>(
        //                receive_buffer.data() ); std::cout << data << std::endl;
        //            }
        //        }
        //        thread.join();
        unsigned char* dataIn = new unsigned char[bigDataSize];
        memset( dataIn, 0, bigDataSize );

        size_t dataWrote  = 0;
        const auto& start = std::chrono::high_resolution_clock::now();
        {
            auto thread = std::thread( [&]() {
                try {
                    for ( int i = 0; i < nWrite; ++i ) {
//                        std::cout << "[test][Boost::asio::ip::tcp] sending data size: "
//                                  << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
                        //                        clientSocket.write( bigData, bigDataSize );
                        const auto& buffer = boost::asio::buffer( bigData, bigDataSize );
                        auto uploadBytes   = boost::asio::write( clientSocket, buffer );
#ifdef DEBUG
                        assert( uploadBytes == bigDataSize );
#endif
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
#ifdef DEBUG
                    assert( downloadBytes == bigDataSize );
#endif
//                    std::cout << "[test][Boost::asio::ip::tcp] reading data size: "
//                              << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
                    //                    clientServerSocket.read( dataIn, bigDataSize );
                    //                    dataIn[100] = 5;
                    // #ifdef DEBUG
                    // #endif
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test][Boost::asio::ip::tcp] catch exception : " << ex.what()
                          << std::endl;
            }

            assert( thread.joinable() );
            thread.join();
        }
        const auto& end = std::chrono::high_resolution_clock::now();

        CHECK( memcmp( bigData, dataIn, bigDataSize ) == 0 );
        delete[] dataIn;

        //        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        //                const auto& bytes               = bigDataSize;
        boostBytesPerSecond     = 1000.0 * dataWrote / duration;
        boostGigaBytesPerSecond = boostBytesPerSecond / 1'000'000'000.0;

        std::cout << "[test][Boost::asio::ip::tcp] Giga byte wrote : " << dataWrote / 1'000'000'000.0
                  << "Go" << std::endl;
        //                  << std::endl;
        std::cout << "[test][Boost::asio::ip::tcp] Giga byte per second : " << boostGigaBytesPerSecond
                  << " Go/s" << std::endl;
    }
#endif

    double sysBytesPerSecond;
    double sysGigaBytesPerSecond;
    // ClientSocket
    {
        const std::string ipv4 = "127.0.0.1";
        //        constexpr int packetSize = MAX_NET_BUFFER_SIZE;
        //        constexpr int nPart      = bigDataSize / packetSize;

        hub::net::ServerSocket serverSocket( port2 );

        hub::net::ClientSocket clientSocket( ipv4, port2 );

        assert( clientSocket.isConnected() );
        auto clientServerSocket = serverSocket.waitNewClient();
        assert( clientServerSocket.isConnected() );

        unsigned char* dataIn = new unsigned char[bigDataSize];
        memset( dataIn, 0, bigDataSize );

        size_t dataWrote  = 0;
        const auto& start = std::chrono::high_resolution_clock::now();
        {
            auto thread = std::thread( [&]() {
                try {
                    for ( int i = 0; i < nWrite; ++i ) {
//                        std::cout << "[test][ClientSocket] sending data size: "
//                                  << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
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
                    //                    dataIn[100] = 5;
//                    std::cout << "[test][ClientSocket] reading data size: "
//                              << bigDataSize / 1'000'000'000.0 << " Go" << std::endl;
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test][ClientSocket] catch exception : " << ex.what() << std::endl;
            }

            assert( thread.joinable() );
            thread.join();
        }
        const auto& end = std::chrono::high_resolution_clock::now();

        CHECK( memcmp( bigData, dataIn, bigDataSize ) == 0 );
        delete[] dataIn;

        //        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        //                const auto& bytes               = bigDataSize;
        sysBytesPerSecond     = 1000.0 * dataWrote / duration;
        sysGigaBytesPerSecond = sysBytesPerSecond / 1'000'000'000.0;

        std::cout << "[test][ClientSocket] Giga byte wrote : " << dataWrote / 1'000'000'000.0 << "Go" << std::endl;
        //                  << std::endl;
        std::cout << "[test][ClientSocket] Giga byte per second : " << sysGigaBytesPerSecond
                  << " Go/s" << std::endl;
    }

#ifdef HUB_USE_BOOST
    const double ratioSysBoost = sysBytesPerSecond / boostBytesPerSecond;
    std::cout <<  "[test] sys socket impl is " << ratioSysBoost
              << " more efficient than boost::asio::ip::tcp" << std::endl;

    CHECK_VALUE(ratioSysBoost, 2, 1, "SysSocketImpl/BoostAsioIpTcp", "/");
#endif

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] bigData;
}
