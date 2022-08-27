#include "Player.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <string>

#include "IO/File.hpp"
//#include "Sensor.hpp"
//#include "Socket.hpp"
#include <Net/ClientSocket.hpp>
#include <IO/Stream.hpp>

// Player::Player(const std::string &path)
//{

//}

Player::Player( const std::string& outputPostFixName ) : m_outputPostfixName( outputPostFixName ) {}

Player::~Player() {
    std::cout << "[Player] ~Player" << std::endl;

    if ( m_thread != nullptr ) stop();

    if ( m_loadedPath != "" ) unload();
}

void Player::load( const std::string& path, int iSensor ) {
    assert( !m_isPlaying );
    assert( m_loadedPath == "" );
    assert( m_snapshots.empty() );
    assert( m_frames.empty() );

    m_currentFrame = -1;
    assert( !path.empty() );
    m_loadedPath = path;
    update( iSensor );
}

void Player::update( int iSensor ) {
    m_frames.clear();
    m_snapshots.clear();

    assert( !m_isPlaying );
    assert( m_loadedPath != "" );
    assert( m_snapshots.empty() );
    assert( m_frames.empty() );

    int nAcqs = -1;

    int iFile = 0;
    // read records in folder
    for ( const auto& fileDir : std::filesystem::directory_iterator( m_loadedPath ) ) {
        if ( iSensor != -1 && iFile != iSensor ) continue;

        const auto filename = fileDir.path().string();
        std::cout << "read '" << filename << "' record" << std::endl;
        assert( std::filesystem::exists( filename ) );

        std::fstream file( filename, std::ios::binary | std::ios::in );
        //        assert(file.is_open());
        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        file.seekg(0, std::ios::end);
        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        file.seekg(0, std::ios::beg);

        assert( !file.eof() );
        //        assert(sizeof(int) == 4);

        try {
            hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );
            auto acqs                     = inputSensor.getAllAcquisitions();
            const auto& header            = inputSensor.m_spec;
            const std::string& sensorName = header.m_sensorName;

            if ( nAcqs == -1 ) {
                nAcqs = (int)acqs.size();
                m_frames.reserve( nAcqs );
                m_frames.resize( nAcqs );
            }
            //            assert(acqs.size() == nAcqs);

            int iAcq = 0;
            for ( const auto& acq : acqs ) {
                //                Snapshot && snapshot{std::move(acq), sensorName};
                //                m_frames[iAcq].m_snapshots.emplace_back(std::move(acq),
                //                sensorName); m_snapshots.insert(std::move(snapshot));
                //                m_snapshots.insert(&m_frames[iAcq].m_snapshots.back());

                //                const Snapshot & snapshot2 = m_snapshots.find
                //                Snapshot snapshot { acq.clone(), sensorName };
                //                m_snapshots.insert(std::move(snapshot));
                //                m_snapshots.insert(Snapshot { acq.clone(), sensorName });
                Snapshot snapshot( sensorName, header.m_format, header.m_dims, acq );
                m_snapshots.insert( std::move( snapshot ) );
                //                m_snapshots.insert(Snapshot(sensorName, inputSensor.getFormat(),
                //                inputSensor.getDims(), acq));

                //                const auto& it = m_snapshots.find(Snapshot { acq.clone(),
                //                sensorName });
                Snapshot snapshot2( sensorName, header.m_format, header.m_dims, acq );
                const auto& it = m_snapshots.find( snapshot2 );
                assert( it != m_snapshots.end() );
                m_frames[iAcq].push_back( *it );

                ++iAcq;
            }

            // here
            //            m_outputs[sensorName] = std::make_unique<hub::OutputSensor>(sensorName + "
            //            (record)", inputSensor.getFormat(), inputSensor.getDims(),
            //            hub::ClientSocket(), inputSensor.getMetaData());

            if ( m_outputs.find( sensorName ) == m_outputs.end() ) {
                m_outputs[sensorName] = std::make_unique<hub::OutputSensor>(
                    hub::SensorSpec { sensorName + m_outputPostfixName,
                                  header.m_format,
                                  header.m_dims,
                                  header.m_metaData },
                    hub::io::OutputStream(sensorName + m_outputPostfixName, hub::net::ClientSocket()) );
            }

            //            m_outputs[sensorName] = std::make_unique<hub::OutputSensor>(sensorName + "
            //            (physical)", inputSensor.getFormat(), inputSensor.getDims(),
            //            hub::ClientSocket(), inputSensor.getMetaData());

            //            int nReadAcqs = 0;
            //            Stream::hub::Acquisition acq;
            //            while (true) {
            //                inputSensor >> acq;
            //                std::cout << "read acquisition : " << acq << std::endl;
            //                m_snapshots.insert(Snapshot { std::move(acq), sensorName });
            //                ++nReadAcqs;
            //            }
            //            std::cout << "read " << nReadAcqs << " acquisitions from file sensor '" <<
            //            sensorName << "'" << std::endl;
            std::cout << "read " << acqs.size() << " acquisitions from file sensor '" << sensorName
                      << "'" << std::endl;
        }
        catch ( hub::Sensor::exception& e ) {
            std::cout << "catch sensor exception : " << e.what() << std::endl;
            throw;
        }
        catch ( std::exception& e ) {
            std::cout << "catch exception : " << e.what() << std::endl;
        }

        //        file.close();
        ++iFile;
    }
    std::cout << "read total of " << m_snapshots.size() << " acquistions" << std::endl;
}

