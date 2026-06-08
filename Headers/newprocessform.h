#ifndef NEWPROCESSFORM_H
#define NEWPROCESSFORM_H

#include <QDialog>

namespace Ui {
class NewProcessForm;
}

class NewProcessForm : public QDialog
{
    Q_OBJECT

public:
    explicit NewProcessForm(QWidget *parent = nullptr);
    ~NewProcessForm();

    int getBurstTime();
    int getIOBurstTime();
    int getPriority();
    int getArrivalTime();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewProcessForm *ui;
};

#endif // NEWPROCESSFORM_H
