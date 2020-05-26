
#ifndef MEMORY_P_H
#define MEMORY_P_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"

#define PAGE_SIZE 4

void swapping_x(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue, int simulation_time_elapsed, int *pages_time);

void initialize_empty_pages(int *pages, int num_pages);

void load_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining);

void swap_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, deque_t *process_queue, int simulation_time_elapsed, int *pages_time);

void discard_pages(int *pages, int num_pages, int *space_available, process_t *least_recent_process, int simulation_time_elapsed, int *pages_time);

void print_memory(int *pages, int num_pages);

void find_process_mem(const int *pages, int num_pages, process_t *process, int *mem_addresses);
#endif