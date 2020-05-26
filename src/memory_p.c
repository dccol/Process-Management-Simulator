
#include "memory_p.h"

void swapping_x(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue, int simulation_time_elapsed, int *pages_time){

    // Pages remaining to load
    int process_pages_req = process->mem_req / PAGE_SIZE;
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

void initialize_empty_pages(int *pages, int num_pages){
    for(int i = 0; i < num_pages; i++){
        pages[i] = -1;
        //printf("Page %d: %d\n", i, pages[i]);
    }
}

void load_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining) {

    for (int i = 0; i < num_pages; i++) {
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
    //print_memory(pages, num_pages);

}

void swap_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, deque_t *process_queue, int simulation_time_elapsed, int *pages_time){

    //printf("Swapping pages\n");
    //printf("Pages remaining: %d\n", pages_remaining);

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
            //printf("curr process: Process %d\n", curr->data.process->pid);
            if (curr->data.process->time_started != -1 && curr->data.process->occupying_memory != -1) {
                least_recent_process = curr->data.process;
                //printf("least recent process: Process %d\n", least_recent_process->pid);
                break;
            }
            else { // if the process at the front of the queue has not yet been executed, there is no memory to replace
                curr = curr->prev;
            }
        }
        // discard its pages from memory
        //print_evicted(pages, num_pages, least_recent_process, simulation_time_elapsed);
        discard_pages(pages, num_pages, space_available, least_recent_process, simulation_time_elapsed, pages_time);
    }
    //printf("Flushed memory\n");
    //print_memory(pages, num_pages);
    //printf("\n");

    /**
     * Once there is enough space available to store all process' pages, load them
     */
    //printf("Enough spcace, load pages\n");
    load_pages(pages, num_pages, space_available, process, pages_remaining);
}

void discard_pages(int *pages, int num_pages, int *space_available, process_t *process, int simulation_time_elapsed, int *pages_time){
    /**
     * DISCARD
     */

    int *mem_addresses = (int *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));
    int index = 0;

    // remove all process pages from memory
    for(int i = 0; i < num_pages; i++){
        if(pages[i] == process->pid){
            pages[i] = -1;
            *space_available = *space_available+1;

            // if using cm update time structure
            if(pages_time != NULL){
                pages_time[i] = -1;
            }

            // add it to evicted memory address
            mem_addresses[index] = i;
            //printf("MEMIndex %d\n", index);
            index++;
            //printf("Evicted page %d\n", i);
        }
    }
    /**
     * Set occupying memory to false
     */
    process->occupying_memory = -1;

    /**
     * Print Evicted
     */
    print_evicted(simulation_time_elapsed, mem_addresses, index);
}

void print_memory(int *pages, int num_pages){
    for(int i = 0; i < num_pages; i++){
        fprintf(stderr, "Page %2d: %2d\n", i, pages[i]);
    }
}
void find_process_mem(int *pages, int num_pages, process_t *process, int *mem_addresses) {

    int index = 0;
    for(int i = 0; i < num_pages; i++){

        // if contains the id we are looking for, append to mem_addresses
        if(pages[i] == process->pid){

            mem_addresses[index] = i;
            index++;
        }
    }
}


//