

#ifndef SORT_H
#define SORT_H

#include "deque.h"

void insertion_sort_pending(data_t *array, int n);

void insertion_sort_evicted(int *array, int n);

void insertion_sort_queue(deque_t *process_queue, int n);
#endif