
#ifndef FF_H
#define FF_H

#include "deque.h"
#include "process.h"
#include "sort.h"
#include "memory_p.h"
#include "round.h"
#include "statistics.h"

/**
 * The First-Come First-Served Scheduling Algorithm
 * @param pending_process_queue
 * @param process_queue
 */
void fc_fs(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, int memory_size);

/**
 * Decrements a processes time remaining attribute
 * @param process
 * @return
 */
int run_process_ff(process_t *process);

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
void step_ff(deque_t *process_queue, process_t *current_process, int *simulation_time_elapsed, int *pages,
        int num_pages, int *space_available, int *state, int *loaded, int *loading_cost);

/**
 * Takes the first item in the pending queue and adds it to the process queue to simulate the arrival of a new process
 * Returns the new pending process arrival time, derived from the process now at the front on the pending queue
 * @param pending_process_queue
 * @param process_queue
 * @return
 */
int insert_pending_ff(deque_t *pending_process_queue, deque_t *process_queue, int time);

void check_pending(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time);


#endif