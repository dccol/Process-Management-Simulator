
#include "statistics.h"
#include "process.h"

void calculate_throughput(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min, long long *throughput_max, long long interval_throughput){
    long long interval_num = (simulation_time_elapsed/60);
    fprintf(stderr,"%lld, Throughput of Interval %lld = %lld\n", simulation_time_elapsed, interval_num, interval_throughput);
    fprintf(stderr,"%lld, Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    double throughput_total = *throughput_av * (interval_num-1);
    *throughput_av = (throughput_total + interval_throughput) / interval_num;


    fprintf(stderr,"%lld, Max-Throughput = %lld\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%lld, Min-Throughput = %lld\n", simulation_time_elapsed, *throughput_min);



    if(interval_throughput > *throughput_max){
        *throughput_max = interval_throughput;
    }
    if(interval_throughput < *throughput_min){
        *throughput_min = interval_throughput;
    }
    fprintf(stderr,"%lld, NEW Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    fprintf(stderr,"%lld, NEW Max-Throughput = %lld\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%lld, NEW Min-Throughput = %lld\n", simulation_time_elapsed, *throughput_min);
}

void calculate_throughput_final(long long simulation_time_elapsed, double *throughput_av, long long *throughput_min, long long *throughput_max, long long interval_throughput){
    long long interval_num = round_up((simulation_time_elapsed/60));
    fprintf(stderr,"%lld, Throughput of Interval %lld = %lld\n", simulation_time_elapsed, interval_num, interval_throughput);
    fprintf(stderr,"%lld, Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    double throughput_total = *throughput_av * (interval_num-1);
    *throughput_av = (throughput_total + interval_throughput) / interval_num;


    fprintf(stderr,"%lld, Max-Throughput = %lld\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%lld, Min-Throughput = %lld\n", simulation_time_elapsed, *throughput_min);



    if(interval_throughput > *throughput_max){
        *throughput_max = interval_throughput;
    }
    if(interval_throughput < *throughput_min){
        *throughput_min = interval_throughput;
    }
    fprintf(stderr,"%lld, NEW Avg-Throughput = %lf\n", simulation_time_elapsed, *throughput_av);
    fprintf(stderr,"%lld, NEW Max-Throughput = %lld\n", simulation_time_elapsed, *throughput_max);
    fprintf(stderr,"%lld, NEW Min-Throughput = %lld\n", simulation_time_elapsed, *throughput_min);
}

void calculate_turnaround_time(long long simulation_time_elapsed, long long turnaround_time, double *turnaround_av, long long num_processes_finished){

    fprintf(stderr,"%lld, TurnAround-time = %lld\n", simulation_time_elapsed, turnaround_time);
    double turnaround_total = *turnaround_av * (num_processes_finished-1);
    *turnaround_av = (turnaround_total + turnaround_time)/num_processes_finished;
    fprintf(stderr,"%lld, TurnAround-avg = %lf\n", simulation_time_elapsed, *turnaround_av);
}

void calculate_overhead(long long simulation_time_elapsed, long long turnaround_time, long long job_time, double *max_overhead, double *overhead_av, long long num_processes_finished){

    double overhead = (double)turnaround_time/(double)job_time;
    fprintf(stderr,"%lld, Overhead = %lf\n", simulation_time_elapsed, overhead);

    fprintf(stderr,"%lld, MAX-Overhead = %lf\n", simulation_time_elapsed, *max_overhead);

    if (overhead > *max_overhead){
        *max_overhead = overhead;
    }
    fprintf(stderr,"%lld, NEW MAX-Overhead = %lf\n", simulation_time_elapsed, *max_overhead);
    double overhead_total = *overhead_av * (num_processes_finished-1);
    *overhead_av = (overhead_total + overhead)/num_processes_finished;
    fprintf(stderr,"%lld, MAX-Overhead = %lf\n", simulation_time_elapsed, *overhead_av);
}