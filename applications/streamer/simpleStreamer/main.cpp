
#include <cassert>
#include <chrono>

#include <stream.h>

//#define ULA_STREAMER

int main()
{

    //    while (true) {
    //        try {

#ifdef ULA_STREAMER
    constexpr int width = 192;
    constexpr int height = 512;

    OutputStream proceduralStream = OutputStream("proceduralStreamer", Stream::Format::Y8, { width, height });

    const size_t imgSize = proceduralStream.getAcquisitionSize();
    assert(imgSize == 192 * 512);

    unsigned char data[192 * 512];

    int dec = 0;
    while (true) {

        //                const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        const auto start = std::chrono::high_resolution_clock::now();
        // generate new image
        for (size_t i = 0; i < imgSize; ++i) {
            data[i] = (i / width + dec) % 256;
        }
        const auto maxFps = 40;
        const auto end = start + std::chrono::microseconds(1'000'000 / maxFps);

        //                const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
        const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();
        ++dec;

        proceduralStream << Stream::Acquisition { timestampStart, timestampEnd, data };

        //                proceduralStream << Stream::Acquisition( start, end, data );
        //                proceduralStream << { start, end, data };
        //                auto acq = { start, end, data };
        std::this_thread::sleep_until(end);
    }
#else

    std::string sensorNames[2] = { "proceduralStreamer", "proceduralStreamer2" };
    int maxFpss[2] = { 1, 3 };
    std::vector<std::thread> threads;

    for (int iThread = 0; iThread < 2; ++iThread) {

        std::thread thread([sensorNames, maxFpss, iThread]() {
            while (true) {
                try {

                    OutputStream proceduralStream = OutputStream(sensorNames[iThread], Stream::Format::DOF6, { 1 });

                    const size_t imgSize = proceduralStream.getAcquisitionSize();
                    assert(imgSize == 28);

                    unsigned char data[28];

                    int dec = 0;
                    while (true) {

                        //                const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        const auto start = std::chrono::high_resolution_clock::now();
                        // generate new image
                        for (size_t i = 0; i < imgSize / 4; ++i) {
                            //                    data[i] = (i / width + dec) % 256;
                            ((float*)data)[i] = (float)dec;
                        }
                        //                const auto end = std::chrono::high_resolution_clock::now();
                        const auto maxFps = maxFpss[iThread];
                        const auto end = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);

                        //                const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
                        const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();
                        ++dec;

                        Stream::Acquisition acq { timestampStart, timestampEnd, data };
                        acq.mSize = proceduralStream.getAcquisitionSize();
                        for (int i = 0; i < iThread; ++i) {
                            std::cout << "\t";
                        }
                        std::cout << "thread " << iThread << " send acq " << acq << std::endl;
                        proceduralStream << acq;

                        //                        proceduralStream << Stream::Acquisition { timestampStart, timestampEnd, data };

                        //                proceduralStream << Stream::Acquisition( start, end, data );
                        //                proceduralStream << { start, end, data };
                        //                auto acq = { start, end, data };

                        //                                const auto now = std::chrono::high_resolution_clock::now();
                        //                                std::cout << "sleep for " <<  (start + std::chrono::microseconds(1'000'000 / maxFps) - now).count() << std::endl;

                        //                const auto end2 = std::chrono::high_resolution_clock::now();
                        const auto end2 = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);
                        //                const auto fps = (1'000'000) / std::chrono::duration_cast<std::chrono::microseconds>(end2 - start).count();
                        //                std::cout << "fps : " << fps << std::endl;
                        //                                std::this_thread::sleep_until(start + std::chrono::microseconds(1'000'000 / maxFps));
                        std::this_thread::sleep_until(end2);
                    } // while (true)
                } catch (std::exception& e) {
                    std::cout << "[streamer] catch exception : " << e.what() << std::endl;
                }
            } // while (true)
        });
        threads.push_back(std::move(thread));
        //        threads.push_back(thread);
        //        thread.detach();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } // for (int i = 0; i < 2; ++i)
#endif

    for (int i = 0; i < 2; ++i) {
        if (threads[i].joinable())
            threads[i].join();
    }
    //    //}
    return 0;
}