void Player::unload() {
    assert( !m_isPlaying );

    assert( !m_outputs.empty() );
    assert( !m_snapshots.empty() );
    assert( !m_frames.empty() );
    assert( m_loadedPath != "" );
    //    for (auto & output : m_outputs) {
    //        delete output.second.get();
    ////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //    }
    m_outputs.clear();
    m_snapshots.clear();
    m_frames.clear();
    m_loadedPath = "";
}

void Player::play() {
    //    m_futureObj = m_exitSignal.get_future();
    std::cout << "start playing" << std::endl;
    assert( m_thread == nullptr );
    assert( !m_frames.empty() );
    assert( !m_snapshots.empty() );

    m_thread    = new std::thread( [this]() {
        //        const long long duration = m_snapshots.end()->getAcq().m_end -
        //        m_snapshots.begin()->getAcq().start;
        const long long duration = m_snapshots.end()->getAcq().m_start -
                                   m_snapshots.begin()->getAcq().m_start;
        // play
        int iLoop = 0;
        //        bool exitSignal = false;
        while ( m_isPlaying ) {
            const auto startRecord  = m_snapshots.begin()->getAcq().m_start;
            const auto& startChrono = std::chrono::high_resolution_clock::now();

            long long dec = iLoop * duration;

            auto it = m_snapshots.begin();
            //            while (it != m_snapshots.end()) {
            while ( m_isPlaying && it != m_snapshots.end() ) {
                const auto& snapshot = *it;

                std::this_thread::sleep_until(
                    startChrono + std::chrono::microseconds( snapshot.getAcq().m_start -
                                                             startRecord ) );

                const auto& acq = snapshot.getAcq();
                hub::Acquisition acq2 { acq.m_start + dec,
                                        acq.m_end + dec
                                        };
                acq2 << acq.getMeasures();

                *m_outputs.at( snapshot.getSensorName() ) << acq2;
                //                    << snapshot.getAcq();

                ++it;
                //                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1))
                //                == std::future_status::timeout;
            }

            if ( !m_isPlaying ) { std::cout << "end record" << std::endl; }
            else { std::cout << "auto loop " << iLoop << std::endl; }
            ++iLoop;
        }
    } );
    m_isPlaying = true;
}

void Player::stop() {
    assert( m_isPlaying );
    std::cout << "stop playing" << std::endl;
    //    m_exitSignal.set_value();
    m_isPlaying = false;
    assert( m_thread != nullptr );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
    //    m_exitSignal.swap(std::promise<void>());
}

void Player::showFrame( int iFrame ) {
    assert( !m_isPlaying );
    assert( m_loadedPath != "" );
    assert( !m_snapshots.empty() );
    assert( !m_frames.empty() );
    assert( m_currentFrame != iFrame );
    assert( iFrame < m_frames.size() );

    const auto& frame = m_frames[iFrame];
    for ( const auto& snapshot : frame ) {
        *m_outputs.at( snapshot.getSensorName() ) << snapshot.getAcq();
    }

    m_currentFrame = iFrame;
}

void Player::setOutputPostFixName( const std::string& outputPostFixName ) {
    m_outputPostfixName = outputPostFixName;
}

const std::vector<Frame>& Player::getFrames() const {
    return m_frames;
}

bool Player::isPlaying() const {
    return m_isPlaying;
}

bool Player::isLoaded() const {
    return m_loadedPath != "";
}

const std::string& Player::getLoadedPath() const {
    return m_loadedPath;
}

int Player::getCurrentFrame() const {
    return m_currentFrame;
}
