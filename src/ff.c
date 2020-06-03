
#include "ff.h"

#define NOT_DONE 1
#define DONE 2

/**
 * STATES
 */
#define LOADING 3
#define RUNNING 4
#define WAITING 5

#define LOADED 6
#define NOT_LOADED 7

void fc_fs(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, long long memory_size){

    // VARIABLES
   long long simulation_time_elapsed = 0;

   long long state = RUNNING;
   long long loading_cost = 0;
   long long loading_status = NOT_LOADED;

    // MEMORY VARIABLES
    long long num_pages = -1;
    long long *pages = NULL;
    long long space_available = -1;

   long long *pages_freq = NULL;

    // STATISTICS VARIABLES
   long long interval_throughput = 0;
    double throughput_av = 0;
   long long throughput_min = 100;
   long long throughput_max = 0;

    double turnaround_av = 0;
   long long num_processes_finished = 0;

    double max_overhead = 0;
    double overhead_av = 0;

    /**
     * If the Memory option is not unlimited, initialize the memory data structure
     */
    if(!strstr(memory_opt, "u")){

        // INITIALISE MEMORY
        num_pages = memory_size/PAGE_SIZE;

        // initialize RAM pages
        pages = (long long*)malloc(sizeof(*pages) * num_pages);
        space_available = num_pages;
        initialize_empty_pages(pages, num_pages);
        //print_memory(pages, num_pages);

        /**
         * If the Memory option is cm, initialize the time data structure to keep track of how long a page has been in memory
         */
        if(strstr(memory_opt, "cm")){

            pages_freq = (long long*)malloc(sizeof(*pages) * num_pages);
            initialize_freq(pages_freq, num_pages);
        }
    }

    /**
     * If a process has been received at time 0 insert it into the process queue (transfer from pending queue)
     */
    check_pending(pending_process_queue, process_queue, simulation_time_elapsed);

    /**
     * While there are processes in the process queue, step the simulation (ALSO IF THERE ARE MORE IN THE PENDING QUEUE)
     */
    while(process_queue->head != NULL || pending_process_queue->head != NULL){

        /**
         * Check whether any new processes have arrived before entering waiting step
         */
        check_pending(pending_process_queue, process_queue, simulation_time_elapsed);

        /**
         * If the process queue is empty, but more processes are scheduled to arrive, wait
         */
        if(process_queue->head == NULL){
            state = WAITING;
            process_t *place_holder_process = new_process();

            step_ff(process_queue, place_holder_process, &simulation_time_elapsed, pages, num_pages,
                    &space_available, &state, &loading_cost, memory_opt, pages_freq, &loading_status);

            /**
             * If interval is over calculate the throughput values
             */
            if((simulation_time_elapsed % 60) == 0){
                calculate_throughput(simulation_time_elapsed, &throughput_av, &throughput_min, &throughput_max, interval_throughput);

                // RESET THROUGHPUT_INTERVAL
                interval_throughput = 0;
            }
        }

        /**
         * Otherwise proceed with the alg as normal
         */
        else {
            data_t data = deque_remove(process_queue);
            process_t *process = data.process;

            // Set start time of process if it has not been run yet
            if(process->time_started == -1) {
                process->time_started = simulation_time_elapsed;
            }

            /**
             * If memory option is swapping x
             */
            if (strstr(memory_opt, "p")) {

                if(process->occupying_memory == -1) {
                    /**
                     * Set STATE to LOADING
                     */
                    state = LOADING;
                    loading_cost = (process->mem_req / PAGE_SIZE) * 2;
                }
            }

            /**
             * If the memory option is virtual memory
             */
            else if (strstr(memory_opt, "v")) {
                state = LOADING;
                loading_cost = 0;
                loading_status = NOT_LOADED;
            }
            /**
             * If memory option is cm
             */
            else if (strstr(memory_opt, "cm")){
                state = LOADING;
                loading_cost = 0;
                loading_status = NOT_LOADED;
            }
            /**
            * If memory is unlimited
            */
            else{
                state = RUNNING;
            }

            /**
             * Prlong long output to stdout
             */
            if (strstr(memory_opt, "u")) {
                printf("%lld, RUNNING, id=%lld, remaining-time=%lld\n", simulation_time_elapsed, process->pid,
                       process->time_remaining);
            }


            // While the process being ran has time remaining, step the simulation
            while (process->time_remaining > 0) {

                /**
                 * If a process has been received at current simulation time, insert it into the process queue (transfer from pending queue)
                 */
                check_pending(pending_process_queue, process_queue, simulation_time_elapsed);
                step_ff(process_queue, process, &simulation_time_elapsed, pages, num_pages,
                        &space_available, &state, &loading_cost, memory_opt, pages_freq, &loading_status);

                /**
                 * If the Process is finished
                 */
                if (process->time_remaining == 0) {


                    /**
                     * IF NOT USING UNLIMITED MEMORY REMOVE PROCESS FROM MEMORY
                     */
                    if (!strstr(memory_opt, "u")) {

                        //print_evicted(pages, num_pages, process, simulation_time_elapsed);

                        discard_pages(pages, num_pages, &space_available, process, simulation_time_elapsed, pages_freq);
                    }
                    printf("%lld, FINISHED, id=%lld, proc-remaining=%d\n", simulation_time_elapsed,
                           process->pid, process_queue->size);

                    /**
                     * KEEP TRACK OF HOW MANY PROCESSES HAVE FINISHED IN THE INTERVAL
                     */
                    interval_throughput++;

                    /**
                     * KEEP TRACK OF HOW MANY PROCESSES HAVE FINSIHED TOTAL
                     */
                    num_processes_finished++;

                    /**
                     * Calculate TurnAround-time of the process
                     */
                   long long turnaround_time = simulation_time_elapsed - process->time_rec;
                    calculate_turnaround_time(simulation_time_elapsed, turnaround_time, &turnaround_av, num_processes_finished);

                    /**
                     * Calculate Overhead of the process
                     */
                    calculate_overhead(simulation_time_elapsed, turnaround_time, process->job_time, &max_overhead, &overhead_av, num_processes_finished);
                }

                /**
                 * If interval is over calculate the throughput values
                 */
                if((simulation_time_elapsed % 60) == 0){

                    calculate_throughput(simulation_time_elapsed, &throughput_av, &throughput_min, &throughput_max, interval_throughput);

                    // RESET THROUGHPUT_INTERVAL
                    interval_throughput = 0;
                }

            }
        }
    }

    /**
      * If simulation ends before final interval, and more processes completed in that interval, calculate final values
      */
    if(interval_throughput > 0) {
        calculate_throughput_final(simulation_time_elapsed, &throughput_av, &throughput_min, &throughput_max,
                             interval_throughput);
    }
    /**
     * PRINT STATISTICS
     */
    // Round up if decimal
    if((throughput_av - (int)throughput_av) != 0){
        throughput_av = round_up(throughput_av);
    }
    else{
        throughput_av = (int)throughput_av;
    }
    printf("Throughput%2.0lf, %lld, %lld\n", throughput_av, throughput_min, throughput_max);

    // Round up if decimal
    if((turnaround_av - (int)turnaround_av) != 0){
        turnaround_av = round_up(turnaround_av);
    }
    else{
        turnaround_av= (int)turnaround_av;
    }
    printf("Turnaround time %2.0lf\n", turnaround_av);

    /**
     * ROUND 2 decimals dont just cut them off
     */
    printf("Time overhead %.2lf %.2lf\n", max_overhead, overhead_av);
    printf("Makespan %lld\n", simulation_time_elapsed);
    //printf("All Processes Complete\n");
    free(pages);
}

