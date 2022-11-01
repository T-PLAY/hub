#include "Recorder.h"

//#include <filesystem>
//#include <fstream>

//#include <IO/File.hpp>
//#include <OutputSensor.hpp>
//#include <IO/Stream.hpp>

//// Recorder::Recorder(const std::string& rootPath)
////     : m_rootPath(rootPath)
////{
//// }

//// Recorder::Recorder(std::string rootPath)
////     : m_rootPath(std::move(rootPath))
////{
//// }

// Recorder::Recorder(const char* rootPath)
//     : m_rootPath(rootPath)
//{
// }

// void Recorder::record(const InputSensorParameters& inputSensorConfigs)
//{
//     assert(!inputSensorConfigs.empty());
//     std::cout << "[Recorder] record()" << std::endl;
//     assert(!m_isRecording);
////    assert(m_thread == nullptr);
//    assert(m_threads.empty());

//    std::vector<std::unique_ptr<hub::InputSensor>> inputSensors;
//    std::vector<std::unique_ptr<hub::OutputSensor>> outputFileStreams;

//    // creating new record folder
//    char folderName[64] = { 0 };
//    time_t rawtime;
//    time(&rawtime);
//    const auto timeinfo = localtime(&rawtime);
//    strftime(folderName, sizeof(folderName), "%Y-%m-%d_%H-%M-%S", timeinfo);
//    std::string newRecordFolder = m_rootPath + "records/" + folderName;
//    std::cout << "create directory " << newRecordFolder << std::endl;
//    assert(!std::filesystem::exists(newRecordFolder));
//    std::filesystem::create_directories(newRecordFolder);

//    const auto nStream = inputSensorConfigs.size();
//    for (int i = 0; i < nStream; ++i) {
//        const auto& inputSensorConfig = inputSensorConfigs.at(i);
//        //        for (const auto & inputSensorConfig : inputSensorConfigs) {
//        const auto& sensorName = inputSensorConfig.first;
//        const auto& syncSensorName = inputSensorConfig.second;
//        inputSensors.push_back(std::make_unique<hub::InputSensor>(hub::io::InputStream(sensorName,
//        syncSensorName)));
//        //        }

//        //        //    std::fstream files[2];
//        //        for (int i = 0; i < 2; ++i) {
//        auto fileName = sensorName;
//        if (syncSensorName != "")
//            fileName += "&" + syncSensorName;
//        auto file = std::fstream(newRecordFolder + "/" + fileName + ".txt", std::ios::binary |
//        std::ios::out); assert(file.is_open());

//        // here
//        outputFileStreams.push_back( std::make_unique<hub::OutputSensor>( inputSensors[i]->m_spec,
//            hub::io::File( std::move( file ) )
//             ) );
//    }

//    m_threads.resize(nStream);
//    for (int i = 0; i <nStream; ++i) {
//    m_threads[i] = new std::thread([this, inputSensor = std::move(inputSensors[i]),
//    outputFileStream = std::move(outputFileStreams[i]), newRecordFolder =
//    std::move(newRecordFolder), nStream]() {
//        try {

//            m_isRecording = true;
//            while (m_isRecording) {
////                Stream::hub::Acquisition acq;
//                for (int i = 0; i < nStream; ++i) {
////                    *inputSensor >> acq;
//                    auto acq = inputSensor->getAcquisition();
//                    std::cout << "[" << i << "] read/write acq : " << acq << std::endl;
//                    *outputFileStream << acq;

//                    //                *inputSensors[1] >> acq;
//                    //                std::cout << "[" << 1 << "] read/write acq : " << acq <<
//                    std::endl;
//                    //                *outputFileStreams[1] << acq;
//                }
//            }

//        } catch (std::exception& e) {
//            std::cout << "catch exception : " << e.what() << std::endl;
//            assert(!m_isRecording);
//            m_isRecording = true;
//        }

//        //    for (int i = 0; i < 2; ++i) {
//        //        files[i].close();
//        //    }

//        // copy this record into latest folder
//        //        {
//        //            std::string latestFolder = m_rootPath + "records/latest/";
//        //            std::filesystem::create_directories(latestFolder);
//        //            for (const auto& dirEntry :
//        std::filesystem::directory_iterator(latestFolder)) {
//        //                std::filesystem::remove(dirEntry);
//        //            }
//        //            std::filesystem::copy(newRecordFolder, latestFolder,
//        std::filesystem::copy_options::recursive);
//        //        }
//    });
//    }
//}

// void Recorder::stop()
//{
//     std::cout << "[Recorder] stop" << std::endl;
//     assert(m_isRecording == true);
//     m_isRecording = false;

//    if (m_snapshots.empty()) {

//        for (auto * thread : m_threads) {
//        thread->join();
//        delete thread;
//        thread = nullptr;
//        }
//        m_threads.clear();

//    } else {
//        saveOnDisk();
//    }
//}

// void Recorder::save(const Frame& frame)
//{
//     assert(!m_isRecording);

//    InputSensorParameters inputSensorConfigs;

//    std::string folderName = "";
//    auto it = frame.begin();
//    while (it != frame.end()) {
//        //    for (const auto & snapshot : frame.m_snapshots) {
//        const auto& snapshot = *it;

//        folderName += snapshot.getSensorName();

//        inputSensorConfigs.push_back({ snapshot.getSensorName(), "" });

//        ++it;
//        if (it != frame.end())
//            folderName += "_";
//    }

