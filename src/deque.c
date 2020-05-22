
/* Deque Module
 * Written by Daniel Coleman 2019
 * Modified by Daniel Coleman 2020*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"

deque_t *new_deque() {
    deque_t *deque;
    deque = (deque_t*)malloc(sizeof(*deque));
    assert(deque!=NULL);
    deque->head = deque->foot = NULL;
    deque->size = 0;
    return deque;
}

/*
data_t *new_data() {
    data_t *data;
    data = (data_t*)malloc(sizeof(*data));
    assert(data!=NULL);
    data->process = NULL;
    return data;
}
 */

void free_deque(deque_t *deque) {
    node_t *curr, *prev;
    assert(deque!=NULL);
    curr = deque->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(deque);
}

void deque_push(deque_t *deque, data_t data) {
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    assert(deque!=NULL && new!=NULL);
    new->data = data;
    new->next = NULL;
    new->prev = NULL;
    if (deque->foot==NULL) { //if queue is empty
        deque->foot = new;
        deque->head = new;
    }
    else {
        deque->foot->next = new;
        new->prev = deque->foot;
        deque->foot = new;
    }
    deque->size++;
}

void deque_insert(deque_t *deque, data_t data) {
    node_t *new;
    new = (node_t *) malloc(sizeof(*new));
    assert(deque != NULL && new != NULL);
    new->data = data;
    new->next = NULL;
    new->prev = NULL;
    if (deque->foot == NULL) { // if queue is empty
        deque->head = new;
        deque->foot = new;
    } else {
        deque->head->prev = new;
        new->next = deque->head;
        deque->head = new;
    }
    deque->size++;
}

data_t deque_remove(deque_t *deque) {
    assert(deque!=NULL && deque->foot!=NULL);
    data_t new;

    // if issues look at this, maybe free it too if possible, but it's linked I think?
    new.process = deque->foot->data.process;
    deque->foot = deque->foot->prev;
    if(deque->foot == NULL){ // queue is now empty
        deque->head = NULL;
    }
    else{
        deque->foot->next = NULL;
    }
    deque->size--;
    return new;
}

data_t deque_pop(deque_t *deque) {
    assert(deque!=NULL && deque->head != NULL);
    data_t new;
    new.process = deque->head->data.process;
    deque->head = deque->head->next;
    if(deque->head == NULL){ // queue now empty
        deque->foot = NULL;
    }
    else{
        deque->head->prev = NULL;
    }
    deque->size--;
    return new;
}

int deque_size(deque_t *deque) {
    return deque->size;
}

void print_deque(deque_t *deque) {
    node_t *curr = deque->head;
    while(curr != NULL){
        printf("%d\t", curr->data.process->pid);
        curr = curr->next;
    }
    printf("\n");
}