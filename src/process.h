
#ifndef PROCESS_H
#define PROCESS_H


/**
 * data structure to contain process information
 */
typedef struct process {
    int time_rec;
    int pid;
    int mem_req;
    int time_remaining;
    int job_time;
    int time_started;
    int occupying_memory;

} process_t;

process_t *new_process();

void populate_process_t(process_t *process, int i, char* data);


#endif