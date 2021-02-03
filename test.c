#include <stdlib.h>
#include <assert.h> 
#include "test.h"

volatile thread_t *volatile CurrentThread; 
thread_t idle_thread;
void _idle(void *ign) { 
  (void)ign;
  while(1) yield();
}

int start_thread(Context ctxt){ 
  ctxt =_swap_context(ctxt);
  CurrentThread->ctxt = ctxt;
  CurrentThread->func(CurrentThread->arg);
}

void setup_thread(Thread t, int stackSize) {
  void *stack = malloc(stackSize+0x80);
  t->ctxt = _setup_context(stack+stackSize+0x40, start_thread);
  t->stack = stack;
  t->ctxt = _swap_context(t->ctxt);
  t->next = idle_thread.next;
  t->prev = &idle_thread;
  t->next->prev = t;
  t->prev->next = t;
}

void wait_threads(){
  CurrentThread = idle_thread.next;
  CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);
}

void yield(void) {
  CurrentThread->ctxt = _swap_context(CurrentThread->ctxt);    
}

void init_threads() {
  // Build Idle Thread
  idle_thread.next = &idle_thread;
  idle_thread.prev = &idle_thread;
  idle_thread.func = (void*) &_idle;
  idle_thread.arg  = NULL;
  setup_thread(&idle_thread, 0x40);
}


Thread create_thread(void (*func)(void*), void *arg, int stackSize) {
  Thread t = (Thread)malloc(sizeof(thread_t));
 
  t->func = func;
  t->arg  = arg;
  
  setup_thread(t, stackSize); 

  return t;
}


void foobar(void* x) { 
  while(1) yield();
  return;
}

int main(void){
  init_threads();
  Thread t = create_thread(&foobar, NULL, 0x100);
  wait_threads();
  while(1) yield();
  return 0;
}

