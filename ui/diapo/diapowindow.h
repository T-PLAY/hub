#ifndef DIAPOWINDOW_H
#define DIAPOWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>

#include "../../src/imLoad/uploader.h"

namespace Ui {
class DiapoWindow;
}

class DiapoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiapoWindow(QWidget *parent = nullptr);
    ~DiapoWindow();

    void setIp(const std::string& ip);

private:
    Ui::DiapoWindow *ui;

private slots :
    void select_clicked();
    void upload_clicked();
};

#endif
