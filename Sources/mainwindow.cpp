#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fcfsscheduler.h"
#include "sjfscheduler.h"
#include "randomscheduler.h"
#include "srtfscheduler.h"
#include "roundrobinscheduler.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), simulationRunning(false)
{
    ui->setupUi(this);
    on_algorithmComboBox_currentIndexChanged(0);

    ui->quantumSpinBox->setVisible(false);
    ui->quantumLabel->setVisible(false);

    simulationTimer = new QTimer(this);
    simulationTimer->setInterval(500); // 500 ms entre ticks en modo auto-play
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::simulationTick);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete simulationTimer;
}

void MainWindow::on_algorithmComboBox_currentIndexChanged(int index)
{
    // Mostrar / ocultar quantum
    bool isRoundRobin = (index == 3);
    ui->quantumLabel->setVisible(isRoundRobin);
    ui->quantumSpinBox->setVisible(isRoundRobin);

    // Crear el scheduler correspondiente
    switch (index)
    {
    case 0: // FCFS
        simulator.setScheduler(std::make_unique<FCFSScheduler>());
        ui->algorithmDescription->setText(
            "FCFS (First-Come, First-Served): El proceso que solicita la CPU "
            "primero es el primero en ser asignado. No expulsivo.");
        break;
    case 1: // SJF
        simulator.setScheduler(std::make_unique<SJFScheduler>());
        ui->algorithmDescription->setText(
            "SJF (Shortest Job First): Selecciona el proceso con el menor tiempo "
            "de ráfaga. No es expulsivo.");
        break;
    case 2: // SRTF
        simulator.setScheduler(std::make_unique<SRTFScheduler>());
        ui->algorithmDescription->setText(
            "SRTF (Shortest Remaining Time First): Versión expulsiva de SJF. "
            "Selecciona el proceso con el menor tiempo restante.");
        break;
    case 3: // Round Robin
        simulator.setScheduler(std::make_unique<RoundRobinScheduler>(ui->quantumSpinBox->value()));
        ui->algorithmDescription->setText(
            "Round Robin: Asigna un quantum de tiempo a cada proceso. "
            "Es expulsivo y circular.");
        break;
    case 6: // Random
        simulator.setScheduler(std::make_unique<RandomScheduler>());
        ui->algorithmDescription->setText(
            "Random: Selecciona un proceso al azar de la cola de listos. "
            "No es expulsivo.");
        break;
    // Los demás algoritmos aún no están implementados
    default:
        simulator.setScheduler(std::make_unique<FCFSScheduler>());
        ui->algorithmDescription->setText(
            "Algoritmo no implementado aún. Se usa FCFS como respaldo.");
        break;
    }
}


void MainWindow::on_startButton_clicked()
{
    if (simulator.getAllProcesses().empty())
    {
        ui->statusbar->showMessage("Primero genera procesos.", 3000);
        return;
    }

    simulationTimer->start();
    simulationRunning = true;

    ui->startButton->setEnabled(false);
    ui->stepButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->generateButton->setEnabled(false);
}


void MainWindow::on_stepButton_clicked()
{
    if (simulator.getAllProcesses().empty())
    {
        ui->statusbar->showMessage("Primero genera procesos.", 3000);
        return;
    }
    if (isSimulationComplete())
    {
        statusBar()->showMessage("La simulación ya ha terminado.", 3000);
        return;
    }

    simulator.nextTick();
    captureGanttState();
    updateUI();
}

void MainWindow::on_pauseButton_clicked()
{
    simulationTimer->stop();
    simulationRunning = false;

    ui->startButton->setEnabled(true);
    ui->stepButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->generateButton->setEnabled(true);
}


void MainWindow::on_resetButton_clicked()
{
    on_pauseButton_clicked();
    simulator.reset();
    ganttHistory.clear();

    // Limpiar toda la UI
    ui->ganttChart->clear();
    ui->ganttChart->setRowCount(0);
    ui->ganttChart->setColumnCount(0);
    ui->processTable->setRowCount(0);
    ui->readyListWidget->clear();
    ui->blockedListWidget->clear();

    ui->tickLabel->setText("Tick: 0");
    ui->cpuProcessLabel->setText("CPU Idle");

    updateStatistics();

    ui->startButton->setEnabled(true);
    ui->stepButton->setEnabled(true);

    statusBar()->showMessage("Simulación reiniciada.", 3000);
}

