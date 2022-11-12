
#include "Recorder.hpp"

Recorder::Recorder( const std::string& ipv4, const int& port ) : m_ipv4( ipv4 ), m_port( port ) {}

void Recorder::record( const std::vector<std::pair<std::string, std::string>>& streamNames ) {
    assert( m_stopThread == false );

    // creating new record folder
    char folderName[64] = { 0 };
    time_t rawtime;
    time( &rawtime );
    const auto timeinfo = localtime( &rawtime );
    strftime( folderName, sizeof( folderName ), "%Y-%m-%d_%H-%M-%S", timeinfo );

    m_recordFolder = m_rootPath + "records/" + folderName + "/";
    std::cout << "create directory " << m_recordFolder << std::endl;
    assert( !std::filesystem::exists( m_recordFolder ) );
    std::filesystem::create_directories( m_recordFolder );

    //    std::vector<std::pair<std::string, std::string>> streamNames {
    //          { "Polhemus Patriot (sensor 2)", ""}, { "ULA-OP 256", "" } };
    //        { "Polhemus Patriot (sensor 2)", ""}, {"ULA-OP 256", ""} };
    //        { "Polhemus Patriot (sensor 2)", ""}};
    //        { "Polhemus Patriot (sensor 2)", ""}, {"Polhemus Patriot (sensor 1)", ""}};
    //    std::vector<std::thread> threads;

    int iThread = 0;
    for ( const auto& streamName : streamNames ) {

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        m_threads.push_back( std::thread( [=]() {
            hub::InputSensor inputSensor( hub::io::InputStream(
                streamName.first,
                streamName.second,
                hub::net::ClientSocket( hub::net::s_defaultServiceIp, m_port ) ) );

            std::fstream recordFile( m_recordFolder + streamName.first + ".txt",
                                     std::ios::out | std::ios::binary | std::ios::trunc );
            assert( recordFile.is_open() );

            hub::OutputSensor outputFile( inputSensor.m_spec,
                                          hub::io::File( std::move( recordFile ) ) );

            int nAcq                = 0;
            long long previousStart = 0;
            while ( !m_stopThread ) {
                auto acq = inputSensor.getAcquisition();
                // ping acq
                if ( acq.m_start == previousStart ) {
                    std::cout << "[Recorder] unable to save ping acq : " << acq << std::endl;
                    continue;
                }

                if ( acq.m_start < previousStart ) {
                    std::cout << "[Recorder] unable to save outdated acq : " << acq << std::endl;
                    continue;
                }

                assert( acq.m_start > previousStart );

                outputFile << acq;
                std::cout << "\033[0;" << std::to_string( 31 + iThread ) << "m+\033[0m"
                          << std::flush;
//                std::cout << "save acq : " << acq << std::endl;
                ++nAcq;
                previousStart = acq.m_start;
            }
            std::cout << std::endl;
            std::cout << "[" << streamName.first << "] " << nAcq << " acq recorded in file '" << m_recordFolder + streamName.first + ".txt'" << std::endl;
        } ) );

        ++iThread;
    }
}

void Recorder::stop() {
    assert( m_stopThread == false );
    m_stopThread = true;
    for ( auto& thread : m_threads ) {
        assert( thread.joinable() );
        thread.join();
    }
    m_stopThread = false;

    // copy this record into latest folder
    {
        std::string latestFolder = m_rootPath + "records/latest/";
        std::filesystem::create_directories( latestFolder );
        for ( const auto& dirEntry : std::filesystem::directory_iterator( latestFolder ) ) {
            std::filesystem::remove( dirEntry );
        }
        std::filesystem::copy(
            m_recordFolder, latestFolder, std::filesystem::copy_options::recursive );
    }
}

// void Recorder::setPort( int port ) {
//     m_port = port;
// }
