
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"
#include "ff.h"
#include "rr.h"

#define DEBUG 0

#define MAX_CHAR 1000

int main(int argc, char **argv) {

    // INSTANTIATE PENDING QUEUE
    deque_t *pending_process_queue = new_deque();

    // INSTANTIATE PROCESS QUEUE
    deque_t *process_queue = new_deque();

    // READ IN THE PROCESSES
    FILE *processes_file = fopen("../test/processes.txt", "r");
    char str[MAX_CHAR];
    if(processes_file == NULL){
        printf("Could not open file\n");
        return 1;
    }

    // READ BY LINE
    while (fgets(str, MAX_CHAR, processes_file) != NULL) {
        //printf("%s", str);

        process_t *process = new_process();

        // SPLIT INTO VARIABLES
        char *token = strtok(str, " ");

        int i = 0;
        while (token != NULL) {

            // POPULATE PROCESS DATA STRUCTURE
            populate_process_t(process, i, token);
            token = strtok(NULL, " ");
            i++;
        }

        data_t data;
        data.process = process;
        deque_insert(pending_process_queue, data);
    }

    printf("\n");

    if(DEBUG == 1) {
        node_t *curr = pending_process_queue->foot;
        while (curr != NULL) {
            printf("Process ID: %d\n", curr->data.process->pid);
            printf("Time Received: %d\n", curr->data.process->time_rec);
            printf("Memory Requirement: %d\n", curr->data.process->mem_req);
            printf("Time Remaining: %d\n", curr->data.process->time_remaining);
            printf("\n");

            curr = curr->prev;
        }
    }
    // DATA READ, CLOSE FILE
    fclose(processes_file);

    // SELECT SCHEDULE ALGORITHM
    char *alg = "rr";
    int quantum = 10;

    if(strstr(alg, "rr")){
        rr(pending_process_queue, process_queue, quantum);
    }
    if(strstr(alg, "ff")) {
        fc_fs(pending_process_queue, process_queue);
    }
    free_deque(pending_process_queue);
    free_deque(process_queue);
    return 0;



}