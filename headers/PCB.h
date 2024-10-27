//
// Created by Joseph on 10/7/2017.
//
#ifndef LAB2_PCB_H
#define LAB2_PCB_H

#include <iostream>

// Data structure to hold process information
struct PCB
{
    int pid, arrival, burst, priority, num_context;
    float time_left, resp_time, wait_time, finish_time;
    bool started;
    float io_burst;            // Total I/O Burst time needed
    bool has_been_blocked;     // Indicates if process has been blocked
    float total_cpu_time_used; // Tracks CPU time used in first quantum

    PCB()
    {
        pid = arrival = burst = priority = num_context = 0;
        time_left = resp_time = wait_time = finish_time = 0;
        started = false;
        io_burst = 0;
        has_been_blocked = false;
        total_cpu_time_used = 0;
    }

    PCB(int id, int arr, int time, int prio, float io_time) : pid(id), arrival(arr), burst(time), time_left(time), priority(prio), io_burst(io_time)
    {
        resp_time = wait_time = finish_time = 0;
        num_context = 0;
        started = false;
        has_been_blocked = false;
        total_cpu_time_used = 0;
    }

    void print()
    {
        std::cout << pid << " " << arrival << " " << time_left << " " << priority << " " << io_burst << std::endl;
    }
};

#endif // LAB2_PCB_H
