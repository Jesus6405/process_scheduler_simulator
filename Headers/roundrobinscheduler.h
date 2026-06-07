#ifndef ROUNDROBINSCHEDULER_H
#define ROUNDROBINSCHEDULER_H

#include "scheduler.h"

class RoundRobinScheduler : public Scheduler
{
private:
    int quantum;
    int currentQuantumTicks;

public:
    RoundRobinScheduler(int quantum = 3);

    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // ROUNDROBINSCHEDULER_H

