// dining philosophers with shared chopsticks

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5
sem_t mutex;

void *Philosopher(void *threadid) {
  int i;
  for (i = 0; i < 1000; i++) {
    // thinking
    sleep(2);

    // wait
    sem_wait(&mutex);
    printf("\n %d Dining..\n", (long)threadid);
    sleep(4);
    printf("\n %d Finished..\n", (long)threadid);
    sem_post(&mutex);
  }
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  sem_init(&mutex, 0, 2); // amount of philosopers dining concurrently

  for (t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, Philosopher, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  sem_destroy(&mutex);
  pthread_exit(NULL);
  return 0;
}
