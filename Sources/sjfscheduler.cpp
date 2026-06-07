#include "sjfscheduler.h"
#include <algorithm>

void SJFScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    // SJF es no expulsivo
    if (currentProcess != nullptr || readyQueue.empty())
    {
        return;
    }

    auto sjfIt = std::min_element(readyQueue.begin(), readyQueue.end(),
                                  [](const Process* a, const Process* b) {
                                      if (a->getBurstTime() == b->getBurstTime()) {
                                          return a->getArrivalTime() < b->getArrivalTime();
                                      }
                                      return a->getBurstTime() < b->getBurstTime();
                                  });

    currentProcess = *sjfIt;
    currentProcess->setState(ProcessState::RUNNING);
    readyQueue.erase(sjfIt);
}

bool SJFScheduler::isPreemptive() const
{
    return false;
}
