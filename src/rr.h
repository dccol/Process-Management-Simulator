
#ifndef RR_H
#define RR_H

#include "deque.h"

void rr(deque_t *process_queue, int quantum);

int run_process_rr(process_t *process, int quantum);

#endif