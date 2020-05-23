
#include "memory_v.h"
void virtual_memory(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue,
        int simulation_time_elapsed, int process_pages_req, int *loading_cost){

    /**
     * WHAT IF ALL PAGES ALREADY LOADED
     */
    /**
     * Requires at least 4 pages to be in memory, then page fault for the rest.
     * Ie. add 1 to the process->time_remaining
     * If require less then 4 pages just load them all
     * However if there are more empty pages available, continue to fill process pages until either all loaded or no empty pages left
     *
     * So basically try to load al the pages, but (if space_available < process_pages,
     *      if at least 4 empty (space_available >= 4           => fill until space_available == 0 || pages_remaining == 0),
     *      if not at least 4 empty pages (space_available < 4) => remove least recent by by page until space_available == 4;
     */

    /**
     * If enough space available load all specified pages
     */

    //printf("process_pages_req: %d\n", process_pages_req);
    //printf("space_available: %d\n", *space_available);
    //printf("loading_cost: %d\n", *loading_cost);



    if(process_pages_req <= *space_available) {

        load_pages_v(pages, num_pages, space_available, process, process_pages_req, loading_cost);
    }
    /**
     * if not enough space available
     */
    else{

        /**
         * if a least 4 pages empty, load these 4 pages or fill all empty pages worth, and proceed. Page fault will mimic other page swaps
         */
        if(*space_available >= 4){

            /**
             * fill as much space as available
             */
            int pages_remaining = *space_available;
            load_pages_v(pages, num_pages, space_available, process, pages_remaining, loading_cost);
        }
        /**
         * if not enough room to fit minimum requirement,
         * swap least recent process pages out of memory one by one until 4 pages available or enough to fit the process_requirement
         */
        else {
            /**
             * If we are swapping check to see if any of the processes pages are currently in memory
             */
            int pages_remaining;
            int currently_in_mem = count_process_mem(pages, num_pages, process);
            /**
             * pass pages_remaining in to be 4 so that once 4 pages are available then it will break the discard process
             */

            // If the process is smaller than 4 then just free enough to fit it
            if((process->mem_req/PAGE_SIZE) < 4){
                pages_remaining = process_pages_req;
                if(currently_in_mem != 0){
                    pages_remaining = pages_remaining - currently_in_mem;
                }
            }

            else{
                pages_remaining = 4;
                if(currently_in_mem != 0){
                    pages_remaining = pages_remaining - currently_in_mem;
                }
            }
            //printf("pages remaining: %d\n", pages_remaining);
            swap_pages_v(pages, num_pages, space_available, process, pages_remaining, process_queue, simulation_time_elapsed, loading_cost);
        }
    }

}

void load_pages_v(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, int *loading_cost){

    for (int i = 0; i < num_pages; i++) {
        if(pages[i] == -1) {

            pages[i] = process->pid;
            pages_remaining--;
            *space_available = *space_available-1;
            *loading_cost = *loading_cost + 2;
            if(pages_remaining == 0){
                break;
            }
        }
    }
    /**
     * Set occupying memory to true
     */
    process->occupying_memory = 1;
    print_memory(pages, num_pages);
    //printf("space_available after load: %d\n", *space_available);
}

void swap_pages_v(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining,
        deque_t *process_queue, int simulation_time_elapsed, int *loading_cost){

    //printf("Swapping pages\n");
    //printf("Pages remaining: %d\n", pages_remaining);

    process_t *least_recent_process = new_process();
    node_t *curr = process_queue->foot;
    /**
     * While there is not enough space to store either minimum pages or all pages if page req is < 4
     */
    while(pages_remaining > *space_available) {
        /**
         * Determine the least recent process and discard its pages from memory ONE BY ONE
         */
        while (curr != NULL) {
            //printf("curr process: Process %d\n", curr->data.process->pid);
            if (curr->data.process->time_started != -1 && curr->data.process->occupying_memory != -1) {
                least_recent_process = curr->data.process;
                //printf("least recent process: Process %d\n", least_recent_process->pid);
                break;
            }
            else { // if the process at the front of the queue has not yet been executed, there is no memory to replace, or its already been discarded
                curr = curr->prev;
            }
        }
        // discard its pages from memory
        //print_evicted(pages, num_pages, least_recent_process, simulation_time_elapsed);

        /**
         * CAREFUL ABOUT SETTING OCCUPYING MEMORY TO FALSE as some pages may still exist in memory
         */
        discard_pages_v(pages, num_pages, space_available, least_recent_process, simulation_time_elapsed, pages_remaining);
    }
    //printf("Flushed memory\n");
    print_memory(pages, num_pages);
    //printf("space_available after discard: %d\n", *space_available);
    fprintf(stderr, "\n");

    /**
     * Once there is enough space available to store all process' pages, load them
     */
    //printf("Enough space, load pages\n");
    load_pages_v(pages, num_pages, space_available, process, pages_remaining, loading_cost);
}

void discard_pages_v(int *pages, int num_pages, int *space_available, process_t *process, int simulation_time_elapsed, int pages_remaining){
    /**
     * DISCARD until space_available == pages_remaining
     * will also print the evicted output
     */

    //printf("pages remaining: %d\n", pages_remaining);
    int *mem_addresses = (int *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));
    int index = 0;

    // remove process pages from memory until space available == 4
    int count = 0;
    int removed_count = 0;
    int remove = 1;
    for(int i = 0; i < num_pages; i++){
        if(pages[i] == process->pid){
            count++;

            if(remove == 1) {
                // discard page
                pages[i] = -1;
                removed_count++;
                *space_available = *space_available + 1;

                // add it to evicted memory address
                mem_addresses[index] = i;
                //printf("MEMIndex %d\n", index);
                index++;
                //printf("Evicted page %d\n", i);

                /**
                 * If we not have space to load the specified number of pages, stop discarding
                 */
                if (*space_available == pages_remaining) {
                    // set a flag to stop removing but continue to count
                    remove = 0;
                }
            }
        }
    }
    /**
     * Set occupying memory to false if all pages were removed
     */
    if(count == removed_count) {
        process->occupying_memory = -1;
    }

    /**
     * Print Evicted
     */

    print_evicted(process, simulation_time_elapsed, mem_addresses, index);
}

int count_process_mem(const int *pages, int num_pages, process_t *process) {

    int count = 0;
    for(int i = 0; i < num_pages; i++){

        // if contains the id we are looking for, update count
        if(pages[i] == process->pid){
            count++;
        }
    }
    return count;
}