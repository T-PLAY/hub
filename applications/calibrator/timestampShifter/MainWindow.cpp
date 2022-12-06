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
    QObject::connect( ui->spinBox_latency_2,
                      static_cast<void ( QSpinBox::* )( int )>( &QSpinBox::valueChanged ),
                      this,
                      &MainWindow::updateLatency );

    std::string filename =
        PROJECT_DIR "data/records/5HzRadialAxial/Polhemus Patriot (sensor 2).txt";
    std::cout << "read '" << filename << "' record" << std::endl;
    assert( std::filesystem::exists( filename ) );

    std::fstream file( filename, std::ios::binary | std::ios::in );
    assert( file.is_open() );

    hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );

    m_acqs = inputSensor.getAllAcquisitions();

    std::cout << m_acqs.size() << " acquisitions read." << std::endl;

    std::string filename2 = PROJECT_DIR "data/records/5HzRadialAxial/ULA-OP 256.txt";
    std::cout << "read '" << filename2 << "' record" << std::endl;
    assert( std::filesystem::exists( filename2 ) );

    std::fstream file2( filename2, std::ios::binary | std::ios::in );
    assert( file2.is_open() );

    hub::InputSensor inputSensor2( hub::io::File( std::move( file2 ) ) );

    m_acqs2 = inputSensor2.getAllAcquisitions();

    std::cout << m_acqs2.size() << " acquisitions read." << std::endl;

    m_outputSensor = new hub::OutputSensor(
        inputSensor.m_spec + inputSensor2.m_spec,
        hub::io::OutputStream( inputSensor.m_spec.getSensorName() + " (timeShifted)",
                               hub::net::ClientSocket( hub::net::s_defaultServiceIp, 4042 ) ) );

    auto it = m_acqs.begin();

    assert( it->getMeasures().size() == 1 );
    assert( m_acqs2.front().getMeasures().size() == 1 );

    for ( const auto& acq2 : m_acqs2 ) {
        const auto acq2Start = acq2.m_start;
        int dist             = std::abs( acq2Start - it->m_start );
        while ( it + 1 != m_acqs.end() && dist > std::abs( acq2Start - ( it + 1 )->m_start ) ) {
            ++it;
            dist = std::abs( acq2Start - it->m_start );
        }
        *m_outputSensor << ( hub::Acquisition { acq2Start, acq2Start } << it->getMeasures()
                                                                       << acq2.getMeasures() );
    }
}

MainWindow::~MainWindow() {
    delete ui;

    delete m_outputSensor;
}

void MainWindow::updateLatency() {
    long long latency  = ui->spinBox_latency->value();
    long long latency2 = ui->spinBox_latency_2->value();

    *m_outputSensor << ( hub::Acquisition( -1, -1 )
                         << m_acqs.front().getMeasures() << m_acqs2.front().getMeasures() );

    auto it = m_acqs.begin();

    assert( it->getMeasures().size() == 1 );
    assert( m_acqs2.front().getMeasures().size() == 1 );

    for ( const auto& acq2 : m_acqs2 ) {
        const auto acq2Start = acq2.m_start + latency2 * 1000;

        int dist = std::abs( acq2Start - ( it->m_start + latency * 1000 ) );
        while ( it + 1 != m_acqs.end() &&
                dist > std::abs( acq2Start - ( ( it + 1 )->m_start + latency * 1000 ) ) ) {
            ++it;
            dist = std::abs( acq2Start - ( it->m_start + latency * 1000 ) );
        }
        if ( dist > m_maxDist ) {
            std::cout << "dist sup 8ms : " << dist << std::endl;
            continue;
        }

        const hub::Dof6& dof6 = it->getMeasures().front();
        auto it2              = m_acqs.begin();

        assert( ( it - 1 )->m_start < it->m_start );
        assert( it->m_start < ( it + 1 )->m_start );

        auto itStart = it->m_start + latency * 1000;
        if ( itStart > acq2Start ) {
            if ( it == m_acqs.begin() ) continue;
            assert( it != m_acqs.begin() );
            it2 = it - 1;
        }
        else {
            assert( it != m_acqs.end() );
            it2 = it + 1;
        }

        const hub::Dof6& dof62 = it2->getMeasures().front();

        auto it2Start = it2->m_start + latency * 1000;

        assert( (itStart <= acq2Start && acq2Start <= it2Start) || (it2Start <= acq2Start && acq2Start <= itStart) );

        double t            = ( acq2Start - itStart ) / ( it2Start - itStart );
        auto acqInterpolate = hub::Dof6::slerp( dof6, dof62, t );

        hub::Acquisition acq { acq2Start, acq2Start };
        if ( m_interpolate ) {
//            acq << acqInterpolate;
//            acq << acqInterpolate.m
            acq << hub::Measure { acqInterpolate.m_data, acqInterpolate.m_size, {{1}, hub::Format::DOF6} };
        }
        else { acq << hub::Measure { dof6.m_data, dof6.m_size, {{1}, hub::Format::DOF6} }; }
        acq << acq2.getMeasures();
        *m_outputSensor << acq;
    }
    std::cout << std::endl;
}

void MainWindow::on_pushButton_minus_clicked() {
    ui->spinBox_latency->setValue( ui->spinBox_latency->value() - 10 );
}

void MainWindow::on_pushButton_plus_clicked() {
    ui->spinBox_latency->setValue( ui->spinBox_latency->value() + 10 );
}

void MainWindow::on_pushButton_minus_2_clicked() {
    ui->spinBox_latency_2->setValue( ui->spinBox_latency_2->value() - 10 );
}

void MainWindow::on_pushButton_plus_2_clicked() {
    ui->spinBox_latency_2->setValue( ui->spinBox_latency_2->value() + 10 );
}

void MainWindow::on_spinBox_valueChanged( int arg1 ) {
    m_maxDist = arg1;
    updateLatency();
}

void MainWindow::on_checkBox_interpolate_toggled( bool checked ) {
    m_interpolate = checked;
    updateLatency();
}
