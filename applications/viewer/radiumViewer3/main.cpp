#include "MainWindow.h"

#include <QApplication>
//#include <stream.h>
// Q_DECLARE_METATYPE(std::string)

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//    qRegisterMetaType<std::string>("std::string");
    MainWindow w;
    w.show();
    return a.exec();
}