/**
 * abstraction of a unit of time (second)
 */
void step_ff(deque_t *process_queue, process_t *current_process,long long *simulation_time_elapsed, long long *pages,
        long long num_pages, long long *space_available, long long *state, long long *loading_cost, char *memory_opt,
        long long *pages_freq, long long *loading_status){

    /**
     * IF LOADING => LOAD PROCESS PAGES INTO MEMORY
     */
    if(*state == LOADING){

        // decrement time takes to load
        fprintf(stderr, "%lld, RUNNING, id=%lld, remaining-time=%lld, load-time=%lld\n",
               *simulation_time_elapsed, current_process->pid, current_process->time_remaining, *loading_cost);

        if (strstr(memory_opt, "p")) {
            // how long we stay in loaded is based on 2*num loaded pages
            if (current_process->occupying_memory == -1) {
                swapping_x(pages, num_pages, space_available, current_process, process_queue, *simulation_time_elapsed, pages_freq);
                /**
                 * PRINT TO STDOUT
                 */
                print_load(pages, num_pages, space_available, current_process, loading_cost, simulation_time_elapsed);

            }
            /**
             *  if loading has been completed in the previous tick, tick until loading cost has been reached,
             *  then change the state so that the next tick runs the process
             */
            else {
                if (*loading_cost == 1) {
                    *state = RUNNING;
                }
            }
            *loading_cost = *loading_cost - 1;
        }
        /**
         * If memory option is virtual memory
         */
        else if(strstr(memory_opt, "v")){

            /**
             * If status = NOT LOADED
             */
            if(*loading_status == NOT_LOADED) {

                /**
                 * If pages is less than 4 and not all the process pages are currently in memory, try and load more
                 */
               long long currently_in_mem = count_process_mem(pages, num_pages, current_process);
                // Pass in total pages NOT already in memory
               long long process_pages_req = (current_process->mem_req / PAGE_SIZE) - currently_in_mem;
                fprintf(stderr, "Process %lld would like %lld pages of memory\n", current_process->pid, process_pages_req);

                //print_memory(pages, num_pages);

                // if currently not all the pages are in memory, check if we can load some more
                if (currently_in_mem != (current_process->mem_req / PAGE_SIZE)) {

                    /**
                     * LOAD
                     */
                   long long result = virtual_memory(pages, num_pages, space_available, current_process, process_queue,
                                                *simulation_time_elapsed, process_pages_req, loading_cost);

                    /**
                     * Once loading has occurred, apply page fault cost
                     */
                    currently_in_mem = count_process_mem(pages, num_pages, current_process);
                   long long pages_not_in_mem = (current_process->mem_req / PAGE_SIZE) - currently_in_mem;
                   long long page_fault_cost = pages_not_in_mem;
                    //printf("%d pages not in memory, page cost added %d\n", pages_not_in_mem, page_fault_cost);
                    current_process->time_remaining = current_process->time_remaining + page_fault_cost;
                    /**
                     * Not applying page fault to processes when they have 4 pages already in memory
                     * so they dont load anymore because the pages are full
                     */
                    //printf("result %d\n", result);
                    /**
                     * A page was actually loaded
                     */
                    if (result == 1) {


                        /**
                         * PRINT TO STDOUT
                         */
                        print_load(pages, num_pages, space_available, current_process, loading_cost,
                                   simulation_time_elapsed);
                    }
                        /**
                         * If no loading occurred check if should change state
                         */
                    else {
                        if (*loading_cost == 1) {
                            *state = RUNNING;

                        } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                            /**
                             * PRINT
                             */
                            *state = RUNNING;
                            print_load(pages, num_pages, space_available, current_process, loading_cost,
                                       simulation_time_elapsed);

                            /**
                             * RETURN SO TICK DOESNT OCCUR
                             */
                            return;
                        }
                    }

                    /**
                     * Set state to loaded
                     */
                    *loading_status = LOADED;
                }
                else {
                    if (*loading_cost == 1) {
                        *state = RUNNING;

                    } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                        /**
                         * PRINT
                         */
                        *state = RUNNING;
                        print_load(pages, num_pages, space_available, current_process, loading_cost,
                                   simulation_time_elapsed);

                        /**
                         * RETURN SO TICK DOESNT OCCUR
                         */
                        return;
                    }
                }
            }
                /**
                 * If no loading occurred check if should change state
                 */
            else {
                if (*loading_cost == 1) {
                    *state = RUNNING;

                } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                    /**
                     * PRINT
                     */
                    *state = RUNNING;
                    print_load(pages, num_pages, space_available, current_process, loading_cost,
                               simulation_time_elapsed);

                    /**
                     * RETURN SO TICK DOESNT OCCUR
                     */
                    return;
                }
            }
            // decrement loading time
            *loading_cost = *loading_cost - 1;
        }
            /**
             * If cm
             */
        else if(strstr(memory_opt, "cm")){

            /**
             * If status = NOT LOADED
             */
            if(*loading_status == NOT_LOADED) {

                /**
                 * If pages is less than 4 and not all the process pages are currently in memory, try and load more
                 */
               long long currently_in_mem = count_process_mem(pages, num_pages, current_process);
                // Pass in total pages NOT already in memory
               long long process_pages_req = (current_process->mem_req / PAGE_SIZE) - currently_in_mem;
                fprintf(stderr, "Process %lld would like %lld pages of memory\n", current_process->pid, process_pages_req);

                //print_memory(pages, num_pages);

                // if currently not all the pages are in memory, check if we can load some more
                if (currently_in_mem != (current_process->mem_req / PAGE_SIZE)) {

                    /**
                     * LOAD
                     */
                   long long result = swapping_least_frequent(pages, num_pages, space_available, current_process, process_queue,
                                                 *simulation_time_elapsed, process_pages_req, loading_cost, pages_freq);

                    /**
                     * Once loading has occurred, apply page fault cost
                     */
                    currently_in_mem = count_process_mem(pages, num_pages, current_process);
                   long long pages_not_in_mem = (current_process->mem_req / PAGE_SIZE) - currently_in_mem;
                   long long page_fault_cost = pages_not_in_mem;
                    //printf("%d pages not in memory, page cost added %d\n", pages_not_in_mem, page_fault_cost);
                    current_process->time_remaining = current_process->time_remaining + page_fault_cost;
                    /**
                     * Not applying page fault to processes when they have 4 pages already in memory
                     * so they dont load anymore because the pages are full
                     */
                    //printf("result %d\n", result);
                    /**
                     * A page was actually loaded
                     */
                    if (result == 1) {


                        /**
                         * PRINT TO STDOUT
                         */
                        print_load(pages, num_pages, space_available, current_process, loading_cost,
                                   simulation_time_elapsed);
                    }
                        /**
                         * If no loading occurred check if should change state
                         */
                    else {
                        if (*loading_cost == 1) {
                            *state = RUNNING;

                        } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                            /**
                             * PRINT
                             */
                            *state = RUNNING;
                            print_load(pages, num_pages, space_available, current_process, loading_cost,
                                       simulation_time_elapsed);

                            /**
                             * RETURN SO TICK DOESNT OCCUR
                             */
                            return;
                        }
                    }

                    /**
                     * Set state to loaded
                     */
                    *loading_status = LOADED;
                }
                else {
                    if (*loading_cost == 1) {
                        *state = RUNNING;

                    } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                        /**
                         * PRINT
                         */
                        *state = RUNNING;
                        print_load(pages, num_pages, space_available, current_process, loading_cost,
                                   simulation_time_elapsed);

                        /**
                         * RETURN SO TICK DOESNT OCCUR
                         */
                        return;
                    }
                }
            }
            /**
             * If no loading occurred check if should change state
             */
            else {
                if (*loading_cost == 1) {
                    *state = RUNNING;

                } else if (*loading_cost == 0) { // if we never tried to load anything because already in memory
                    /**
                     * PRINT
                     */
                    *state = RUNNING;
                    print_load(pages, num_pages, space_available, current_process, loading_cost,
                               simulation_time_elapsed);

                    /**
                     * RETURN SO TICK DOESNT OCCUR
                     */
                    return;
                }
            }
            // decrement loading time
            *loading_cost = *loading_cost - 1;
        }
    }
    /**
     * IF RUNNING => RUN THE PROCESS
     */
    else if(*state == RUNNING) {

        fprintf(stderr, "%lld, RUNNING, id=%lld, remaining-time=%lld\n", *simulation_time_elapsed, current_process->pid, current_process->time_remaining);
        run_process_ff(current_process);

        /**
         * INCREMENT ALL ACTIVE PAGES AGE BY ONE
         */
        if(strstr(memory_opt, "cm")){
            update_pages_freq(pages, pages_freq, num_pages, current_process);
        }
    }

    /**
     * If WAITING => DO NOTHING, but check incoming process' and tick time
     */
    else if(*state == WAITING){
        fprintf(stderr, "%lld, WAITING\n", *simulation_time_elapsed);
    }
    /**
     * TICK
     */
    *simulation_time_elapsed = *simulation_time_elapsed + 1;
}

