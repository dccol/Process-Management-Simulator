
#include <stdio.h>

#include "rr.h"
#include "process.h"

#define NOT_DONE 1
#define DONE 2

void rr(deque_t *pending_process_queue, deque_t *process_queue, int quantum){

    int simulation_time_elapsed = 0;
    int next_process_arrival_time = pending_process_queue->foot->data.process->time_rec;

    printf("Next Process will arrive at %d\n", next_process_arrival_time);

    // if a process is received insert it into the process queue (transfer from pending queue)
    if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);
        printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }

    // while there are processes in the process queue, step the simulation
    while(process_queue->head != NULL){

        data_t data = deque_remove(process_queue);
        process_t *process = data.process;

        printf("%3d| Current Process Running: %d\tTime Remaining: %d\tQuantum: %d\n", simulation_time_elapsed, process->pid, process->time_remaining, quantum);

        int quantum_rr = quantum;
        while(process->time_remaining > 0 && quantum_rr > 0) {

            simulation_time_elapsed++;
            quantum_rr--;

            // keep track of the next process arrival time
            next_process_arrival_time = step_rr(pending_process_queue, process_queue, process, simulation_time_elapsed, next_process_arrival_time, quantum_rr);
        }
    }
    printf("All Processes Complete\n");
}

/**
 * abstraction of a unit of time (second)
 */
int step_rr(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
         int simulation_time_elapsed, int next_process_arrival_time, int quantum_rr){

    int status = run_process_rr(current_process, quantum_rr);

    printf("%3d| Current Process Running: %d\tTime Remaining: %d\tQuantum: %d\n", simulation_time_elapsed, current_process->pid, current_process->time_remaining, quantum_rr);

    if(status == NOT_DONE) {
        printf("Inserting Process %d back into the queue. It has %d seconds left\n", current_process->pid,
               current_process->time_remaining);
        data_t data;
        data.process = current_process;
        deque_insert(process_queue, data);
    }
    if(status == DONE){
        printf("Process %d is done!\n", current_process->pid);
    }

    /* if we have reach a time where a process would have arrived, simulate its arrival by removing it from the
     * pending queue and inserting it into the process queue
     */
    if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);
        printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }

    return next_process_arrival_time;
}

int run_process_rr(process_t *process, int quantum_rr){

    // decrement process timer
    process->time_remaining--;

    if(quantum_rr == 0 && process->time_remaining > 0){
        return NOT_DONE;
    }
    else if(quantum_rr == 0 && process->time_remaining == 0) {
        return DONE;
    }
    else if(process->time_remaining == 0) {
        return DONE;
    }
    return -1;
}

int insert_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int time){
    data_t data = deque_remove(pending_process_queue);
    printf("Process %d arrived at %d\n", data.process->pid, time);
    deque_insert(process_queue, data);
    if(pending_process_queue->head != NULL) {
        return pending_process_queue->foot->data.process->time_rec;
    }
    else{ // no more pending
        return -1;
    }
}