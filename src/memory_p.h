
#ifndef MEMORY_P_H
#define MEMORY_P_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"

#define PAGE_SIZE 4

/**
 * Evicts and Loads pages into memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param process_queue
 * @param simulation_time_elapsed
 * @param pages_time
 */
void swapping_x(long long *pages, long long num_pages, long long *space_available, process_t *process,
        deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time);

/**
 * Initializes an empty page table
 * @param pages
 * @param num_pages
 */
void initialize_empty_pages(long long *pages, long long num_pages);

/**
 * Loads pages into memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 */
void load_pages(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining);

/**
 * Swaps pages in and out of memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param process_queue
 * @param simulation_time_elapsed
 * @param pages_time
 */
void swap_pages(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time);

/**
 * Evicts pages from memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param least_recent_process
 * @param simulation_time_elapsed
 * @param pages_time
 */
void discard_pages(long long *pages, long long num_pages, long long *space_available, process_t *least_recent_process,
        long long simulation_time_elapsed, long long *pages_time);

/**
 * Prints the state of memory
 * @param pages
 * @param num_pages
 */
void print_memory(long long *pages, long long num_pages);

/**
 * Finds process pages in memory
 * @param pages
 * @param num_pages
 * @param process
 * @param mem_addresses
 */
void find_process_mem(const long long *pages, long long num_pages, process_t *process, long long *mem_addresses);
#endif