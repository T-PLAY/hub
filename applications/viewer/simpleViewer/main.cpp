

#include <iostream>
#include <stream.h>
#include <cassert>

int main(int argc, char* argv[])
{

    while (true) {
        try {
            InputStream inputStream("proceduralStreamer");
            //        Stream stream("ee", Stream::Format::NONE, {});

            Stream::Acquisition acq;

            while (true) {
                inputStream >> acq;

                const size_t acquisitionSize = inputStream.getAcquisitionSize();
                int dec = acq.data[0];
                int width = inputStream.getDims().at(0);
                for (size_t i = 0; i < acquisitionSize; ++i) {
                    if (acq.data[i] != (i / width + dec) % 256) {
                        int tmp = acq.data[i];
                        unsigned char* ptr = &acq.data[i];
                        assert(false);
                    }
                }

                std::cout << "[main] receive acquisition : " << acq << std::endl;
            }

        } catch (const Socket::exception& e) {
            std::cout << "[main] catch socket exception : " << e.what() << std::endl;

        } catch (const Stream::exception& e) {
            std::cout << "[main] catch stream exception : " << e.what() << std::endl;

        } catch (const std::exception& e) {
            std::cout << "[main] catch exception : " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "--------------------------------------------" << std::endl;
    }
}
