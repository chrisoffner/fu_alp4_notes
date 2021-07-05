// LAMPORT'S BAKERY

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 6

int account[NUM_THREADS];
char enter[NUM_THREADS]; // expresses thread's desire to enter
int tickets[NUM_THREADS];

int lock(long tid) {
  int max = 0;
  enter[tid] = 1; // declare ticket pulling phase

  // sets max to the highest ticket number
  for (int i = 0; i < NUM_THREADS; i++) {
    if (max < tickets[i]) {
      max = tickets[i];
    }
  }
  tickets[tid] = max + 1; // take the next available ticket
  enter[tid] = 0;         // finished pulling ticket

  for (int i = 0; i < NUM_THREADS; i++) {
    if (i != tid) {
      // wait if thread i is currently pulling ticket
      while (enter[i])
        ;
      // wait if thread i has a lower ticket number than us or
      // wait if thread i has the same ticket number but a lower thread id
      while ((tickets[i] != 0) && ((tickets[tid] > tickets[i]) ||
                                   ((tickets[tid] == tickets[i]) && (tid > i))))
        ;
    }
  }
  return 0;
}

int unlock(long tid) {
  tickets[tid] = 0;
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
  for (i = 0; i < 300000; i++) {
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

  // testing
  scanf("%d", &i);

  // init data
  srand((unsigned)time(NULL));
  for (i = 0; i < NUM_THREADS; i++) {
    account[i] = 100;
    enter[i] = 0;
    tickets[i] = 0;
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
