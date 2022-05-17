#include "Loader.h"

#include <FileIO.h>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <socket.h>
#include <stream.h>
#include <string>

#include <RamIO.h>

// Loader::Loader(const std::string &path)
//{

//}

// Loader::Loader( const std::string& outputPostFixName )
//     : m_outputPostfixName( outputPostFixName )
//{}

Loader::~Loader()
{
    std::cout << "[Loader] ~Loader" << std::endl;

    //    if (m_thread != nullptr)
    //        stop();

    if (m_loadedPath != "")
        unload();
}

void Loader::load(const std::string& path)
{
    //    assert(!m_isPlaying);
    assert(std::filesystem::exists(path));
    assert(m_loadedPath == "");
    assert(m_snapshots.empty());
    assert(m_frames.empty());
    assert(m_inputStreams.empty());

    //    m_currentFrame = -1;
    assert(!path.empty());
    m_loadedPath = path;
    update();
}

void Loader::update()
{
    m_frames.clear();
    m_inputStreams.clear();
    m_snapshots.clear();

    //    assert(!m_isPlaying);
    assert(m_loadedPath != "");
    assert(m_snapshots.empty());
    assert(m_frames.empty());

    int nAcqs = -1;

    // read records in folder
    for (const auto& fileDir : std::filesystem::directory_iterator(m_loadedPath)) {
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

        //        try {
        //        m_inputStream = new InputStream(FileIO(std::move(file)));
        //        m_inputStreams.push_back(new InputStream(FileIO(std::move(file))));
        //        m_inputStreams.emplace_back(FileIO(std::move(file)));

        //        m_inputStreams.push_back(std::make_unique<InputStream>(ClientSocket()));
        m_inputStreams.push_back(std::make_unique<InputStream>(FileIO(std::move(file))));

        //        m_inputStreams.emplace_back(FileIO(std::move(file)));
        //        m_inputStreams.emplace_back(ClientSocket());

        auto& inputStream = *m_inputStreams.back();

        auto acqs = inputStream.getAllAcquisition();
        const std::string& sensorName = inputStream.getSensorName();

        if (nAcqs == -1) {
            nAcqs = acqs.size();
            m_frames.reserve(nAcqs);
            m_frames.resize(nAcqs);
        }
        assert(acqs.size() == nAcqs);

        int iAcq = 0;
        for (const auto& acq : acqs) {
            //                Snapshot && snapshot{std::move(acq), sensorName};
            //                m_frames[iAcq].m_snapshots.emplace_back(std::move(acq), sensorName);
            //                m_snapshots.insert(std::move(snapshot));
            //                m_snapshots.insert(&m_frames[iAcq].m_snapshots.back());

            //                const Snapshot & snapshot2 = m_snapshots.find
            //                Snapshot snapshot { acq.clone(), sensorName };
            //                m_snapshots.insert(std::move(snapshot));
            //                m_snapshots.insert(Snapshot { acq.clone(), sensorName });
            Snapshot snapshot(inputStream, acq);
            m_snapshots.insert(std::move(snapshot));
            //                m_snapshots.insert(Snapshot(sensorName, inputStream.getFormat(),
            //                inputStream.getDims(), acq));

            //                const auto& it = m_snapshots.find(Snapshot { acq.clone(), sensorName
            //                });
            Snapshot snapshot2(inputStream, acq);
            const auto& it = m_snapshots.find(snapshot2);
            assert(it != m_snapshots.end());
            m_frames[iAcq].push_back(*it);

            ++iAcq;
        }

        // here
        //            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName + "
        //            (record)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(),
        //            inputStream.getMetaData());

        //            if (m_outputs.find(sensorName) == m_outputs.end()) {
        //                m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName +
        //                m_outputPostfixName, inputStream.getFormat(), inputStream.getDims(),
        //                ClientSocket(), inputStream.getMetaData());
        //            }

        //            m_outputs[sensorName] = std::make_unique<OutputStream>(sensorName + "
        //            (physical)", inputStream.getFormat(), inputStream.getDims(), ClientSocket(),
        //            inputStream.getMetaData());

        //            int nReadAcqs = 0;
        //            Stream::Acquisition acq;
        //            while (true) {
        //                inputStream >> acq;
        //                std::cout << "read acquisition : " << acq << std::endl;
        //                m_snapshots.insert(Snapshot { std::move(acq), sensorName });
        //                ++nReadAcqs;
        //            }
        //            std::cout << "read " << nReadAcqs << " acquisitions from file sensor '" <<
        //            sensorName << "'" << std::endl;
        std::cout << "read " << acqs.size() << " acquisitions from file sensor '" << sensorName
                  << "'" << std::endl;
        //        } catch (Stream::exception& e) {
        //            std::cout << "catch stream exception : " << e.what() << std::endl;
        //            throw;
        //        } catch (std::exception& e) {
        //            std::cout << "catch exception : " << e.what() << std::endl;
        //        }

        //        file.close();
    }
    std::cout << "read total of " << m_snapshots.size() << " acquistions" << std::endl;

    //    //    auto & listModel = ui->listView_acqs->model();
    //    //    auto & view = ui->tableView_acqs;
    //    //    listView->model().re

    QStringList stringList;
    for (int i = 0; i < m_frames.size(); ++i) {
        //        ui->listView_acqs.add
        //        view->addAction((std::string("Frame ") + std::to_string(i)).c_str());
        stringList << ("Frame " + std::to_string(i)).c_str();
    }
    m_frameModel.setStringList(stringList);

    assert(m_outputStreams.empty());
    for (const auto & inputStream : m_inputStreams) {
        const auto & sensorName = inputStream->getSensorName();
//        OutputStream && ramOutputStream = OutputStream(inputStream->getSensorName(), inputStream->getFormat(), inputStream->getDims(), RamIO());
//        CyclicBuff buff;
        m_outputStreamBuffs[sensorName] = CyclicBuff();
        auto & cyclicBuff = m_outputStreamBuffs.at(sensorName);
        m_outputStreams[inputStream->getSensorName()] = std::make_unique<OutputStream>(inputStream->getSensorName(), inputStream->getFormat(), inputStream->getDims(), RamIO(cyclicBuff));
    }
    //        ui->listView_recordFrames->show();
    //        view->show();
    emit pathLoaded();
}

