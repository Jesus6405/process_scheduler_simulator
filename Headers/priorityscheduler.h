#ifndef PRIORITYSCHEDULER_H
#define PRIORITYSCHEDULER_H

#include "scheduler.h"

class PriorityScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // PRIORITYSCHEDULER_H