void MainWindow::simulationTick()
{
    if (isSimulationComplete())
    {
        on_pauseButton_clicked();
        statusBar()->showMessage("Simulación completada.", 5000);
        return;
    }

    simulator.nextTick();
    captureGanttState();
    updateUI();
}


void MainWindow::on_generateButton_clicked()
{
    on_pauseButton_clicked();

    int count = ui->processCountSpinBox->value();
    simulator.generateRandomProcesses(count, 10, 5, 5);

    // Limpiar historial del Gantt
    ganttHistory.clear();
    ui->ganttChart->clear();
    ui->ganttChart->setRowCount(0);
    ui->ganttChart->setColumnCount(0);

    ui->tickLabel->setText("Tick: 0");
    ui->cpuProcessLabel->setText("CPU Idle");

    updateProcessTable();
    updateQueues();
    updateStatistics();

    ui->startButton->setEnabled(true);
    ui->stepButton->setEnabled(true);
    simulationRunning = false;

    statusBar()->showMessage("Se generaron " + QString::number(count) + " procesos aleatorios.", 3000);
}

void MainWindow::updateUI()
{
    ui->tickLabel->setText("Tick: " + QString::number(simulator.getCurrentTick()));

    // CPU label
    Process *cpu = simulator.getCpuProcess();
    if (cpu)
    {
        ui->cpuProcessLabel->setText("P" + QString::number(cpu->getId()) +" —  Restante: " + QString::number(cpu->getRemainingTime()) + " ticks");
    }
    else
    {
        ui->cpuProcessLabel->setText("CPU Idle");
    }

    updateProcessTable();
    updateGanttChart();
    updateQueues();
    updateStatistics();
}

void MainWindow::updateProcessTable()
{
    const auto &processes = simulator.getAllProcesses();
    ui->processTable->setRowCount(static_cast<int>(processes.size()));

    for (int i = 0; i < static_cast<int>(processes.size()); ++i)
    {
        Process *p = processes[i];

        ui->processTable->setItem(i, 0, new QTableWidgetItem(QString::number(p->getId())));
        ui->processTable->setItem(i, 1, new QTableWidgetItem(QString::number(p->getPriority())));
        ui->processTable->setItem(i, 2, new QTableWidgetItem(QString::number(p->getArrivalTime())));
        ui->processTable->setItem(i, 3, new QTableWidgetItem(stateToString(p->getState())));
        ui->processTable->setItem(i, 4, new QTableWidgetItem(QString("%1 / %2").arg(p->getBurstTime()).arg(p->getRemainingTime())));
        ui->processTable->setItem(i, 5, new QTableWidgetItem(QString("%1 / %2").arg(p->getIoBurstTime()).arg(p->getIoRemainingTime())));
        ui->processTable->setItem(i, 6, new QTableWidgetItem(QString::number(p->getBlockedTime())));
        ui->processTable->setItem(i, 7, new QTableWidgetItem(QString::number(p->getWaitingTime())));
        ui->processTable->setItem(i, 8, new QTableWidgetItem(QString::number(p->getTurnaroundTime())));

        // Colorear la celda de estado
        QTableWidgetItem *stateItem = ui->processTable->item(i, 3);
        if (stateItem)
        {
            stateItem->setBackground(stateToColor(p->getState()));
            stateItem->setForeground(Qt::white);
        }
    }

    ui->processTable->resizeColumnsToContents();
}

