#include "randomscheduler.h"
#include <cstdlib>

void RandomScheduler::schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick)
{
    // Random es no expulsivo
    if (currentProcess != nullptr || readyQueue.empty())
    {
        return;
    }

    int randomIndex = std::rand() % readyQueue.size();
    auto it = readyQueue.begin() + randomIndex;

    currentProcess = *it;
    currentProcess->setState(ProcessState::RUNNING);
    readyQueue.erase(it);
}

bool RandomScheduler::isPreemptive() const
{
    return false;
}
