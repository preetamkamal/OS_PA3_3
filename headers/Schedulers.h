#ifndef LAB2_SCHEDULER_H
#define LAB2_SCHEDULER_H

#include "DList.h"
#include "CPU.h"

class CPU;
class Scheduler;
class StatUpdater;

class Dispatcher
{
private:
    CPU *cpu;
    Scheduler *scheduler;
    DList<PCB> *ready_queue;
    Clock *clock;
    bool _interrupt;
    StatUpdater *statupdater;

public:
    Dispatcher();
    Dispatcher(CPU *cp, Scheduler *sch, DList<PCB> *rq, Clock *cl, StatUpdater *su);
    PCB *switchcontext(int index);
    void execute();
    void interrupt();
};

class Scheduler
{
private:
    int next_pcb_index;
    DList<PCB> *ready_queue;
    CPU *cpu;
    Dispatcher *dispatcher;
    int algorithm;
    float timeq, timer; // time quantum
public:
    Scheduler();
    Scheduler(DList<PCB> *rq, CPU *cp, int alg);
    Scheduler(DList<PCB> *rq, CPU *cp, int alg, int tq);
    void setdispatcher(Dispatcher *disp);
    int getnext();
    void execute();
    void fcfs();
    void srtf();
    void rr();
    void pp();
    void pr();
};

#endif // LAB2_SCHEDULER_H
