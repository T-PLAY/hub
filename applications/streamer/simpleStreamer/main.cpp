
#include <cassert>
#include <chrono>

#include <stream.h>

int main()
{

    while (true) {
        try {

            constexpr int width = 192;
            constexpr int height = 512;

            OutputStream proceduralStream = OutputStream("proceduralStreamer", Stream::Format::Y8, { width, height });

            const size_t imgSize = proceduralStream.getAcquisitionSize();
            assert(imgSize == 192 * 512);

            unsigned char data[imgSize];

            int dec = 0;
            while (true) {

                const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                // generate new image
                for (size_t i = 0; i < imgSize; ++i) {
                    data[i] = (i / width + dec) % 256;
                }
                const auto& end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                ++dec;

                proceduralStream << Stream::Acquisition { start, end, data };
            }

        } catch (std::exception& e) {
            std::cout << "[streamer] catch exception : " << e.what() << std::endl;
        }
    }
    return 0;
}
