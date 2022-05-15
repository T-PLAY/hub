#include "Player.h"

#include <FileIO.h>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <socket.h>
#include <stream.h>
#include <string>

// Player::Player(const std::string &path)
//{

//}

Player::~Player()
{
    std::cout << "[Player] ~Player" << std::endl;

    if (m_thread != nullptr)
        stop();
}

void Player::load(const std::string& path)
{
    assert(! m_isPlaying);
    assert(m_loadedPath == "");

    // read records in folder
    for (const auto& fileDir : std::filesystem::directory_iterator(path)) {
        const auto filename = fileDir.path().string();
        std::cout << "read '" << filename << "' record" << std::endl;
        assert(std::filesystem::exists(filename));

        std::fstream file(filename, std::ios::binary | std::ios::in);
        //        assert(file.is_open());
        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        file.seekg(0, std::ios::end);
        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        file.seekg(0, std::ios::beg);

        assert(!file.eof());
        //        assert(sizeof(int) == 4);

        try {
            InputStream inputStream(FileIO(std::move(file)));
            auto acqs = inputStream.getAllAcquisition();
            const std::string& sensorName = inputStream.getSensorName();

            for (auto& acq : acqs) {
                m_snapshots.insert(Snapshot { std::move(acq), sensorName });
            }

            // here
//            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName + " (record)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(), inputStream.getMetaData());
            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName, inputStream.getFormat(), inputStream.getDims(), ClientSocket(), inputStream.getMetaData());
//            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName + " (physical)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(), inputStream.getMetaData());

            //            int nReadAcqs = 0;
            //            Stream::Acquisition acq;
            //            while (true) {
            //                inputStream >> acq;
            //                std::cout << "read acquisition : " << acq << std::endl;
            //                m_snapshots.insert(Snapshot { std::move(acq), sensorName });
            //                ++nReadAcqs;
            //            }
            //            std::cout << "read " << nReadAcqs << " acquisitions from file sensor '" << sensorName << "'" << std::endl;
            std::cout << "read " << acqs.size() << " acquisitions from file sensor '" << sensorName << "'" << std::endl;
        } catch (Stream::exception& e) {
            std::cout << "catch stream exception : " << e.what() << std::endl;
            throw;

        } catch (std::exception& e) {
            std::cout << "catch exception : " << e.what() << std::endl;
        }

//        file.close();
    }
    std::cout << "read total of " << m_snapshots.size() << " acquistions" << std::endl;

    m_loadedPath = path;
}

void Player::unload()
{
    assert(! m_isPlaying);

    assert(! m_outputs.empty());
    assert(! m_snapshots.empty());
    assert(m_loadedPath != "");
//    for (auto & output : m_outputs) {
//        delete output.second.get();
////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//    }
    m_outputs.clear();
    m_snapshots.clear();
    m_loadedPath = "";
}

void Player::play()
{
//    m_futureObj = m_exitSignal.get_future();
    std::cout << "start playing" << std::endl;
    assert(m_thread == nullptr);
    m_thread = new std::thread([this]() {
        // play
        int iLoop = 0;
//        bool exitSignal = false;
        while (m_isPlaying) {
            const auto startRecord = m_snapshots.begin()->mAcq.mBackendTimestamp;
            const auto& startChrono = std::chrono::high_resolution_clock::now();

            auto it = m_snapshots.begin();
            //            while (it != m_snapshots.end()) {
            while (m_isPlaying && it != m_snapshots.end()) {
                const auto& snapshot = *it;

                std::this_thread::sleep_until(startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord));
                *m_outputs.at(snapshot.mSensorName) << snapshot.mAcq;

                ++it;
//                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout;
            }

            if (!m_isPlaying) {
                std::cout << "end record, auto loop " << iLoop << std::endl;
            }
            ++iLoop;
        }
    });
    m_isPlaying = true;
}

void Player::stop()
{
    assert(m_isPlaying);
    std::cout << "stop playing" << std::endl;
//    m_exitSignal.set_value();
    m_isPlaying = false;
    assert(m_thread != nullptr);
    m_thread->join();
    delete  m_thread;
    m_thread = nullptr;
//    m_exitSignal.swap(std::promise<void>());
}

const std::string& Player::getLoadedPath() const
{
    return m_loadedPath;
}