void run_process_ff(process_t *process){

    // decrement process timer
    process->time_remaining--;
}

/**
 * If there are pending processes, check if any arrives at time = simulation time elapsed
 * @param pending_process_queue
 * @param process_queue
 * @param simulation_time
 */
void check_pending(deque_t *pending_process_queue, deque_t *process_queue,long long simulation_time){

    if(pending_process_queue->head != NULL) {

        data_t *processes_to_insert = (data_t*)malloc(sizeof(*processes_to_insert) * pending_process_queue->size);
        process_t *next_process_to_arrive = pending_process_queue->foot->data.process;

       long long index = 0;
        while (next_process_to_arrive->time_rec == simulation_time) {

            // remove from queue and add to array
            data_t data = deque_remove(pending_process_queue);
            processes_to_insert[index] = data;
            index++;

            // check next process in the queue if the pending queue is not empty
            if (pending_process_queue->head != NULL) {
                next_process_to_arrive = pending_process_queue->foot->data.process;
            }
            else{
                break;
            }
        }

        // if nothing added, return
        if(index == 0){
            return;
        }
        // once no more processes to add to the array. Sort
        insertion_sort_pending(processes_to_insert, index);

        for (long long i = 0; i < index; i++) {
            fprintf(stderr,"%3lld, Process ID: %lld arrived\n", simulation_time, processes_to_insert[i].process->pid);
            deque_insert(process_queue, processes_to_insert[i]);
        }
        free(processes_to_insert);
    }
}

