#include "Headers/simulator.h"
#include <cstdlib>
#include <ctime>

Simulator::Simulator() : currentTick(0), cpuBusyTicks(0), cpuProcess(nullptr), scheduler(nullptr), newArrivalsInCurrentTick(0)
{
    std::srand(std::time(nullptr)); // Semilla para números aleatorios
}

Simulator::~Simulator() {
    reset();
}

void Simulator::reset()
{
    // Liberar la memoria de todos los procesos creados
    for (Process* p : allProcesses) {
        delete p;
    }
    allProcesses.clear();
    readyQueue.clear();
    blockedQueue.clear();
    terminatedQueue.clear();
    cpuProcess = nullptr;
    currentTick = 0;
    cpuBusyTicks = 0;
    newArrivalsInCurrentTick = 0;
}

void Simulator::setScheduler(std::unique_ptr<Scheduler> newScheduler)
{
    scheduler = std::move(newScheduler);
}

void Simulator::addProcess(Process* p)
{
    allProcesses.push_back(p);
}

void Simulator::generateRandomProcesses(int count, int maxBurst, int maxIO, int maxPriority)
{
    reset();
    for (int i = 1; i <= count; ++i)
    {
        int burst = (std::rand() % maxBurst) + 1; // Mínimo 1 tick
        int io = (std::rand() % (maxIO + 1));     // Puede ser 0 si no requiere E/S
        int priority = (std::rand() % maxPriority) + 1;
        int arrival = std::rand() % 5;            // Llegan entre el tick 0 y 4

        addProcess(new Process(i, burst, io, priority, arrival));
    }
}

// --- EL MOTOR DE LA SIMULACIÓN ---
void Simulator::nextTick()
{
    newArrivalsInCurrentTick = 0;

    // Manejar las nuevas llegadas
    for (Process* p : allProcesses)
    {
        if (p->getArrivalTime() == currentTick && p->getState() == ProcessState::NEW)
        {
            p->setState(ProcessState::READY);
            readyQueue.push_back(p);
            newArrivalsInCurrentTick++;
        }
    }

    // Manejar procesos bloqueados en E/S
    for (auto it = blockedQueue.begin(); it != blockedQueue.end(); )
    {
        Process* p = *it;
        p->decrementIoRemainingTime();

        if (p->getIoRemainingTime() == 0)
        {
            p->setState(ProcessState::READY);
            readyQueue.push_back(p);
            it = blockedQueue.erase(it); // Lo sacamos de bloqueados
        }
        else
        {
            ++it;
        }
    }

    // Ejecutar proceso en CPU (si existe)
    if (cpuProcess != nullptr)
    {
        cpuProcess->decrementRemainingTime();
        cpuBusyTicks++;

        // Caso A: El proceso terminó su ejecución
        if (cpuProcess->getRemainingTime() == 0)
        {
            cpuProcess->setState(ProcessState::TERMINATED);
            cpuProcess->calculateTurnaroundTime(currentTick + 1); // +1 porque termina al final de este tick
            terminatedQueue.push_back(cpuProcess);
            cpuProcess = nullptr;
        }
        // Caso B: El proceso llegó a la mitad de su vida y necesita E/S (Bloqueo)
        else if (cpuProcess->getIoBurstTime() > 0 &&
                 cpuProcess->getRemainingTime() == (cpuProcess->getBurstTime() / 2) &&
                 cpuProcess->getIoRemainingTime() == cpuProcess->getIoBurstTime())
        {
            cpuProcess->setState(ProcessState::BLOCKED);
            blockedQueue.push_back(cpuProcess);
            cpuProcess = nullptr; // La CPU queda libre
        }
    }

    // Incrementar métricas de espera y bloqueo para los que están en las colas
    for (Process* p : readyQueue)
    {
        p->incrementWaitingTime();
    }

    for (Process* p : blockedQueue)
    {
        p->incrementBlockedTime();
    }

    // Invocar al Planificador (Scheduler)
    if (scheduler)
    {
        // Si el algoritmo es expulsivo, planifica en cada tick.
        // Si no lo es, solo planifica si la CPU está vacía.
        if (scheduler->isPreemptive() || cpuProcess == nullptr)
        {
            scheduler->schedule(readyQueue, cpuProcess, currentTick);
        }
    }

    // Avanzar el reloj global
    currentTick++;
}

// --- CÁLCULO DE ESTADÍSTICAS ---
double Simulator::getCpuUtilization() const
{
    if (currentTick == 0)
    {
        return 0.0;
    }

    return (static_cast<double>(cpuBusyTicks) / currentTick) * 100.0;
}

double Simulator::getAverageWaitingTime() const
{
    if (terminatedQueue.empty())
    {
        return 0.0;
    }

    double sum = 0;

    for (Process* p : terminatedQueue)
    {
        sum += p->getWaitingTime();
    }

    return sum / terminatedQueue.size();
}

double Simulator::getAverageBlockedTime() const
{
    if (terminatedQueue.empty())
    {
        return 0.0;
    }

    double sum = 0;

    for (Process* p : terminatedQueue)
    {
        sum += p->getBlockedTime();
    }

    return sum / terminatedQueue.size();
}

double Simulator::getAverageTurnaroundTime() const
{
    if (terminatedQueue.empty())
    {
        return 0.0;
    }

    double sum = 0;

    for (Process* p : terminatedQueue)
    {
        sum += p->getTurnaroundTime();
    }

    return sum / terminatedQueue.size();
}

int Simulator::getTotalCompletedProcesses() const { return terminatedQueue.size(); }
int Simulator::getNewArrivalsInCurrentTick() const { return newArrivalsInCurrentTick; }
int Simulator::getCurrentTick() const { return currentTick; }
Process* Simulator::getCpuProcess() const { return cpuProcess; }
const std::vector<Process*>& Simulator::getReadyQueue() const { return readyQueue; }
const std::vector<Process*>& Simulator::getBlockedQueue() const { return blockedQueue; }
const std::vector<Process*>& Simulator::getTerminatedQueue() const { return terminatedQueue; }
const std::vector<Process*>& Simulator::getAllProcesses() const { return allProcesses; }
