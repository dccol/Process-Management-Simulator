#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "ff.h"

#define NOT_DONE 1
#define DONE 2

#define LOADING 3
#define RUNNING 4

#define LOADED 5
#define NOT_LOADED 6


void fc_fs(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, int memory_size){

    // VARIABLES
    int simulation_time_elapsed = 0;

    int state = RUNNING;
    int loaded = NOT_LOADED;
    int loading_cost = 0;

    // MEMORY VARIABLES
    int num_pages = -1;
    int *pages = NULL;
    int space_available = -1;

    /**
     * If the Memory option is not unlimited, initialize the memory data structure
     */
    if(!strstr(memory_opt, "u")){

        // INITIALISE MEMORY
        num_pages = memory_size/PAGE_SIZE;

        // initialize RAM pages
        pages = (int*)malloc(sizeof(*pages) * num_pages);

        space_available = num_pages;

        initialize_empty_pages(pages, num_pages);
        print_memory(pages, num_pages);
    }

    /**
     * If a process has been received at time 0 insert it into the process queue (transfer from pending queue)
     */
    check_pending(pending_process_queue, process_queue, simulation_time_elapsed);

    /**
     * While there are processes in the process queue, step the simulation
     */
    while(process_queue->head != NULL){

        data_t data = deque_remove(process_queue);
        process_t *process = data.process;

        /**
         * If memory option not unlimited
         */
        if(strstr(memory_opt, "p")) {

            /**
             * Set STATE to LOADING
             */
            state = LOADING;
            loaded = NOT_LOADED;
            loading_cost = (process->mem_req / PAGE_SIZE) * 2;
            //printf("Loading time: %d\n", loading_cost);
            /*printf("%3d, RUNNING, id: %d, remaining-time: %d, load-time: %d\n",
                   simulation_time_elapsed, process->pid, process->time_remaining, loading_cost);*/
        }
        else if(strstr(memory_opt, "v")){
            // virtual memory
        }

        /**
         * Print output to stdout
         */
        if(!strstr(memory_opt, "u")){

            int mem_usage = ((num_pages - space_available) / num_pages) * 100;

            int num_process_pages = process->mem_req / PAGE_SIZE;
            int *mem_addresses = (int*)malloc(sizeof(*mem_addresses) * num_process_pages);
            for(int i = 0; i < num_pages; i++){
                if(pages[i] == process->pid){
                    mem_addresses[i] = i;
                }
            }
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d, mem-addresses=[",
                   simulation_time_elapsed, process->pid, process->time_remaining, loading_cost, mem_usage);
            for(int i = 0; i < num_process_pages -1 ; i++){
                printf("%d, ", mem_addresses[i]);
            }
            printf("%d]\n", mem_addresses[num_process_pages-1]);
        }
        else {
            printf("%d, RUNNING, id=%d, remaining-time=%d\n", simulation_time_elapsed, process->pid,
                   process->time_remaining);
        }


        // While the process being ran has time remaining, step the simulation
        while(process->time_remaining > 0) {

            //simulation_time_elapsed++;

            // also pass in a state
            // if state is loading => load pages but simulation time increase by 2 not 1, then change state
            // if state running => proceed as normal
            step_ff(pending_process_queue, process_queue, process, &simulation_time_elapsed, pages, num_pages,
                    space_available, memory_opt, &state, &loaded, &loading_cost);

            if(process->time_remaining == 0){
                printf("%d, FINISHED, id=%d, proc-remaining=%d\n", simulation_time_elapsed,
                       process->pid, process_queue->size);
            }

        }
    }
    printf("All Processes Complete\n");
    free(pages);
}

/**
 * abstraction of a unit of time (second)
 */
