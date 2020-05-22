
#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

void calculate_throughput(int simulation_time_elapsed, double *throughput_av, int *throughput_min, int *throughput_max, int interval_throughput);

void calculate_turnarround_time();

void calculate_overhead();

#endif