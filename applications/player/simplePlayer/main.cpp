
//#include <cassert>
//#include <filesystem>
//#include <iostream>
//#include <thread>

//#include <Player.h>

//#include <InputSensor.hpp>
//#include <OutputSensor.hpp>

//#include <IO/File.hpp>
//#include <IO/Stream.hpp>

//#include <filesystem>
//#include <set>
#include <Configurations.hpp>
#include <Loader.h>

int main( int argc, char* argv[] ) {
    std::string recordPath = PROJECT_DIR "data/records/latest/";

    Loader loader;
    loader.load( recordPath );

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    return 0;
}

// struct Snap {
//     hub::Acquisition m_acq;
//     int iStream;
////    std::string m_streamName;

//    bool operator<(const Snap& other) const
//    {
//        return m_acq.m_start < other.m_acq.m_start;
//    }
//};

// int main(int argc, char* argv[])
//{

////    std::map<std::string, std::unique_ptr<hub::OutputSensor>> outputStreams;
//    std::vector<std::unique_ptr<hub::OutputSensor>> outputStreams;
////    std::vector<hub::OutputSensor> outputStreams;
//    outputStreams.reserve(10);
//    std::set<Snap> snaps;

//    std::string recordPath = PROJECT_DIR "data/records/latest/";
//    //    std::vector<std::string> streamNames { "Keyboard", "ProceduralStreamer" };
//    //    std::vector<std::string> streamNames { "Polhemus Patriot (sensor 2)" };
//    //    const std::string streamName = "Keyboard";

//    //    for (const auto& streamName : streamNames) {
//    //        std::fstream recordFile(recordPath + streamName + ".txt", std::ios::in |
//    std::ios::binary | std::ios::beg);
//    //        assert(recordFile.is_open());

//    for (const auto& fileDir : std::filesystem::directory_iterator(recordPath)) {
//        //        if ( iSensor != -1 && iFile != iSensor ) continue;

//        const auto filename = fileDir.path().string();
//        std::cout << "read '" << filename << "' record" << std::endl;
//        assert(std::filesystem::exists(filename));

////        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::beg);
//        std::fstream file(filename, std::ios::binary | std::ios::in );
//        assert(file.is_open());
//        //        //        std::cout << "tellg" << file.tellg() << std::endl;
//        //        //        file.seekg(0, std::ios::end);
//        //        //        std::cout << "tellg" << file.tellg() << std::endl;
//        //        //        file.seekg(0, std::ios::beg);

//        //        assert( !file.eof() );
//        //        //        assert(sizeof(int) == 4);

//        const std::string streamName = filename;

//        hub::InputSensor inputSensor(hub::io::File(std::move(file)));
//        //        hub::InputSensor inputSensor(hub::io::File(std::move(recordFile)));

//        auto acqs = inputSensor.getAllAcquisitions();

//        //        hub::OutputSensor outputSensor2(inputSensor.m_spec,
//        hub::io::OutputStream("Player (" + streamName + ")"));
////        outputStreams[streamName] = std::make_unique<hub::OutputSensor>(inputSensor.m_spec,
///hub::io::OutputStream("Player (" + streamName + ")"));
//        outputStreams.push_back(std::make_unique<hub::OutputSensor>(inputSensor.m_spec,
//        hub::io::OutputStream("Player : " + inputSensor.m_spec.m_sensorName)));
////        outputStreams.emplace_back(inputSensor.m_spec, hub::io::OutputStream("Player (" +
///streamName + ")"));

//        for (const auto& acq : acqs) {
//            //            std::cout << "read acq : " << acq << std::endl;
//            Snap snap { acq.clone(), (int)(outputStreams.size() - 1) };
//            snaps.insert(std::move(snap));
//        }
//        std::cout << acqs.size() << " acquisitions read." << std::endl;
//    }

//    //    hub::SensorSpec sensorSpec = inputSensor.m_spec;
//    //    auto& metaData             = sensorSpec.m_metaData;
//    //    metaData["type"]           = "record";
//    //    metaData["nAcqs"]          = (unsigned int)acqs.size();

//    //    hub::OutputSensor outputSensor( inputSensor.m_spec, hub::io::OutputStream( "Player
//    (record)" ) );

//    //    for ( const auto& acq : acqs ) {
//    //        outputSensor << acq;
//    //    }

//    //        while (true) {
//    //            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    //        }

//    //    const long long duration = acqs.back().m_start - acqs.front().m_start;
//    int iLoop = 0;
//    while (true) {
//        //        const auto startRecord = acqs.front().m_start;
//        const auto startRecord = snaps.begin()->m_acq.m_start;
//        const auto& startChrono = std::chrono::high_resolution_clock::now();

//        //        long long dec = iLoop * duration;

//        auto it = snaps.begin();
//        //            while (it != m_snapshots.end()) {
//        while (it != snaps.end()) {
//            const auto& snap = *it;

//            std::this_thread::sleep_until(startChrono +
//            std::chrono::microseconds(snap.m_acq.m_start - startRecord));
//            //            std::this_thread::sleep_for(
//            //                                           std::chrono::milliseconds( 1000));

//            //                const auto& acq = snapshot.getAcq();
//            //                hub::Acquisition acq2 { acq.m_start + dec,
//            //                                        acq.m_end + dec
//            //                                        };
//            //                acq2 << acq.getMeasures();

//            //                *m_outputs.at( snapshot.getSensorName() ) << acq2;
//            //                    << snapshot.getAcq();
//            //            outputSensor2 << snap;
//            *outputStreams.at(snap.iStream) << snap.m_acq;
////            outputStreams.at(snap.iStream) << snap.m_acq;

//            ++it;
//            //                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1))
//            //                == std::future_status::timeout;
//        }

//        //            if ( !m_isPlaying ) { std::cout << "end record" << std::endl; }
//        //            else { std::cout << "auto loop " << iLoop << std::endl; }
//        std::cout << "auto loop " << iLoop << std::endl;
//        ++iLoop;
//    }

//    //    std::string recordFolder;
//    //    int iSensor = -1;
//    //    if (argc == 2) {
//    ////        recordFolder = argv[1];
//    //        iSensor = atoi(argv[1]);
//    //    } else {
//    ////        recordFolder = PROJECT_DIR "data/records/latest/";
//    //    }
//    //        recordFolder = PROJECT_DIR "data/records/latest/";

//    //    std::cout << "record folder : " << recordFolder << std::endl;
//    //    assert(std::filesystem::exists(recordFolder));

//    //    Player player;
//    //        player.load(recordFolder, iSensor);
//    //        player.play();
//    //        std::this_thread::sleep_for(std::chrono::seconds(3600));

//    ////    for (int i = 0; i < 10; ++i) {
//    ////        player.load(recordFolder);
//    ////        player.play();
//    //////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//    ////        player.stop();
//    ////        player.unload();
//    ////    }
//    //    return 0;
//}
