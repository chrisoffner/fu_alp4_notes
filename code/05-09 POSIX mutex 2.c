// POSIX mutex with critical section split into two smaller sections

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 6

int account[NUM_THREADS];
pthread_mutex_t lock;

void *bank_action(void *threadid) {
  long tid;
  int i;
  int amount = 0;
  int acc_nr = 0;
  int _error = 0;

  tid = (long)threadid;
  printf("Hello World! It's me, thread #%ld !\n", tid);
  //  for (i = 0; i < 300000000; i++) {
  for (i = 0; i < 300000; i++) {
    acc_nr = (int)(((double)rand() / (RAND_MAX - 1)) * NUM_THREADS);
    amount = (int)(((double)rand() / (RAND_MAX - 1)) * 100);

    // try to enter the critical section
    _error = pthread_mutex_lock(&lock);
    // critical section
    account[tid] -= amount;
    // return from critical section
    _error = pthread_mutex_unlock(&lock);
    _error = pthread_mutex_lock(&lock);
    // critical section
    account[acc_nr] += amount;
    // return from critical section
    _error = pthread_mutex_unlock(&lock);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  int i;

  // testing
  scanf("%d", &i);

  // init lock
  pthread_mutex_init(&lock, NULL);

  // init data
  srand((unsigned)time(NULL));
  for (i = 0; i < NUM_THREADS; i++) {
    account[i] = 100;
  }

  for (t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, bank_action, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }

  // joining threads
  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  // output
  for (i = 0; i < NUM_THREADS; i++) {
    printf(" account_%d: %d \n", i, account[i]);
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
