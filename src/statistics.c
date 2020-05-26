
#include "statistics.h"
#include "process.h"

void calculate_throughput(int simulation_time_elapsed, double *throughput_av, int *throughput_min, int *throughput_max, int interval_throughput){
    int interval_num = round_up((double)simulation_time_elapsed/(double)60);
    fprintf(stderr,"%d, Throughput of Interval %d = %d\n", simulation_time_elapsed, interval_num, interval_throughput);
    fprintf(stderr,"%d, Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    double throughput_total = *throughput_av * (interval_num-1);
    *throughput_av = (throughput_total + interval_throughput) / interval_num;


    fprintf(stderr,"%d, Max-Throughput = %d\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%d, Min-Throughput = %d\n", simulation_time_elapsed, *throughput_min);



    if(interval_throughput > *throughput_max){
        *throughput_max = interval_throughput;
    }
    if(interval_throughput < *throughput_min){
        *throughput_min = interval_throughput;
    }
    fprintf(stderr,"%d, NEW Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    fprintf(stderr,"%d, NEW Max-Throughput = %d\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%d, NEW Min-Throughput = %d\n", simulation_time_elapsed, *throughput_min);
}

void calculate_turnaround_time(int simulation_time_elapsed, int turnaround_time, double *turnaround_av, int num_processes_finished){

    fprintf(stderr,"%d, TurnAround-time = %d\n", simulation_time_elapsed, turnaround_time);
    double turnaround_total = *turnaround_av * (num_processes_finished-1);
    *turnaround_av = (turnaround_total + turnaround_time)/num_processes_finished;
    fprintf(stderr,"%d, TurnAround-avg = %lf\n", simulation_time_elapsed, *turnaround_av);
}

void calculate_overhead(int simulation_time_elapsed, int turnaround_time, int job_time, double *max_overhead, double *overhead_av, int num_processes_finished){

    double overhead = (double)turnaround_time/(double)job_time;
    fprintf(stderr,"%d, Overhead = %lf\n", simulation_time_elapsed, overhead);

    fprintf(stderr,"%d, MAX-Overhead = %lf\n", simulation_time_elapsed, *max_overhead);

    if (overhead > *max_overhead){
        *max_overhead = overhead;
    }
    fprintf(stderr,"%d, NEW MAX-Overhead = %lf\n", simulation_time_elapsed, *max_overhead);
    double overhead_total = *overhead_av * (num_processes_finished-1);
    *overhead_av = (overhead_total + overhead)/num_processes_finished;
    fprintf(stderr,"%d, MAX-Overhead = %lf\n", simulation_time_elapsed, *overhead_av);
}