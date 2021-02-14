#include <stdlib.h>
#include <assert.h> 
#include <stdio.h>
#include "thread.h"
#include "pool.h"

typedef struct { 
  node_t n;
  int x; 
} test_t; 


int test_pool(void) { 
  test_t t[8];
  Pool p = pool_init();
  for (int ii = 0; ii < 8; ii++) {
    t[ii].x = ii;
    pool_insert(p, &t[ii].n);
  }
  for (int ii = 0; ii < 8; ii++) { 
    test_t *next = (test_t*)pool_next(p, &t[ii].n) ;
    assert( next->x == (ii+1) % 8);
  } 
  for (int ii = 0; ii < 8; ii+=2) {
    pool_remove(p, &t[ii].n);
  }
  for (int ii = 1; ii < 8; ii+=2) { 
    test_t *next = (test_t*)pool_next(p, &t[ii].n) ;
    assert( next->x == (ii+2) % 8);
  }
}


int count1 = 0;
int count2 = 0;

void foobar(void* x) { 
  long ii = (long)x;
  while(ii--) { 
    count1++;
    yield();
  }
  return;
}
void barbaz(void* x) { 
  long ii = (long)x;
  while(ii--) {
    count2++;
    yield();
  }
  return;
}

int test_threads(void){
  Thread t2 = create_thread(&barbaz, (void*)25, 0x100);
  Thread t1 = create_thread(&foobar, (void*)25, 0x100);
  wait_threads();
  printf("%d %d\n", count1, count2);
  assert(count1 == count2);
  return 0;
}

void main(void) { 
  //test_pool();
  test_threads();
} 
