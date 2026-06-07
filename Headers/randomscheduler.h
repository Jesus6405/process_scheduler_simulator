#ifndef RANDOMSCHEDULER_H
#define RANDOMSCHEDULER_H

#include "scheduler.h"

class RandomScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // RANDOMSCHEDULER_H
