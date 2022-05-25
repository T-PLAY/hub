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
//    assert(m_thread == nullptr);
    assert(m_threads.empty());

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
        auto fileName = sensorName;
        if (syncSensorName != "")
            fileName += "&" + syncSensorName;
        auto file = std::fstream(newRecordFolder + "/" + fileName + ".txt", std::ios::binary | std::ios::out);
        assert(file.is_open());

        // here
        outputFileStreams.push_back(std::make_unique<OutputStream>(sensorName, inputStreams[i]->getFormat(), inputStreams[i]->getDims(), FileIO(std::move(file)), inputStreams[i]->getMetaData()));
    }

    m_threads.resize(nStream);
    for (int i = 0; i <nStream; ++i) {
    m_threads[i] = new std::thread([this, inputStream = std::move(inputStreams[i]), outputFileStream = std::move(outputFileStreams[i]), newRecordFolder = std::move(newRecordFolder), nStream]() {
        try {

            m_isRecording = true;
            while (m_isRecording) {
//                Stream::Acquisition acq;
                for (int i = 0; i < nStream; ++i) {
//                    *inputStream >> acq;
                    auto acq = inputStream->getAcquisition();
                    std::cout << "[" << i << "] read/write acq : " << acq << std::endl;
                    *outputFileStream << acq;

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
}

void Recorder::stop()
{
    std::cout << "[Recorder] stop" << std::endl;
    assert(m_isRecording == true);
    m_isRecording = false;

    if (m_snapshots.empty()) {

        for (auto * thread : m_threads) {
        thread->join();
        delete thread;
        thread = nullptr;
        }
        m_threads.clear();

    } else {
        saveOnDisk();
    }
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
        const auto& filename = newRecordFolder + "/" + sensorName + ".txt";
        if (std::filesystem::exists(filename)) {
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

        for (const auto& acq : acqs) {
            outputStream << acq;
        }
    }
}

// void Recorder::add(const std::string &streamName, const Stream::Acquisition &acq)
//{
//     m_snapshots[streamName].push_back(acq.clone());
// }

void Recorder::saveOnDisk()
{
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

    auto min = m_snapshots.begin()->second.size();
    for (const auto& pair : m_snapshots) {
        min = std::min(pair.second.size(), min);
    }

    //    for (int i = 0; i < nStream; ++i) {
    for (const auto& pair : m_snapshots) {
        //        const auto& inputStreamConfig = inputStreamConfigs.at(i);
        //        for (const auto & inputStreamConfig : inputStreamConfigs) {
        const auto& sensorName = pair.first;
        const auto& snapshots = pair.second;
        //        const auto& syncSensorName = inputStreamConfig.second;
        //        inputStreams.push_back(std::make_unique<InputStream>(ClientSocket(sensorName, syncSensorName)));
        //        }

        //        //    std::fstream files[2];
        //        for (int i = 0; i < 2; ++i) {
        auto file = std::fstream(newRecordFolder + "/" + sensorName + ".txt", std::ios::binary | std::ios::out);
        assert(file.is_open());

        const auto& firstSnapshot = snapshots.front();
        OutputStream outputStream(sensorName, firstSnapshot.getFormat(), firstSnapshot.getDims(), FileIO(std::move(file)));

        //        for (const auto& snapshot : snapshots) {
        for (int i = 0; i < min; ++i) {
            outputStream << snapshots[i].getAcq();
        }
        // here
    }
    m_snapshots.clear();
}

void Recorder::add(Snapshot&& snapshot)
{
    assert(m_isRecording);
    //    m_snapshots.push_back(std::move(snapshot));
    m_snapshots[snapshot.getSensorName()].push_back(std::move(snapshot));
}

void Recorder::record()
{
    m_isRecording = true;
}

bool Recorder::isRecording() const
{
    return m_isRecording;
}
