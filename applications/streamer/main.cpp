
#include <cassert>
#include <chrono>
#include <hub.h>
#include <server.h>
#include <socket.h>

int main(void)
{

    //    Hub hub("127.0.0.1", 4043);
    while (true) {
        try {

            constexpr int width = 192;
            constexpr int height = 512;

            OutputStream proceduralStream = OutputStream("proceduralStreamer", Stream::Format::Y8, { width, height });
            //            ClientSocket sock("127.0.0.1");
            //            sock.write(Client::Type::STREAMER);

            //            Stream::InitPacket initPacket = { width, height, Stream::Format::Y8, Device::NONE, Sensor::NONE };
            //            sock.write(initPacket);

            //    OutputStream proceduralStream(width, height, Stream::Format::Y8);

            constexpr size_t imgSize = width * height;
            //            unsigned char img[imgSize];
            unsigned char img[imgSize];
            //            for (int i = 0; i < width; ++i) {
            //                for (int j = 0; j < height; ++j) {
            //                    img[i + j * width] = 127;
            //                }
            //            }
            for (size_t i = 0; i < imgSize; ++i) {
                img[i] = 127;
            }

            //    return 0;

            int dec = 0;
            //        int byteSent = 1;
            //        do {
            //            bool serverClose = false;
            //            while (!serverClose) {
            while (true) {
                const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                // generate new image
                //                    for (int i = 0; i < width; ++i) {
                //                        for (int j = 0; j < height; ++j) {
                //                            img[i + j * width] = (j + dec) % 256;
                ////                            img[i + j * width] = 0;
                //                            //                            img[i + j * width] = dec % 256;
                //                        }
                //                    }
                for (size_t i = 0; i < imgSize; ++i) {
                    img[i] = (i / width + dec) % 256;
                }
                const auto& end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                ++dec;
                //                    Stream::TimestampInterval timestampInterval { start, end };

                Stream::Acquisition acq = { start, end, img };
                proceduralStream << acq;
            }

            //        } while (byteSent > 0);

        } catch (std::exception& e) {
            std::cout << "[streamer] catch exception : " << e.what() << std::endl;
        }
        //        return 0;
    }
    return 0;
}
