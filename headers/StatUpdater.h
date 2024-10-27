#ifndef LAB2_STATUPDATER_H
#define LAB2_STATUPDATER_H

#include "DList.h"
#include "PCB.h"
#include "Clock.h"
#include "CPU.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

using std::cout;

// Class that handles updating waiting times, response times, etc.
class StatUpdater
{
private:
    DList<PCB> *ready_queue;
    DList<PCB> *finished_queue;
    DList<PCB> *blocked_queue; // Added blocked queue
    Clock *clock;
    CPU *cpu;
    int algorithm, num_tasks, timeq;
    float last_update;
    std::string filename;
    std::vector<std::string> logs;

public:
    StatUpdater(DList<PCB> *rq, DList<PCB> *fq, DList<PCB> *bq, Clock *cl, int alg, std::string fn, int tq, CPU *cp);
    void execute();
    void print();
    void addLogEntry(const std::string &entry);
};

#endif // LAB2_STATUPDATER_H
