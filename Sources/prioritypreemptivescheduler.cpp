#include "prioritypreemptivescheduler.h"
#include <algorithm>

void PriorityPreemptiveScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    if (readyQueue.empty())
    {
        return;
    }

    auto priorityIt = std::min_element(readyQueue.begin(), readyQueue.end(),
                                       [](const Process* a, const Process* b)
                                       {
                                            if (a->getPriority() == b->getPriority())
                                            {
                                               return a->getArrivalTime() < b->getArrivalTime();
                                            }
                                            return a->getPriority() < b->getPriority(); // Menor número = Mayor prioridad
                                       });

    if (currentProcess != nullptr)
    {
        // Expulsión si hay un proceso con mayor prioridad (número menor)
        if ((*priorityIt)->getPriority() < currentProcess->getPriority())
        {
            currentProcess->setState(ProcessState::READY);
            readyQueue.push_back(currentProcess);

            currentProcess = *priorityIt;
            currentProcess->setState(ProcessState::RUNNING);
            readyQueue.erase(priorityIt);
        }
    }
    else
    {
        currentProcess = *priorityIt;
        currentProcess->setState(ProcessState::RUNNING);
        readyQueue.erase(priorityIt);
    }
}

bool PriorityPreemptiveScheduler::isPreemptive() const
{
    return true;
}
