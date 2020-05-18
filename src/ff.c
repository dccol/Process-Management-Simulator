#include <stdio.h>
#include "ff.h"

#define NOT_DONE 1
#define DONE 2


void fc_fs(deque_t *pending_process_queue, deque_t *process_queue){


    int simulation_time_elapsed = 0;
    int next_process_arrival_time = pending_process_queue->foot->data.process->time_rec;

    printf("Next Process will arrive at %d\n", next_process_arrival_time);

    // if a process is received insert it into the process queue (transfer from pending queue)
    if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending(pending_process_queue, process_queue, simulation_time_elapsed);
        printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }

    // while there are processes in the process queue, step the simulation
    while(process_queue->head != NULL){

        data_t data = deque_remove(process_queue);
        process_t *process = data.process;

        printf("%3d| Current Process Running: %d\tTime Remaining: %d\n", simulation_time_elapsed, process->pid, process->time_remaining);

        while(process->time_remaining > 0) {

            simulation_time_elapsed++;

            // keep track of the next process arrival time
            next_process_arrival_time = step(pending_process_queue, process_queue, process, simulation_time_elapsed, next_process_arrival_time);
        }
    }
    printf("All Processes Complete\n");
}

/**
 * abstraction of a unit of time (second)
 */
int step(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
        int simulation_time_elapsed, int next_process_arrival_time){

    int status = run_process_ff(current_process);

    /* if we have reach a time where a process would have arrived, simulate its arrival by removing it from the
     * pending queue and inserting it into the process queue
     */
    if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending(pending_process_queue, process_queue, simulation_time_elapsed);
        printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }

    printf("%3d| Current Process Running: %d\tTime Remaining: %d\n", simulation_time_elapsed, current_process->pid, current_process->time_remaining);
    return next_process_arrival_time;
}

int run_process_ff(process_t *process){

    // decrement process timer
    process->time_remaining--;

    if(process->time_remaining == 0){
        return DONE;
    }
    return NOT_DONE;
}

int insert_pending(deque_t *pending_process_queue, deque_t *process_queue, int time){
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
