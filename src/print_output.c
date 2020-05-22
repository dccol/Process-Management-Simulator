
#include "print_output.h"

void print_evicted(int *pages, int num_pages, process_t *process, int simulation_time_elapsed){
    int num_process_pages = process->mem_req / PAGE_SIZE;
    int *mem_addresses = (int *) malloc(sizeof(*mem_addresses) * num_process_pages);
    find_process_mem(pages, num_pages, process, mem_addresses);

    // Print
    printf("%d, EVICTED, mem-addresses=[", simulation_time_elapsed);
    for (int i = 0; i < num_process_pages - 1; i++) {
        printf("%d,", mem_addresses[i]);
    }
    printf("%d]\n", mem_addresses[num_process_pages - 1]);
    free(mem_addresses);
}