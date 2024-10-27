#include <iostream>
#include <stdlib.h>
#include "../headers/Clock.h"
#include "../headers/PCBGenerator.h"
#include "../headers/CPU.h"
#include "../headers/StatUpdater.h"
using namespace std;

int main(int argc, char *argv[])
{
    // Initial args validation
    if (argc < 4)
    {
        cout << "Not enough arguments sent to main." << endl;
        cout << "Format should be: ./lab2 inputfile outputfile algorithm timequantum(if algorithm == 2)" << endl;
        return EXIT_FAILURE;
    }
    int algorithm = atoi(argv[3]);
    if ((algorithm == 2 || algorithm == 3 || algorithm == 4) && argc == 4)
    {
        cout << "Need to provide time quantum when using Round Robin or PP algorithm" << endl;
        return EXIT_FAILURE;
    }

    int timeq = -1;
    if (algorithm == 2 || algorithm == 3 || algorithm == 4)
        timeq = atoi(argv[4]);

    // Queues to hold PCBs throughout
    auto ready_queue = new DList<PCB>();
    auto finished_queue = new DList<PCB>();
    auto blocked_queue = new DList<PCB>(); // Added blocked queue

    try
    {
        // Objects that will work together to simulate OS process management
        Clock clock;
        PCBGenerator pgen(argv[1], ready_queue, &clock);
        CPU cpu(finished_queue, &clock, blocked_queue, timeq, algorithm);                                       // Modified constructor
        StatUpdater stats(ready_queue, finished_queue, blocked_queue, &clock, algorithm, argv[2], timeq, &cpu); // Modified constructor
        Scheduler scheduler(ready_queue, &cpu, algorithm, timeq);
        Dispatcher dispatcher(&cpu, &scheduler, ready_queue, &clock, &stats);
        scheduler.setdispatcher(&dispatcher);

        // Main loop
        while (!pgen.finished() || ready_queue->size() || !cpu.isidle() || blocked_queue->size())
        {
            pgen.generate();
            scheduler.execute();
            dispatcher.execute();
            cpu.execute();
            stats.execute();
            clock.step();
        }

        // Final printing of stats
        stats.print();
    }
    catch (int)
    {
        delete ready_queue;
        delete finished_queue;
        delete blocked_queue;
        return EXIT_FAILURE;
    }

    delete ready_queue;
    delete finished_queue;
    delete blocked_queue;

    return 0;
}
