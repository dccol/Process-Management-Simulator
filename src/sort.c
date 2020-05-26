
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "sort.h"

void insertion_sort_pending(data_t *array, int n) {

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

void insertion_sort_queue(deque_t *process_queue, int n) {

    // TAKE ALL THE PROCESSES AND STORE THEM IN AN ARRAY
    //process_t *process = process_queue->foot->data.process;
    data_t *array = (data_t*)malloc(sizeof(*array) * 10);

    int index = 0;
    while(process_queue->foot != NULL){
        data_t data = deque_remove(process_queue);
        array[index] = data;
        index++;
    }

    // SORT ARRAY BASED ON TIME REMAINING
    int i, j;
    data_t next;

    for (i = 1; i < n; i++) {
        next = array[i];

        for (j = i-1; j >= 0 && array[j].process->time_remaining > next.process->time_remaining; j--) {
            // Move the element to the right.
            array[j + 1] = array[j];
        }
        array[j + 1] = next;

    }

    // REINSERT PROCESSES IN NEW ORDER
    for (i = 0; i < index; i++) {

        deque_insert(process_queue, array[i]);
    }
    // MEMORY LEAK
    free(array);
}