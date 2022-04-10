#include "formsensorview.h"
#include "ui_formsensorview.h"

FormSensorView::FormSensorView(QWidget* parent, std::string sensorName, std::string format, std::string dims, std::string size, std::string metaData)
    : QWidget(parent)
    , ui(new Ui::FormSensorView)
    , mSensorName(sensorName)
{
    ui->setupUi(this);

    ui->labelSensorName->setText(sensorName.c_str());
    ui->labelFormat->setText(format.c_str());
    ui->laberDimensions->setText(dims.c_str());
    ui->labelSize->setText((size + " bytes").c_str());

    if (metaData.empty()) {
        delete ui->label_metadata;
        delete ui->title_metadata;
        delete ui->line_metadata;
        delete ui->horizontalLayout_metaData;
        ui->verticalLayout_4->removeItem(ui->verticalLayout_metaData);
        delete ui->verticalLayout_metaData;

    } else {
        ui->label_metadata->setText(metaData.c_str());
    }
}

FormSensorView::~FormSensorView()
{
    delete ui;
}

void FormSensorView::on_radioButtonOnOff_clicked(bool checked)
{
    if (checked) {
        ui->radioButtonOnOff->setText("on ");
        ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: lightgreen");
        emit addViewStreamSignal(mSensorName);
    } else {
        ui->radioButtonOnOff->setText("off");
        ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: red");
        emit delViewStreamSignal(mSensorName);
    }
}

void FormSensorView::setRadioButtonOff()
{
    ui->radioButtonOnOff->setChecked(false);
    ui->radioButtonOnOff->setText("off");
    ui->frameButtonOnOff->setStyleSheet("border-radius: 10px; background-color: red");
}
