
#include <cassert>
#include <chrono>

#include <stream.h>

//#define ULA_STREAMER

int main(int argc, char* argv[])
{

#ifdef ULA_STREAMER
    constexpr int width = 192;
    constexpr int height = 512;

    OutputStream proceduralStream = OutputStream("proceduralStreamer", Stream::Format::Y8, { width, height });

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

    std::string sensorNames[2] = { "proceduralStreamer", "proceduralStreamer2" };
    int maxFpss[2] = { 40, 60 };
    //    int maxFpss[2] = { 1, 10 };

    assert(argc == 2);
    int iThread = atoi(argv[1]);
    std::cout << "iThread = " << iThread << std::endl;
    assert(iThread == 0 || iThread == 1);
    while (true) {
        try {

            OutputStream proceduralStream = OutputStream(sensorNames[iThread], Stream::Format::DOF6, { 1 });

            const size_t imgSize = proceduralStream.getAcquisitionSize();
            assert(imgSize == 28);

            unsigned char data[28];
            const auto maxFps = maxFpss[iThread];

            //            int dec = 0;
            float dec = 0.0f;
            while (true) {

                const auto start = std::chrono::high_resolution_clock::now();
                // generate new image
                for (size_t i = 0; i < imgSize / 4; ++i) {
                    ((float*)data)[i] = dec;
                }
                ++dec;
                const auto end = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);

                //                std::this_thread::sleep_until(end);
                //                std::this_thread::sleep_for(std::chrono::nanoseconds(1'000'000'000 / maxFps - 10'000'000));
                while (std::chrono::high_resolution_clock::now() < end);
//{
//                    std::this_thread::sleep_for(std::chrono::microseconds(1));
//                }
                //                const auto now = std::chrono::high_resolution_clock::now();

                //                for (int i = 0; i < iThread; ++i) {
                //                    std::cout << "\t";
                //                }
                const auto now = std::chrono::high_resolution_clock::now();
                const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
                const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
                Stream::Acquisition acq { timestampStart, timestampEnd, data };
                acq.mSize = imgSize;
                proceduralStream << acq;
                std::cout << "thread " << iThread << " send acq " << acq << std::endl;

                //                const auto end2 = start + std::chrono::nanoseconds(1'000'000'000 / maxFps);
                //                std::this_thread::sleep_until(end2);
            } // while (true)
        } catch (std::exception& e) {
            std::cout << "[streamer] catch exception : " << e.what() << std::endl;
        }
    } // while (true)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif

    return 0;
}
