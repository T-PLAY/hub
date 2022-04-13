
#include <cassert>
#include <chrono>

#include <socket.h>
#include <stream.h>
#include <tuple>

#define ULA_STREAMER

int main(int argc, char* argv[])
{
//    std::cout << typeid(int).operator==( << std::endl;
//    return 0;

#ifdef ULA_STREAMER
    constexpr int width = 192;
    constexpr int height = 512;

    Stream::MetaData metaData;
    metaData["depth"] = 3.0;
    metaData["name"] = "L533";
    metaData["val"] = 1;

    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, ClientSocket(), metaData);

//    return 0;

    const size_t imgSize = proceduralStream.getAcquisitionSize();
    assert(imgSize == 192 * 512);

    unsigned char data[192 * 512];

    int dec = 0;
    while (true) {

        const auto start = std::chrono::high_resolution_clock::now();
        // generate new image
        for (size_t i = 0; i < imgSize; ++i) {
            data[i] = (i / width + dec) % 256;
        }
        const auto maxFps = 40;
        const auto end = start + std::chrono::microseconds(1'000'000 / maxFps);

        const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
        const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();
        ++dec;

        proceduralStream << Stream::Acquisition { timestampStart, timestampEnd, data };

        std::this_thread::sleep_until(end);
    }
#else

    const std::string sensorNames[2] = { "ULA-OP 256", "Polhemus Patriot (probe)" };
    constexpr Stream::Format sensorFormats[2] = { Stream::Format::Y8, Stream::Format::DOF6 };
    const std::vector<int> sensorDims[2] = { { 512, 192 }, { 1 } };
    constexpr int maxFpss[2] = { 40, 60 };
    constexpr size_t sensorDataSize[2] = { 192 * 512, 28 };
    std::vector<std::thread> threads;

    int nSensor = (argc == 2) ? (1) : (2);
    for (int i = 0; i < nSensor; ++i) {
        const int iSensor = (nSensor == 1) ? (atoi(argv[1])) : (i);

        std::thread thread([=]() {
            unsigned char* data = new unsigned char[sensorDataSize[iSensor]];

            while (true) {
                try {

                    OutputStream proceduralStream = OutputStream(sensorNames[iSensor], sensorFormats[iSensor], sensorDims[iSensor]);

                    const size_t imgSize = proceduralStream.getAcquisitionSize();
                    assert(imgSize == 28 || imgSize == 98304);

                    const auto maxFps = maxFpss[iSensor];

                    int dec = 0;
                    while (true) {

                        const auto start = std::chrono::high_resolution_clock::now();
                        // generate new image
                        if (iSensor == 0) {
                            for (size_t i = 0; i < imgSize; ++i) {
                                data[i] = (i / 192 + dec) % 256;
                            }

                        } else {
                            for (size_t i = 0; i < imgSize / 4; ++i) {
                                ((float*)data)[i] = dec;
                            }
                        }
                        ++dec;
                        const auto end = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);

                        while (std::chrono::high_resolution_clock::now() < end)
                            ;
                        //{

                        const auto now = std::chrono::high_resolution_clock::now();
                        const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
                        const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
                        Stream::Acquisition acq { timestampStart, timestampEnd, data };
                        acq.mSize = imgSize;
                        proceduralStream << acq;
                        // std::cout << "thread " << iSensor << " send acq " << acq << std::endl;

                    } // while (true)
                } catch (std::exception& e) {
                    std::cout << "[streamer] catch exception : " << e.what() << std::endl;
                }
            } // while (true)
            delete[] data;
        });
        threads.emplace_back(std::move(thread));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } // for (int i = 0; i < nSensor; ++i)

    for (int i = 0; i < nSensor; ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }
#endif

    return 0;
}
