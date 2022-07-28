

#include <cassert>
#include <iostream>

#include <chrono>
#include <socket.h>
#include <stream.h>

int main(int argc, char* argv[])
{
    argc = 2;
    std::string sensorNames[2] = { "proceduralStreamer", "proceduralStreamer2" };
    std::string sensorMasterNames[2] = { "", "proceduralStreamer" };
    assert(argc == 2);
    // int i = atoi(argv[1]);
    int i = 0;
    assert(i == 0 || i == 1);

    std::cout << "inputStream" << std::endl;
    InputStream inputStream(ClientSocket(sensorNames[i], sensorMasterNames[i]));
    std::cout << "proceduralStreamer inited" << std::endl;
    const Stream::MetaData& metaData = inputStream.getMetaData();
    std::cout << "metadata : " << Stream::to_string(metaData) << std::endl;
    assert(std::any_cast<double>(metaData.at("depth")) == 3.0);
    std::string name(std::any_cast<const char*>(metaData.at("name")));
    assert(name == "L533");
    assert(std::string(std::any_cast<const char*>(metaData.at("name"))) == "L533");

    // InputStream inputStream("L500 Depth Sensor (Depth)");

    const size_t acquisitionSize = inputStream.getAcquisitionSize();
    std::cout << "acquisitionSize = " << acquisitionSize << std::endl;
    // const int width = inputStream.getDims().at(0);

//    Stream::Acquisition acq;

    while (true) {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 10; ++i) {
//            inputStream >> acq;
            auto acq = inputStream.getAcquisition();

            std::cout << acq << std::endl;

            // const int dec = acq.mData[0];
            // for (size_t i = 0; i < acquisitionSize; ++i) {
            // }
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto fps = (10.0 * 1'000'000) / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "fps : " << fps << std::endl;
    }
}
