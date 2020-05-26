
#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round.h"

void calculate_throughput(int simulation_time_elapsed, double *throughput_av, int *throughput_min, int *throughput_max, int interval_throughput);

void calculate_turnaround_time(int simulation_time_elapsed, int turnaround_time, double *turnaround_av, int num_processes_finished);

void calculate_overhead(int simulation_time_elapsed, int turnaround_time, int job_time, double *max_overhead, double *overhead_av, int num_processes_finished);

#endif