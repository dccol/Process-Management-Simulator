
#ifndef MEMORY_CM_H
#define MEMORY_CM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"
#include "sort.h"

long long swapping_least_frequent(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue,
                    long long simulation_time_elapsed, long long process_pages_req, long long *loading_cost, long long *pages_freq);

void load_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, long long *loading_cost, long long *pages_freq);

void swap_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining,
                   deque_t *process_queue, long long simulation_time_elapsed, long long *loading_cost, long long *pages_freq);

void discard_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, long long *mem_addresses, long long *mem_addresses_len, long long *pages_freq);

void initialize_time(long long *pages_freq, long long num_pages);

void update_pages_time(const long long *pages, long long *pages_freq, long long num_pages, process_t *process);

long long determine_least_frequent_process(long long *pages, const long long *pages_freq, long long num_pages, process_t *process);

void print_memory_cm(long long *pages, long long num_pages, long long *pages_freq);
#endif