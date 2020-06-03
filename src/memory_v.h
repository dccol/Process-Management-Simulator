
#ifndef MEMORY_V_H
#define MEMORY_V_H

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
 * @return
 */
long long virtual_memory(long long *pages, long long num_pages, long long *space_available, process_t *process,
        deque_t *process_queue, long long simulation_time_elapsed, long long process_pages_req, long long *loading_cost);

/**
 * Loads pages into memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param loading_cost
 */
void load_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, long long *loading_cost);

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
 */
void swap_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, deque_t *process_queue, long long simulation_time_elapsed, long long *loading_cost);

/**
 * Evicts pages from memory
 * @param pages
 * @param num_pages
 * @param space_available
 * @param process
 * @param pages_remaining
 * @param mem_addresses
 * @param mem_addresses_len
 */
void discard_pages_v(long long *pages, long long num_pages, long long *space_available, process_t *process,
        long long pages_remaining, long long *mem_addresses, long long *mem_addresses_len);

/**
 * Returns how many pages the process currently occupies in memory
 * @param pages
 * @param num_pages
 * @param process
 * @return
 */
long long count_process_mem(const long long *pages, long long num_pages, process_t *process);

#endif