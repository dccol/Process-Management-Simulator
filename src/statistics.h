
#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round.h"

void calculate_throughput(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min, long long *throughput_max, long long interval_throughput);
void calculate_throughput_final(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min, long long *throughput_max, long long interval_throughput);
void calculate_turnaround_time(long long simulation_time_elapsed, long long turnaround_time, double *turnaround_av, long long num_processes_finished);

void calculate_overhead(long long simulation_time_elapsed, long long turnaround_time, long long job_time, double *max_overhead, double *overhead_av, long long num_processes_finished);

#endif