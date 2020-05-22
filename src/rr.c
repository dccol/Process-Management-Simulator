
#include "rr.h"

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

void rr(deque_t *pending_process_queue, deque_t *process_queue, char *memory_opt, int memory_size, int quantum){

    // VARIABLES
    int simulation_time_elapsed = 0;

    int state = RUNNING;
    int loaded = NOT_LOADED;
    int loading_cost = 0;

    int status = NOT_DONE;

    // MEMORY VARIABLES
    int num_pages = -1;
    int *pages = NULL;
    int space_available = -1;

    // STATISTIC VARIABLES
    int interval_throughput = 0;
    double throughput_av = 0;
    int throughput_min = 100;
    int throughput_max = 0;

    double turnaround_av = 0;
    int num_processes_finished = 0;

    double max_overhead = 0;
    double overhead_av = 0;

    /**
     * If the Memory option is not unlimited, initialize the memory data structure
     */
    if(!strstr(memory_opt, "u")){

        // INITIALISE MEMORY
        num_pages = memory_size/PAGE_SIZE;

        // initialize RAM pages
        pages = (int*)malloc(sizeof(*pages) * num_pages);

        space_available = num_pages;

        initialize_empty_pages(pages, num_pages);
        print_memory(pages, num_pages);
    }

    /**
     * If a process has been received at time 0 insert it into the process queue (transfer from pending queue)
     */
    check_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);

    /**
     * While the process queue is non-empty, remove the first process in the queue and run it.
     * This process will run until either it's time remaining attribute hits 0, or quantum hits 0
     * Quantum resets once a new process has been removed
     */
    while(process_queue->head != NULL || pending_process_queue->head != NULL){

        /**
         * Check whether any new processes have arrived before entering waiting step
         */
        check_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);

        /**
         * If the process queue is empty, but more processes are scheduled to arrive, wait
         */
        if(process_queue->head == NULL){
            state = WAITING;
            process_t *place_holder_process = new_process();

            int quantum_rr = quantum;
            step_rr(process_queue, place_holder_process, &simulation_time_elapsed, pages, num_pages,
                    &space_available, &state, &loaded, &loading_cost, &quantum_rr, &status);

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

            /**
             * Remove the first process from the queue
             */
            data_t data = deque_remove(process_queue);
            process_t *process = data.process;

            // Set start time of process if it has not been run yet
            if(process->time_started == -1) {
                process->time_started = simulation_time_elapsed;
            }

            /**
             * If memory option not unlimited
             */
            if (strstr(memory_opt, "p")) {

                /**
                 * Set STATE to LOADING
                 */
                state = LOADING;
                loaded = NOT_LOADED;
                loading_cost = (process->mem_req / PAGE_SIZE) * 2;
            }
            else if (strstr(memory_opt, "v")) {
                // virtual memory
            }
            else{ // If memory is unlimited
                state = RUNNING;
            }

            /**
             * Print output to stdout
             */
            if (strstr(memory_opt, "u")) {
                printf("%d, RUNNING, id=%d, remaining-time=%d\n", simulation_time_elapsed, process->pid,
                       process->time_remaining);
            }

            // Set the quantum value of the process run time, resets once a new process is ran
            int quantum_rr = quantum;

            /**
             * This is our time step.
             * Simulation time elapsed will increase 1:1 per step
             * Quantum will decrease 1:1 per step
             */
            while (process->time_remaining > 0 && quantum_rr > 0) {



                /**
                 * COME BACK TO HERE
                 */
                // Keep track of the next process arrival time
                step_rr(process_queue, process, &simulation_time_elapsed, pages, num_pages,
                        &space_available, &state, &loaded, &loading_cost, &quantum_rr, &status);

                /**
                 * If a process has been received at current simulation time, insert it into the process queue (transfer from pending queue)
                 */
                check_pending_rr(pending_process_queue, process_queue, simulation_time_elapsed);

                /**
                 * If the process is not done, reinsert it into the process queue
                 */
                if (status == NOT_DONE && quantum_rr == 0) {
                    fprintf(stderr,"%d, Inserting Process %d back into the queue. It has %d seconds left\n", simulation_time_elapsed, process->pid,
                            process->time_remaining);

                    deque_insert(process_queue, data);
                    //print_deque(process_queue);
                }

                /**
                 * If the Process is finished
                 */
                if (process->time_remaining == 0) {


                    /**
                     * IF NOT USING UNLIMITED MEMORY REMOVE PROCESS FROM MEMORY
                     */
                    if (!strstr(memory_opt, "u")) {

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

                        discard_pages(pages, num_pages, &space_available, process);
                    }
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", simulation_time_elapsed,
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
                    int turnaround_time = simulation_time_elapsed - process->time_rec;
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
     * PRINT STATISTICS
     */
    // Round up if decimal
    if((throughput_av - (int)throughput_av) != 0){
        throughput_av = round_up(throughput_av);
    }
    else{
        throughput_av = (int)throughput_av;
    }
    printf("Throughput%2.0lf, %d, %d\n", throughput_av, throughput_min, throughput_max);

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
    printf("Makespan %d\n", simulation_time_elapsed);
    //printf("All Processes Complete\n");
    free(pages);
}

/**
 * abstraction of a unit of time (second)
 */
void step_rr(deque_t *process_queue, process_t *current_process, int *simulation_time_elapsed,int *pages,
        int num_pages, int *space_available, int *state, int *loaded, int *loading_cost, int *quantum_rr, int *status){

    if(*state == LOADING){

        // decrement time takes to load
        fprintf(stderr, "%d, RUNNING, id=%d, remaining-time=%d, load-time=%d\n",
                *simulation_time_elapsed, current_process->pid, current_process->time_remaining, *loading_cost);


        // how long we stay in loaded is based on 2*num loaded pages
        if(*loaded != LOADED) {

            swapping_x(pages, num_pages, space_available, current_process, process_queue);
            *loaded = LOADED;

            /**
             * PRINT TO STDOUT
             */
            double mem_usage = ((double)num_pages - (double)*space_available) / (double)num_pages * 100;
            if((mem_usage - (int)mem_usage) != 0){
                mem_usage = round_up(mem_usage);
            }
            else{
                mem_usage = (int)mem_usage;
            }

            int num_process_pages = current_process->mem_req / PAGE_SIZE;

            int *mem_addresses = (int*)malloc(sizeof(*mem_addresses) * num_process_pages);
            find_process_mem(pages, num_pages, current_process, mem_addresses);

            // Print
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%2.0lf%%, mem-addresses=[",
                   *simulation_time_elapsed, current_process->pid, current_process->time_remaining, *loading_cost, mem_usage);
            for(int i = 0; i < num_process_pages -1 ; i++){
                printf("%d,", mem_addresses[i]);
            }
            printf("%d]\n", mem_addresses[num_process_pages-1]);
            free(mem_addresses);

        }
        // if loading has been completed in the previous tick, tick until loading cost has been reached,
        // then change the state so that the next tick runs the process
        else{
            if(*loading_cost == 1) {
                *state = RUNNING;
                //fprintf(stderr, "LOADING COMPLETE\n");
            }
        }

        *loading_cost = *loading_cost - 1;
    }

    /**
     * IF RUNNING => RUN THE PROCESS
     */
    else if(*state == RUNNING) {

        //printf("Process %d beginning at time %d\n", current_process->pid, current_process->time_started);
        fprintf(stderr, "%d, RUNNING, id=%d, remaining-time=%d, quantum=%d\n", *simulation_time_elapsed, current_process->pid,
                current_process->time_remaining, *quantum_rr);

        // Status used to determine whether the process should be re-inserted into the process queue
        *status = run_process_rr(current_process, quantum_rr);
    }

    /**
     * If WAITING => DO NOTHING, but check incoming process' and tick time
     */
    else if(*state == WAITING){
        fprintf(stderr, "%d, WAITING\n", *simulation_time_elapsed);
    }

    /**
     * TICK
     */
    *simulation_time_elapsed = *simulation_time_elapsed + 1;
}

int run_process_rr(process_t *process, int *quantum_rr){

    // decrement process timer
    process->time_remaining--;

    // decrement quantum
    *quantum_rr = *quantum_rr - 1;

    if(*quantum_rr == 0 && process->time_remaining > 0){
        return NOT_DONE;
    }
    else if(*quantum_rr == 0 && process->time_remaining == 0) {
        return DONE;
    }
    else if(process->time_remaining == 0) {
        return DONE;
    }
    return -1;
}

void check_pending_rr(deque_t *pending_process_queue, deque_t *process_queue, int simulation_time){

    if(pending_process_queue->head != NULL) {
        // dynamically add memory in future
        data_t processes_to_insert[10];
        process_t *next_process_to_arrive = pending_process_queue->foot->data.process;

        int index = 0;
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
        insertion_sort(processes_to_insert, index);

        for (int i = 0; i < index; i++) {
            fprintf(stderr, "%3d, Process ID: %d\n", simulation_time, processes_to_insert[i].process->pid);
            deque_insert(process_queue, processes_to_insert[i]);
        }
    }
}
