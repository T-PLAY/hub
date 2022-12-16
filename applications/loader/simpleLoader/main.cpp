#include "MainWindow.h"

#include <QApplication>

int main( int argc, char* argv[] ) {
    qRegisterMetaType<std::string>( "std::string" );

    QApplication a( argc, argv );
    MainWindow w;
    w.show();


    return a.exec();
}
