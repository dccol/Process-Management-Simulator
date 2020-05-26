
#include "print_output.h"

void print_evicted(int simulation_time_elapsed, int *mem_addresses, int mem_addresses_len){

    // Print
    printf("%d, EVICTED, mem-addresses=[", simulation_time_elapsed);
    for (int i = 0; i < mem_addresses_len - 1; i++) {
        printf("%d,", mem_addresses[i]);
    }
    printf("%d]\n", mem_addresses[mem_addresses_len - 1]);
    free(mem_addresses);
}
//
void print_load(int *pages, int num_pages, const int *space_available, process_t *process, const int *loading_cost, const int *simulation_time_elapsed){
    double mem_usage = ((double)num_pages - (double)*space_available) / (double)num_pages * 100;
    if((mem_usage - (int)mem_usage) != 0){
        mem_usage = round_up(mem_usage);
    }
    else{
        mem_usage = (int)mem_usage;
    }

    int num_process_pages = process->mem_req / PAGE_SIZE;

    int *mem_addresses = (int*)malloc(sizeof(*mem_addresses) * num_process_pages);
    int currently_in_mem = count_process_mem(pages, num_pages, process);
    find_process_mem(pages, num_pages, process, mem_addresses);

    // Print
    printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%2.0lf%%, mem-addresses=[",
           *simulation_time_elapsed, process->pid, process->time_remaining, *loading_cost, mem_usage);
    for(int i = 0; i < currently_in_mem -1 ; i++){
        printf("%d,", mem_addresses[i]);
    }
    printf("%d]\n", mem_addresses[currently_in_mem-1]);
    free(mem_addresses);
}

void print_statistics(){

}