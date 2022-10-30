#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIntValidator>
#include <QObject>
#include <QSpinBox>

#include <Acquisition.hpp>
//#include <IO/Stream.hpp>
#include <Configurations.hpp>
#include <IO/File.hpp>

#include <filesystem>
#include <fstream>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QObject::connect( ui->spinBox_latency,
                      static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
                      this,
                      &MainWindow::updateLatency );

    //    {
    std::string filename =
        PROJECT_DIR "data/records/5HzRadialAxial/Polhemus Patriot (sensor 2).txt";
    std::cout << "read '" << filename << "' record" << std::endl;
    assert( std::filesystem::exists( filename ) );

    std::fstream file( filename, std::ios::binary | std::ios::in );
    assert( file.is_open() );

    hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );

    m_acqs = inputSensor.getAllAcquisitions();

    std::cout << m_acqs.size() << " acquisitions read." << std::endl;
    //    }

    //    {
    std::string filename2 = PROJECT_DIR "data/records/5HzRadialAxial/ULA-OP 256.txt";
    std::cout << "read '" << filename2 << "' record" << std::endl;
    assert( std::filesystem::exists( filename2 ) );

    std::fstream file2( filename2, std::ios::binary | std::ios::in );
    assert( file2.is_open() );

    hub::InputSensor inputSensor2( hub::io::File( std::move( file2 ) ) );

    m_acqs2 = inputSensor2.getAllAcquisitions();

    std::cout << m_acqs2.size() << " acquisitions read." << std::endl;
    //    }

    m_outputSensor = new hub::OutputSensor(
        inputSensor.m_spec + inputSensor2.m_spec,
        hub::io::OutputStream( inputSensor.m_spec.m_sensorName + " (timeShifted)",
                               hub::net::ClientSocket( hub::net::s_defaultServiceIp, 4042 ) ) );

    auto it = m_acqs.begin();

    assert( it->getMeasures().size() == 1 );
    assert( m_acqs2.front().getMeasures().size() == 1 );

    for ( const auto& acq : m_acqs2 ) {
        const auto startMaster = acq.m_start + m_latency * 1000;
        int dist               = std::abs( startMaster - it->m_start );
        while ( it + 1 != m_acqs.end() && dist > std::abs( startMaster - ( it + 1 )->m_start ) ) {
            ++it;
            dist = std::abs( startMaster - it->m_start );
        }
        *m_outputSensor << ( hub::Acquisition { startMaster, startMaster } << it->getMeasures()
                                                                           << acq.getMeasures() );
    }
}

MainWindow::~MainWindow() {
    delete ui;

    delete m_outputSensor;
}

void MainWindow::updateLatency() {
    m_latency = ui->spinBox_latency->value();
    *m_outputSensor << ( hub::Acquisition( -1, -1 )
                         << m_acqs.front().getMeasures() << m_acqs2.front().getMeasures() );

    //    for ( const auto& acq : m_acqs ) {
    //        *m_outputSensor
    //            << ( hub::Acquisition { acq.m_start + m_latency * 1000, acq.m_end + m_latency *
    //            1000 }
    //                 << acq.getMeasures() );
    //    }
    auto it = m_acqs.begin();

    assert( it->getMeasures().size() == 1 );
    assert( m_acqs2.front().getMeasures().size() == 1 );

    for ( const auto& acq : m_acqs2 ) {
        const auto startMaster = acq.m_start + m_latency * 1000;
        int dist               = std::abs( startMaster - it->m_start );
        while ( it + 1 != m_acqs.end() && dist > std::abs( startMaster - ( it + 1 )->m_start ) ) {
            ++it;
            dist = std::abs( startMaster - it->m_start );
        }
        *m_outputSensor << ( hub::Acquisition { startMaster, startMaster } << it->getMeasures()
                                                                           << acq.getMeasures() );
    }
}

void MainWindow::on_pushButton_minus_clicked() {
    ui->spinBox_latency->setValue( ui->spinBox_latency->value() - 10 );
}

void MainWindow::on_pushButton_plus_clicked() {
    ui->spinBox_latency->setValue( ui->spinBox_latency->value() + 10 );
}
