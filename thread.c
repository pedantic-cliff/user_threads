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

  int   isDone;

} thread_t; 

struct { 
  Pool p;
} threads = {NULL};

thread_t *CurrentThread;
thread_t idle_thread;

extern Context _setup_context(void *stack, void *addr);
extern Context _swap_context(Context);
extern Context _enter_context(Context);

static int run_thread(Context ctxt);
static void setup_thread(Thread t, int stackSize);

void init_threads() {
  threads.p = pool_init();
}

Thread create_thread(void (*func)(void*), void *arg, int stackSize) {
  if (threads.p == NULL) {
    init_threads();
  }

  Thread t = (Thread)malloc(sizeof(thread_t));
 
  t->func = func;
  t->arg  = arg;
  t->isDone = 0;
  
  setup_thread(t, stackSize); 

  return t;
}

void wait_threads(void){
  Thread next; 
  CurrentThread = (Thread)pool_first(threads.p);
  while ( pool_count(threads.p) ) {
    CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);
    
    next = (Thread) pool_next(threads.p, &CurrentThread->node);
    if (CurrentThread->isDone) {
      pool_remove(threads.p, &CurrentThread->node);
      free(CurrentThread->stack);
      free(CurrentThread);
    }
    CurrentThread = next;
  }
  free(threads.p);
  threads.p = NULL;
}

void yield(void) {
  CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);    
}


static void setup_thread(Thread t, int stackSize) {
  void *stack = malloc(stackSize+0x80);
  t->ctxt = _setup_context(stack+stackSize+0x80, run_thread);
  t->stack = stack;
  t->ctxt = _swap_context(t->ctxt);
  
  pool_insert(threads.p, &t->node);
}

static int run_thread(Context ctxt){ 
  ctxt =_swap_context(ctxt);
  CurrentThread->ctxt = ctxt;
  CurrentThread->func(CurrentThread->arg);
  
  CurrentThread->isDone = 1;
  yield();
}


