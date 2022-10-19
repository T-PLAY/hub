
//#include <cassert>
//#include <filesystem>
//#include <iostream>
//#include <thread>

//#include <Player.h>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>

int main( int argc, char* argv[] ) {

    std::string recordPath = PROJECT_DIR "data/";
    std::fstream recordFile( recordPath + "latest.txt", std::ios::in );
    assert( recordFile.is_open() );

    hub::InputSensor inputSensor( hub::io::File( std::move( recordFile ) ) );

    auto acqs = inputSensor.getAllAcquisitions();

    hub::SensorSpec sensorSpec = inputSensor.m_spec;
//    auto& metaData             = sensorSpec.m_metaData;
//    metaData["type"]           = "record";
//    metaData["nAcqs"]          = (unsigned int)acqs.size();
    hub::OutputSensor outputSensor( sensorSpec, hub::io::OutputStream( "Player (record)" ) );

    for ( const auto& acq : acqs ) {
        outputSensor << acq;
    }

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    hub::OutputSensor outputSensor2( inputSensor.m_spec, hub::io::OutputStream( "Player (live)" ) );

    //    const long long duration = acqs.back().m_start - acqs.front().m_start;
    int iLoop = 0;
    while ( true ) {
        const auto startRecord  = acqs.front().m_start;
        const auto& startChrono = std::chrono::high_resolution_clock::now();

        //        long long dec = iLoop * duration;

        auto it = acqs.begin();
        //            while (it != m_snapshots.end()) {
        while ( it != acqs.end() ) {
            const auto& acq = *it;

            std::this_thread::sleep_until( startChrono +
                                           std::chrono::microseconds( acq.m_start - startRecord ) );
            //            std::this_thread::sleep_for(
            //                                           std::chrono::milliseconds( 1000));

            //                const auto& acq = snapshot.getAcq();
            //                hub::Acquisition acq2 { acq.m_start + dec,
            //                                        acq.m_end + dec
            //                                        };
            //                acq2 << acq.getMeasures();

            //                *m_outputs.at( snapshot.getSensorName() ) << acq2;
            //                    << snapshot.getAcq();
            outputSensor2 << acq;

            ++it;
            //                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1))
            //                == std::future_status::timeout;
        }

        //            if ( !m_isPlaying ) { std::cout << "end record" << std::endl; }
        //            else { std::cout << "auto loop " << iLoop << std::endl; }
        std::cout << "auto loop " << iLoop << std::endl;
        ++iLoop;
    }

    //    std::string recordFolder;
    //    int iSensor = -1;
    //    if (argc == 2) {
    ////        recordFolder = argv[1];
    //        iSensor = atoi(argv[1]);
    //    } else {
    ////        recordFolder = PROJECT_DIR "data/records/latest/";
    //    }
    //        recordFolder = PROJECT_DIR "data/records/latest/";

    //    std::cout << "record folder : " << recordFolder << std::endl;
    //    assert(std::filesystem::exists(recordFolder));

    //    Player player;
    //        player.load(recordFolder, iSensor);
    //        player.play();
    //        std::this_thread::sleep_for(std::chrono::seconds(3600));

    ////    for (int i = 0; i < 10; ++i) {
    ////        player.load(recordFolder);
    ////        player.play();
    //////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ////        player.stop();
    ////        player.unload();
    ////    }
    //    return 0;
}
