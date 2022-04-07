
#include <cassert>
#include <chrono>

#include <stream.h>
#include <tuple>
#include <socket.h>

#define ULA_STREAMER

int main(int argc, char* argv[])
{
//    std::cout << typeid(5.0).name() << typeid(5.0).hash_code() << std::endl;

#ifdef ULA_STREAMER
    constexpr int width = 192;
    constexpr int height = 512;

    Stream::MetaData metaData;
    metaData["depth"] = 3.0;
    metaData["name"] = std::string("L533");
//    OutputStream proceduralStream = OutputStream(ClientSocket(ClientSocket::Type::STREAMER, "proceduralStreamer"), Stream::Format::Y8, { width, height }, metaData);
//    OutputStream proceduralStream = OutputStream(ClientSocket(ClientSocket::Type::STREAMER, "proceduralStreamer"), Stream::Format::Y8, { width, height }, metaData);
//    ClientSocket sock;
//    auto&& sock2 = std::move(sock);

//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, ClientSocket());
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, ClientSocket());
//    ClientSocket sock;
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(sock));
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, sock2);
    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, {width, height});
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(sock2));
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, ClientSocket());
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(ClientSocket()));
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(*new ClientSocket()));
//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(ClientSocket()));

//    OutputStream proceduralStream("proceduralStreamer", Stream::Format::Y8, { width, height }, std::move(*new ClientSocket()));

//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//    std::cout << "slept" << std::endl;
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
    //    int maxFpss[2] = { 1, 10 };
    std::vector<std::thread> threads;

    int nSensor = (argc == 2) ? (1) : (2);
    //    int iSensor = atoi(argv[1]);
    //    std::cout << "iSensor = " << iSensor << std::endl;
    //    assert(iSensor == 0 || iSensor == 1);
    for (int i = 0; i < nSensor; ++i) {
        const int iSensor = (nSensor == 1) ? (atoi(argv[1])) : (i);

        std::thread thread([=]() {
            unsigned char * data = new unsigned char[sensorDataSize[iSensor]];

            while (true) {
                try {

                    OutputStream proceduralStream = OutputStream(sensorNames[iSensor], sensorFormats[iSensor], sensorDims[iSensor]);

                    const size_t imgSize = proceduralStream.getAcquisitionSize();
                    assert(imgSize == 28 || imgSize == 98304);

                    const auto maxFps = maxFpss[iSensor];

                    //            int dec = 0;
                    //                    float dec = 0.0f;
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

                        //                std::this_thread::sleep_until(end);
                        //                std::this_thread::sleep_for(std::chrono::nanoseconds(1'000'000'000 / maxFps - 10'000'000));
                        while (std::chrono::high_resolution_clock::now() < end)
                            ;
                        //{
                        //                    std::this_thread::sleep_for(std::chrono::microseconds(1));
                        //                }
                        //                const auto now = std::chrono::high_resolution_clock::now();

                        //                for (int i = 0; i < iSensor; ++i) {
                        //                    std::cout << "\t";
                        //                }
                        const auto now = std::chrono::high_resolution_clock::now();
                        const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
                        const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
                        Stream::Acquisition acq { timestampStart, timestampEnd, data };
                        acq.mSize = imgSize;
                        proceduralStream << acq;
                        // std::cout << "thread " << iSensor << " send acq " << acq << std::endl;

                        //                const auto end2 = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);
                        //                std::this_thread::sleep_until(end2);
                    } // while (true)
                } catch (std::exception& e) {
                    std::cout << "[streamer] catch exception : " << e.what() << std::endl;
                }
            } // while (true)
            delete [] data;
        });
        //        thread.detach();
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
