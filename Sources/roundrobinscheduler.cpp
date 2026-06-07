#include "roundrobinscheduler.h"

RoundRobinScheduler::RoundRobinScheduler(int quantum) : quantum(quantum), currentQuantumTicks(0)
{
}

void RoundRobinScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    if (currentProcess != nullptr)
    {
        currentQuantumTicks++;
        if (currentQuantumTicks >= quantum)
        {
            // Expulsar si hay otros procesos esperando
            if (!readyQueue.empty())
            {
                currentProcess->setState(ProcessState::READY);
                readyQueue.push_back(currentProcess);
                currentProcess = nullptr;
            }
            else
            {
                // Si no hay procesos, reiniciar el quantum pero mantenerlo
                currentQuantumTicks = 0;
            }
        }
    }

    if (currentProcess == nullptr && !readyQueue.empty())
    {
        currentProcess = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        currentProcess->setState(ProcessState::RUNNING);
        currentQuantumTicks = 0;
    }
}

bool RoundRobinScheduler::isPreemptive() const
{
    return true;
}
