#include <stdlib.h> 
#include "pool.h"


typedef struct pool { 
  node_t guard;
  int   count;
} pool_t; 

Pool pool_init(void) { 
  Pool p = (Pool) malloc( sizeof(pool_t) );
  p->guard.next = &p->guard;
  p->guard.prev = &p->guard;
  p->count = 0;
}


Node pool_first(Pool p) { 
  return p->guard.next;
}

void pool_insert(Pool p, Node n) {
  p->count++;
  n->next = &p->guard;
  n->prev = p->guard.prev;
  n->prev->next = n;
  p->guard.prev = n; 
}

void pool_remove(Pool p, Node n) { 
  n->prev->next = n->next;
  n->next->prev = n->prev;
  n->next = n;
  n->prev = n;
  p->count--;
} 

int pool_count(Pool p) {
  return p->count;
}

Node pool_next(Pool p, Node n) {
  Node next = n->next;
  if (next == &p->guard)
    next = p->guard.next;
  return next;
}

Node pool_prev(Pool p, Node n) {
  Node prev = n->prev;
  if (prev == &p->guard)
    prev = p->guard.prev;
  return prev;
}
