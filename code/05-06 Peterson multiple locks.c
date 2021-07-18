// Multiple locks with mutual access – Peterson
// - Mutual exclusion ✅
// - Deadlock-free ✅
// - fair ✅
// - low overhead ✅

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4

int account[NUM_THREADS];
int level[NUM_THREADS];
int last[NUM_THREADS];

int lock(long tid) {
  for (int i = 1; i < NUM_THREADS; i++) {
    level[tid] = i;
    last[i] = tid;
    int loop = 1;
    while ((loop) && (last[i] == tid)) {
      loop = 0;
      int j = 0;
      while ((j < NUM_THREADS) && ((level[j] < i) || (j == tid)))
        j++;
      if (j < NUM_THREADS)
        loop = 1;
    }
  }
  return 0;
}

int unlock(long tid) {
  level[tid] = 0;
  return 0;
}

void *bank_action(void *threadid) {
  long tid;
  int i;
  int amount = 0;
  int acc_nr = 0;

  tid = (long)threadid;
  printf("Hello World! It's me, thread #%ld !\n", tid);
  //  for (i = 0; i < 300000000; i++) {
  for (i = 0; i < 3000; i++) {
    acc_nr = (int)(((double)rand() / (RAND_MAX - 1)) * NUM_THREADS);
    amount = (int)(((double)rand() / (RAND_MAX - 1)) * 100);

    // try to enter the critical section
    lock(tid);
    // critical section
    account[tid] -= amount;
    account[acc_nr] += amount;
    // return from critical section
    unlock(tid);
    //    printf ("thread %d, account_%d: %d, account_%d: %d \n", tid, tid,
    //    account[tid], acc_nr, account[acc_nr]);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  int i;
  int sum = 0;

  // testing
  scanf("%d", &i);

  // init data
  //  srand ((unsigned) time (NULL));
  if (argc > 1)
    srand(21234 * atoi(argv[1]));
  for (i = 0; i < NUM_THREADS; i++) {
    account[i] = 100;
    level[i] = 0;
    last[i] = 0;
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
    sum += account[i];
  }

  printf(" sum: %d\n", sum);
  fflush(stdout);

  /* Last thing that main() should do */
  pthread_exit(NULL);

  return 0;
}
