// Twofold lock with priorities and reservation
// - Mutual exclusion ✅
// - Deadlock-free ✅
// - not fair because lower tid consistently gets preference ❌
// - some overhead by active waiting ❌

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

int account[2];
int prio[2];
char interested[2];

int lock(long tid) {
  interested[tid] = 1;
  prio[tid] = prio[NUM_THREADS - 1 - tid] + 1;
  interested[tid] = 0;
  while (interested[NUM_THREADS - 1 - tid])
    ;
  while ((prio[NUM_THREADS - 1 - tid] != 0) &&
         ((prio[NUM_THREADS - 1 - tid] < prio[tid]) ||
          ((prio[NUM_THREADS - 1 - tid] == prio[tid]) &&
           ((NUM_THREADS - 1 - tid) < tid))))
    ; // not fair because lower tid gets preference
  return 0;
}

int unlock(long tid) {
  prio[tid] = 0;
  return 0;
}

void *bank_action(void *threadid) {
  long tid = (long)threadid;
  int amount = 0;

  printf("Hello World! It's me, thread #%ld !\n", tid);
  //  for (i = 0; i < 300000000; i++) {
  for (int i = 0; i < 300000; i++) {
    amount = (int)(((double)rand() / (RAND_MAX - 1)) * 100);

    // try to enter the critical section
    lock(tid);
    // critical section
    account[tid] -= amount;
    account[NUM_THREADS - 1 - tid] += amount;
    // return from critical section
    unlock(tid);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];

  // init data
  srand((unsigned)time(NULL));
  account[0] = account[1] = 100;
  prio[0] = prio[1] = 0;
  interested[0] = interested[1] = 0;

  for (long t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    int rc = pthread_create(&threads[t], NULL, bank_action, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }

  // join threads
  for (long t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  // output
  for (int i = 0; i < NUM_THREADS; i++) {
    printf(" account_%d: %d \n", i, account[i]);
  }

  // last thing that main() should do
  pthread_exit(NULL);
}
