#include "MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

#include <SensorSpec.hpp>

int main( int argc, char* argv[] ) {

    // Set application and organization names in order to ensure uniform
    // QSettings configurations.
    // \see http://doc.qt.io/qt-5/qsettings.html#QSettings-4
    QCoreApplication::setOrganizationName( "STORM-IRIT" );
    QCoreApplication::setApplicationName( "RadiumViewer" );

    // Create default format for Qt.
    QSurfaceFormat format;
    format.setVersion( 4, 4 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setDepthBufferSize( 24 );
    format.setStencilBufferSize( 8 );
    // format.setSamples( 16 );
    format.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
    format.setSwapInterval( 0 );
    QSurfaceFormat::setDefaultFormat( format );

    QApplication a( argc, argv );
    qRegisterMetaType<std::string>( "std::string" );
    qRegisterMetaType<hub::SensorSpec>( "hub::SensorSpec" );
    MainWindow w;
    //    w.resize(1280, 840);
    w.show();
    return a.exec();
}
