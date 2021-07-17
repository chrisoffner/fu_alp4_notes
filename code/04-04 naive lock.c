// simple accounting with pthreads

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

int account[2];
char lock = 0;

void *bank_action(void *threadid) {
  int amount = 0;
  long tid = (long)threadid;

  printf("Hello World! It's me, thread #%ld !\n", tid);
  for (int i = 0; i < 300000; i++) {
    amount = (int)(((double)rand() / (RAND_MAX - 1)) * 100);

    // try to enter the critical section
    while (lock)
      ;
    lock = 1;
    // critical section
    account[tid] -= amount;
    account[NUM_THREADS - 1 - tid] += amount;
    // return from critical section
    lock = 0;
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  // init data
  srand((unsigned)time(NULL));
  account[0] = account[1] = 100;

  for (long t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    int rc = pthread_create(&threads[t], NULL, bank_action, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }

  // joining threads
  for (long t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  // output
  for (int i = 0; i < NUM_THREADS; i++) {
    printf(" account_%d: %d \n", i, account[i]);
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
