#ifndef PROCESS_H
#define PROCESS_H

enum class ProcessState
{
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

class Process
{
private:
    // Atributos principales
    int id;
    int burstTime;
    int remainingTime;
    int ioBurstTime;
    int ioRemainingTime;
    int priority;
    int arrivalTime;
    ProcessState state;

    // Estadísticas individuales
    int waitingTime;
    int blockedTime;
    int turnaroundTime;

public:
    // Constructor
    Process(int id, int burstTime, int ioBurstTime, int priority, int arrivalTime);

    // Getters y Setters
    int getId() const;
    int getBurstTime() const;
    int getRemainingTime() const;
    int getIoBurstTime() const;
    int getIoRemainingTime() const;
    int getPriority() const;
    int getArrivalTime() const;
    ProcessState getState() const;
    void setState(ProcessState newState);

    int getWaitingTime() const;
    int getBlockedTime() const;
    int getTurnaroundTime() const;

    // Métodos para la simulación
    void decrementRemainingTime();
    void decrementIoRemainingTime();
    void resetIoRemainingTime();
    void incrementWaitingTime();
    void incrementBlockedTime();
    void calculateTurnaroundTime(int currentTime);
};

#endif // PROCESS_H