void MainWindow::updateGanttChart()
{
    const auto &processes = simulator.getAllProcesses();
    int numProcesses = static_cast<int>(processes.size());
    int numTicks = static_cast<int>(ganttHistory.size());

    ui->ganttChart->setRowCount(numProcesses);
    ui->ganttChart->setColumnCount(numTicks);

    // Encabezados de fila (IDs de proceso)
    QStringList rowHeaders;
    for (int i = 0; i < numProcesses; ++i)
    {
        rowHeaders << "P" + QString::number(processes[i]->getId());
    }

    ui->ganttChart->setVerticalHeaderLabels(rowHeaders);

    // Encabezados de columna (número de tick)
    QStringList colHeaders;
    for (int t = 0; t < numTicks; ++t)
    {
        colHeaders << QString::number(t);
    }

    ui->ganttChart->setHorizontalHeaderLabels(colHeaders);

    // Pintar solo la columna más reciente (las anteriores ya están pintadas)
    int latestTick = numTicks - 1;
    if (latestTick >= 0)
    {
        for (int i = 0; i < numProcesses && i < static_cast<int>(ganttHistory[latestTick].size()); ++i)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            ProcessState state = ganttHistory[latestTick][i];

            if (state != ProcessState::NEW)
            {
                item->setBackground(stateToColor(state));
                item->setForeground(Qt::white);

                switch (state)
                {
                case ProcessState::RUNNING:
                    item->setText("E");
                    break;
                case ProcessState::READY:
                    item->setText("L");
                    break;
                case ProcessState::BLOCKED:
                    item->setText("B");
                    break;
                case ProcessState::TERMINATED:
                    item->setText("T");
                    break;
                default:
                    break;
                }
            }

            item->setTextAlignment(Qt::AlignCenter);
            ui->ganttChart->setItem(i, latestTick, item);
        }
    }

    // Auto-scroll al último tick
    if (numTicks > 0)
        ui->ganttChart->scrollToItem(ui->ganttChart->item(0, numTicks - 1));
}

void MainWindow::updateQueues()
{
    // Cola de Listos
    ui->readyListWidget->clear();
    for (Process *p : simulator.getReadyQueue())
    {
        ui->readyListWidget->addItem("P" + QString::number(p->getId()) +" (Prio: " + QString::number(p->getPriority()) + ")");
    }

    // Cola de Bloqueados
    ui->blockedListWidget->clear();
    for (Process *p : simulator.getBlockedQueue())
    {
        ui->blockedListWidget->addItem("P" + QString::number(p->getId()) +" (E/S rest: " + QString::number(p->getIoRemainingTime()) + ")");
    }
}

void MainWindow::updateStatistics()
{
    ui->cpuUtilLabel->setText("CPU: " + QString::number(simulator.getCpuUtilization(), 'f', 1) + "%");
    ui->avgWaitLabel->setText("Espera Promedio: " + QString::number(simulator.getAverageWaitingTime(), 'f', 1));
    ui->avgBlockedLabel->setText("Bloqueo Promedio: " + QString::number(simulator.getAverageBlockedTime(), 'f', 1));
    ui->avgTurnaroundLabel->setText("Ciclo Promedio: " + QString::number(simulator.getAverageTurnaroundTime(), 'f', 1));
}

void MainWindow::captureGanttState()
{
    const auto &processes = simulator.getAllProcesses();
    std::vector<ProcessState> tickStates;
    tickStates.reserve(processes.size());

    for (Process *p : processes)
    {
        tickStates.push_back(p->getState());
    }

    ganttHistory.push_back(tickStates);
}

QString MainWindow::stateToString(ProcessState state) const
{
    switch (state)
    {
    case ProcessState::NEW:
        return "Nuevo";
    case ProcessState::READY:
        return "Listo";
    case ProcessState::RUNNING:
        return "Ejecutando";
    case ProcessState::BLOCKED:
        return "Bloqueado";
    case ProcessState::TERMINATED:
        return "Terminado";
    default:
        return "—";
    }
}

QColor MainWindow::stateToColor(ProcessState state) const
{
    switch (state)
    {
    case ProcessState::RUNNING:
        return QColor("#4CAF50"); // Verde
    case ProcessState::READY:
        return QColor("#FFC107"); // Ámbar
    case ProcessState::BLOCKED:
        return QColor("#F44336"); // Rojo
    case ProcessState::TERMINATED:
        return QColor("#9E9E9E"); // Gris
    case ProcessState::NEW:
        return QColor("#2196F3"); // Azul
    default:
        return QColor("#FFFFFF");
    }
}

bool MainWindow::isSimulationComplete() const
{
    const auto &processes = simulator.getAllProcesses();
    if (processes.empty())
    {
        return true;
    }

    for (Process *p : processes)
    {
        if (p->getState() != ProcessState::TERMINATED)
        {
            return false;
        }
    }
    return true;
}


