
#include "print_output.h"

void print_evicted(long long simulation_time_elapsed, long long *mem_addresses, long long mem_addresses_len){

    // Print
    printf("%lld, EVICTED, mem-addresses=[", simulation_time_elapsed);
    for (long long i = 0; i < mem_addresses_len - 1; i++) {
        printf("%lld,", mem_addresses[i]);
    }
    printf("%lld]\n", mem_addresses[mem_addresses_len - 1]);
    free(mem_addresses);
}

void print_load(long long *pages, long long num_pages, const long long *space_available, process_t *process, const long long *loading_cost, const long long *simulation_time_elapsed){
    double mem_usage = ((double)num_pages - (double)*space_available) / (double)num_pages * 100;
    if((mem_usage - (int)mem_usage) != 0){
        mem_usage = round_up(mem_usage);
    }
    else{
        mem_usage = (int)mem_usage;
    }

    long long num_process_pages = process->mem_req / PAGE_SIZE;

    long long *mem_addresses = (long long*) malloc(sizeof(*mem_addresses) * num_process_pages);
    long long currently_in_mem = count_process_mem(pages, num_pages, process);
    find_process_mem(pages, num_pages, process, mem_addresses);

    // Print
    printf("%lld, RUNNING, id=%lld, remaining-time=%lld, load-time=%lld, mem-usage=%2.0lf%%, mem-addresses=[",
           *simulation_time_elapsed, process->pid, process->time_remaining, *loading_cost, mem_usage);
    for(long long i = 0; i < currently_in_mem -1 ; i++){
        printf("%lld,", mem_addresses[i]);
    }
    printf("%lld]\n", mem_addresses[currently_in_mem-1]);
    free(mem_addresses);
}
