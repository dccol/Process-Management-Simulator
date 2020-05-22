
#include "statistics.h"

void calculate_throughput(int simulation_time_elapsed, double *throughput_av, int *throughput_min, int *throughput_max, int interval_throughput){
    int interval_num = simulation_time_elapsed/60;
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

void calculate_turnaround_time(){

}

void calculate_overhead(){

}