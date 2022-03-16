
#include <filesystem>
#include <fstream>
#include <stream.h>
#include <string>

int main()
{

    std::filesystem::current_path("../../recorder/simpleRecorder/records/");

    for (const auto& fileDir : std::filesystem::directory_iterator(".")) {
        const std::string sensorName = (const char*)fileDir.path().filename().c_str();

        std::fstream file(sensorName, std::ios::binary | std::ios::in);
        assert(file.is_open());

        InputStream inputStream(sensorName, file);
        OutputStream outputStream(sensorName, inputStream.getFormat(), inputStream.getDims());
        try {
            Stream::Acquisition acq;
            while (true) {
                inputStream >> acq;
                std::cout << "read acquisition : " << acq << std::endl;
                outputStream << std::move(acq);
            }
        } catch (std::exception& e) {
            std::cout << "catch exception : " << e.what() << std::endl;
        }

        file.close();
    }
}
