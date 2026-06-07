#include "priorityscheduler.h"
#include <algorithm>

void PriorityScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    if (currentProcess != nullptr || readyQueue.empty())
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

    currentProcess = *priorityIt;
    currentProcess->setState(ProcessState::RUNNING);
    readyQueue.erase(priorityIt);
}

bool PriorityScheduler::isPreemptive() const
{
    return false;
}
