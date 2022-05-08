
#include <MainWindowStreamView.h>
//#include "../libViewer/MainWindowStreamView.h"

#include <QApplication>

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );
    qRegisterMetaType<std::string>( "std::string" );
    //    MainWindowStreamView w(nullptr, "proceduralStreamer");
    //     MainWindowStreamView w(nullptr, "L500 Depth Sensor (Depth)");

//    MainWindowStreamView w( nullptr, "ULA-OP 256" );
//    w.show();

    return a.exec();
}
