
#ifndef RR_H
#define RR_H

#include "deque.h"

void rr(deque_t *pending_process_queue, deque_t *process_queue, int quantum);

int step_rr(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
            int simulation_time_elapsed, int next_process_arrival_time, int quantum);

int run_process_rr(process_t *process, int quantum);

/**
 * Takes the first item in the pending queue and adds it to the process queue to simulate the arrival of a new process
 * Returns the new pending process arrival time
 * @param pending_process_queue
 * @param process_queue
 * @return
 */
int insert_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int time);

#endif