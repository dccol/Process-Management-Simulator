
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "process.h"


process_t *new_process() {
    process_t *process;
    process = (process_t*)malloc(sizeof(*process));
    assert(process!=NULL);
    process->time_started = -1;
    /*process->time_rec = (int)malloc(sizeof(int));
    process->pid = (int)malloc(sizeof(int));
    process->mem_req = (int)malloc(sizeof(int));
    process->time_remaining = (int)malloc(sizeof(int));*/

    return process;
}

void populate_process_t(process_t *process, int i, char* data){

    int i_data = atoi(data);

    if(i == 0){ // time_rec
        process->time_rec = i_data;
    }
    if(i == 1){ // pid
        process->pid = i_data;
    }
    if(i == 2){ // mem_req
        process->mem_req = i_data;
    }
    if(i == 3){ // time_remaining
        process->time_remaining = i_data;
    }
}

