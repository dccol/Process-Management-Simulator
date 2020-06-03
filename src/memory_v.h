
#ifndef MEMORY_V_H
#define MEMORY_V_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"
#include "sort.h"

long long virtual_memory(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue,
        long long simulation_time_elapsed, long long process_pages_req, long long *loading_cost);

void load_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, long long *loading_cost);

void swap_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining,
        deque_t *process_queue, long long simulation_time_elapsed, long long *loading_cost);

void discard_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, long long *mem_addresses, long long *mem_addresses_len);

long long count_process_mem(const long long *pages, long long num_pages, process_t *process);

#endif