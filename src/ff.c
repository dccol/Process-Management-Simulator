#include <stdio.h>
#include "ff.h"

void fc_fs(deque_t *process_queue){

    while(process_queue->head != NULL){
        data_t data = deque_remove(process_queue);
        process_t *process = data.process;
        run_process(process);
    }
}