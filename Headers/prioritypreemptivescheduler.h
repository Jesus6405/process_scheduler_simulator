#ifndef PRIORITYPREEMPTIVESCHEDULER_H
#define PRIORITYPREEMPTIVESCHEDULER_H

#include "scheduler.h"

class PriorityPreemptiveScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // PRIORITYPREEMPTIVESCHEDULER_H
