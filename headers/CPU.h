#ifndef LAB2_CPU_H
#define LAB2_CPU_H

#include "PCB.h"
#include "DList.h"
#include "Clock.h"
#include "Schedulers.h"

// Forward declaration so that CPU can declare dispatcher as friend
class Dispatcher;

class CPU
{
private:
    PCB *pcb;
    bool idle;
    Clock *clock;
    DList<PCB> *finished_queue; // For terminated processes
    DList<PCB> *blocked_queue;  // Added blocked queue
    int timeq;                  // Time quantum
    int algorithm;              // Scheduling algorithm
    friend Dispatcher;          // Allows dispatcher to switch out processes

public:
    CPU(DList<PCB> *fq, Clock *cl, DList<PCB> *bq, int tq, int alg);
    PCB *getpcb();
    bool isidle();
    void execute();
    void terminate();
};

#endif
