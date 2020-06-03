

#ifndef SORT_H
#define SORT_H

#include "deque.h"

/**
 * Sorts incoming processes by pid
 * @param array
 * @param n
 */
void insertion_sort_pending(data_t *array, long long n);

/**
 * Sorts eviction page addresses
 * @param array
 * @param n
 */
void insertion_sort_evicted(long long *array, long long n);

/**
 * Sorts process queue by shortest to longest execution time
 * @param process_queue
 * @param n
 */
void insertion_sort_queue(deque_t *process_queue, long long n);
#endif