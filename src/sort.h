

#ifndef SORT_H
#define SORT_H

#include "deque.h"

void insertion_sort_pending(data_t *array, long long n);

void insertion_sort_evicted(long long *array, long long n);

void insertion_sort_queue(deque_t *process_queue, long long n);
#endif