#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include <vector>
#include <string>

class Scheduler
{
public:
    virtual ~Scheduler() = default;

    /**
     * Core scheduling logic.
     * @param readyQueue Reference to the simulator's ready queue.
     * @param currentProcess Reference to the pointer of the process currently in CPU.
     * @param currentTick The current time step of the simulation.
     */
    virtual void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) = 0;

    /**
     * Helper to identify if the algorithm allows preemption.
     */
    virtual bool isPreemptive() const = 0;

    /**
     * Returns the name of the algorithm.
     */
    virtual std::string getName() const = 0;

    /**
     * Returns the description of the algorithm.
     */
    virtual std::string getDescription() const = 0;
};

#endif // SCHEDULER_H
