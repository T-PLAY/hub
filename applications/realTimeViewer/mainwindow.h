#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <server.h>

//Q_DECLARE_METATYPE(Streamer);

namespace Ui {
class MainWindow;
}

class Thread_Client : public QThread
{
    Q_OBJECT
public:
    // constructor
    explicit Thread_Client(QObject * parent = 0);

signals:
//    void addViewStreamSignal(Streamer streamer);
    void addViewStreamSignal(int iStreamer);
//    void addViewStreamSignal();

public:
    // overriding the QThread's run() method
    void run();
private:
//    Ui::MainWindow * mUi;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:


public slots:
//    void addViewStream(Streamer streamer);
    void addViewStream(int iStreamer);
//    void addViewStream();


private:
    Ui::MainWindow *ui;
//    Thread_Client mThread;
    Thread_Client * mThreadClient;
};

#endif // MAINWINDOW_H
