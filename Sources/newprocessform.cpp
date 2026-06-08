#include "newprocessform.h"
#include "ui_newprocessform.h"

NewProcessForm::NewProcessForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewProcessForm)
{
    ui->setupUi(this);
    setWindowTitle("Agregar Proceso");
}

NewProcessForm::~NewProcessForm()
{
    delete ui;
}

int NewProcessForm::getBurstTime()
{
    return ui->burstSpinBox->value();
}

int NewProcessForm::getIOBurstTime()
{
    return ui->ioBurstSpinBox->value();
}

int NewProcessForm::getPriority()
{
    return ui->prioritySpinBox->value();
}

int NewProcessForm::getArrivalTime()
{
    return ui->arrivalTimeSpinBox->value();
}

void NewProcessForm::on_buttonBox_accepted()
{
    accept();
}

