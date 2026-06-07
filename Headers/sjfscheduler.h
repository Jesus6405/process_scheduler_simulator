#ifndef SJFSCHEDULER_H
#define SJFSCHEDULER_H

#include "scheduler.h"

class SJFScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // SJFSCHEDULER_H
