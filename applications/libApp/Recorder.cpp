#include "Recorder.h"

#include <FileIO.h>
#include <filesystem>
#include <fstream>
#include <stream.h>

// Recorder::Recorder(const std::string& rootPath)
//     : m_rootPath(rootPath)
//{
// }

// Recorder::Recorder(std::string rootPath)
//     : m_rootPath(std::move(rootPath))
//{
// }

Recorder::Recorder(const char* rootPath)
    : m_rootPath(rootPath)
{
}

void Recorder::record(const InputStreamParameters& inputStreamConfigs)
{
    assert(!inputStreamConfigs.empty());
    std::cout << "[Recorder] record()" << std::endl;
    assert(!m_isRecording);
    assert(m_thread == nullptr);

    std::vector<std::unique_ptr<InputStream>> inputStreams;
    std::vector<std::unique_ptr<OutputStream>> outputFileStreams;

    // creating new record folder
    char folderName[64] = { 0 };
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = localtime(&rawtime);
    strftime(folderName, sizeof(folderName), "%Y-%m-%d_%H-%M-%S", timeinfo);
    std::string newRecordFolder = m_rootPath + "records/" + folderName;
    std::cout << "create directory " << newRecordFolder << std::endl;
    assert(!std::filesystem::exists(newRecordFolder));
    std::filesystem::create_directories(newRecordFolder);

    const int nStream = inputStreamConfigs.size();
    for (int i = 0; i < nStream; ++i) {
        const auto& inputStreamConfig = inputStreamConfigs.at(i);
        //        for (const auto & inputStreamConfig : inputStreamConfigs) {
        const auto& sensorName = inputStreamConfig.first;
        const auto& syncSensorName = inputStreamConfig.second;
        inputStreams.push_back(std::make_unique<InputStream>(ClientSocket(sensorName, syncSensorName)));
        //        }

        //        //    std::fstream files[2];
        //        for (int i = 0; i < 2; ++i) {
        auto file = std::fstream(newRecordFolder + "/" + sensorName + ".txt", std::ios::binary | std::ios::out);
        assert(file.is_open());

        // here
        outputFileStreams.push_back(std::make_unique<OutputStream>(sensorName, inputStreams[i]->getFormat(), inputStreams[i]->getDims(), FileIO(std::move(file)), inputStreams[i]->getMetaData()));
    }

    m_thread = new std::thread([this, inputStreams = std::move(inputStreams), outputFileStreams = std::move(outputFileStreams), newRecordFolder = std::move(newRecordFolder), nStream]() {
        try {

            m_isRecording = true;
            while (m_isRecording) {
                Stream::Acquisition acq;
                for (int i = 0; i < nStream; ++i) {
                    *inputStreams[i] >> acq;
                    std::cout << "[" << i << "] read/write acq : " << acq << std::endl;
                    *outputFileStreams[i] << acq;

                    //                *inputStreams[1] >> acq;
                    //                std::cout << "[" << 1 << "] read/write acq : " << acq << std::endl;
                    //                *outputFileStreams[1] << acq;
                }
            }

        } catch (std::exception& e) {
            std::cout << "catch exception : " << e.what() << std::endl;
            assert(!m_isRecording);
            m_isRecording = true;
        }

        //    for (int i = 0; i < 2; ++i) {
        //        files[i].close();
        //    }

        // copy this record into latest folder
        //        {
        //            std::string latestFolder = m_rootPath + "records/latest/";
        //            std::filesystem::create_directories(latestFolder);
        //            for (const auto& dirEntry : std::filesystem::directory_iterator(latestFolder)) {
        //                std::filesystem::remove(dirEntry);
        //            }
        //            std::filesystem::copy(newRecordFolder, latestFolder, std::filesystem::copy_options::recursive);
        //        }
    });
}

void Recorder::stop()
{
    std::cout << "[Recorder] stop()" << std::endl;
    assert(m_isRecording == true);
    m_isRecording = false;

    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
}

void Recorder::save(const Frame& frame)
{
    assert(!m_isRecording);

    InputStreamParameters inputStreamConfigs;

    std::string folderName = "";
    auto it = frame.begin();
    while (it != frame.end()) {
        //    for (const auto & snapshot : frame.m_snapshots) {
        const auto& snapshot = *it;

        folderName += snapshot.getSensorName();

        inputStreamConfigs.push_back({ snapshot.getSensorName(), "" });

        ++it;
        if (it != frame.end())
            folderName += "_";
    }

    std::string newRecordFolder = m_rootPath + "snapshots/" + folderName;
    std::cout << "create directory " << newRecordFolder << std::endl;
//    assert(!std::filesystem::exists(newRecordFolder));
    std::filesystem::create_directories(newRecordFolder);

//    std::vector<std::unique_ptr<InputStream>> inputStreams;
//    std::vector<std::unique_ptr<OutputStream>> outputFileStreams;

    const int nStream = inputStreamConfigs.size();
    for (int i = 0; i < nStream; ++i) {
        const auto& inputStreamConfig = inputStreamConfigs.at(i);
        //        for (const auto & inputStreamConfig : inputStreamConfigs) {
        const auto& sensorName = inputStreamConfig.first;
//        const auto& syncSensorName = inputStreamConfig.second;
        //        inputStreams.push_back(std::make_unique<InputStream>(ClientSocket(sensorName, syncSensorName)));
        //        }
        std::vector<Stream::Acquisition> acqs;
        //        //    std::fstream files[2];
        //        for (int i = 0; i < 2; ++i) {
        //        auto file = std::fstream(newRecordFolder + "/" + sensorName + ".txt", std::ios::binary | std::ios::out);
//        Stream::Format format;
//        std::vector<int> dims;
            const auto & filename = newRecordFolder + "/" + sensorName + ".txt";
        if (std::filesystem::exists(filename))
        {
            auto file = std::fstream(filename, std::ios::binary | std::ios::in);
            InputStream inputStream(FileIO(std::move(file)));
            //        assert(file.is_open());

            acqs = inputStream.getAllAcquisition();
//            format = inputStream.getFormat();
//            dims = inputStream.getDims();
        }

        acqs.push_back(frame[i].getAcq().clone());

        // here
        //        outputFileStreams.push_back(std::make_unique<OutputStream>(sensorName, inputStreams[i]->getFormat(), inputStreams[i]->getDims(), FileIO(std::move(file)), inputStreams[i]->getMetaData()));
        auto file = std::fstream(filename, std::ios::binary | std::ios::out);
        OutputStream outputStream(sensorName, frame[i].getFormat(), frame[i].getDims(), FileIO(std::move(file)), {});

        for (const auto & acq : acqs) {
            outputStream << acq;
        }
    }
}

bool Recorder::isRecording() const
{
    return m_isRecording;
}
