//#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <server/Server.hpp>

#include <filesystem>

#define CHECK

//TEST_CASE("Server test : speed test")
int main()
{
    constexpr int nAcqs = 100;
    constexpr int width = 1920;
    constexpr int height = 1080;
    constexpr size_t dataSize = width * height * 3;
    //    unsigned char datas[nAcqs][dataSize];
    unsigned char** datas = new unsigned char*[nAcqs];
    for (int i = 0; i < nAcqs; ++i) {
        datas[i] = new unsigned char[dataSize];
    }

    //    return 0;
    std::vector<hub::Acquisition> acqs(nAcqs);

    for (int iAcq = 0; iAcq < nAcqs; ++iAcq) {
        for (int i = 0; i < dataSize; ++i) {
            datas[iAcq][i] = iAcq;
        }
        hub::Acquisition acq(iAcq, iAcq);
        acq << hub::data::Measure(reinterpret_cast<unsigned const char*>(datas[iAcq]),
            dataSize,
            { { width, height }, hub::Format::BGR8 });
        acqs.at(iAcq) = std::move(acq);
    }
    //    delete[] data;

    //    return 0;

    //    /////////////////////////////////////////////////

    const std::string ipv4 = "127.0.0.1";
    const int port = GET_RANDOM_PORT;
    //    std::vector<unsigned char> data(dataSize);
    unsigned char* data = new unsigned char[dataSize];
    //    std::mutex mtx;

    std::cout << "[test][ClientSocket] start streaming" << std::endl;
    const auto& start = std::chrono::high_resolution_clock::now();
    //    if (false)
    {
        //        unsigned char data[dataSize];
        //        std::thread threadServer = std::thread([&]() {
        //            for (int iAcq = 0; iAcq < nAcqs; ++iAcq) {
        //                clientSocket.read(data, dataSize);
        //                mtx.lock();
        //                std::cout << "[ServerSocket] read data : ";
        //                for (int i = 0; i <dataSize; ++i) {
        //                    std::cout << (int)data[i] << " ";
        //                }
        //                std::cout << std::endl;
        //                mtx.unlock();
        //            }
        //        });
        hub::net::ServerSocket serverSocket(port);
        hub::net::ClientSocket clientSocket(ipv4, port);
        auto clientServerSocket = serverSocket.waitNewClient();
        for (int iAcq = 0; iAcq < nAcqs; ++iAcq) {
            //                mtx.lock();
            //                std::cout << "[ClientSocket] write data : ";
            //                for (int i = 0; i <dataSize; ++i) {
            //                    std::cout << (int)datas[iAcq][i] << " ";
            //                }
            //                std::cout << std::endl;
            //                mtx.unlock();
            clientSocket.write(datas[iAcq], dataSize);
            clientServerSocket.read(data, dataSize);
            assert(!memcmp(data, datas[iAcq], dataSize));
        }
        //        threadServer.join();
    }
    const auto& end = std::chrono::high_resolution_clock::now();
    std::cout << "[test][ClientSocket] end streaming" << std::endl;
    const auto& duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    const auto& bytes = dataSize * nAcqs;
    const auto& bytesPerSeconds = 1000 * bytes / duration;
    const auto& megaBytesPerSeconds = bytesPerSeconds / 1000'000;

    std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000 << " Mo" << std::endl;
    std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds << " Mo/s" << std::endl;

    delete[] data;

    for (int i = 0; i < nAcqs; ++i) {
        delete[] datas[i];
    }
    delete[] datas;

    //    return 0;

    //    const std::string ipv4 = "127.0.0.1";
    //    srand( (unsigned)time( NULL ) );
    //    const int port = rand() % 65535;
    {
        const int port2 = port + 1;

        std::cout << "[test][InputOutputSensor] ############################### server start" << std::endl;
        hub::server::Server server(port2);
        server.setMaxClients(2);
        server.asyncRun();
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "[test][InputOutputSensor] server end ------------------------------" << std::endl;

        {
            std::cout << "[test][InputOutputSensor] ############################### outputStream start" << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec("sensorName", { { { width, height }, hub::Format::BGR8 } }),
                hub::io::OutputStream("streamName", hub::net::ClientSocket(ipv4, port2)));

            //            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            std::cout << "[test][InputOutputSensor] ############################### inputStream start" << std::endl;
            hub::InputSensor inputSensor(
                hub::io::InputStream("streamName", hub::net::ClientSocket(ipv4, port2)));

            const auto& inputSensorSpec = inputSensor.getSpec();
            CHECK(inputSensorSpec.getAcquisitionSize() == dataSize);
            CHECK(inputSensorSpec.getSensorName() == "sensorName");
            CHECK(inputSensorSpec.getResolutions().size() == 1);
            CHECK(inputSensorSpec.getResolutions()[0].first.size() == 2);
            CHECK(inputSensorSpec.getResolutions()[0].first.at(0) == width);
            CHECK(inputSensorSpec.getResolutions()[0].first.at(1) == height);
            CHECK(inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8);
            std::cout << "[test][InputOutputSensor] inputStream end ---------------------------------" << std::endl;

            std::cout << "[test][InputOutputSensor] ############################### send acquisitions" << std::endl;
            const auto& start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < nAcqs; ++i) {
                outputSensor << acqs.at(i);
                auto acq = inputSensor.getAcq();
                CHECK(acq == acqs.at(i));
            }
            const auto& end = std::chrono::high_resolution_clock::now();
            const auto& duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            const auto& bytes2 = dataSize * nAcqs;
            const auto& bytesPerSeconds2 = 1000 * bytes / duration2;
            const auto& megaBytesPerSeconds2 = bytesPerSeconds2 / 1000'000;

            std::cout << "[test][InputOutputSensor] Mega byte wrote : " << bytes2 / 1000'000 << " Mo" << std::endl;
            std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds2 << " Mo/s" << std::endl;

            const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
            std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %" << std::endl;
            CHECK (ratio > 45);
//            CHECK(megaBytesPerSeconds >= 800); // server IRIT
            // CHECK( megaBytesPerSeconds >= 2700 ); // home linux
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    //    server.stop();
}
