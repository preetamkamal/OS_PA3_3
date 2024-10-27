#include "../headers/CPU.h"

CPU::CPU(DList<PCB> *fq, Clock *cl, DList<PCB> *bq, int tq, int alg)
{
    pcb = NULL;
    idle = true;
    finished_queue = fq;
    clock = cl;
    blocked_queue = bq; // Initialize blocked queue
    timeq = tq;         // Initialize time quantum
    algorithm = alg;    // Initialize algorithm
}

// used by others to determine what the cpu is working on like priority and time left
PCB *CPU::getpcb()
{
    return pcb;
}

// check to see if cpu is currently working on a process
bool CPU::isidle()
{
    return idle;
}
void CPU::execute()
{
    if (pcb != NULL)
    {
        idle = false;
        if (!pcb->started)
        {
            pcb->started = true;
            pcb->resp_time = clock->gettime() - pcb->arrival;
        }
        pcb->time_left -= .5;
        pcb->total_cpu_time_used += .5;

        if (algorithm == 3 && pcb->io_burst > 0 && !pcb->has_been_blocked && pcb->total_cpu_time_used >= 0.5 * timeq)
        {
            // Move process to Blocked queue after 50% of first CPU time quantum
            pcb->has_been_blocked = true;
            blocked_queue->add_end(*pcb);
            pcb = NULL;
            idle = true;
        }
        else if (pcb->time_left <= 0)
        {
            terminate();
            idle = true;
        }
    }
    else
    {
        idle = true;
    }
}

void CPU::terminate()
{
    pcb->finish_time = clock->gettime() + .5;
    finished_queue->add_end(*pcb);
    delete pcb;
    pcb = NULL;
}