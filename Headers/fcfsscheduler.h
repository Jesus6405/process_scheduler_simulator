#ifndef FCFS_SCHEDULER_H
#define FCFS_SCHEDULER_H

#include "scheduler.h"

class FCFSScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
    std::string getName() const override;
    std::string getDescription() const override;
};

#endif // FCFS_SCHEDULER_H
