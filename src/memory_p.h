
#ifndef MEMORY_P_H
#define MEMORY_P_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"

#define PAGE_SIZE 4

void swapping_x(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time);

void initialize_empty_pages(long long *pages, long long num_pages);

void load_pages(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining);

void swap_pages(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time);

void discard_pages(long long *pages, long long num_pages, long long *space_available, process_t *least_recent_process, long long simulation_time_elapsed, long long *pages_time);

void print_memory(long long *pages, long long num_pages);

void find_process_mem(const long long *pages, long long num_pages, process_t *process, long long *mem_addresses);
#endif