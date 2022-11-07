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

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

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
    unsigned char* texturesData = volumeData;

    m_streamView2D = new WidgetStreamView2D( this );
    m_streamView2D->init( sliceWidth, sliceHeight );
    m_streamView2D->setData( texturesData,
                             sliceSize,
                             { (int)sliceWidth, (int)sliceHeight },
                             hub::SensorSpec::Format::Y16 );

    ui->mdiArea->addSubWindow( m_streamView2D );

    /// sensorSpec.m_dims, sensorSpec.m_format);

    m_imageManipulator = new FormImageManipulator( this );
    m_imageManipulator->setMinimumWidth( 400 );
    m_imageManipulator->setMinimumHeight( 900 );
    auto& widgetStreamView = m_imageManipulator->getWidgetStreamView();
    widgetStreamView.init( sliceWidth, sliceHeight, 35.0, 50.0 );
    widgetStreamView.setData( texturesData,
                              sliceSize,
                              { (int)sliceWidth, (int)sliceHeight },
                              hub::SensorSpec::Format::Y16 );

    ui->mdiArea->addSubWindow( m_imageManipulator );
}

MainWindow::~MainWindow() {
    delete m_streamView2D;
    delete m_streamView2D_2;
    delete ui;
}
