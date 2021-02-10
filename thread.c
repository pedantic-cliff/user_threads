#include <stdlib.h>
#include <assert.h> 
#include "thread.h"
#include "pool.h"

typedef void * Context;

typedef struct thread { 
  node_t  node;

  Context   ctxt;
  void     *stack;

  void (*func)(void *);
  void *  arg;

} thread_t; 

struct { 
  Pool p;
} threads;

thread_t *CurrentThread;
thread_t idle_thread;

Context _setup_context(void *stack, void *addr);
Context _swap_context(Context);
void _enter_context(Context);
int run_thread(Context ctxt);

void setup_thread(Thread t, int stackSize);

void _idle(void *ign) { 
  (void)ign;
  while(1) yield();
}

void init_threads() {
  threads.p = pool_init();
}

Thread create_thread(void (*func)(void*), void *arg, int stackSize) {
  Thread t = (Thread)malloc(sizeof(thread_t));
 
  t->func = func;
  t->arg  = arg;
  
  setup_thread(t, stackSize); 

  return t;
}


void setup_thread(Thread t, int stackSize) {
  void *stack = malloc(stackSize+0x80);
  t->ctxt = _setup_context(stack+stackSize+0x40, run_thread);
  t->stack = stack;
  t->ctxt = _swap_context(t->ctxt);
  
  pool_insert(threads.p, &t->node);
}

int run_thread(Context ctxt){ 
  ctxt =_swap_context(ctxt);
  CurrentThread->ctxt = ctxt;
  CurrentThread->func(CurrentThread->arg);
  pool_remove(threads.p, &CurrentThread->node);
  _swap_context(ctxt);
}

void wait_threads(void){
  CurrentThread = (Thread)pool_first(threads.p);
  while ( pool_count(threads.p) ) {
    CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);
    CurrentThread = (Thread) pool_next(threads.p, &CurrentThread->node);
  }
}

void yield(void) {
  CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);    
}


