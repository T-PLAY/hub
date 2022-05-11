
#include <FileIO.h>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <socket.h>
#include <stream.h>
#include <string>

struct Snapshot {
    Stream::Acquisition mAcq;
    std::string mSensorName;

    bool operator<(const Snapshot& snapshot) const
    {
        return mAcq.mBackendTimestamp < snapshot.mAcq.mBackendTimestamp;
    }
};

int main(int argc, char* argv[])
{
    std::string recordFolder;
    if (argc == 2) {
        recordFolder = argv[1];
    } else {
        recordFolder = PROJECT_DIR "records/latest/";
    }

    assert(std::filesystem::exists(recordFolder));
    std::cout << "record folder : " << recordFolder << std::endl;

    std::map<std::string, std::unique_ptr<OutputStream>> outputs;
    std::set<Snapshot> snapshots;

    // read record
    for (const auto& fileDir : std::filesystem::directory_iterator(recordFolder)) {
        const auto filename = fileDir.path().string();
        std::cout << "read '" << filename << "' record" << std::endl;
        assert(std::filesystem::exists(filename));

        std::fstream file(filename, std::ios::binary | std::ios::in);
        assert(file.is_open());
        std::cout << "tellg" << file.tellg() << std::endl;
        file.seekg(0, std::ios::end);
        std::cout << "tellg" << file.tellg() << std::endl;
        file.seekg(0, std::ios::beg);

        assert(!file.eof());
        assert(sizeof(int) == 4);

        try {
            InputStream inputStream(FileIO { file });

            const std::string& sensorName = inputStream.getSensorName();

            // here
            outputs[sensorName] = std::make_unique<OutputStream>(sensorName + " (record)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(), inputStream.getMetaData());
            int nReadAcqs = 0;
            Stream::Acquisition acq;
            while (true) {
                inputStream >> acq;
                std::cout << "read acquisition : " << acq << std::endl;
                snapshots.insert(Snapshot { std::move(acq), sensorName });
                ++nReadAcqs;
            }
            std::cout << "read " << nReadAcqs << " acquisitions from file sensor '" << sensorName << "'" << std::endl;
        } catch (Stream::exception& e) {
            std::cout << "catch stream exception : " << e.what() << std::endl;
            throw;

        } catch (std::exception& e) {
            std::cout << "catch exception : " << e.what() << std::endl;
        }

        file.close();
    }
    std::cout << "read " << snapshots.size() << " acquistions" << std::endl;

    // play
    std::cout << "start playing" << std::endl;
    int iLoop = 0;
    while (true) {
        const auto startRecord = snapshots.begin()->mAcq.mBackendTimestamp;
        const auto& startChrono = std::chrono::high_resolution_clock::now();

        for (const auto& snapshot : snapshots) {
            std::this_thread::sleep_until(startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord));

            *outputs.at(snapshot.mSensorName) << snapshot.mAcq;
        }
        std::cout << "end record, auto loop " << iLoop << std::endl;
        ++iLoop;
    }
}
