#include "memory_cm.h"

long long swapping_least_frequent(long long *pages, long long num_pages, long long *space_available, process_t *process, deque_t *process_queue,
                    long long simulation_time_elapsed, long long process_pages_req, long long *loading_cost, long long *pages_freq){
    /**
     * Try to load al the pages, but (if space_available < process_pages,
     *      if at least 4 empty (space_available >= 4           => fill until space_available == 0 || pages_remaining == 0),
     *      if not at least 4 empty pages (space_available < 4) => remove least recent by by page until space_available == 4;
     */

    long long currently_in_mem = count_process_mem(pages, num_pages, process);
    /**
     *  If the process already meet memory requirement, and there is no space in memory to load additional pages
     *  DO NOT LOAD
     */
    if(currently_in_mem >= 4 && *space_available == 0){
        return -1;
    }

    if(process_pages_req <= *space_available) {

        load_pages_cm(pages, num_pages, space_available, process, process_pages_req, loading_cost, pages_freq);
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
            long long pages_remaining = *space_available;
            load_pages_cm(pages, num_pages, space_available, process, pages_remaining, loading_cost, pages_freq);
        }
            /**
             * if not enough room to fit minimum requirement,
             * swap least recent process pages out of memory one by one until 4 pages available or enough to fit the process_requirement
             */
        else {
            /**
             * If we are swapping check to see if any of the processes pages are currently in memory
             */
            long long pages_remaining;
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
            swap_pages_cm(pages, num_pages, space_available, process, pages_remaining, process_queue,
                          simulation_time_elapsed, loading_cost, pages_freq);
        }
    }
    return 1;
}

void load_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining, long long *loading_cost, long long *pages_freq){

    for (long long i = 0; i < num_pages; i++) {
        if(pages[i] == -1) {

            pages[i] = process->pid;
            pages_remaining--;
            *space_available = *space_available-1;
            *loading_cost = *loading_cost + 2;

            /**
             * SET FREQUENCY = 1
             */
            pages_freq[i] = 1;
            if(pages_remaining == 0){
                break;
            }
        }
    }
    /**
     * Set occupying memory to true
     */
    process->occupying_memory = 1;
    print_memory_cm(pages, num_pages, pages_freq);
}

void swap_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining,
                   deque_t *process_queue, long long simulation_time_elapsed, long long *loading_cost, long long *pages_freq){


    long long *mem_addresses = (long long *) malloc(sizeof(*mem_addresses) * (process->mem_req / PAGE_SIZE));
    long long mem_addresses_len = 0;

    /**
     * While there is not enough space to store either minimum pages or all pages if page req is < 4
     */
    while(pages_remaining > *space_available) {
        /**
         * Determine the process/pages least frequently accessed and discard its pages from memory ONE BY ONE
         * NOT INCLUDING CURRENT PROCESS
         */
        print_memory_cm(pages, num_pages, pages_freq);
        long long pid = determine_least_frequent_process(pages, pages_freq, num_pages, process);
        fprintf(stderr, "Least frequent access process: %lld\n", pid);

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
        discard_pages_cm(pages, num_pages, space_available, oldest_process, pages_remaining, mem_addresses, &mem_addresses_len, pages_freq);
    }
    print_memory_cm(pages, num_pages, pages_freq);

    /**
     * Once there is enough space available to store all process' pages, load them
     */
    load_pages_cm(pages, num_pages, space_available, process, pages_remaining, loading_cost, pages_freq);

    insertion_sort_evicted(mem_addresses, mem_addresses_len);
    print_evicted(simulation_time_elapsed, mem_addresses, mem_addresses_len);
}

void discard_pages_cm(long long *pages, long long num_pages, long long *space_available, process_t *process, long long pages_remaining,
                      long long *mem_addresses, long long *mem_addresses_len, long long *pages_freq){
    /**
     * DISCARD until space_available == pages_remaining
     * will also prlong long the evicted output
     */

    // remove process pages from memory until space available == 4
    long long count = 0;
    long long removed_count = 0;
    long long remove = 1;
    for(long long i = 0; i < num_pages; i++){
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
                pages_freq[i] = -1;

                // add it to evicted memory address
                mem_addresses[*mem_addresses_len] = i;
                *mem_addresses_len = *mem_addresses_len + 1;

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

void initialize_time(long long *pages_freq, long long num_pages){

    for(long long i = 0; i < num_pages; i++){
        pages_freq[i] = -1;
    }
}

void update_pages_time(const long long *pages, long long *pages_freq, long long num_pages, process_t *process){

    for(long long i = 0; i < num_pages; i++){
        if(pages[i] == process->pid) {

            // update access frequency
            pages_freq[i] = pages_freq[i]+1;
        }
    }
}

long long determine_least_frequent_process(long long *pages, const long long *pages_freq, long long num_pages, process_t *process){
    long long min_frequency = 100;
    long long min_frequency_index = 0;
    for(long long i = 0; i < num_pages; i++){
        if(pages_freq[i] < min_frequency && pages[i] != process->pid && pages[i] != -1) {
            min_frequency = pages_freq[i];
            min_frequency_index = i;
        }
    }
    return pages[min_frequency_index];
}

void print_memory_cm(long long *pages, long long num_pages, long long *pages_freq){
    for(long long i = 0; i < num_pages; i++){
        fprintf(stderr, "Page %2lld: %2lld\tFreq: %lld\n", i, pages[i], pages_freq[i]);
    }
}
