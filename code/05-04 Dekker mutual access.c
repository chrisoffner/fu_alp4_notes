// Twofold lock with Mutual Access – Dekker
// - Mutual exclusion ✅
// - Deadlock-free ✅
// - fair ✅
// - low overhead ✅

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 2

int account[2];
char _lock[2];
int favoured;

int lock(long tid) {
  _lock[tid] = 1;
  while (_lock[NUM_THREADS - 1 - tid]) {
    if (favoured != tid) {
      _lock[tid] = 0;
      while (favoured != tid)
        ;
      _lock[tid] = 1;
    }
  }
  return 0;
}

int unlock(long tid) {
  favoured = NUM_THREADS - 1 - tid;
  _lock[tid] = 0;
  return 0;
}

void *bank_action(void *threadid) {
  long tid = (long)threadid;
  int amount = 0;

  printf("Hello World! It's me, thread #%ld !\n", tid);
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
  _lock[0] = _lock[1] = 0;
  favoured = 1;

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
  for (long i = 0; i < NUM_THREADS; i++) {
    printf(" account_%d: %d \n", i, account[i]);
  }

  // last thing that main() should do
  pthread_exit(NULL);
}
