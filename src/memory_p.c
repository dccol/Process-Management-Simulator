
#include "memory_p.h"

void swapping_x(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue, int simulation_time_elapsed){

    // Pages remaining to load
    int process_pages_req = process->mem_req / PAGE_SIZE;
    //printf("Process %d needs %d pages of memory\n", process->pid, process_pages_req);

    /**
     * If enough space available load pages
     */
    if(process_pages_req <= *space_available) {
        // Load the pages into memory
        load_pages(pages, num_pages, space_available, process, process_pages_req, process_queue);
    }
    else{
        swap_pages(pages, num_pages, space_available, process, process_pages_req, process_queue, simulation_time_elapsed);
    }

}

void initialize_empty_pages(int *pages, int num_pages){
    for(int i = 0; i < num_pages; i++){
        pages[i] = -1;
        //printf("Page %d: %d\n", i, pages[i]);
    }
}

void load_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, deque_t *process_queue) {

    //int process_pages_req = process->mem_req / PAGE_SIZE;
    //int pages_remaining = process_pages_req;

    // find the next empty page !!
    //int pages_loaded = 0;
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
    /*if (pages_remaining != 0) {

        swap_pages(pages, num_pages, space_available, process, pages_remaining, process_queue);
    }*/
}

void swap_pages(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, deque_t *process_queue, int simulation_time_elapsed){

    // find the process least recently executed and replace its pages
    // how to find the process least recently executed?
    // the first visited!!!! process in the process queue

    //printf("Swapping pages\n");
    //printf("Pages remaining: %d\n", pages_remaining);

    /**
     * determine least recent process
     */
    process_t *least_recent_process = new_process();

    node_t *curr = process_queue->foot;
    while (curr != NULL) {
        //printf("curr process: Process %d\n", curr->data.process->pid);
        if (curr->data.process->time_started != -1 && curr->data.process->occupying_memory != -1) {
            least_recent_process = curr->data.process;
            //printf("least recent process: Process %d\n", least_recent_process->pid);
            break;
        } else { // if the process at the front of the queue has not yet been executed, there is no memory to replace
            curr = curr->prev;
        }
    }
    // discard its pages from memory
    print_evicted(pages, num_pages, least_recent_process, simulation_time_elapsed);
    discard_pages(pages, num_pages, space_available, least_recent_process);
    //printf("Flushed memory\n");
    //print_memory(pages, num_pages);
    //printf("\n");

    // call function again?

    // in memory tag on processes
    /**
     * If we return from this and there is still not enough space we will continue back tracking in the queue to find
     * the next least recently executed process and remove it's pages also
     */
    while(pages_remaining > *space_available) {

        // determine the least recently executed process
        assert(curr != NULL);
        curr = curr->prev;
        while (curr != NULL) {

            if (curr->data.process->time_started != -1 && curr->data.process->occupying_memory != -1) {

                least_recent_process = process_queue->foot->data.process;
            } else { // if the process at the front of the queue has not yet been executed, there is no memory to replace
                curr = curr->prev;
            }
        }

        // discard its pages from memory
        print_evicted(pages, num_pages, least_recent_process, simulation_time_elapsed);
        discard_pages(pages, num_pages, space_available, least_recent_process);
    }
    //free(least_recent_process);

    /**
     * Once there is enough space available to store all process' pages, load them
     */

    /**
     * LOAD
     */

    //printf("Enough spcace, load pages\n");
    load_pages(pages, num_pages, space_available, process, pages_remaining, process_queue);
    // fil the pages with the process to be executed pages
    /*for(int i = 0; i < num_pages; i++){
        if(pages[i] == -1){
            pages[i] = process->pid;
            pages_remaining--;
            space_available--;
            if(pages_remaining == 0){
                break;
            }
        }
    }*/
}

void discard_pages(int *pages, int num_pages, int *space_available, process_t *process){
    /**
     * DISCARD
     */
    // remove all process pages from memory
    for(int i = 0; i < num_pages; i++){
        if(pages[i] == process->pid){
            pages[i] = -1;
            *space_available = *space_available+1;
        }
    }
    /**
     * Set occupying memory to false
     */
    process->occupying_memory = -1;
}

void print_memory(int *pages, int num_pages){
    for(int i = 0; i < num_pages; i++){
        printf("Page %2d: %2d\n", i, pages[i]);
    }
}
void find_process_mem(int *pages, int num_pages, process_t *process, int *mem_addresses) {
    for(int i = 0; i < num_pages; i++){
        if(pages[i] == process->pid){
            mem_addresses[i] = i;
        }
    }
}


