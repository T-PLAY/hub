#ifndef FORMSENSORVIEW_H
#define FORMSENSORVIEW_H

#include <QWidget>

namespace Ui {
class FormSensorView;
}

class FormSensorView : public QWidget {
    Q_OBJECT

public:
    explicit FormSensorView(QWidget* parent = nullptr, std::string sensorName = "", std::string format = "", std::string dims = "", std::string size = "");
    ~FormSensorView();

    void setRadioButtonOff();

signals:
    void addViewStreamSignal(std::string streamerSensorName);
    void delViewStreamSignal(std::string streamerSensorName);

private slots:
    void on_radioButtonOnOff_clicked(bool checked);

private:
    Ui::FormSensorView* ui;
    std::string mSensorName;
};

#endif // FORMSENSORVIEW_H
