#ifndef GENERATORFORM_H
#define GENERATORFORM_H

#include <QDialog>

namespace Ui {
class GeneratorForm;
}

class GeneratorForm : public QDialog
{
    Q_OBJECT

public:
    explicit GeneratorForm(QWidget *parent = nullptr);
    ~GeneratorForm();

    int getMaxBurst();
    int getMaxIOBurst();
    int getMaxPriority();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GeneratorForm *ui;
};

#endif // GENERATORFORM_H
