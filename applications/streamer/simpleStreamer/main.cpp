
#include <cassert>
#include <chrono>

#include <stream.h>

//#define ULA_STREAMER

int main()
{

    while (true) {
        try {

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


            OutputStream proceduralStream = OutputStream("proceduralStreamer", Stream::Format::DOF6, { 1 });

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
                    ((float*)data)[i] = dec;
                }
                const auto end = std::chrono::high_resolution_clock::now();

//                const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                const auto& timestampStart = std::chrono::duration_cast<std::chrono::microseconds>(start.time_since_epoch()).count();
                const auto& timestampEnd = std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count();
                ++dec;

                proceduralStream << Stream::Acquisition { timestampStart, timestampEnd, data };

                //                proceduralStream << Stream::Acquisition( start, end, data );
                //                proceduralStream << { start, end, data };
                //                auto acq = { start, end, data };
                const auto maxFps = 30;
//                const auto now = std::chrono::high_resolution_clock::now();
//                std::cout << "sleep for " <<  (start + std::chrono::microseconds(1'000'000 / maxFps) - now).count() << std::endl;
                std::this_thread::sleep_until(start + std::chrono::microseconds(1'000'000 / maxFps));
            }
#endif

        } catch (std::exception& e) {
            std::cout << "[streamer] catch exception : " << e.what() << std::endl;
        }
    }
    return 0;
}
