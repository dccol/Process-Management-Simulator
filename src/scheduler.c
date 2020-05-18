
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"

#define DEBUG 1

#define MAX_CHAR 1000

int main(int argc, char **argv) {

    // INSTANTIATE QUEUE
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
        printf("%s", str);

        process_t process;

        // SPLIT INTO VARIABLES
        char *token = strtok(str, " ");

        int i = 0;
        while (token != NULL) {

            populate_process_t(&process, i, token);
            token = strtok(NULL, " ");
            i++;
        }

        /*printf("Process ID: %d\n", process.pid);
        printf("Time Received: %d\n", process.time_rec);
        printf("Memory Requirement: %d\n", process.mem_req);
        printf("Time Remaining: %d\n", process.time_remaining);
        printf("\n");
         */

        data_t data;
        data.process = process;
        deque_insert(process_queue, data);
    }

    printf("\n");

    if(DEBUG == 1) {
        node_t *curr = process_queue->foot;
        while (curr != NULL) {
            printf("Process ID: %d\n", curr->data.process.pid);
            printf("Time Received: %d\n", curr->data.process.time_rec);
            printf("Memory Requirement: %d\n", curr->data.process.mem_req);
            printf("Time Remaining: %d\n", curr->data.process.time_remaining);
            printf("\n");

            curr = curr->prev;
        }
    }

    fclose(processes_file);
    return 0;



}