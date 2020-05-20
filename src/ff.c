#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "ff.h"

#define NOT_DONE 1
#define DONE 2


void fc_fs(deque_t *pending_process_queue, deque_t *process_queue){

    // VARIABLES
    int simulation_time_elapsed = 0;
    int next_process_arrival_time = pending_process_queue->foot->data.process->time_rec;

    //printf("Next Process will arrive at %d\n", next_process_arrival_time);

    /* If a process has been received insert it into the process queue (transfer from pending queue)
     * Inserting the first process received at time = 0
     * */
    /*if(simulation_time_elapsed == next_process_arrival_time){

        // Update the time of the next process arrival
        next_process_arrival_time = insert_pending_ff(pending_process_queue, process_queue, simulation_time_elapsed);
        //printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }*/
    check_pending(pending_process_queue, process_queue, simulation_time_elapsed);

    /**
     * While there are processes in the process queue, step the simulation
     */

    while(process_queue->head != NULL){

        data_t data = deque_remove(process_queue);
        process_t *process = data.process;

        printf("%3d, RUNNING, id: %d, remaining-time: %d\n", simulation_time_elapsed, process->pid, process->time_remaining);

        // While the process being ran has time remaining, step the simulation
        while(process->time_remaining > 0) {

            simulation_time_elapsed++;

            // Keep track of the next process arrival time
            next_process_arrival_time = step_ff(pending_process_queue, process_queue, process, simulation_time_elapsed, next_process_arrival_time);
        }
    }
    printf("All Processes Complete\n");
}

/**
 * abstraction of a unit of time (second)
 */
int step_ff(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
        int simulation_time_elapsed, int next_process_arrival_time){

    int status = run_process_ff(current_process);
    if(status == DONE){
        printf("%3d, FINISHED, id: %d, proc-remaining: %d\n", simulation_time_elapsed, current_process->pid, process_queue->size);
    }

    /* if we have reach a time where a process would have arrived, simulate its arrival by removing it from the
     * pending queue and inserting it into the process queue
     */
    /*if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending_ff(pending_process_queue, process_queue, simulation_time_elapsed);
        //printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }*/

    check_pending(pending_process_queue, process_queue, simulation_time_elapsed);

    //printf("%3d| RUNNING, id: %d, remaining-time: %d\n", simulation_time_elapsed, current_process->pid, current_process->time_remaining);
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

int insert_pending_ff(deque_t *pending_process_queue, deque_t *process_queue, int time){
    data_t data = deque_remove(pending_process_queue);
    //printf("Process %d arrived at %d\n", data.process->pid, time);
    deque_insert(process_queue, data);
    if(pending_process_queue->head != NULL) {
        return pending_process_queue->foot->data.process->time_rec;
    }
    else{ // no more pending
        return -1;
    }
}

/**
 * If there are pending processes, check if any arrives at time = simulation time elapsed
 * @param pending_process_queue
 * @param process_queue
 * @param simulation_time
 */
void check_pending(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time){

    if(pending_process_queue->head != NULL) {
        // dynamically add memory in future
        data_t *processes_to_insert = (data_t*)malloc(sizeof(*processes_to_insert) * 10);
        process_t *next_process_to_arrive = pending_process_queue->foot->data.process;

        int index = 0;
        while (next_process_to_arrive->time_rec == simulation_time) {

            // remove from queue and add to array
            data_t data = deque_remove(pending_process_queue);
            processes_to_insert[index] = data;
            index++;

            // check next process in the queue if the pending queue is not empty
            if (pending_process_queue->head != NULL) {
                next_process_to_arrive = pending_process_queue->foot->data.process;
            }
            else{
                break;
            }
        }

        // if nothing added, return
        if(index == 0){
            return;
        }
        // once no more processes to add to the array. Sort
        insertion_sort(processes_to_insert, index);

        for (int i = 0; i < index; i++) {
            printf("%3d, Process ID: %d\n", simulation_time, processes_to_insert[i].process->pid);
            deque_insert(process_queue, processes_to_insert[i]);
        }
        free(processes_to_insert);
    }
}
