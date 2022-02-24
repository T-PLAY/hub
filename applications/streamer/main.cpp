
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

            //    OutputStream proceduralStream = hub.createOutputStream(width, height, Stream::Format::Y8);
            ClientSocket sock("127.0.0.1");
            sock.write(Client::Type::STREAMER);

            Stream::InitPacket initPacket = { width, height, Stream::Format::Y8, Device::NONE, Sensor::NONE };
            sock.write(initPacket);

            //    OutputStream proceduralStream(width, height, Stream::Format::Y8);

            constexpr size_t imgSize = width * height;
//            unsigned char img[imgSize];
            unsigned char img[imgSize];

            //    return 0;

            int dec = 0;
            //        int byteSent = 1;
            //        do {
            bool serverClose = false;
            while (! serverClose) {

                Server::Message serverMessage;
                sock.read(serverMessage);

                switch (serverMessage) {
                case Server::Message::PING:
//                    std::cout << "[streamer] server ping" << std::endl;
                    break;

                case Server::Message::CLOSE:
                    std::cout << "[streamer] server close" << std::endl;
                    serverClose = true;
                    break;

                case Server::Message::SYNC: {
                    std::cout << "[streamer] server sync" << std::endl;

                    sock.write(Client::Message::DATA);
                    //                char sync;
                    //                sock.read(sync);
                    //                assert(sync == 's');
                    //            unsigned char a;

                    //            int byteRead = recv(new_socket, (char*)&a, 1, 0);
                    //            if (byteRead != 1) {
                    //                std::cout << "can't read sync byte " << byteRead << std::endl;
                    //                break;
                    //            }
                    //            std::cout << "read a : " << (int)a << std::endl;

                    const auto& start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    // generate new image
                    for (int i = 0; i < width; ++i) {
                        for (int j = 0; j < height; ++j) {
                            img[i + j * width] = j + dec;
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
                    sock.write((unsigned char*)img, imgSize);
                } break;

                default:
                    std::cout << "unknown message from server" << std::endl;
                    exit(1);
                }
            }
            //        } while (byteSent > 0);

        } catch (std::exception& e) {
            std::cout << "[streamer] catch exception : " << e.what() << std::endl;
        }
//        return 0;
    }
    return 0;
}
