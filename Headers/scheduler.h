#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include <vector>

class Scheduler
{
public:
    virtual ~Scheduler() = default;

    /**
     * Lógica central de la planificación
     * @param readyQueue Referencia a la cola de procesos listos de la simulación
     * @param currentProcess Referencia al apuntador del proceso actual en la CPU
     * @param currentTick Tiempo actual de la simulación
     */
    virtual void schedule(std::vector<Process*>& readyQueue, Process*& currentProcess, int currentTick) = 0;

    /**
     * Indica si el algoritmo es expulsivo (true) o no expulsivo (false)
     */
    virtual bool isPreemptive() const = 0;
};

#endif // SCHEDULER_H