//    std::string newRecordFolder = m_rootPath + "snapshots/" + folderName;
//    std::cout << "create directory " << newRecordFolder << std::endl;
//    //    assert(!std::filesystem::exists(newRecordFolder));
//    std::filesystem::create_directories(newRecordFolder);

//    //    std::vector<std::unique_ptr<hub::InputSensor>> inputSensors;
//    //    std::vector<std::unique_ptr<hub::OutputSensor>> outputFileStreams;

//    const auto nStream = inputSensorConfigs.size();
//    for (int i = 0; i < nStream; ++i) {
//        const auto& inputSensorConfig = inputSensorConfigs.at(i);
//        //        for (const auto & inputSensorConfig : inputSensorConfigs) {
//        const auto& sensorName = inputSensorConfig.first;
//        //        const auto& syncSensorName = inputSensorConfig.second;
//        // inputSensors.push_back(std::make_unique<hub::InputSensor>(hub::ClientSocket(sensorName,
//        syncSensorName)));
//        //        }
//        std::vector<hub::Acquisition> acqs;
//        //        //    std::fstream files[2];
//        //        for (int i = 0; i < 2; ++i) {
//        //        auto file = std::fstream(newRecordFolder + "/" + sensorName + ".txt",
//        std::ios::binary | std::ios::out);
//        //        Stream::Format format;
//        //        std::vector<int> dims;
//        const auto& filename = newRecordFolder + "/" + sensorName + ".txt";
//        if (std::filesystem::exists(filename)) {
//            auto file = std::fstream(filename, std::ios::binary | std::ios::in);
//            hub::InputSensor inputSensor(hub::io::File(std::move(file)));
//            //        assert(file.is_open());

//            acqs = inputSensor.getAllAcquisitions();
//            //            format = inputSensor.getFormat();
//            //            dims = inputSensor.getDims();
//        }

//        acqs.push_back(frame[i].getAcq().clone());

//        // here
//        //        outputFileStreams.push_back(std::make_unique<hub::OutputSensor>(sensorName,
//        inputSensors[i]->getFormat(), inputSensors[i]->getDims(), hub::File(std::move(file)),
//        inputSensors[i]->getMetaData())); auto file = std::fstream(filename, std::ios::binary |
//        std::ios::out); hub::SensorSpec header( sensorName, frame[i].getFormat(),
//        frame[i].getDims(), {} ); hub::OutputSensor outputSensor(header,
//        hub::io::File(std::move(file)));
//        //hub::OutputSensor outputSensor( { sensorName, frame[i].getFormat(), frame[i].getDims(),
//        {} },
//        //                           hub::File( std::move( file ) ) );

//        for (const auto& acq : acqs) {
//            outputSensor << acq;
//        }
//    }
//}

//// void Recorder::add(const std::string &streamName, const Stream::hub::Acquisition &acq)
////{
////     m_snapshots[streamName].push_back(acq.clone());
//// }

// void Recorder::saveOnDisk()
//{
//     // creating new record folder
//     char folderName[64] = { 0 };
//     time_t rawtime;
//     time(&rawtime);
//     const auto timeinfo = localtime(&rawtime);
//     strftime(folderName, sizeof(folderName), "%Y-%m-%d_%H-%M-%S", timeinfo);
//     std::string newRecordFolder = m_rootPath + "records/" + folderName;
//     std::cout << "create directory " << newRecordFolder << std::endl;
//     assert(!std::filesystem::exists(newRecordFolder));
//     std::filesystem::create_directories(newRecordFolder);

//    auto min = m_snapshots.begin()->second.size();
//    for (const auto& pair : m_snapshots) {
//        min = std::min(pair.second.size(), min);
//    }

//    //    for (int i = 0; i < nStream; ++i) {
//    for (const auto& pair : m_snapshots) {
//        //        const auto& inputSensorConfig = inputSensorConfigs.at(i);
//        //        for (const auto & inputSensorConfig : inputSensorConfigs) {
//        const auto& sensorName = pair.first;
//        const auto& snapshots = pair.second;
//        //        const auto& syncSensorName = inputSensorConfig.second;
//        // inputSensors.push_back(std::make_unique<hub::InputSensor>(hub::ClientSocket(sensorName,
//        syncSensorName)));
//        //        }

//        //        //    std::fstream files[2];
//        //        for (int i = 0; i < 2; ++i) {
//        auto file = std::fstream(newRecordFolder + "/" + sensorName + ".txt", std::ios::binary |
//        std::ios::out); assert(file.is_open());

//        const auto& firstSnapshot = snapshots.front();
//        hub::OutputSensor outputSensor(
//            { sensorName, firstSnapshot.getFormat(), firstSnapshot.getDims() },
//            hub::io::File( std::move( file ) ) );

//        //        for (const auto& snapshot : snapshots) {
//        for (int i = 0; i < min; ++i) {
//            outputSensor << snapshots[i].getAcq();
//        }
//        // here
//    }
//    m_snapshots.clear();
//}

// void Recorder::add(Snapshot&& snapshot)
//{
//     assert(m_isRecording);
//     //    m_snapshots.push_back(std::move(snapshot));
//     m_snapshots[snapshot.getSensorName()].push_back(std::move(snapshot));
// }

// void Recorder::record()
//{
//     m_isRecording = true;
// }

// bool Recorder::isRecording() const
//{
//     return m_isRecording;
// }
