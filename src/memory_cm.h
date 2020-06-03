
#ifndef MEMORY_CM_H
#define MEMORY_CM_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"
#include "print_output.h"
#include "sort.h"

/**
 * Evicts and Loads pages into memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param process_queue
 * @param simulation_time_elapsed
 * @param process_pages_req
 * @param loading_cost
 * @param pages_freq
 * @return
 */
long long swapping_least_frequent(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue,
                    long long simulation_time_elapsed, long long process_pages_req, long long *loading_cost, long long *pages_freq);

/**
 * Loads pages into memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param loading_cost
 * @param pages_freq
 */
void load_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, long long *loading_cost, long long *pages_freq);

/**
 * Swaps pages in and out of memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param process_queue
 * @param simulation_time_elapsed
 * @param loading_cost
 * @param pages_freq
 */
void swap_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining,
                   deque_t *process_queue, long long simulation_time_elapsed, long long *loading_cost, long long *pages_freq);

/**
 * Evicts pages from memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param mem_addresses
 * @param mem_addresses_len
 * @param pages_freq
 */
void discard_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, long long *mem_addresses, long long *mem_addresses_len, long long *pages_freq);

/**
 * Initialize frequency table to empty
 * @param pages_freq
 * @param num_pages
 */
void initialize_freq(long long *pages_freq, long long num_pages);

/**
 *
 * @param pages
 * @param pages_freq
 * @param num_pages
 * @param process
 */
void update_pages_freq(const long long *pages, long long *pages_freq, long long num_pages, process_t *process);

/**
 * Returns the pid of the process belonging to the least frequenctly accessed pages
 * @param pages
 * @param pages_freq
 * @param num_pages
 * @param process
 * @return
 */
long long determine_least_frequent_process(long long *pages, const long long *pages_freq, long long num_pages, process_t *process);

/**
 * Print the state of RAM
 * @param pages
 * @param num_pages
 * @param pages_freq
 */
void print_memory_cm(long long *pages, long long num_pages, long long *pages_freq);
#endif