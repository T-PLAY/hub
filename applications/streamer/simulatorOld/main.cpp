
//#include <constants.h>

#include <Scus.h>

#include <DicomUtils/DicomUtils.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
//#include <Streamer.hpp>
//#include <Viewer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

int main( int argc, char* argv[] ) {

    const auto filename     = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
    std::string filenameStr = std::string( filename );
    std::string mriName     = filenameStr.substr( filenameStr.find( "MRI/" ) + 4 );
    mriName                 = mriName.substr( 0, mriName.find_last_of( '/' ) );
    std::cout << "mriName = '" << mriName << "'" << std::endl;

    std::cout << "open '" << filename << " dicom file." << std::endl;

    unsigned int sliceWidth, sliceHeight, nSlices, bytePerVoxel;
    float pixelSpacingWidth, pixelSpacingHeight, sliceThickness;
    auto* volumeData = DICOM::readDicomVolume( filename,
                                               &sliceWidth,
                                               &sliceHeight,
                                               &nSlices,
                                               &bytePerVoxel,
                                               &pixelSpacingWidth,
                                               &pixelSpacingHeight,
                                               &sliceThickness );

    assert( bytePerVoxel == 2 );
    double sliceRealWidth = sliceWidth * pixelSpacingWidth;
    double sliceRealDepth = sliceHeight * pixelSpacingHeight;

    hub::SensorSpec::MetaData metaData;
    metaData["type"]      = "record";
    metaData["nAcqs"]     = nSlices;
    metaData["series"]    = mriName;
    metaData["parent"]    = "calibrator";
    metaData["scanWidth"] = sliceRealWidth;
    metaData["scanDepth"] = sliceRealDepth;

    glm::mat4 transform( 1.0 );
    transform =
        glm::scale( transform, glm::vec3( sliceRealDepth / 2.0, 1.0, sliceRealWidth / 2.0 ) );
    transform = glm::translate( transform, glm::vec3( 1.0, 0.0, 1.0 ) );
    transform = glm::rotate( transform, glm::radians( 180.0f ), glm::vec3( 1.0, 0.0, 0.0 ) );
    const float* array    = glm::value_ptr( transform );
    metaData["transform"] = array;

    const int sliceSize = sliceWidth * sliceHeight * 2;

#ifdef USE_RGBA

    hub::SensorSpec sensorSpec( "mri",
                                { { { 1 }, hub::Format::DOF6 },
                                  { { sliceWidth, sliceHeight }, hub::Format::RGBA8 } },
                                std::move( metaData ) );
    const int volumeSize   = sliceSize * nSlices;
    const int textureSize  = sliceWidth * sliceHeight * 4;
    const auto& resolution = sensorSpec.m_resolutions.at( 1 );
    assert( hub::SensorSpec::computeAcquisitionSize( resolution ) == textureSize );
    unsigned char* texturesData = new unsigned char[textureSize * nSlices];
    for ( int i = 0; i < volumeSize / 2; ++i ) {
        unsigned char color = ( (uint16_t*)volumeData )[i] / 256;

        assert( 0 <= color && color <= 255 );

        texturesData[4 * i]     = color;
        texturesData[4 * i + 1] = color;
        texturesData[4 * i + 2] = color;
        texturesData[4 * i + 3] = color;
    }

#else
    unsigned char* texturesData       = volumeData;
    const int textureSize             = sliceSize;
    const std::string dicomStreamName = "dicomStream";
    hub::SensorSpec sensorSpec(
        "mri",
        { { { 1 }, hub::Format::DOF6 },
          { { (int)sliceWidth, (int)sliceHeight }, hub::Format::Y16 } },
        std::move( metaData ) );
#endif

    hub::OutputSensor outputSensor( std::move( sensorSpec ),
                                    hub::io::OutputStream( dicomStreamName ) );
    for ( int iImage = 0; iImage < nSlices; ++iImage ) {
        glm::quat quat( 1.0, 0.0, 0.0, 0.0 );
        hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0, quat.w, quat.x, quat.y, quat.z );

        hub::Measure image( &texturesData[textureSize * iImage], textureSize,  { { (int)sliceWidth, (int)sliceHeight }, hub::Format::Y16 });
        outputSensor << ( hub::Acquisition { iImage, iImage } << std::move( dof6 )
                                                              << std::move( image ) );

        std::cout << "[Simulator][Streamer] added new acq " << iImage << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr int scanWidth  = 192;
    constexpr int scanHeight = 512;
    constexpr int scanSize   = scanWidth * scanHeight;

    hub::SensorSpec::MetaData metaData2;
    glm::mat4 transform2( 1.0 );

    double scale         = 1.5;
    double scanRealWidth = 50.0 * scale;
    double scanRealDepth = 35.0 * scale;
    transform2 =
        glm::scale( transform2, glm::vec3( scanRealDepth / 2.0, 1.0, scanRealWidth / 2.0 ) );
    transform2             = glm::translate( transform2, glm::vec3( 1.0, 0.0, 0.0 ) );
    const float* array2    = glm::value_ptr( transform2 );
    metaData2["transform"] = array2;
    metaData2["scanWidth"] = scanRealWidth;
    metaData2["scanDepth"] = scanRealDepth;
    metaData2["series"]    = mriName;
    hub::SensorSpec sensorSpec2( "",
                                 { { { 1 }, hub::Format::DOF6 },
                                   { { scanWidth, scanHeight }, hub::Format::Y8 } },
                                 std::move( metaData2 ) );

    const std::string simulatorStreamName = "simulator";
    hub::OutputSensor outputSensor2( std::move( sensorSpec2 ),
                                     hub::io::OutputStream( "simulator" ) );

    if ( !std::filesystem::exists( filename ) ) {
        std::cout << "file '" << filename << "' doesn't exist" << std::endl;
        exit( 1 );
    }
    assert( std::filesystem::exists( filename ) );
    auto filepath = std::filesystem::path( filename ).remove_filename();

    std::vector<std::filesystem::path> files_in_directory;
    for ( const auto& entry : std::filesystem::directory_iterator( filepath.remove_filename() ) ) {
        if ( !std::filesystem::is_directory( entry ) ) { files_in_directory.push_back( entry ); }
    }
    std::sort( files_in_directory.begin(), files_in_directory.end() );

    std::vector<std::string> fileList;
    for ( const auto& file : files_in_directory ) {
        std::cout << file << std::endl; // printed in alphabetical order
        fileList.push_back( file.string() );
    }

    if ( files_in_directory.empty() ) {
        std::cerr << "Name list empty" << std::endl;
        exit( -1 );
    }

    const std::shared_ptr<WeightingFunction> wf( new Dicom( fileList ) );

    const std::shared_ptr<Sampler> sampler( new utk::SamplerDartThrowing );

    std::cout << "generating grid ..." << std::endl;
    Grid grid( 250, 80, 250, 8, wf, sampler, true );
    std::cout << "grid created" << std::endl;
    BasicUS bu( grid );
    bu.setSigLateral( 1.25 );
    bu.setSigAxial( 0.2 );
    bu.setDynamicRange( 25.0 );

    ////////////////////////////////////////////////////////// INIT STREAMERS

    ////////////////////////////////////////////////////////// INIT VIEWER

    std::cout << "Ready to simulate" << std::endl;

    size_t iFrame               = 0;
    long long lastFrameDuration = 0;
    long long lastAcqStart      = 0;

    hub::InputSensor inputPosSensor( hub::io::InputStream( "Polhemus Patriot (sensor 1)" ) );

    constexpr int width  = 192;
    constexpr int height = 512;
    assert( scanSize == width * height );

    while ( true ) {

        auto acq = inputPosSensor.getAcquisition();

        if ( acq.m_start < lastAcqStart + lastFrameDuration ) continue;

        if ( acq.m_start == lastAcqStart ) { continue; }
        lastAcqStart = acq.m_start;

        const auto& startChrono = std::chrono::high_resolution_clock::now();

        const auto& measures = acq.getMeasures();
        assert( measures.size() == 1 );

        const hub::Dof6& dof6 = measures[0];

        glm::vec3 position( dof6.m_x, dof6.m_y, dof6.m_z );
        glm::quat orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

        const AcquisitionZone acqZone( position, orientation, scanRealWidth, 1.1, scanRealDepth );

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

#ifdef DEBUG
        const auto& scanImage = bu.getCorrespondingUS( acqZone, scanWidth, scanHeight );
#else
        const auto& scanImage = bu.getCorrespondingRealUS( acqZone, grid, scanWidth, scanHeight );
#endif
        assert( scanImage.size() == scanSize );
        const unsigned char* imageData = scanImage.data();

        ////			unsigned char* scanData = scanImage.data();

        ////			unsigned char otherData[192 * 512];

        outputSensor2 << ( hub::Acquisition { acq.m_start, acq.m_end }
                           << dof6.clone() << hub::Measure { imageData, scanSize,  { { scanWidth, scanHeight }, hub::Format::Y8 }} );

        std::cout << "Computed frame " << iFrame << std::endl;
        ++iFrame;
        const auto& endChrono = std::chrono::high_resolution_clock::now();

        lastFrameDuration =
            std::chrono::duration_cast<std::chrono::microseconds>( endChrono - startChrono )
                .count();
    };

    /// 0.0));

    /// quat.y, quat.z ); /        hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0, quat.w,
    /// quat.x, quat.y, quat.z );

    return 0;
}
