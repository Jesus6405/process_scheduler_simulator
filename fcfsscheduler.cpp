#include "fcfsscheduler.h"
#include <algorithm>

void FCFSScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    // FCFS is non-preemptive: If a process is currently running, it keeps the CPU.
    if (currentProcess != nullptr)
    {
        return;
    }

    // If there are no processes waiting in the ready queue, CPU remains idle.
    if (readyQueue.empty())
    {
        return;
    }

    // Since FCFS depends on arrival time, we look for the process that arrived first.
    auto firstArrivedIt = std::min_element(readyQueue.begin(), readyQueue.end(),
                                           [](const Process* a, const Process* b) {
                                               return a->getArrivalTime() < b->getArrivalTime();
                                           });

    // Assign the selected process to the CPU
    currentProcess = *firstArrivedIt;
    currentProcess->setState(ProcessState::RUNNING);

    // Remove it from the ready queue
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
