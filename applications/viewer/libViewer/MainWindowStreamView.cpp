#include "MainWindowStreamView.h"
#include "ui_MainWindowStreamView.h"

#include <WidgetStreamView.h>
#include <cmath>
//#include <socket.h>

//////////////////////////////////////////////////////////////////////

MainWindowStreamView::MainWindowStreamView( const hub::InputSensor& inputStream, QWidget* parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindowStreamView ),
    m_inputStream( inputStream )
//    , mThread(this, sensorName)
//    , mSensorName(sensorName)
{
    const auto& sensorSpec = m_inputStream.m_spec;
    std::cout << "MainWindow::MainWindowStreamView(parent, " << sensorSpec.m_sensorName << ")"
              << std::endl;

    ui->setupUi( this );

    resize( 1, 1 );

    assert( ui->centralwidget != nullptr );
    delete ui->centralwidget;

    const auto& resolutions = sensorSpec.m_resolutions;
    assert( resolutions.size() == 1 );
    const auto& dims = resolutions.at( 0 ).first;
    if ( dims.size() == 1 ) {
        ui->centralwidget = new WidgetStreamView1D( this );

        ui->centralwidget->setMinimumSize( 350, 35 );
    }
    else if ( dims.size() == 2 ) {

        ui->centralwidget = new WidgetStreamView2D( this );

        ui->centralwidget->setMinimumWidth( dims.at( 0 ) );
        ui->centralwidget->setMinimumHeight( dims.at( 1 ) );
    }
    else {
        std::cout << "unprocessed dimension" << std::endl;
        exit( 1 );
    }
    ui->centralwidget->setObjectName( QString::fromUtf8( "centralwidget" ) );
    setCentralWidget( ui->centralwidget );

    //    QObject::connect(&mThread, &Thread_InputStream::newImage, this,
    //    &MainWindowStreamView::newImage); mThread.start();

    //    mStartFps = std::chrono::high_resolution_clock::now();
}

MainWindowStreamView::~MainWindowStreamView() {
    std::cout << "~MainWindowStreamView()" << std::endl;

    delete ui;

    //    mThread.requestInterruption();
    //    std::cout << "~MainWindowStreamView() requested interruption" << std::endl;
    //    mThread.wait();
    //    std::cout << "~MainWindowStreamView() mThread join" << std::endl;

    emit onCloseStreamViewSignal();
}

void MainWindowStreamView::setData( unsigned char* img_ptr,
                                    std::vector<int> dims,
                                    hub::SensorSpec::Format format ) {
    //    ( static_cast<WidgetStreamView*>( ui->centralwidget ) )->setData( img_ptr, dims, format );
    ( static_cast<WidgetStreamView*>( ui->centralwidget ) )
        ->setData( img_ptr, 192 * 512, dims, format );
}

// std::string MainWindowStreamView::getStreamerSensorName() const
//{
//     return m_inputStream.getSensorName();
// }
