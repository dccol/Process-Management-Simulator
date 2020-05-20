
#include <stdio.h>

#include "rr.h"
#include "process.h"

#define NOT_DONE 1
#define DONE 2

void rr(deque_t *pending_process_queue, deque_t *process_queue, int quantum){

    // VARIABLES
    int simulation_time_elapsed = 0;

    //printf("Next Process will arrive at %d\n", next_process_arrival_time);

    // If a process is received, insert it into the process queue (transfer from pending queue)
    /*if(simulation_time_elapsed == next_process_arrival_time){

        // Update the time of the next process arrival
        next_process_arrival_time = insert_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);
        printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }*/

    check_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);

    /**
     * While the process queue is non-empty, remove the first process in the queue and run it.
     * This process will run until either it's time remaining attribute hits 0, or quantum hits 0
     * Quantum resets once a new process has been removed
     */
    while(process_queue->head != NULL){

        // move declarations outside loop to perhaps save memory? (CHECK VALGRIND)
        data_t data = deque_remove(process_queue);
        process_t *process = data.process;

        printf("%3d, RUNNING, id: %d, remaining-time: %d\n", simulation_time_elapsed, process->pid, process->time_remaining);

        // Set the quantum value of the process run time, resets once a new process is ran
        int quantum_rr = quantum;

        /**
         * This is our time step.
         * Simulation time elapsed will increase 1:1 per step
         * Quantum will decrease 1:1 per step
         */
        while(process->time_remaining > 0 && quantum_rr > 0) {

            simulation_time_elapsed++;
            quantum_rr--;

            // Keep track of the next process arrival time
            step_rr(pending_process_queue, process_queue, process, simulation_time_elapsed, quantum_rr);
        }
    }
    printf("All Processes Complete\n");
}

/**
 * abstraction of a unit of time (second)
 */
void step_rr(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
         int simulation_time_elapsed, int quantum_rr){

    // Status used to determine whether the process should be re-inserted into the process queue
    int status = run_process_rr(current_process, quantum_rr);

    //printf("%3d| Current Process Running: %d\tTime Remaining: %d\tQuantum: %d\n", simulation_time_elapsed, current_process->pid, current_process->time_remaining, quantum_rr);

    if(status == NOT_DONE) {
        /*printf("Inserting Process %d back into the queue. It has %d seconds left\n", current_process->pid,
               current_process->time_remaining);*/
        data_t data;
        data.process = current_process;
        deque_insert(process_queue, data);
    }
    if(status == DONE){
        printf("%3d, FINISHED, id: %d, proc-remaining: %d\n", simulation_time_elapsed, current_process->pid, process_queue->size);
    }

    /* if we have reach a time where a process would have arrived, simulate its arrival by removing it from the
     * pending queue and inserting it into the process queue
     */
    /*if(simulation_time_elapsed == next_process_arrival_time){

        // update the time of the next process arrival
        next_process_arrival_time = insert_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);
        //printf("Next Process will arrive at %d\n", next_process_arrival_time);
    }*/

    check_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);
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
    //printf("Process %d arrived at %d\n", data.process->pid, time);
    deque_insert(process_queue, data);
    if(pending_process_queue->head != NULL) {
        return pending_process_queue->foot->data.process->time_rec;
    }
    else{ // no more pending
        return -1;
    }
}
void check_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time){

    if(pending_process_queue->head != NULL) {
        // dynamically add memory in future
        data_t processes_to_insert[10];
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
    }


}
