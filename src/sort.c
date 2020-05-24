
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.h"

void insertion_sort(data_t *array, int n) {

    int i, j;
    data_t next;

    for (i = 1; i < n; i++) {
        next = array[i];

        for (j = i-1; j >= 0 && array[j].process->pid > next.process->pid; j--) {
            // Move the element to the right.
            array[j + 1] = array[j];
        }
        array[j + 1] = next;

    }
}

void insertion_sort_evicted(int *array, int n) {

    int i, j;
    int next;

    for (i = 1; i < n; i++) {
        next = array[i];

        for (j = i-1; j >= 0 && array[j] > next; j--) {
            // Move the element to the right.
            array[j + 1] = array[j];
        }
        array[j + 1] = next;

    }
}