#include <QApplication>
#include "ui/hub/hub.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Hub h;
    h.show();

    return QApplication::exec();
}