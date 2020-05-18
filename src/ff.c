#include <stdio.h>
#include "ff.h"

void fc_fs(deque_t *process_queue){

    while(process_queue->head != NULL){
        data_t data = deque_remove(process_queue);
        process_t *process = data.process;
        run_process_ff(process);
    }
    printf("All Processes Complete\n");
}

void run_process_ff(process_t *process){

    while(process->time_remaining > 0){
        printf("Process %d has %d seconds left\n", process->pid, process->time_remaining);
        process->time_remaining--;
    }
}