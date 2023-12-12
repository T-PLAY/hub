// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

//#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"
#include "test_io_common.hpp"

// #include <core/Utils.hpp>
#include <io/InputOutputSocket.hpp>
#include <net/ServerSocket.hpp>

TEST_CASE( "InputOutputSocket test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;
//    const auto port2    = port + 1;

    const std::string ipv4 = "127.0.0.1";
    //        constexpr int packetSize = MAX_NET_BUFFER_SIZE;
    //        constexpr int nPart      = bigDataSize / packetSize;

    hub::net::ServerSocket serverSocket( port );
    hub::net::ClientSocket clientSocket( ipv4, port );
    assert( clientSocket.isConnected() );
    std::cout << "clientSocket: " << clientSocket << std::endl;
    auto clientServerSocket = serverSocket.waitNewClient();
    assert( clientServerSocket.isConnected() );
    std::cout << "clientServerSocket: " << clientServerSocket << std::endl;

    hub::io::InputOutputSocket inputOutputSocket(std::move(clientSocket));
    hub::io::InputOutputSocket inputOutputSocket2(std::move(clientServerSocket));

    inputOutputBench(inputOutputSocket, inputOutputSocket2);
    inputOutputBench(inputOutputSocket2, inputOutputSocket);

//    inputOutputSensorBench(inputOutputSocket, inputOutputSocket2);

//    inputOutputSensorBench<hub::io::InputOutputSocket>();

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    TEST_END()
}
