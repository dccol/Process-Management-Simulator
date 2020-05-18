
#include <stdio.h>

#include "rr.h"

#define NOT_DONE 1
#define DONE 2

void rr(deque_t *process_queue, int quantum){

    while(process_queue->head != NULL){
        data_t data = deque_remove(process_queue);
        process_t *process = data.process;
        int status = run_process_rr(process, quantum);

        // if the process is still not complete, insert into the queue
        if(status == 1){
            printf("Inserting Process %d back into the queue. It has %d seconds left\n", process->pid, process->time_remaining);
            deque_insert(process_queue, data);
        }
        if(status == 2){
            printf("Process %d is done!\n", process->pid);
        }
    }
    printf("All Processes Complete\n");
}

int run_process_rr(process_t *process, int quantum){

    while(process->time_remaining > 0){
        printf("Process %d has %d seconds left\n", process->pid, process->time_remaining);
        printf("Quantum value: %d\n", quantum);
        process->time_remaining--;
        quantum--;

        if(quantum == 0 && process->time_remaining != 0){
            // process still has time left, return 1 to indicate process is not finished
            return NOT_DONE;
        }
    }
    return DONE;
}