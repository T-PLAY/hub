#ifndef STREAMWINDOW_H
#define STREAMWINDOW_H

#include <QMainWindow>

#include "../../src/bridge/Bridge.h"
#include "../../src/imStream/ImageStream.h"

namespace Ui {
    class StreamWindow;
}

class StreamWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StreamWindow(QWidget *parent = nullptr);
    ~StreamWindow();

    void setIp(const std::string& ip);

private:
    Ui::StreamWindow *ui;

    bool _status;
    SyncInfo _infos;
    Bridge _bridge;
    std::unique_ptr<ImageStream> _stream;
    std::thread _streamUpdateThread;
    std::thread _updateThread;
    bool _alive = true;

    std::string _baseIp = "127.0.0.1";
    std::string _baseMessage = "Stream is stopped";

    void update_values();
    void update_fields();
    void update_infos();

    bool start_sync();
    bool stop_sync();

    void message_update();
    void stream_update();

private slots :
    void start_clicked();
    void start_stream_clicked();
    void stop_clicked();
    void stop_stream_clicked();
    void update_clicked();
    void select_clicked();
    void tracker_changed(int index);
    void sender_changed(int index);
    void source_changed(int index);
    void updateStream_clicked();

};

#endif
