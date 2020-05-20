
#include "memory_p.h"

void swapping_x(int total_memory, process_t *process, deque_t *process_queue){

    int num_pages = total_memory/PAGE_SIZE;

    // initialize RAM pages
    int *pages = (int*)malloc(sizeof(*pages) * num_pages);

    int space_available = num_pages;

    initialize_empty_pages(pages, num_pages);

    // pages remaining
    int process_pages_req = process->mem_req / PAGE_SIZE;

    // load the pages into memory
    load_pages(pages, num_pages, space_available, process, process_pages_req, process_queue);

    free(pages);
}

void initialize_empty_pages(int *pages, int num_pages){
    for(int i = 0; i < num_pages; i++){
        pages[i] = -1;
    }
}

void load_pages(int *pages, int num_pages, int space_available, process_t *process, int pages_remaining, deque_t *process_queue) {

    //int process_pages_req = process->mem_req / PAGE_SIZE;
    //int pages_remaining = process_pages_req;

    // find the next empty page !!
    //int pages_loaded = 0;
    for (int i = 0; i < num_pages; i++) {
        if(pages[i] == -1) {

            pages[i] = process->pid;
            pages_remaining--;
            space_available--;
            if(pages_remaining == 0){
                break;
            }
        }
    }
    if (pages_remaining != 0) {

        swap_pages(pages, num_pages, space_available, process, pages_remaining, process_queue);
    }
}

void swap_pages(int *pages, int num_pages, int space_available, process_t *process, int pages_remaining, deque_t *process_queue){

    // find the process least recently executed and replace its pages
    // how to find the process least recently executed?
    // the first visited!!!! process in the process queue

    /**
     * determine least recent process
     */
    process_t *least_recent_process = new_process();

    node_t *curr = process_queue->foot;
    while (curr != NULL) {

        if (curr->data.process->time_started != NULL) {

            least_recent_process = process_queue->foot->data.process;
            break;
        } else { // if the process at the front of the queue has not yet been executed, there is no memory to replace
            curr = curr->prev;
        }
    }
    // discard its pages from memory
    space_available = discard_pages(pages, num_pages, space_available, least_recent_process, pages_remaining, process_queue);

    /**
     * If we return from this and there is still not enough space we will continue back tracking in the queue to find
     * the next least recently executed process and remove it's pages also
     */
    while(pages_remaining > space_available) {

        // determine the least recently executed process
        assert(curr != NULL);
        curr = curr->prev;
        while (curr != NULL) {

            if (curr->data.process->time_started != NULL) {

                least_recent_process = process_queue->foot->data.process;
            } else { // if the process at the front of the queue has not yet been executed, there is no memory to replace
                curr = curr->prev;
            }
        }

        // discard its pages from memory
        space_available = discard_pages(pages, num_pages, space_available, least_recent_process, pages_remaining, process_queue);
    }

    /**
     * Once there is enough space available to store all process' pages, load them
     */

    /**
     * LOAD
     */

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

int discard_pages(int *pages, int num_pages, int space_available, process_t *least_recent_process){
    /**
     * DISCARD
     */
    // remove all 'least_recent_process' pages from memory
    for(int i = 0; i < num_pages; i++){
        if(pages[i] == least_recent_process->pid){
            pages[i] = -1;
            space_available++;
        }
    }
    free(least_recent_process);

    return space_available;
    // will return back to swap() where the next least recent will be determined and passed back into this function
}



