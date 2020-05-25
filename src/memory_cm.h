
#ifndef MEMORY_CM_H
#define MEMORY_CM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"
#include "sort.h"

int swapping_oldest(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue,
                    int simulation_time_elapsed, int process_pages_req, int *loading_cost);

void load_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, int *loading_cost);

void swap_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining,
                   deque_t *process_queue, int simulation_time_elapsed, int *loading_cost);

void discard_pages_cm(int *pages, int num_pages, int *space_available, process_t *process,
        int simulation_time_elapsed, int pages_remaining, int *mem_addresses, int *mem_addresses_len);
#endif