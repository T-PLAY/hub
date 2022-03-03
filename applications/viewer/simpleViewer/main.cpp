

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

            auto acq = inputStream.acquisition();

            while (true) {
                inputStream >> acq;

                const int dec = acq.data[0];
                for (size_t i = 0; i < acquisitionSize; ++i) {
                    assert(acq.data[i] == (i / width + dec) % 256);
                }

                std::cout << "[simpleViewer] receive acquisition : " << acq << std::endl;
            }

//        } catch (const Socket::exception& e) {
//            std::cout << "[main] catch socket exception : " << e.what() << std::endl;

//        } catch (const Stream::exception& e) {
//            std::cout << "[main] catch stream exception : " << e.what() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "[simpleViewer] catch exception : " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "--------------------------------------------" << std::endl;
    }
}
