
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
} process_t;

process_t *new_process();

void populate_process_t(process_t *process, int i, char* data);

void run_process(process_t *process);
#endif