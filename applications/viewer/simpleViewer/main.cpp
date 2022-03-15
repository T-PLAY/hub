

#include <cassert>
#include <iostream>

#include <stream.h>

int main()
{

    while (true) {
        try {
            InputStream inputStream("proceduralStreamer");
            std::cout << "proceduralStreamer inited" << std::endl;
            InputStream inputStream2("proceduralStreamer2", "proceduralStreamer");
//            InputStream inputStream2("proceduralStreamer2");
            std::cout << "proceduralStreamer2 inited" << std::endl;
            // InputStream inputStream("L500 Depth Sensor (Depth)");
            //             InputStream inputStream("Polhemus Patriot (probe)");

            const size_t acquisitionSize = inputStream.getAcquisitionSize();
            std::cout << "acquisitionSize = " << acquisitionSize << std::endl;
            // const int width = inputStream.getDims().at(0);

            Stream::Acquisition acq, acq2;

            while (true) {
                const auto start = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < 10; ++i) {
                    inputStream >> acq;
                    std::cout << "inputStream acq : " << acq << std::endl;

                    inputStream2 >> acq2;
                    std::cout << "inputStream2 acq : " << acq2 << std::endl;
                    std::cout << "timestamp start diff : " << std::abs(acq.mBackendTimestamp - acq2.mBackendTimestamp) / 1000.0 << " milliseconds" << std::endl;

                    // const int dec = acq.mData[0];
                    // for (size_t i = 0; i < acquisitionSize; ++i) {
                    //     assert(acq.mData[i] == (i / width + dec) % 256);
                    // }

                    //                std::cout << "[simpleViewer] receive acquisition : " << acq << std::endl;
                    //                float* data = (float*)acq.mData;
                    //                float* translation = (float*)acq.mData;
                    //                float* quaternion = (float*)&acq.mData[12];
                    //                std::cout << "x:" << translation[0] << ", y:" << translation[1] << ", z:" << translation[2] << ", az:" << quaternion[0] << ", el:" << quaternion[1] << ", ro:" << quaternion[2] << ", q4:" << quaternion[3] << std::endl;
                }
                const auto end = std::chrono::high_resolution_clock::now();
                const auto fps = (10.0 * 1'000'000) / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                std::cout << "fps : " << fps << std::endl;
            }

        } catch (const std::exception& e) {
            std::cout << "[simpleViewer] catch exception : " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "--------------------------------------------" << std::endl;
    }
}
