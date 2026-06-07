#ifndef SRTFSCHEDULER_H
#define SRTFSCHEDULER_H

#include "scheduler.h"

class SRTFScheduler : public Scheduler
{
public:
    void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) override;
    bool isPreemptive() const override;
};

#endif // SRTFSCHEDULER_H
