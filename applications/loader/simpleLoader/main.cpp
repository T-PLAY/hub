#include "MainWindow.h"

#include <QApplication>
// Q_DECLARE_METATYPE(std::string)

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );
    MainWindow w;
    w.show();

    return a.exec();
}
