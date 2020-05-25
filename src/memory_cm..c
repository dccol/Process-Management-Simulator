
#include "memory_cm.h"

int swapping_oldest(int *pages, int num_pages, int *space_available, process_t *process, deque_t *process_queue,
                   int simulation_time_elapsed, int process_pages_req, int *loading_cost, int *pages_time){

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


    int currently_in_mem = count_process_mem(pages, num_pages, process);

    /**
     *  If the process already meet memory requirement, and there is no space in memory to load additional pages
     *  DO NOT LOAD
     */
    if(currently_in_mem >= 4 && *space_available == 0){
        return -1;
    }

    if(process_pages_req <= *space_available) {

        load_pages_cm(pages, num_pages, space_available, process, process_pages_req, loading_cost, pages_time);
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
            load_pages_cm(pages, num_pages, space_available, process, pages_remaining, loading_cost, pages_time);
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
            currently_in_mem = count_process_mem(pages, num_pages, process);
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
            swap_pages_cm(pages, num_pages, space_available, process, pages_remaining, process_queue,
                    simulation_time_elapsed, loading_cost, pages_time);
        }
    }
    return 1;
}

void load_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining, int *loading_cost, int *pages_time){

    for (int i = 0; i < num_pages; i++) {
        if(pages[i] == -1) {

            pages[i] = process->pid;
            pages_remaining--;
            *space_available = *space_available-1;
            *loading_cost = *loading_cost + 2;

            /**
             * TIME
             */
            pages_time[i] = 0;
            if(pages_remaining == 0){
                break;
            }
        }
    }
    /**
     * Set occupying memory to true
     */
    process->occupying_memory = 1;
    print_memory_cm(pages, num_pages, pages_time);
    //printf("space_available after load: %d\n", *space_available);
}

void swap_pages_cm(int *pages, int num_pages, int *space_available, process_t *process, int pages_remaining,
                  deque_t *process_queue, int simulation_time_elapsed, int *loading_cost, int *pages_time){

    //printf("Swapping pages\n");
    //printf("Pages remaining: %d\n", pages_remaining);

    int *mem_addresses = (int *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));
    int mem_addresses_len = 0;

    /**
     * While there is not enough space to store either minimum pages or all pages if page req is < 4
     */
    while(pages_remaining > *space_available) {
        /**
         * Determine the OLDEST process/pages belong to process and discard its pages from memory ONE BY ONE
         * NOT INCLUDING CURRENT PROCESS
         */
        print_memory_cm(pages, num_pages, pages_time);
        int pid = determine_oldest_process(pages, pages_time, num_pages, process);
        fprintf(stderr, "Oldest Process: %d\n", pid);

        /**
         * Retrieve this process from the queue to pass to discard
         */
        process_t *oldest_process = new_process();
        node_t *curr = process_queue->foot;
        while(curr != NULL){
            if(curr->data.process->pid == pid){
                oldest_process = curr->data.process;
            }
            curr = curr->prev;
        }

        // discard its pages from memory
        discard_pages_cm(pages, num_pages, space_available, oldest_process, simulation_time_elapsed, pages_remaining, mem_addresses, &mem_addresses_len, pages_time);
    }
    //printf("Flushed memory\n");
    print_memory_cm(pages, num_pages, pages_time);
    //printf("space_available after discard: %d\n", *space_available);
    fprintf(stderr, "\n");

    /**
     * Once there is enough space available to store all process' pages, load them
     */
    //printf("Enough space, load pages\n");
    load_pages_cm(pages, num_pages, space_available, process, pages_remaining, loading_cost, pages_time);

    insertion_sort_evicted(mem_addresses, mem_addresses_len);
    print_evicted(process, simulation_time_elapsed, mem_addresses, mem_addresses_len);
}

void discard_pages_cm(int *pages, int num_pages, int *space_available, process_t *process,
        int simulation_time_elapsed, int pages_remaining, int *mem_addresses, int *mem_addresses_len, int *pages_time){
    /**
     * DISCARD until space_available == pages_remaining
     * will also print the evicted output
     */

    //printf("pages remaining: %d\n", pages_remaining);
    //int *mem_addresses = (int *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));

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

                /**
                 * TIME
                 */
                pages_time[i] = -1;

                // add it to evicted memory address
                mem_addresses[*mem_addresses_len] = i;
                //printf("MEMIndex %d\n", index);
                *mem_addresses_len = *mem_addresses_len + 1;
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
}

void initialize_time(int *pages_time, int num_pages){

    for(int i = 0; i < num_pages; i++){
        pages_time[i] = -1;
        //printf("Page %d: %d\n", i, pages[i]);
    }
}

void update_pages_time(int *pages_time, int num_pages){
    for(int i = 0; i < num_pages; i++){
        if(pages_time[i] != -1) {
            pages_time[i] = pages_time[i]+1;
            //printf("Page %d: %d\n", i, pages[i]);
        }
    }
}

int determine_oldest_process(int *pages, int *pages_time, int num_pages, process_t *process){
    int max_age = 0;
    int max_age_index = 0;
    for(int i = 0; i < num_pages; i++){
        if(pages_time[i] > max_age && pages[i] != process->pid) {
            max_age = pages_time[i];
            max_age_index = i;
        }
    }
    return pages[max_age_index];
}

void print_memory_cm(int *pages, int num_pages, int *pages_time){
    for(int i = 0; i < num_pages; i++){
        fprintf(stderr, "Page %2d: %2d\tAge: %d\n", i, pages[i], pages_time[i]);
    }
}

void flush_pages(int *pages, int num_pages, int *space_available, process_t *process, int simulation_time_elapsed, int *pages_time){
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

            /**
             * TIME
             */
            pages_time[i] = -1;

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
    print_evicted(process, simulation_time_elapsed, mem_addresses, index);
}