#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "process.h"
#include "scheduler.h"
#include <vector>
#include <memory>

class Simulator
{
private:
    int currentTick;
    int cpuBusyTicks; // Para calcular el % de uso del procesador

    Process* cpuProcess;                  // Proceso actualmente en CPU
    std::unique_ptr<Scheduler> scheduler; // Algoritmo de planificación activo

    // Colas del sistema
    std::vector<Process*> allProcesses;   // Todos los procesos generados
    std::vector<Process*> readyQueue;     // Cola de Listos
    std::vector<Process*> blockedQueue;   // Cola de Bloqueados
    std::vector<Process*> terminatedQueue;// Cola de Terminados

    // Estadísticas del último paso
    int newArrivalsInCurrentTick;

public:
    Simulator();
    ~Simulator();

    // Configuración de la simulación
    void setScheduler(std::unique_ptr<Scheduler> newScheduler);
    void generateRandomProcesses(int count, int maxBurst, int maxIO, int maxPriority);
    void addProcess(Process* p);
    void reset();

    // El motor avanza un paso en el tiempo
    void nextTick();

    // Getters para la GUI
    int getCurrentTick() const;
    Process* getCpuProcess() const;
    const std::vector<Process*>& getReadyQueue() const;
    const std::vector<Process*>& getBlockedQueue() const;
    const std::vector<Process*>& getTerminatedQueue() const;

    // Métricas de rendimiento
    double getCpuUtilization() const;
    double getAverageWaitingTime() const;
    double getAverageBlockedTime() const;
    double getAverageTurnaroundTime() const;
    int getTotalCompletedProcesses() const;
    int getNewArrivalsInCurrentTick() const;
};

#endif // SIMULATOR_H
