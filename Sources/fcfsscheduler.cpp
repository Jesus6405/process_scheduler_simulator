#include "fcfsscheduler.h"

void FCFSScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) {
    // FCFS no es expulsivo. Si ya hay algo corriendo, no hacemos nada.
    if (currentProcess != nullptr)
    {
        return;
    }

    // Si la cola de listos está vacía, la CPU se queda inactiva
    if (readyQueue.empty())
    {
        return;
    }

    currentProcess = readyQueue.front();
    currentProcess->setState(ProcessState::RUNNING);

    // Lo sacamos de la cola de listos
    readyQueue.erase(readyQueue.begin());
}

bool FCFSScheduler::isPreemptive() const
{
    return false;
}
