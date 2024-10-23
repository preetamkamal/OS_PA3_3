#include "../headers/Schedulers.h"

Scheduler::Scheduler()
{
    next_pcb_index = -1;
    ready_queue = NULL;
}

// constructor for all algs except RR and PP
Scheduler::Scheduler(DList<PCB> *rq, CPU *cp, int alg)
{
    ready_queue = rq;
    cpu = cp;
    dispatcher = NULL;
    next_pcb_index = -1;
    algorithm = alg;
}

// constructor for RR and PP alg
Scheduler::Scheduler(DList<PCB> *rq, CPU *cp, int alg, int tq)
{
    ready_queue = rq;
    cpu = cp;
    dispatcher = NULL;
    next_pcb_index = -1;
    algorithm = alg;
    timeq = timer = tq;
}

// dispatcher needed to be set after construction since they mutually include each other
// can only be set once
void Scheduler::setdispatcher(Dispatcher *disp)
{
    if (dispatcher == NULL)
        dispatcher = disp;
}

// dispatcher uses this to determine which process in the queue to grab
int Scheduler::getnext()
{
    return next_pcb_index;
}

// switch for the different algorithms
void Scheduler::execute()
{
    if (timer > 0)
        timer -= .5;
    if (ready_queue->size())
    {
        switch (algorithm)
        {
        case 0:
            fcfs();
            break;
        case 1:
            srtf();
            break;
        case 2:
            rr();
            break;
        case 3:
            pp();
            break;
        default:
            break;
        }
    }
}

// simply waits for cpu to go idle and then tells dispatcher to load next in queue
void Scheduler::fcfs()
{
    next_pcb_index = 0;
    if (cpu->isidle())
        dispatcher->interrupt();
}

// shortest remaining time first
void Scheduler::srtf()
{
    float short_time;
    int short_index = -1;

    // if cpu is idle, initialize shortest time to head of queue
    if (!cpu->isidle())
        short_time = cpu->getpcb()->time_left;
    else
    {
        short_time = ready_queue->gethead()->time_left;
        short_index = 0;
    }

    // now search through queue for actual shortest time
    for (int index = 0; index < ready_queue->size(); ++index)
    {
        if (ready_queue->getindex(index)->time_left < short_time)
        { // less than ensures FCFS is used for tie
            short_index = index;
            short_time = ready_queue->getindex(index)->time_left;
        }
    }

    //-1 means nothing to schedule, only happens if cpu is already working on shortest
    if (short_index >= 0)
    {
        next_pcb_index = short_index;
        dispatcher->interrupt();
    }
}

// round robin, simply uses timer and interrupts dispatcher when timer is up, schedules next in queue
void Scheduler::rr()
{
    if (cpu->isidle() || timer <= 0)
    {
        timer = timeq;
        next_pcb_index = 0;
        dispatcher->interrupt();
    }
}

// preemptive priority
void Scheduler::pp()
{
    int low_prio = cpu->isidle() ? INT_MAX : cpu->getpcb()->priority;
    int low_index = -1;

    // search through entire queue for actual lowest priority
    for (int index = 0; index < ready_queue->size(); ++index)
    {
        int temp_prio = ready_queue->getindex(index)->priority;
        if (temp_prio < low_prio)
        { // less than ensures FCFS is used for ties
            low_prio = temp_prio;
            low_index = index;
        }
    }

    // only -1 if couldn't find a pcb to schedule, happens if cpu is already working on lowest priority
    if (low_index >= 0)
    {
        next_pcb_index = low_index;
        timer = timeq; // reset timer when starting a new process
        dispatcher->interrupt();
    }
    else
    {
        // if no higher priority process found, run RR
        if (cpu->isidle() || timer <= 0)
        {
            int current_prio = cpu->isidle() ? ready_queue->gethead()->priority : cpu->getpcb()->priority;
            int size = ready_queue->size();
            int found = 0;
            int start_index = -1;

            if (!cpu->isidle())
            {
                int current_pid = cpu->getpcb()->pid;
                for (int index = 0; index < size; ++index)
                {
                    if (ready_queue->getindex(index)->pid == current_pid)
                    {
                        start_index = index;
                        break;
                    }
                }
            }
            else
            {
                start_index = -1;
            }

            // find next process of same priority
            for (int i = 1; i <= size; ++i)
            {
                int index = (start_index + i) % size;
                PCB *pcb = ready_queue->getindex(index);
                if (pcb->priority == current_prio)
                {
                    next_pcb_index = index;
                    found = 1;
                    break;
                }
            }

            if (found)
            {
                timer = timeq;
                dispatcher->interrupt();
            }
            else
            {
                timer = timeq;
            }
        }
    }
}

/*
 *
 * Dispatcher Implementation
 *
 */
Dispatcher::Dispatcher()
{
    cpu = NULL;
    scheduler = NULL;
    ready_queue = NULL;
    clock = NULL;
    _interrupt = false;
}

Dispatcher::Dispatcher(CPU *cp, Scheduler *sch, DList<PCB> *rq, Clock *cl)
{
    cpu = cp;
    scheduler = sch;
    ready_queue = rq;
    clock = cl;
    _interrupt = false;
};

// function to handle switching out pcbs and storing back into ready queue
PCB *Dispatcher::switchcontext(int index)
{
    PCB *old_pcb = cpu->pcb;
    PCB *new_pcb = new PCB(ready_queue->removeindex(scheduler->getnext()));
    cpu->pcb = new_pcb;
    return old_pcb;
}

// executed every clock cycle, only if scheduler interrupts it
void Dispatcher::execute()
{
    if (_interrupt)
    {
        PCB *old_pcb = switchcontext(scheduler->getnext());
        if (old_pcb != NULL)
        { // only consider it a switch if cpu was still working on process
            old_pcb->num_context++;
            cpu->getpcb()->wait_time += .5;
            clock->step();
            ready_queue->add_end(*old_pcb);
            delete old_pcb;
        }
        _interrupt = false;
    }
}

// routine for scheudler to interrupt it
void Dispatcher::interrupt()
{
    _interrupt = true;
}