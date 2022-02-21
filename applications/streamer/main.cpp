
#include <cassert>
#include <chrono>
#include <hub.h>
#include <socket.h>

int main(void)
{

    //    Hub hub("127.0.0.1", 4043);

    constexpr int width = 192;
    constexpr int height = 512;

    //    OutputStream proceduralStream = hub.createOutputStream(width, height, Stream::Format::Y8);
    ClientSocket sock("127.0.0.1", 4043);
    sock.write(Client::Type::STREAMER);

    Stream::InitPacket initPacket = { width, height, Stream::Format::Y8, Device::NONE, Sensor::NONE };
    sock.write(initPacket);

    //    OutputStream proceduralStream(width, height, Stream::Format::Y8);

    constexpr size_t imgSize = width * height;
    unsigned char img[imgSize];

    //    return 0;

    {
        int dec = 0;
        //        int byteSent = 1;
        //        do {
        while (true) {

            char sync;
            sock.read(sync);
            assert(sync == 's');
            //            unsigned char a;

            //            int byteRead = recv(new_socket, (char*)&a, 1, 0);
            //            if (byteRead != 1) {
            //                std::cout << "can't read sync byte " << byteRead << std::endl;
            //                break;
            //            }
            //            std::cout << "read a : " << (int)a << std::endl;

            const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            // generate new image
            for (int i = 0; i < 512; ++i) {
                for (int j = 0; j < 192; ++j) {
                }
            }
            const auto& end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            ++dec;

            //            int byteSent = send(new_socket, (char*)img, imgSize, 0);
            //            if (byteSent != imgSize) {
            //                std::cout << "can't send sync byte " << byteSent << std::endl;
            //                break;
            //            }

            //            std::cout << "sent " << byteSent << " bytes" << std::endl;
            //            proceduralStream << Stream::Acquisition{{start, end}, (char*)img};
            Stream::TimestampInterval timestampInterval { start, end };
            sock.write(timestampInterval);
            sock.write((char*)img, imgSize);
        }
        //        } while (byteSent > 0);
    }

    return 0;
}
