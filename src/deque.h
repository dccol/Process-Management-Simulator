
/* Deque Module
 * Written by Daniel Coleman 2019*/

#ifndef DEQUE_H
#define DEQUE_H

#include "process.h"

typedef struct data {
    process_t *process;
} data_t;

typedef struct node node_t;

struct node{
    data_t data;
    node_t *next;
    node_t *prev;
};


typedef struct deque {
    int size;
    node_t *head;
    node_t *foot;
} deque_t;


deque_t *new_deque();

data_t *new_data();

void free_deque(deque_t *deque);

void deque_push(deque_t *deque, data_t data);

void deque_insert(deque_t *deque, data_t data);

data_t deque_pop(deque_t *deque);

data_t deque_remove(deque_t *deque);

int deque_size(deque_t *deque);

void print_deque(deque_t *deque);

#endif