
#ifndef MEMORY_P_H
#define MEMORY_P_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"

#define PAGE_SIZE 4

void swapping_x(int total_memory, process_t *process, deque_t *process_queue);

void initialize_empty_pages(int *pages, int num_pages);

void load_pages(int *pages, int num_pages, int space_available, process_t *process, int pages_remaining, deque_t *process_queue);

void swap_pages(int *pages, int num_pages, int space_available, process_t *process, int pages_remaining, deque_t *process_queue);

int discard_pages(int *pages, int num_pages, int space_available, process_t *least_recent_process);

#endif