
#include "memory_p.h"

void swapping_x(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time){

    // Pages remaining to load
    long long process_pages_req = process->mem_req / PAGE_SIZE;
    //printf("Process %d needs %d pages of memory\n", process->pid, process_pages_req);

    /**
     * If enough space available load pages
     */
    if(process_pages_req <= *space_available) {
        // Load the pages into memory
        load_pages(pages, num_pages, space_available, process, process_pages_req);
    }
    else{
        swap_pages(pages, num_pages, space_available, process, process_pages_req, process_queue, simulation_time_elapsed, pages_time);
    }

}

void initialize_empty_pages(long long *pages, long long num_pages){
    for(long long i = 0; i < num_pages; i++){
        pages[i] = -1;
    }
}

void load_pages(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining) {

    for (long long i = 0; i < num_pages; i++) {
        if(pages[i] == -1) {

            pages[i] = process->pid;
            pages_remaining--;
            *space_available = *space_available-1;
            if(pages_remaining == 0){
                break;
            }
        }
    }
    /**
     * Set occupying memory to true
     */
    process->occupying_memory = 1;
}

void swap_pages(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, deque_t *process_queue, long long simulation_time_elapsed, long long *pages_time){

    process_t *least_recent_process = new_process();
    node_t *curr = process_queue->foot;
    /**
     * While there is not enough space to store all the pages
     */
    while(pages_remaining > *space_available) {
        /**
         * Determine the least recent process and discard its pages from memory
         */
        while (curr != NULL) {

            if (curr->data.process->time_started != -1 && curr->data.process->occupying_memory != -1) {
                least_recent_process = curr->data.process;
                break;
            }
            else {
                curr = curr->prev;
            }
        }
        // discard its pages from memory
        discard_pages(pages, num_pages, space_available, least_recent_process, simulation_time_elapsed, pages_time);
    }

    /**
     * Once there is enough space available to store all process' pages, load them
     */
    load_pages(pages, num_pages, space_available, process, pages_remaining);
}

void discard_pages(long long *pages, long long num_pages, long long *space_available, process_t *process, long long simulation_time_elapsed, long long *pages_time){
    /**
     * DISCARD
     */

    long long *mem_addresses = (long long *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));
    long long index = 0;

    // remove all process pages from memory
    for(long long i = 0; i < num_pages; i++){
        if(pages[i] == process->pid){
            pages[i] = -1;
            *space_available = *space_available+1;

            // if using cm update time structure
            if(pages_time != NULL){
                pages_time[i] = -1;
            }

            // add it to evicted memory address
            mem_addresses[index] = i;
            index++;
        }
    }
    /**
     * Set occupying memory to false
     */
    process->occupying_memory = -1;

    /**
     * Prlong long Evicted
     */
    print_evicted(simulation_time_elapsed, mem_addresses, index);
}

void print_memory(long long *pages, long long num_pages){
    for(long long i = 0; i < num_pages; i++){
        fprintf(stderr, "Page %2lld: %2lld\n", i, pages[i]);
    }
}
void find_process_mem(const long long *pages, long long num_pages, process_t *process, long long *mem_addresses) {

    long long index = 0;
    for(long long i = 0; i < num_pages; i++){

        // if contains the id we are looking for, append to mem_addresses
        if(pages[i] == process->pid){

            mem_addresses[index] = i;
            index++;
        }
    }
}