void Loader::unload()
{
    //    assert(!m_isPlaying);

    //    assert(!m_outputs.empty());
    assert(isLoaded());
    assert(!m_snapshots.empty());
    assert(!m_frames.empty());
    assert(!m_inputStreams.empty());
    //    for (auto & output : m_outputs) {
    //        delete output.second.get();
    ////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //    }
    //    m_outputs.clear();
    m_snapshots.clear();
    m_frames.clear();
    m_inputStreams.clear();
    m_frameModel.setStringList(QStringList());
    m_loadedPath = "";
}

void Loader::onFrame_selectionChange(const QModelIndexList &selectedRows)
{
//    const auto & frames = m_recordLoader.getFrames();
    std::cout << "[Loader] onFrame_selectionChange " << std::endl;

    for ( const QModelIndex index : selectedRows ) {
//        m_selectedRecordFrames.push_back(m_frames[index.row()]);

        for (const auto & snapshot : m_frames[index.row()] ) {
            const auto & sensorName = snapshot.getSensorName();

            const auto & acq = snapshot.getAcq();
            std::cout << "send acq : " << acq << std::endl;

            assert(m_outputStreams.find(sensorName) != m_outputStreams.end());
            *m_outputStreams.at(sensorName) << acq;
        }

    }


}

//void Loader::onFrame_selectionChange(const QItemSelection &selectedRows, const QItemSelection &deselected)
//{
//    const auto & selectedRows = ui->listView_recordFrames->selectionModel()->selectedRows();

//    std::cout << "[FormWidgetLoader] on_listView_recordFrames_selectionChanged : "
//              << selectedRows.size() << std::endl;

//    m_selectedRecordFrames.clear();
//    m_selectedRecordFrames.reserve(selectedRows.size());

