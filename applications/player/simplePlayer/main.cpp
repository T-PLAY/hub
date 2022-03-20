
#include <filesystem>
#include <fstream>
#include <map>
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

int main(int argc, char * argv[])
{
    std::string recordFolder;
    if (argc == 2) {
        recordFolder = argv[1];
    }
    else {
        recordFolder = "../../recorder/simpleRecorder/records/latest/";
    }

    assert(std::filesystem::exists(recordFolder));
//    std::filesystem::current_path(recordFolder);
    std::cout << "record folder : " << recordFolder << std::endl;

    //    std::vector<Record> records;
    //    std::map<std::string, std::vector<Stream::Acquisition>> records;
    std::map<std::string, std::unique_ptr<OutputStream>> outputs;
    //    std::list<Stream::Acquisition> acqs;
    std::set<Snapshot> snapshots;

    // read record
    for (const auto& fileDir : std::filesystem::directory_iterator(recordFolder)) {
//        std::cout << "path : " << fileDir.path().parent_path() << std::endl;
//        const std::string filename = (const char*)fileDir.path().filename().c_str();
        const auto filename = fileDir.path().relative_path().string();
        std::cout << "read '" << filename << "' record" << std::endl;
        assert(std::filesystem::exists(filename));

        std::fstream file(filename, std::ios::binary | std::ios::in);
        assert(file.is_open());

        InputStream inputStream(file);
        //        records.push_back({sensorName, {}, {sensorName, inputStream.getFormat(), inputStream.getDims()}});
        const std::string & sensorName = inputStream.getSensorName();
        outputs[sensorName] = std::make_unique<OutputStream>(sensorName, inputStream.getFormat(), inputStream.getDims());
        //        OutputStream outputStream(sensorName, inputStream.getFormat(), inputStream.getDims());
        int nReadAcqs = 0;
        try {
            Stream::Acquisition acq;
            while (true) {
                inputStream >> acq;
                //                std::cout << "read acquisition : " << acq << std::endl;
                //                outputStream << std::move(acq);
                snapshots.insert(Snapshot { std::move(acq), sensorName });
                ++nReadAcqs;
            }
        } catch (std::exception& e) {
            std::cout << "catch exception : " << e.what() << std::endl;
        }

        file.close();

        std::cout << "read " << nReadAcqs << " acquisitions from file sensor '" << sensorName << "'" << std::endl;
    }
    std::cout << "read all records" << std::endl;

    // play
    std::cout << "start playing" << std::endl;
    while (true) {
        const auto startRecord = snapshots.begin()->mAcq.mBackendTimestamp;
        const auto& startChrono = std::chrono::high_resolution_clock::now();

        for (const auto& snapshot : snapshots) {
            //            std::cout << "send snapshot" << std::endl;
            //            const auto& elapsedTime = std::chrono::high_resolution_clock::now() - startChrono;
            std::this_thread::sleep_until(startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord));
            //            auto end = startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord);
            //            while (std::chrono::high_resolution_clock::now() < end)
            //                ; // wait

            *outputs.at(snapshot.mSensorName) << snapshot.mAcq;
        }
        std::cout << "end record, auto loop" << std::endl;
    }
}
