
#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "round.h"

/**
 * Calculates throughput
 * @param simulation_time_elapsed
 * @param throughput_av
 * @param throughput_min
 * @param throughput_max
 * @param interval_throughput
 */
void calculate_throughput(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min,
        long long *throughput_max, long long interval_throughput);

/**
 * Calculates throughput of the final interval
 * @param simulation_time_elapsed
 * @param throughput_av
 * @param throughput_min
 * @param throughput_max
 * @param interval_throughput
 */
void calculate_throughput_final(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min,
        long long *throughput_max, long long interval_throughput);

/**
 * Calculates turnaround time
 * @param simulation_time_elapsed
 * @param turnaround_time
 * @param turnaround_av
 * @param num_processes_finished
 */
void calculate_turnaround_time(long long simulation_time_elapsed, long long turnaround_time, double *turnaround_av,
        long long num_processes_finished);

/**
 * Calculates overhead
 * @param simulation_time_elapsed
 * @param turnaround_time
 * @param job_time
 * @param max_overhead
 * @param overhead_av
 * @param num_processes_finished
 */
void calculate_overhead(long long simulation_time_elapsed, long long turnaround_time, long long job_time, double *max_overhead, double *overhead_av, long long num_processes_finished);

#endif