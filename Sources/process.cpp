#include "Headers/process.h"

// Constructor
Process::Process(int id, int burstTime, int ioBurstTime, int priority, int arrivalTime)
    : id(id),
    burstTime(burstTime),
    remainingTime(burstTime),
    ioBurstTime(ioBurstTime),
    ioRemainingTime(ioBurstTime),
    priority(priority),
    arrivalTime(arrivalTime),
    state(ProcessState::NEW),
    waitingTime(0),
    blockedTime(0),
    turnaroundTime(0) {}

// Getters y Setters
int Process::getId() const { return id; }
int Process::getBurstTime() const { return burstTime; }
int Process::getRemainingTime() const { return remainingTime; }
int Process::getIoBurstTime() const { return ioBurstTime; }
int Process::getIoRemainingTime() const { return ioRemainingTime; }
int Process::getPriority() const { return priority; }
int Process::getArrivalTime() const { return arrivalTime; }
ProcessState Process::getState() const { return state; }
void Process::setState(ProcessState newState) { state = newState; }

int Process::getWaitingTime() const { return waitingTime; }
int Process::getBlockedTime() const { return blockedTime; }
int Process::getTurnaroundTime() const { return turnaroundTime; }

// --- Métodos de la simulación ---

void Process::decrementRemainingTime()
{
    if (remainingTime > 0)
    {
        remainingTime--;
    }
}

void Process::decrementIoRemainingTime()
{
    if (ioRemainingTime > 0)
    {
        ioRemainingTime--;
    }
}

void Process::resetIoRemainingTime()
{
    ioRemainingTime = ioBurstTime;
}

void Process::incrementWaitingTime()
{
    waitingTime++;
}

void Process::incrementBlockedTime()
{
    blockedTime++;
}

void Process::calculateTurnaroundTime(int currentTime)
{
    turnaroundTime = currentTime - arrivalTime;
}
