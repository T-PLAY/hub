#ifndef HUB_H
#define HUB_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QErrorMessage>
#include <QMainWindow>
#include <QMessageBox>

#include "../diapo/diapowindow.h"
#include "../select/selectwindow.h"
#include "../stream/streamwindow.h"
#include "../../src/hub/modesender.h"



enum ProgramMode {
    DIAPO,
    SELECT,
    STREAM
};

namespace Ui {
class Hub;
}

class Hub final : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hub(QWidget *parent = nullptr);
    ~Hub() final;

private:
    Ui::Hub *ui;

    DiapoWindow _dw;
    StreamWindow _stw;
    SelectWindow _sw;

    QErrorMessage _em;

    uint _mode;

    bool sendMode();
    void showError(const QString& msg);

private slots:
    void OnDetectClicked();
    void OnLaunchClicked();
};

#endif // HUB_H
