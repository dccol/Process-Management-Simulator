
#ifndef PROCESS_H
#define PROCESS_H


/**
 * data structure to contain process information
 */
typedef struct process {
    long long time_rec;
    long long pid;
    long long  mem_req;
    long long time_remaining;
    long long job_time;
    long long time_started;
    long long occupying_memory;

} process_t;

process_t *new_process();

/**
 * Populates process fields
 * @param process
 * @param i
 * @param data
 */
void populate_process_t(process_t *process, long long i, char* data);


#endif