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

Loader::Loader( const std::string& ipv4, const int& port ) : m_ipv4( ipv4 ), m_port( port ) {}

Loader::~Loader() {
    std::cout << "[Loader] ~Loader" << std::endl;

    if ( isPlaying() ) stop();
    if ( isLoaded() ) unload();
}

void Loader::load( const std::string& path ) {
    std::cout << "[Loader] load(" << path << ")" << std::endl;
    if ( isPlaying() ) stop();

    assert( m_thread == nullptr );
    m_snaps.clear();
    m_loadedPath = "";

    //////////////////////////////////////

    assert( std::filesystem::exists( path ) );
    assert( m_loadedPath == "" );
    assert( m_thread == nullptr );
    assert( m_snaps.empty() );

    assert( !path.empty() );
    std::set<Snap> sortedSnaps;

    std::list<std::string> sensorNamesToRemove;
    for ( const auto& [key, value] : m_outputStreams ) {
        sensorNamesToRemove.push_back( key );
    }

    std::list<std::filesystem::path> filePaths;
    if ( std::filesystem::is_directory( path ) ) {
        for ( const auto& fileDir : std::filesystem::directory_iterator( path ) ) {
            filePaths.push_back( fileDir );
        }
    }
    else {
        assert(std::filesystem::is_regular_file(path));
        filePaths.push_back( path );
    }

    // read records in folder
    //    for ( const auto& fileDir : std::filesystem::directory_iterator( path ) ) {
    for ( const auto& fileDir : filePaths ) {
        const auto& filepath = fileDir.string();
        const auto& filename = fileDir.filename();
        if ( filename == "export" ) continue;
        std::cout << "[Loader] read '" << filename << "' record" << std::endl;
        assert( std::filesystem::exists( filepath ) );

        std::fstream file( filepath, std::ios::binary | std::ios::in );

        assert( !file.eof() );
        hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );

        const auto& sensorSpec        = inputSensor.m_spec;
        const std::string& sensorName = sensorSpec.m_sensorName;

        auto acqs = inputSensor.getAllAcquisitions();

        if ( m_outputStreams.find( sensorName ) == m_outputStreams.end() ) {
            assert( m_outputStreams.find( sensorName ) == m_outputStreams.end() );
            m_outputStreams[sensorName] = std::make_unique<hub::OutputSensor>(
                inputSensor.m_spec,
                hub::io::OutputStream( sensorName, hub::net::ClientSocket( m_ipv4, m_port ) ) );
        }
        else {
            assert( std::find( sensorNamesToRemove.begin(),
                               sensorNamesToRemove.end(),
                               sensorName ) != sensorNamesToRemove.end() );
            sensorNamesToRemove.remove( sensorName );
        }

        for ( const auto& acq : acqs ) {

            /// localTransform[3][1], localTransform[3][2]); // column major /            glm::vec3
            /// translation = glm::vec3(localTransform[0][3], localTransform[1][3],
            /// localTransform[2][3]); // row major

            ////			newAcq << hub::Measure(std::move(newDof6));

            Snap snap { acq.clone(), sensorName };
            sortedSnaps.insert( std::move( snap ) );
        }

        std::cout << "[Loader] read " << acqs.size() << " acquisitions from file sensor '"
                  << sensorName << "'" << std::endl;

    } // for ( const auto& fileDir : std::filesystem::directory_iterator( path ) )

    for ( const auto& sensorNameToRemove : sensorNamesToRemove ) {
        assert( m_outputStreams.find( sensorNameToRemove ) != m_outputStreams.end() );
        m_outputStreams.erase( sensorNameToRemove );
    }

    m_snaps.reserve( sortedSnaps.size() );
    for ( auto& acq : sortedSnaps ) {
        m_snaps.push_back( Snap { acq.m_acq.clone(), acq.m_sensorName } );
    }

    m_loadedPath = path;
    emit pathLoaded();

    for ( const auto& snap : m_snaps ) {
        if ( snap.m_acq.getMeasures().size() == 2 ) {
            auto& outputStream = *m_outputStreams.at( snap.m_sensorName );
            outputStream << snap.m_acq;
        }
    }

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

    if ( isPlaying() ) stop();

    assert( m_thread == nullptr );
    assert( isLoaded() );
    assert( !m_outputStreams.empty() );
    m_snaps.clear();
    m_loadedPath = "";

    m_outputStreams.clear();
}

// const std::vector<hub::Acquisition>&
// Loader::getAcquisitions( const std::string& sensorName ) const {
// }

void Loader::play() {
    std::cout << "[Loader] start playing" << std::endl;
    assert( m_thread == nullptr );
    assert( !m_isPlaying );

    m_isPlaying = true;
    m_thread    = new std::thread( [this]() {
        // play
        int iLoop = 0;
        do {
            const auto startRecord  = m_snaps.at( m_iAcq ).m_acq.m_start;
            const auto& startChrono = std::chrono::high_resolution_clock::now();

            while ( m_isPlaying && m_iAcq < m_snaps.size() ) {
                const auto& snap = m_snaps.at( m_iAcq );

                std::this_thread::sleep_until(
                    startChrono + std::chrono::microseconds( snap.m_acq.m_start - startRecord ) );
                *m_outputStreams.at( snap.m_sensorName ) << snap.m_acq;
                emit acqChanged( m_iAcq );

                ++m_iAcq;
            }
            if ( m_isPlaying ) {
                std::cout << "[Loader] end record, auto loop " << iLoop << std::endl;
                m_iAcq = 0;
            }
            else { std::cout << "[Loader] record stopped by user" << std::endl; }
            ++iLoop;

        } while ( m_isPlaying && m_autoLoop );

        if ( m_isPlaying ) { emit playEnded(); }
    } );
}

void Loader::stop() {
    std::cout << "[Loader] stop playing" << std::endl;
    assert( m_isPlaying );
    m_isPlaying = false;
    assert( m_thread != nullptr );
    m_thread->join();
    delete m_thread;
    m_thread = nullptr;
}

bool Loader::isPlaying() const {
    return m_isPlaying;
}

bool Loader::isLoaded() const {
    return m_loadedPath != "";
}

// const std::string& Loader::getLoadedPath() const {
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
