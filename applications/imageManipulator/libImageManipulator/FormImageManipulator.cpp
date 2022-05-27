#include "FormImageManipulator.h"
#include "ui_FormImageManipulator.h"

FormImageManipulator::FormImageManipulator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormImageManipulator)
{
    ui->setupUi(this);
}

FormImageManipulator::~FormImageManipulator()
{
    delete ui;
}
