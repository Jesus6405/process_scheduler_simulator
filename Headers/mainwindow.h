#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "simulator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_algorithmComboBox_currentIndexChanged(int index);
    void on_startButton_clicked();
    void on_stepButton_clicked();
    void on_pauseButton_clicked();
    void on_resetButton_clicked();
    void simulationTick();
    void on_generateButton_clicked();
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

    void on_quantumSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    Simulator simulator;
    QTimer *simulationTimer;
    bool simulationRunning;
    // Gantt history: ganttHistory[tick][processIndex] = state
    std::vector<std::vector<ProcessState>> ganttHistory;

    void updateUI();
    void updateProcessTable();
    void updateGanttChart();
    void updateQueues();
    void updateStatistics();
    void captureGanttState();

    QString stateToString(ProcessState state) const;
    QColor stateToColor(ProcessState state) const;
    bool isSimulationComplete() const;
};
#endif // MAINWINDOW_H
