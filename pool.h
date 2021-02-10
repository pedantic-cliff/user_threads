#pragma once 

typedef struct pool *Pool;
typedef struct node *Node;

typedef struct node { 
  Node next;
  Node prev;
} node_t;


Pool pool_init(void);
Node pool_first(Pool p);
void pool_insert(Pool p, Node n);
void pool_remove(Pool p, Node n); 
Node pool_next(Pool p, Node n);
Node pool_prev(Pool p, Node n);
int pool_count(Pool p);
