#include "srtfscheduler.h"
#include <algorithm>

void SRTFScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    if (readyQueue.empty())
    {
        return;
    }

    auto srtfIt = std::min_element(readyQueue.begin(), readyQueue.end(),
                                   [](const Process* a, const Process* b)
                                   {
                                       if (a->getRemainingTime() == b->getRemainingTime())
                                        {
                                           return a->getArrivalTime() < b->getArrivalTime();
                                        }
                                        return a->getRemainingTime() < b->getRemainingTime();
                                   });

    if (currentProcess != nullptr)
    {
        // Expulsión si hay un proceso con menor tiempo restante
        if ((*srtfIt)->getRemainingTime() < currentProcess->getRemainingTime())
        {
            currentProcess->setState(ProcessState::READY);
            readyQueue.push_back(currentProcess);

            currentProcess = *srtfIt;
            currentProcess->setState(ProcessState::RUNNING);
            readyQueue.erase(srtfIt);
        }
    }
    else
    {
        currentProcess = *srtfIt;
        currentProcess->setState(ProcessState::RUNNING);
        readyQueue.erase(srtfIt);
    }
}

bool SRTFScheduler::isPreemptive() const
{
    return true;
}

