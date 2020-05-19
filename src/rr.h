
#ifndef RR_H
#define RR_H

#include "deque.h"
#include "sort.h"

/**
 * Round-Robin scheduling algorithm
 * @param pending_process_queue
 * @param process_queue
 * @param quantum
 */
void rr(deque_t *pending_process_queue, deque_t *process_queue, int quantum);

/**
 * Abstration of a unit of time (second) of the simulation
 * Running process till will decrease
 * @param pending_process_queue
 * @param process_queue
 * @param current_process
 * @param simulation_time_elapsed
 * @param next_process_arrival_time
 * @param quantum
 * @return
 */
int step_rr(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
            int simulation_time_elapsed, int next_process_arrival_time, int quantum);

/**
 * DEcrements a processes time remaining attribute
 * @param process
 * @param quantum
 * @return
 */
int run_process_rr(process_t *process, int quantum);

/**
 * Takes the first item in the pending queue and adds it to the process queue to simulate the arrival of a new process
 * Returns the new pending process arrival time
 * @param pending_process_queue
 * @param process_queue
 * @return
 */
int insert_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int time);

void check_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time);

#endif