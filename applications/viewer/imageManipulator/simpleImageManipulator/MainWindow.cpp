#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <filesystem>
#include <fstream>
#include <string>

#include <DicomUtils/DicomUtils.hpp>
#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#define USE_MRI

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

#ifdef USE_MRI
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

    const int sliceSize         = sliceWidth * sliceHeight * 2;
    unsigned char* data = volumeData; // first slice in volume

    constexpr auto format     = hub::SensorSpec::Format::Y16;
//    assert( mFormat == hub::SensorSpec::Format::Y16 );

#else

    //    auto data = new unsigned char[512 * 512];
    //    assert(m_imageSize == 512 * 512 * 2);
//    constexpr auto sliceSideSize = 256;
//    constexpr auto sliceSideSize = 512;
    constexpr auto sliceSideSize = 1024;
    constexpr int sliceWidth  = sliceSideSize;
    constexpr int sliceHeight = sliceSideSize;
    constexpr auto format     = hub::SensorSpec::Format::RGB8;
//    constexpr auto format     = hub::SensorSpec::Format::BGR8;
    constexpr auto nChannel   = hub::SensorSpec::format2nByte( format );
    assert(nChannel == 3);
    constexpr auto sliceSize  = sliceWidth * sliceHeight * nChannel;
    unsigned char data[sliceSize];
    for ( int i = 0; i < sliceHeight; ++i ) {
        for ( int j = 0; j < sliceWidth; ++j ) {
            for ( int k = 0; k < nChannel; ++k ) {
                data[( i * sliceWidth + j ) * nChannel + k] = (i + j) / (sliceSideSize / 256.0);
                //            data[(i * 512 + j) * 2 + 1] = i + j;
            }
        }
    }
#endif

    //    const int sliceSize         = sliceWidth * sliceHeight * 2;

    m_streamView2D = new WidgetStreamView2D( this );
    m_streamView2D->init( sliceWidth, sliceHeight );
    m_streamView2D->setData( data,
                             sliceSize,
                             { (int)sliceWidth, (int)sliceHeight },
                             format );

    ui->mdiArea->addSubWindow( m_streamView2D );

    /// sensorSpec.m_dims, sensorSpec.m_format);

    m_imageManipulator = new FormImageManipulator( this );
    m_imageManipulator->setMinimumWidth( sliceWidth );
    m_imageManipulator->setMinimumHeight( sliceHeight );
    auto& widgetStreamView = m_imageManipulator->getWidgetStreamView();
    widgetStreamView.init( sliceWidth, sliceHeight );
    widgetStreamView.setData( data,
                              sliceSize,
                              { (int)sliceWidth, (int)sliceHeight },
                              format );

    ui->mdiArea->addSubWindow( m_imageManipulator );
}

MainWindow::~MainWindow() {
    delete m_streamView2D;
    delete m_streamView2D_2;
    delete ui;
}
