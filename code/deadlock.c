#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 2

void *PrintHello(void *threadid) {
  int tid = (long)threadid;
  printf("Hello World! It's me, thread #%d!\n", tid);
  pthread_exit(NULL);
}

int main(void) {
  pthread_t threads[NUM_THREADS];

  for (long t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    int rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }

  pthread_exit(NULL);
}
