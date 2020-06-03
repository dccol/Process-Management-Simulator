
#ifndef SF_H
#define SF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"
#include "sort.h"
#include "memory_p.h"
#include "memory_v.h"
#include "memory_cm.h"
#include "round.h"
#include "statistics.h"
#include "print_output.h"

void sf(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, long long memory_size);


/**
 * Decrements a processes time remaining attribute
 * @param process
 * @return
 */
void run_process_sf(process_t *process);

/**
 * Abstraction of a unit of time (second) in the simulation
 * Running process time remaining will decrease
 * if a process 'arrives' it is added to the process queue
 * @param pending_process_queue
 * @param process_queue
 * @param current_process
 * @param simulation_time_elapsed
 * @param next_process_arrival_time
 * @return
 */
void step_sf(deque_t *process_queue, process_t *current_process, long long *simulation_time_elapsed, long long *pages,
             long long num_pages, long long *space_available, long long *state, long long *loading_cost, char *memory_opt, long long *pages_time,
             long long *loading_status);

/**
 * Takes processes from pending queue and adds them to the process queue to simulate the arrival of a new process
 * @param pending_process_queue
 * @param process_queue
 * @return
 */
void check_pending_sf(deque_t *pending_process_queue, deque_t *process_queue, long long simulation_time);
#endif