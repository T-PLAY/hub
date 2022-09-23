
#include <constants.h>

#include <Scus.h>

#include <DicomUtils/DicomUtils.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

int main( int argc, char* argv[] ) {

    const auto filename = MRI_PATH "AXT2_ligaments_uterosacres/D0010525.dcm";
    //            const auto filename = MRI_PATH "Ax_T2_PROP_5MM/D0010275.dcm";
    //        const auto filename = MRI_PATH "Loc/D0010001.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_AX_T2/D0010551.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_Coro_T2/D0010710.dcm"; // bad
    //        const auto filename = MRI_PATH "MPR_Sag_T2/D0010652.dcm"; // bad
    //        const auto filename = MRI_PATH "PACS_AXIAL_ABDO/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_AXIAL_AP/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_AXIAL_TAP/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "PACS_PARENCHYME/IM0001.dcm"; // CT
    //        const auto filename = MRI_PATH "Sag_CUBE_T2/D0010031.dcm";
    //        const auto filename = MRI_PATH "Scouts/IM0001.dcm";
    //        const auto filename = MRI_PATH "SerieSmartPrep/IM0001.dcm";
    //        const auto filename = MRI_PATH "WATER:AX_LAVA-Flex/D0010333.dcm";
    std::string filenameStr = std::string( filename );
    std::string mriName     = filenameStr.substr( filenameStr.find( "MRI/" ) + 4 );
    mriName                 = mriName.substr( 0, mriName.find_last_of( '/' ) );
    std::cout << "mriName = '" << mriName << "'" << std::endl;

    //    if (false)
    //    {
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
    transform             = glm::translate( transform, glm::vec3( 1.0, 0.0, 1.0 ) );
    const float* array    = glm::value_ptr( transform );
    metaData["transform"] = array;

    const int sliceSize = sliceWidth * sliceHeight * 2;

#ifdef USE_RGBA

    hub::SensorSpec sensorSpec( "mri",
                                { { { 1 }, hub::SensorSpec::Format::DOF6 },
                                  { { sliceWidth, sliceHeight }, hub::SensorSpec::Format::RGBA8 } },
                                std::move( metaData ) );
    const int volumeSize   = sliceSize * nSlices;
    const int textureSize  = sliceWidth * sliceHeight * 4;
    const auto& resolution = sensorSpec.m_resolutions.at( 1 );
    assert( hub::SensorSpec::computeAcquisitionSize( resolution ) == textureSize );
    unsigned char* texturesData = new unsigned char[textureSize * nSlices];
    for ( int i = 0; i < volumeSize / 2; ++i ) {
        //        auto * pixel = &volumeData[i * 2];
        //        unsigned char color = volumeData[2 * i + 1];
        //        unsigned char color = (volumeData[2 * i] + volumeData[2 * i + 1] * 256) / 256;
        unsigned char color = ( (uint16_t*)volumeData )[i] / 256;

        assert( 0 <= color && color <= 255 );

        texturesData[4 * i]     = color;
        texturesData[4 * i + 1] = color;
        texturesData[4 * i + 2] = color;
        texturesData[4 * i + 3] = color;
    }

#else
    unsigned char* texturesData = volumeData;
    const int textureSize       = sliceSize;
    hub::SensorSpec sensorSpec(
        "dicomStream",
        { { { 1 }, hub::SensorSpec::Format::DOF6 },
          { { (int)sliceWidth, (int)sliceHeight }, hub::SensorSpec::Format::Y16 } },
        std::move( metaData ) );
#endif

    hub::OutputSensor outputSensor( std::move( sensorSpec ),
                                    hub::io::OutputStream( "dicomStream" ) );

    for ( int iImage = 0; iImage < nSlices; ++iImage ) {
//        hub::Dof6 dof6( 0.0, iImage * sliceThickness, 0.0 );
        hub::Dof6 dof6( 0.0, (nSlices - iImage - 1) * sliceThickness, 0.0 );
        hub::Measure image( &texturesData[textureSize * iImage], textureSize );
        outputSensor << ( hub::Acquisition { iImage, iImage } << std::move( dof6 )
                                                              << std::move( image ) );
    }

    //    while ( true ) {
    //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //    }

    delete[] texturesData;
    //    }

    ////////////////////////////////////////////////////////////////////////////////////////////////

    constexpr int scanWidth  = 192;
    constexpr int scanHeight = 512;
    constexpr int scanSize   = scanWidth * scanHeight;
    //    Stream::MetaData metaData;
    //    glm::mat3 transform = glm::mat3(1.0);

    ////    float transformPtr[9] = { 0.0 };
    //    std::vector<float> transformPtr;
    //    transformPtr.resize(9);
    //    memcpy(transformPtr.data(), glm::value_ptr(transform), 9 * 4);

    //    IOStream::Mat3 mat3;
    //    memcpy(mat3.data, glm::value_ptr(transform), sizeof(mat3));

    hub::SensorSpec::MetaData metaData2;
    //    metaData2["scanWidth"] = 256.0;
    //    metaData2["scanDepth"] = 256.0;
    //    metaData2["parent"] = "Keyboard";
    //    metaData2["transform"] = transformPtr;
//    glm::mat4 worldTransform(1.0);
//    worldTransform = glm::translate(worldTransform, glm::vec3(100, 100, 100));
//    glm::mat4 worldTransformInv = glm::inverse(worldTransform);
    glm::mat4 transform2( 1.0 );

//    transform2 *= worldTransform;
    //    const float sliceRealWidth = 50.0;
    //    const float sliceRealDepth = 35.0;
//    double scanRealWidth = 200;
//    double scanRealDepth = 200;
    double scanRealWidth = 50.0 * 1.5;
    double scanRealDepth = 35.0 * 1.5;
    //    transform2 = glm::rotate(transform2, glm::radians(90.0), glm::vec3(0.0, 1.0, 0.0));
    transform2 =
        glm::scale( transform2, glm::vec3( scanRealDepth / 2.0, 1.0, scanRealWidth / 2.0 ) );
    transform2             = glm::translate( transform2, glm::vec3( 1.0, 0.0, 0.0 ) );
    const float* array2    = glm::value_ptr( transform2 );
    metaData2["transform"] = array2;
    //    metaData2["transform3"] = mat3;
    //    metaData2["x"] = 0.0;
    //    metaData2["y"] = 0.0;
    //    metaData2["z"] = 0.0;
    //    metaData2["series"] = "AXT2_ligaments_uterosacres/D0010525.dcm";
    metaData2["scanWidth"] = scanRealWidth;
    metaData2["scanDepth"] = scanRealDepth;
    metaData2["series"]    = mriName;
    //    OutputStream outputScanStream("Simulator", Stream::Format::Y8, { scanWidth, scanDepth },
    //    ClientSocket(), metaData2);
    hub::SensorSpec sensorSpec2( "simulator",
                                 { { { 1 }, hub::SensorSpec::Format::DOF6 },
                                   { { scanWidth, scanHeight }, hub::SensorSpec::Format::Y8 } },
                                 std::move( metaData2 ) );

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
    std::reverse(files_in_directory.begin(), files_in_directory.end());

    std::vector<std::string> fileList;
    for ( const auto& file : files_in_directory ) {
        std::cout << file << std::endl; // printed in alphabetical order
//        fileList.push_back( file );
        fileList.push_back( file.string() );
    }

    if ( files_in_directory.empty() ) {
        std::cerr << "Name list empty" << std::endl;
        exit( -1 );
    }

    //    std::cout << "Opening Dicom with " << files_in_directory.size() << " slices" << std::endl;

    const std::shared_ptr<WeightingFunction> wf( new Dicom( fileList ) );
    //    const std::shared_ptr<WeightingFunction> wf(new Sphere(glm::vec3(100.0, 100.0,
    //    100.0), 50.0)); const std::shared_ptr<WeightingFunction> wf(new
    //    Cube(glm::vec3(50.0, 50.0, 50.0), 100.0)); const std::shared_ptr<WeightingFunction> wf(new
    //    Cube(glm::vec3(10.0, 10.0, 10.0), 100.0));

    const std::shared_ptr<Sampler> sampler( new utk::SamplerDartThrowing );

    std::cout << "generating grid ..." << std::endl;
    //    const Grid grid(250, 100, 250, 1, wf, sampler, true);
        Grid grid(250, 80, 250, 8, wf, sampler, true);
//    Grid grid( 10, 10, 10, 1, wf, sampler, true );
//    Grid grid( sliceRealDepth, nSlices * sliceThickness, sliceRealWidth, 16, wf, sampler, true );
    std::cout << "grid created" << std::endl;
    BasicUS bu( grid );
    bu.setSigLateral(1.25);
    bu.setSigAxial(0.2);
    bu.setDynamicRange(25.0);

    //    AcquisitionZone acq(glm::vec3(25, 50, 50), glm::angleAxis(0.f, glm::vec3(1, 0, 0)),
    //    50, 1.1f, 60); AcquisitionZone acq(glm::vec3(0, 30, 125), glm::angleAxis(0.f, glm::vec3(0,
    //    0, 1)), 256, 0.0, 256);

    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //    stbi_write_jpg("test.jpg", 1024, 1024, 1, (void*)bu.getCorrespondingUS(acq, 1024,
    //    1024).data(), 90);

    //    InputStream inputPosSensor(std::string("Keyboard"));
    //    InputStream inputPosSensor(ClientSocket("Keyboard", ""));
//    hub::InputSensor inputPosSensor( hub::io::InputStream( "Keyboard" ) );
    hub::InputSensor inputPosSensor( hub::io::InputStream( "Polhemus Patriot (sensor 1)" ) );
    //    OutputStream outputPosStream("Simulator", inputPosSensor.getFormat(),
    //    inputPosSensor.getDims(), ClientSocket(), metaData2);

    std::cout << "Ready to simulate" << std::endl;

    size_t iFrame       = 0;
    long long lastFrameDuration = 0;
    long long lastAcqStart = 0;
    while ( true ) {
        const auto& acq = inputPosSensor.getAcquisition();

        if (acq.m_start < lastAcqStart + lastFrameDuration)
            continue;

        if ( acq.m_start == lastAcqStart ) { continue; }
        lastAcqStart = acq.m_start;


        const auto& startChrono = std::chrono::high_resolution_clock::now();

        //        outputPosStream << acq;
        const auto& measures = acq.getMeasures();
        assert( measures.size() == 1 );

        const hub::Dof6& dof6 = measures[0];
        //        const auto & image = measures[1];

        //        float* translationData = (float*)acq.mData;
        //        float* quaternionData = (float*)&acq.mData[12];

        //        glm::vec3 pos(translationData[0], translationData[1], translationData[2]);
        //        const auto position = glm::make_vec3(translationData);
        //        const auto orientation = glm::make_quat(quaternionData);

        glm::vec3 position( dof6.m_x, dof6.m_y, dof6.m_z );
//        glm::vec3 position( -dof6.m_x, -dof6.m_y, -dof6.m_z );
        glm::quat orientation( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

//        position = worldTransformInv * glm::vec4(position, 1.0);
//        orientation *= glm::mat3(worldTransformInv);

        //    float* translation = glm::value_ptr(pos);
        //    float* orientation = glm::value_ptr(quat);
        //    memcpy(data, translation, 12);
        //    memcpy(&data[12], orientation, 16);

        const AcquisitionZone acqZone( position, orientation, scanRealWidth, 1.1, scanRealDepth );
        //        AcquisitionZone acqZone(glm::vec3(0, 30, 125), glm::angleAxis(0.f, glm::vec3(0, 0,
        //        1)), 256, 256, 256);
//        const auto& scanImage = bu.getCorrespondingUS( acqZone, scanWidth, scanHeight );
                const auto& scanImage = bu.getCorrespondingRealUS( acqZone, grid, scanWidth, scanHeight );
        assert( scanImage.size() == scanSize );
        const unsigned char* scanData = scanImage.data();

        //        unsigned char transpose[256 * 256];
        //        for (int i = 0; i < 256; ++i) {
        //            for (int j = 0; j < 256; ++j) {
        //                transpose[i * 256 + j] = data[j * 256 + i];
        //            }
        //        }
        //        unsigned char verticalMirror[256 * 256];
        //        for (int i = 0; i < 256; ++i) {
        //            for (int j = 0; j < 256; ++j) {
        //                verticalMirror[i * 256 + j] = data[i * 256 + (256 - j - 1)];
        //            }
        //        }
        //        outputScanStream << Stream::Acquisition { acq.start, acq.mBackendTimeOfArrival,
        //        verticalMirror, width * height };
        outputSensor2 << ( hub::Acquisition { acq.m_start, acq.m_end }
                           << dof6.clone() << hub::Measure { scanData, scanSize } );

        //        outputScanStream << Stream::Acquisition { acq.start, acq.mBackendTimeOfArrival,
        //        transpose, width * height };

        //    stbi_write_jpg("test.jpg", 1024, 1024, 1, (void*)bu.getCorrespondingUS(acq, 1024,
        //    1024).data(), 90);
        std::cout << "Computed frame " << iFrame << std::endl;
        ++iFrame;
        const auto& endChrono = std::chrono::high_resolution_clock::now();
//        const auto duration = std::cast
//                std::this_thread::sleep_until(
//                    startChrono + std::chrono::microseconds( snapshot.getAcq().m_start -
//                                                             startRecord ) );
        lastFrameDuration = std::chrono::duration_cast<std::chrono::microseconds>(endChrono - startChrono).count();
//                        const auto& timestampStart =
//                            std::chrono::duration_cast<std::chrono::microseconds>(
//                                start.time_since_epoch() )
//                                .count();
    }

    return 0;
}
