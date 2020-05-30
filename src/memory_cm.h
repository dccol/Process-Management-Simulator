
#ifndef MEMORY_CM_H
#define MEMORY_CM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"
#include "sort.h"

int swapping_least_frequent(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue,
                    int simulation_time_elapsed, int process_pages_req, int *loading_cost, int *pages_freq);

void load_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, int *loading_cost, int *pages_freq);

void swap_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining,
                   deque_t *process_queue, int simulation_time_elapsed, int *loading_cost, int *pages_freq);

void discard_pages_cm(int *pages, int num_pages, int *space_available, process_t *process,
        int pages_remaining, int *mem_addresses, int *mem_addresses_len, int *pages_freq);

void initialize_time(int *pages_freq, int num_pages);

void update_pages_time(const int *pages, int *pages_freq, int num_pages, process_t *process);

int determine_least_frequent_process(int *pages, const int *pages_freq, int num_pages, process_t *process);

void print_memory_cm(int *pages, int num_pages, int *pages_freq);
#endif