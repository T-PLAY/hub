#include "Loader.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <string>

#include <IO/File.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// Loader::Loader( const std::string& outputPostFixName, bool autoPlay ) :
//     m_outputPostfixName( outputPostFixName ), m_autoPlay( autoPlay ) {}

Loader::Loader( const std::string& ipv4, const int& port ) : m_ipv4( ipv4 ), m_port( port ) {}

Loader::~Loader() {
    std::cout << "[Loader] ~Loader" << std::endl;

    if ( isPlaying() ) stop();
    if ( isLoaded() ) unload();
}

void Loader::load( const std::string& path ) {
    std::cout << "[Loader] load(" << path << ")" << std::endl;
    //    return;
    if ( isPlaying() ) stop();

    assert( m_thread == nullptr );
    //    assert( isLoaded() );
    //    assert( !m_inputStreams.empty() );
    //    assert( !m_outputStreams.empty() );
    //    m_inputStreams.clear();
    m_snaps.clear();
    m_loadedPath = "";

    //////////////////////////////////////

    assert( std::filesystem::exists( path ) );
    assert( m_loadedPath == "" );
    //    assert( m_inputStreams.empty() );
    assert( m_thread == nullptr );
    //    assert( m_outputStreams.empty() );
    assert( m_snaps.empty() );

    //    m_inputStreams.clear();
    //    m_sensorName2acquisitions.clear();

    assert( !path.empty() );
    //    assert( path != "" );
    std::set<Snap> sortedSnaps;

    // read records in folder
    for ( const auto& fileDir : std::filesystem::directory_iterator( path ) ) {
        const auto& filepath = fileDir.path().string();
        const auto& filename = fileDir.path().filename();
        if ( filename == "export" ) continue;
        //        std::cout << "[Loader] read '" << filepath << "' record" << std::endl;
        std::cout << "[Loader] read '" << filename << "' record" << std::endl;
        assert( std::filesystem::exists( filepath ) );

        std::fstream file( filepath, std::ios::binary | std::ios::in );

        assert( !file.eof() );
        //        m_inputStreams.push_back(
        //            std::make_unique<hub::InputSensor>( hub::io::File( std::move( file ) ) ) );
        //        const std::string streamName = filename;
        hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );

        //        auto& inputStream                    = *m_inputStreams.back();
        const auto& sensorSpec        = inputSensor.m_spec;
        const std::string& sensorName = sensorSpec.m_sensorName;
        //        const std::string& sensorNamePostFix = sensorName;

        //        assert( m_sensorName2acquisitions.find( sensorName ) ==
        //        m_sensorName2acquisitions.end() ); m_sensorName2acquisitions[sensorName] =
        //        inputSensor.getAllAcquisitions();

        //        const auto& acqs = m_sensorName2acquisitions.at( sensorName );
//        hub::SensorSpec sensorSpec2 = sensorSpec;
//        const auto & metaData = sensorSpec.m_metaData;
//        glm::mat4 localTransform;
//        if ( metaData.find( "transform" ) != metaData.end() ) {
//            const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
//            localTransform          = glm::make_mat4( array );
//        }
//        sensorSpec2.m_metaData.erase("transform");

        auto acqs = inputSensor.getAllAcquisitions();

        //        hub::OutputSensor outputSensor2(inputSensor.m_spec, hub::io::OutputStream("Player
        //        (" + streamName + ")"));

        if ( m_outputStreams.find( sensorName ) == m_outputStreams.end() ) {
            assert( m_outputStreams.find( sensorName ) == m_outputStreams.end() );
            m_outputStreams[sensorName] = std::make_unique<hub::OutputSensor>(
                inputSensor.m_spec,
//                sensorSpec2,
                //                hub::io::OutputStream( "Loader : " + sensorName + ")",
                hub::io::OutputStream( sensorName, hub::net::ClientSocket( m_ipv4, m_port ) ) );
        }

        //        m_outputStreams.push_back( std::make_unique<hub::OutputSensor>(
        //            inputSensor.m_spec,
        //            hub::io::OutputStream( "Loader : " + inputSensor.m_spec.m_sensorName ) ) );
        //        outputStreams.emplace_back(inputSensor.m_spec, hub::io::OutputStream("Player (" +
        //        streamName + ")"));

        for ( const auto& acq : acqs ) {
//                        std::cout << "read acq : " << acq << std::endl;
            //            Snap snap { acq.clone(), (int)( m_outputStreams.size() - 1 ) };
//            const auto& measures = acq.getMeasures();
//            assert( measures.size() == 2 );
//            const hub::Dof6& dof6 = measures.at( 0 );
////            std::cout << dof6 << std::endl;
//            glm::vec3 position    = glm::vec3( dof6.m_x, dof6.m_y, dof6.m_z );
//            glm::quat orientation = glm::quat( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

////            glm::vec3 translation = glm::vec3(localTransform[3][0], localTransform[3][1], localTransform[3][2]); // column major
////            glm::vec3 translation = glm::vec3(localTransform[0][3], localTransform[1][3], localTransform[2][3]); // row major
//            glm::vec3 scale;
//            glm::quat rotation;
//            glm::vec3 translation;
//            glm::vec3 skew;
//            glm::vec4 perspective;
//            glm::decompose(localTransform, scale, rotation, translation, skew, perspective);
//            rotation = glm::conjugate(rotation);

//            position += translation;
//            orientation *= rotation;

//            hub::Dof6 newDof6(position.x, position.y, position.z, orientation.w, orientation.x, orientation.y, orientation.z);

//            hub::Acquisition newAcq(acq.m_start, acq.m_end);
////			newAcq << hub::Measure(std::move(newDof6));
//            newAcq << std::move(newDof6);
//            newAcq << hub::Measure(measures.at(1).m_data, measures.at(1).m_size);
//            Snap snap { std::move(newAcq), sensorName };

            Snap snap { acq.clone(), sensorName };
            sortedSnaps.insert( std::move( snap ) );
        }

        std::cout << "[Loader] read " << acqs.size() << " acquisitions from file sensor '"
                  << sensorName << "'" << std::endl;

    } // for ( const auto& fileDir : std::filesystem::directory_iterator( path ) )

    m_snaps.reserve( sortedSnaps.size() );
    //    m_snaps.insert(m_snaps.begin(), sortedSnaps.begin(), sortedSnaps.end());
    for ( auto& acq : sortedSnaps ) {
        m_snaps.push_back( Snap { acq.m_acq.clone(), acq.m_sensorName } );
    }

    m_loadedPath = path;
    emit pathLoaded();

    m_iAcq = 0;
    if ( m_autoPlay )
        play();
    else {
        const auto& snap = m_snaps.at( m_iAcq );
        *m_outputStreams.at( snap.m_sensorName ) << snap.m_acq;
        emit acqChanged( m_iAcq );
    }
}

