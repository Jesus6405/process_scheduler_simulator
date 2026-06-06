#include "fcfsscheduler.h"
#include <algorithm>

void FCFSScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    // FCFS es no expulsivo (Si un proceso tiene la CPU, mantiene la CPU)
    if (currentProcess != nullptr)
    {
        return;
    }

    // Si no hay procesos esperando en la cola de listos, la CPU permanece inactiva
    if (readyQueue.empty())
    {
        return;
    }

    // Como FCFS depende del tiempo de llegada, se busca el proceso que llegó primero
    auto firstArrivedIt = std::min_element(readyQueue.begin(), readyQueue.end(),
                                           [](const Process* a, const Process* b) {
                                               return a->getArrivalTime() < b->getArrivalTime();
                                           });

    // Se asigna el proceso encontrado a la CPU
    currentProcess = *firstArrivedIt;
    currentProcess->setState(ProcessState::RUNNING);

    // Se quita de la cola de listos
    readyQueue.erase(firstArrivedIt);
}

bool FCFSScheduler::isPreemptive() const
{
    return false;
}

std::string FCFSScheduler::getName() const
{
    return "First-Come, First-Served (FCFS)";
}

std::string FCFSScheduler::getDescription() const
{
    return "FCFS se considera el algoritmo de planificación de CPU más simple. En el algoritmo FCFS, el proceso que solicita la CPU primero es el primero en ser asignado.";
}
