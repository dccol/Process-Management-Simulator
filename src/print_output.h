
#ifndef PRINT_OUTPUT_H
#define PRINT_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "memory_p.h"
#include "memory_v.h"
#include "round.h"


void print_evicted(process_t *process, int simulation_time_elapsed, int *mem_addresses, int mem_addresses_len);

void print_load(int *pages, int num_pages, const int *space_available, process_t *process, const int *loading_cost, const int *simulation_time_elapsed);

void print_statistics();

#endif