void Loader::unload() {
    std::cout << "[Loader] unload()" << std::endl;
    //    return;

    if ( isPlaying() ) stop();

    assert( m_thread == nullptr );
    assert( isLoaded() );
    //    assert( !m_inputStreams.empty() );
    assert( !m_outputStreams.empty() );
    //    m_inputStreams.clear();
    m_snaps.clear();
    m_loadedPath = "";

    m_outputStreams.clear();

    //    std::cout << "[Loader] unload : end" << std::endl;
}

// const std::vector<hub::Acquisition>&
// Loader::getAcquisitions( const std::string& sensorName ) const {
//     assert( m_sensorName2acquisitions.find( sensorName ) != m_sensorName2acquisitions.end() );
//     return m_sensorName2acquisitions.at( sensorName );
// }

void Loader::play() {
    //    assert( m_autoPlay );
    //    m_futureObj = m_exitSignal.get_future();
    std::cout << "[Loader] start playing" << std::endl;
    assert( m_thread == nullptr );
    //    assert( !m_frames.empty() );
    //    assert( !m_snapshots.empty() );
    assert( !m_isPlaying );

    m_isPlaying = true;
    m_thread    = new std::thread( [this]() {
        // play
        int iLoop = 0;
        //        bool exitSignal = false;
        do {
            //            const auto startRecord  = m_snaps.begin()->m_acq.m_start;
            const auto startRecord  = m_snaps.at( m_iAcq ).m_acq.m_start;
            const auto& startChrono = std::chrono::high_resolution_clock::now();

            //            auto it = m_snaps.begin();
            //            while (it != m_snapshots.end()) {
            //            int iAcq = 0;
            //            while ( m_isPlaying && it != m_snaps.end() ) {
            while ( m_isPlaying && m_iAcq < m_snaps.size() ) {
                //                const auto& snap = *it;
                const auto& snap = m_snaps.at( m_iAcq );

                std::this_thread::sleep_until(
                    startChrono + std::chrono::microseconds( snap.m_acq.m_start - startRecord ) );
                *m_outputStreams.at( snap.m_sensorName ) << snap.m_acq;
                emit acqChanged( m_iAcq );

                //                ++it;
                ++m_iAcq;
            }
            if ( m_isPlaying ) {
                std::cout << "[Loader] end record, auto loop " << iLoop << std::endl;
                m_iAcq = 0;
            }
            else { std::cout << "[Loader] record stopped by user" << std::endl; }
            ++iLoop;

        } while ( m_isPlaying && m_autoLoop );

        if ( m_isPlaying ) {
            //            m_isPlaying = false;
            emit playEnded();
        }
    } );
}

void Loader::stop() {
    std::cout << "[Loader] stop playing" << std::endl;
    assert( m_isPlaying );
    //    m_exitSignal.set_value();
    m_isPlaying = false;
    assert( m_thread != nullptr );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
    //    m_exitSignal.swap(std::promise<void>());
}

bool Loader::isPlaying() const {
    return m_isPlaying;
}

bool Loader::isLoaded() const {
    return m_loadedPath != "";
}

// const std::string& Loader::getLoadedPath() const {
//     assert( isLoaded() );
//     return m_loadedPath;
// }

void Loader::setAutoLoop( bool newAutoLoop ) {
    m_autoLoop = newAutoLoop;
}
void Loader::setAutoPlay( bool newAutoPlay ) {
    m_autoPlay = newAutoPlay;
}

int Loader::getNAcq() const {
    return m_snaps.size();
}

void Loader::setIAcq( int newIAcq ) {
    assert( 0 <= newIAcq && newIAcq < m_snaps.size() );
    m_iAcq           = newIAcq;
    const auto& snap = m_snaps.at( m_iAcq );
    *m_outputStreams.at( snap.m_sensorName ) << snap.m_acq;
}
