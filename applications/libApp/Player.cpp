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

void Player::load(const std::string& path)
{
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
            InputStream inputStream(FileIO { file });
            auto acqs = inputStream.getAllAcquisition();
            const std::string& sensorName = inputStream.getSensorName();

            for (auto& acq : acqs) {
                m_snapshots.insert(Snapshot { std::move(acq), sensorName });
            }

            // here
            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName + " (record)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(), inputStream.getMetaData());

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

        file.close();
    }
    std::cout << "read total of " << m_snapshots.size() << " acquistions" << std::endl;
}

void Player::unload()
{
    m_outputs.clear();
    m_snapshots.clear();
    m_loadedPath = "";
}

void Player::play()
{
    std::cout << "start playing" << std::endl;
    m_thread = std::thread([this]() {
        // play
        int iLoop = 0;
        bool exitSignal = false;
        while (!exitSignal) {
            const auto startRecord = m_snapshots.begin()->mAcq.mBackendTimestamp;
            const auto& startChrono = std::chrono::high_resolution_clock::now();

            auto it = m_snapshots.begin();
            //            while (it != m_snapshots.end()) {
            while (!exitSignal && it != m_snapshots.end()) {
                const auto& snapshot = *it;

                std::this_thread::sleep_until(startChrono + std::chrono::microseconds(snapshot.mAcq.mBackendTimestamp - startRecord));
                *m_outputs.at(snapshot.mSensorName) << snapshot.mAcq;

                ++it;
                exitSignal = m_futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout;
            }

            if (!exitSignal) {
                std::cout << "end record, auto loop " << iLoop << std::endl;
            }
            ++iLoop;
        }
    });
}

void Player::stop()
{
    std::cout << "stop playing" << std::endl;
    m_exitSignal.set_value();
    m_thread.join();
}

const std::string& Player::getLoadedPath() const
{
    return m_loadedPath;
}