//    const auto & frames = m_recordLoader.getFrames();

//    for ( QModelIndex index : selectedRows ) {
//        m_selectedRecordFrames.push_back(frames[index.row()]);
//    }

//    emit recordFrames_selectionChanged();

//}

const std::map<std::string, CyclicBuff> &Loader::getOutputStreamBuffs() const
{
    return m_outputStreamBuffs;
}

// const std::vector<std::unique_ptr<InputStream>> &Loader::getInputStreams() const
//{
//     return m_inputStreams;
// }

QStringListModel& Loader::getFrameModel()
{
    return m_frameModel;
}

//const std::map<std::string, std::unique_ptr<OutputStream> > &Loader::getRamOutputStreams() const
//{
////    std::vector<std::unique_ptr<OutputStream>> ramOutputStreams;

//    return m_ramOutputStreams;
////    return ramOutputStreams;
//}

//std::vector<std::unique_ptr<InputStream>> Loader::getInputStreams()
//{
//    std::vector<std::unique_ptr<InputStream>> inputStreams;

//    for (const auto & inputStream : m_inputStreams) {

//    }

//    return inputStreams;
//}

// void Loader::play()
//{
//    //    m_futureObj = m_exitSignal.get_future();
//    std::cout << "start playing" << std::endl;
//    assert(m_thread == nullptr);
//    assert(!m_frames.empty());
//    assert(!m_snapshots.empty());

//    m_thread = new std::thread([this]() {
//        // play
//        int iLoop = 0;
//        //        bool exitSignal = false;
//        while (m_isPlaying) {
//            const auto startRecord = m_snapshots.begin()->getAcq().mBackendTimestamp;
//            const auto& startChrono = std::chrono::high_resolution_clock::now();

//            auto it = m_snapshots.begin();
//            //            while (it != m_snapshots.end()) {
//            while (m_isPlaying && it != m_snapshots.end()) {
//                const auto& snapshot = *it;

//                std::this_thread::sleep_until(startChrono +
//                std::chrono::microseconds(snapshot.getAcq().mBackendTimestamp - startRecord));
//                *m_outputs.at(snapshot.getSensorName()) << snapshot.getAcq();

//                ++it;
//                //                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1))
//                == std::future_status::timeout;
//            }

//            if (!m_isPlaying) {
//                std::cout << "end record, auto loop " << iLoop << std::endl;
//            }
//            ++iLoop;
//        }
//    });
//    m_isPlaying = true;
//}

// void Loader::stop()
//{
//    assert(m_isPlaying);
//    std::cout << "stop playing" << std::endl;
//    //    m_exitSignal.set_value();
//    m_isPlaying = false;
//    assert(m_thread != nullptr);
//    m_thread->join();
//    delete m_thread;
//    m_thread = nullptr;
//    //    m_exitSignal.swap(std::promise<void>());
//}

// void Loader::showFrame(int iFrame)
//{
//    assert(!m_isPlaying);
//    assert(m_loadedPath != "");
//    assert(!m_snapshots.empty());
//    assert(!m_frames.empty());
//    assert(m_currentFrame != iFrame);
//    assert(iFrame < m_frames.size());

//    const auto& frame = m_frames[iFrame];
//    for (const auto& snapshot : frame) {
//        *m_outputs.at(snapshot.getSensorName()) << snapshot.getAcq();
//    }

//    m_currentFrame = iFrame;
//}

// void Loader::setOutputPostFixName(const std::string& outputPostFixName)
//{
//    m_outputPostfixName = outputPostFixName;
//}

const std::vector<Frame>& Loader::getFrames() const
{
    assert(isLoaded());
    return m_frames;
}

// bool Loader::isPlaying() const
//{
//    return m_isPlaying;
//}

bool Loader::isLoaded() const
{
    return m_loadedPath != "";
}

const std::string& Loader::getLoadedPath() const
{
    assert(isLoaded());
    return m_loadedPath;
}

// int Loader::getCurrentFrame() const {
//     assert( isLoaded() );
////    return m_currentFrame;
//}
