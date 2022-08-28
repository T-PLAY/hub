#include "Form3DToolBox.h"
#include "ui_Form3DToolBox.h"

Form3DToolBox::Form3DToolBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form3DToolBox)
{
    ui->setupUi(this);
}

Form3DToolBox::~Form3DToolBox()
{
    delete ui;
}

void Form3DToolBox::on_pushButton_fitScene_clicked()
{
    emit pushButton_fitScene_clicked();
}


void Form3DToolBox::on_pushButton_fitTrace_clicked()
{
    emit pushButton_fitTrace_clicked();
}


void Form3DToolBox::on_checkBox_showGrid_toggled(bool checked)
{
    emit checkBox_showGrid_toggled(checked);
}


void Form3DToolBox::on_checkBox_showTrace_toggled(bool checked)
{
    emit checkBox_showTrace_toggled(checked);
}



void Form3DToolBox::on_doubleSpinBox_transparency_valueChanged(double tranparency)
{
    emit doubleSpinBox_transparency_valueChanged(tranparency);
}


void Form3DToolBox::on_doubleSpinBox_transparency_2_valueChanged(double transparency)
{
    emit doubleSpinBox_transparency2_valueChanged(transparency);

}

