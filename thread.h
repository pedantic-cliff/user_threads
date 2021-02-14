#pragma once

typedef struct thread *Thread;

void init_threads();

Thread create_thread(void (*func)(void*), void *arg, int stackSize);

void yield(void);

void wait_threads(void);
