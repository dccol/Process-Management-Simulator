
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "deque.h"
#include "process.h"
#include "ff.h"
#include "rr.h"

#define DEBUG 0

#define MAX_CHAR 1000

void initialize_queue(FILE *processes_file, deque_t *pending_process_queue);

int main(int argc, char **argv) {

    // INSTANTIATE PENDING QUEUE
    deque_t *pending_process_queue = new_deque();

    // INSTANTIATE PROCESS QUEUE
    deque_t *process_queue = new_deque();

    // Options
    int option_index = 0;
    char *schedule_alg = NULL;
    char *memory_opt = NULL;
    int memory_size = 0;
    int quantum = 10;

    FILE *processes_file;

    // Read in Options from command line
    while((option_index = getopt(argc, argv, "f:a:m:s:q:")) != -1){
        switch(option_index) {
            case 'f':
                processes_file = fopen(optarg, "r");
                if(processes_file == NULL){
                    printf("Could not open file\n");
                    return 1;
                }
                initialize_queue(processes_file, pending_process_queue);
                break;
            case 'a':
                schedule_alg = (char*)malloc(sizeof(*schedule_alg) * strlen(optarg) + 1);
                strncpy(schedule_alg, optarg, strlen(optarg));
                break;
            case 'm':
                memory_opt = (char*)malloc(sizeof(*memory_opt) * strlen(optarg) + 1);
                strncpy(memory_opt, optarg, strlen(optarg));
                break;
            case 's':
                memory_size = atoi(optarg);
                break;
            case 'q':
                quantum = atoi(optarg);
                break;
            default:
                printf("Incorrect option");
                return 1;
        }
    }

    if(DEBUG == 1) {
        printf("here");
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

    // Run provided Scheduling Algorithm
    if(strstr(schedule_alg, "rr")){
        rr(pending_process_queue, process_queue, quantum);
    }
    else if(strstr(schedule_alg, "ff")) {
        fc_fs(pending_process_queue, process_queue, memory_opt, memory_size);
    }
    free_deque(pending_process_queue);
    free_deque(process_queue);
    return 0;
}

void initialize_queue(FILE *processes_file, deque_t *pending_process_queue){

    char str[MAX_CHAR];

    // READ BY LINE
    while (fgets(str, MAX_CHAR, processes_file) != NULL) {

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
    fclose(processes_file);
}