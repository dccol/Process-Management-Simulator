
#ifndef RR_H
#define RR_H

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

/**
 * Round-Robin scheduling algorithm
 * @param pending_process_queue
 * @param process_queue
 * @param quantum
 */
void rr(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, int memory_size, int quantum);

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
void step_rr(deque_t *process_queue, process_t *current_process, int *simulation_time_elapsed,int *pages,int num_pages,
        int *space_available, int *state, int *loading_cost, int *quantum_rr, int *status, char *memory_opt, int *pages_time,
        int *loading_status);

/**
 * DEcrements a processes time remaining attribute
 * @param process
 * @param quantum
 * @return
 */
int run_process_rr(process_t *process, int *quantum);

/**
 * Takes processes from pending queue and adds them to the process queue to simulate the arrival of a new process
 * @param pending_process_queue
 * @param process_queue
 * @return
 */
void check_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time);

#endif