int step_ff(deque_t *pending_process_queue, deque_t *process_queue, process_t *current_process,
        int *simulation_time_elapsed, int *pages, int num_pages, int space_available, char *memory_opt, int* state, int *loaded, int *loading_cost){


    /**
     * IF LOADING => LOAD PROCESS PAGES INTO MEMORY
     */
    if(*state == LOADING){

        // decrement time takes to load
        fprintf(stderr, "%d, RUNNING, id=%d, remaining-time=%d, load-time=%d\n",
               *simulation_time_elapsed, current_process->pid, current_process->time_remaining, *loading_cost);

        // how long we stay in loaded is based on 2*num loaded pages
        if(*loaded != LOADED) {

            swapping_x(pages, num_pages, space_available, current_process, process_queue);
            *loaded = LOADED;

            /**
             * PRINT TO STDOUT
             */
            int mem_usage = ((num_pages - space_available) / num_pages) * 100;

            int num_process_pages = current_process->mem_req / PAGE_SIZE;
            int *mem_addresses = (int*)malloc(sizeof(*mem_addresses) * num_process_pages);
            for(int i = 0; i < num_pages; i++){
                if(pages[i] == current_process->pid){
                    mem_addresses[i] = i;
                }
            }
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d, mem-addresses=[",
                   *simulation_time_elapsed, current_process->pid, current_process->time_remaining, *loading_cost, mem_usage);
            for(int i = 0; i < num_process_pages -1 ; i++){
                printf("%d, ", mem_addresses[i]);
            }
            printf("%d]\n", mem_addresses[num_process_pages-1]);

        }
        // if loading has been completed in the previous tick, tick until loading cost has been reached,
        // then change the state so that the next tick runs the process
        else{
            if(*loading_cost == 0) {
                *state = RUNNING;
                fprintf(stderr, "LOADING COMPLETE\n");
            }
        }
        *loading_cost = *loading_cost - 1;
    }

    /**
     * OTHERWISE RUN THE PROCESS
     */
    else if(*state == RUNNING) {

        /**
         * BEGIN RUNNING THE PROCESS
         */
        current_process->time_started = *simulation_time_elapsed;
        //printf("Process %d beginning at time %d\n", current_process->pid, current_process->time_started);
        fprintf(stderr, "%d, RUNNING, id=%d, remaining-time=%d\n", *simulation_time_elapsed, current_process->pid, current_process->time_remaining);
        int status = run_process_ff(current_process);

        //printf("%3d, RUNNING, id: %d, remaining-time: %d\n", *simulation_time_elapsed, current_process->pid, current_process->time_remaining);
        /**
         * If the process is done
         */
        if (status == DONE) {
            /*printf("%3d, FINISHED, id: %d, remaining-time %d, proc-remaining: %d\n", *simulation_time_elapsed,
                    current_process->pid, current_process->time_remaining, process_queue->size);*/
            /**
             * IF NOT USING UNLIMITED MEMORY REMOVE PROCESS FROM MEMORY
             */
            if (!strstr(memory_opt, "u")) {
                space_available = discard_pages(pages, num_pages, space_available, current_process);
            }
        }
    }


    /**
     * If a process has been received at current simulation time, insert it into the process queue (transfer from pending queue)
     */
    check_pending(pending_process_queue, process_queue, *simulation_time_elapsed);

    *simulation_time_elapsed = *simulation_time_elapsed + 1;

    //printf("%3d| RUNNING, id: %d, remaining-time: %d\n", simulation_time_elapsed, current_process->pid, current_process->time_remaining);
    //}
    return space_available;
}

int run_process_ff(process_t *process){

    // decrement process timer
    process->time_remaining--;

    if(process->time_remaining == 0){
        return DONE;
    }
    return NOT_DONE;
}

int insert_pending_ff(deque_t *pending_process_queue, deque_t *process_queue, int time){
    data_t data = deque_remove(pending_process_queue);
    //printf("Process %d arrived at %d\n", data.process->pid, time);
    deque_insert(process_queue, data);
    if(pending_process_queue->head != NULL) {
        return pending_process_queue->foot->data.process->time_rec;
    }
    else{ // no more pending
        return -1;
    }
}

/**
 * If there are pending processes, check if any arrives at time = simulation time elapsed
 * @param pending_process_queue
 * @param process_queue
 * @param simulation_time
 */
void check_pending(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time){

    if(pending_process_queue->head != NULL) {

        // dynamically add memory in future , realloc?
        data_t *processes_to_insert = (data_t*)malloc(sizeof(*processes_to_insert) * 10);
        process_t *next_process_to_arrive = pending_process_queue->foot->data.process;

        int index = 0;
        while (next_process_to_arrive->time_rec == simulation_time) {

            // remove from queue and add to array
            data_t data = deque_remove(pending_process_queue);
            processes_to_insert[index] = data;
            index++;

            // check next process in the queue if the pending queue is not empty
            if (pending_process_queue->head != NULL) {
                next_process_to_arrive = pending_process_queue->foot->data.process;
            }
            else{
                break;
            }
        }

        // if nothing added, return
        if(index == 0){
            return;
        }
        // once no more processes to add to the array. Sort
        insertion_sort(processes_to_insert, index);

        for (int i = 0; i < index; i++) {
            //printf("%3d, Process ID: %d\n", simulation_time, processes_to_insert[i].process->pid);
            deque_insert(process_queue, processes_to_insert[i]);
        }
        free(processes_to_insert);
    }
}
