#pragma once
typedef void * Context;

Context _setup_context(void *stack, void *addr);
Context _swap_context(Context);
void _enter_context(Context);

typedef struct thread *Thread;
typedef struct thread { 
  Context   ctxt;
  void     *stack;

  void (*func)(void *);
  void *  arg;

  Thread    next;
  Thread    prev;
} thread_t; 


void yield(void);
void start_threads();
void init_threads();
Thread create_thread(void (*func)(void*), void *arg, int stackSize);
