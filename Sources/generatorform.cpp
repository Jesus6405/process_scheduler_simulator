#include "generatorform.h"
#include "ui_generatorform.h"

GeneratorForm::GeneratorForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GeneratorForm)
{
    ui->setupUi(this);
    setWindowTitle("Generar Aleatoriamente");
}

GeneratorForm::~GeneratorForm()
{
    delete ui;
}

int GeneratorForm::getMaxBurst()
{
    return ui->maxBurstSpinBox->value();
}

int GeneratorForm::getMaxIOBurst()
{
    return ui->maxIOBurstSpinBox->value();
}

int GeneratorForm::getMaxPriority()
{
    return ui->maxPrioritySpinBox->value();
}

void GeneratorForm::on_buttonBox_accepted()
{
    accept();
}

