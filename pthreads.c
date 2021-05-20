#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *hello_world(void *threadid) {
  long tid = (long)threadid;
  printf("Hello World, this is thread %ld.\n", tid);
  pthread_exit(NULL);
}
int main(void) {
  pthread_t threads[NUM_THREADS];
  int rc;

  for (long t = 0; t < NUM_THREADS; ++t) {
    printf("Main creates thread %ld.\n", t);
    rc = pthread_create(&threads[t], NULL, hello_world, (void *)t);

    if (rc) {
      printf("ERROR: Return code %d!\n", rc);
      exit(-1);
    }
  }

  pthread_exit(NULL);
}