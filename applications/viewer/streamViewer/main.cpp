
#include <MainWindowStreamView.h>
//#include "../libViewer/MainWindowStreamView.h"

#include <QApplication>

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );
    qRegisterMetaType<std::string>( "std::string" );


    return a.exec();
}
