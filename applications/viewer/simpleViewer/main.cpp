

#include <cassert>
#include <iostream>

#include <stream.h>

int main()
{

    while (true) {
        try {
            InputStream inputStream("proceduralStreamer");

            const size_t acquisitionSize = inputStream.getAcquisitionSize();
            const int width = inputStream.getDims().at(0);

            Stream::Acquisition acq;

            while (true) {
                inputStream >> acq;

                const int dec = acq.mData[0];
                for (size_t i = 0; i < acquisitionSize; ++i) {
                    assert(acq.mData[i] == (i / width + dec) % 256);
                }

                std::cout << "[simpleViewer] receive acquisition : " << acq << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "[simpleViewer] catch exception : " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "--------------------------------------------" << std::endl;
    }
}
