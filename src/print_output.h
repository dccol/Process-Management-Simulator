
#ifndef PRINT_OUTPUT_H
#define PRINT_OUTPUT_H

#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "memory_p.h"
#include "memory_v.h"
#include "round.h"


void print_evicted(long long simulation_time_elapsed, long long *mem_addresses, long long mem_addresses_len);

void print_load(long long *pages, long long num_pages, const long long *space_available, process_t *process, const long long *loading_cost, const long long *simulation_time_elapsed);

#endif
