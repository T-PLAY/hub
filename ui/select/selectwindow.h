#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>

#include "../../src/imLoad/uploader.h"
#include "../../src/bridge/Bridge.h"

namespace Ui {
    class SelectWindow;
}

class SelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectWindow(QWidget *parent = nullptr);
    ~SelectWindow();

    void setIp(const std::string& ip);

private:
    Ui::SelectWindow *ui;
    bool _status;
    SyncInfo _infos;
    Bridge _bridge;
    std::thread _updateThread;
    bool _alive = true;

    void update_values();
    void update_fields();
    void update_infos();

    bool start_sync();
    bool stop_sync();

    void message_update();

private slots :
    void select_clicked();
    void upload_clicked();
    void start_clicked();
    void stop_clicked();
    void update_clicked();

    void tracker_changed(int index);
    void sender_changed(int index);
};

#endif
