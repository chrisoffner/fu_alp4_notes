// simple example of the first attempt to solve the cigarette smoker`s problem

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 5
sem_t resource[3];
sem_t finished;
int tids[NUM_THREADS];
sem_t res_0_1;
sem_t res_1_2;
sem_t res_0_2;
sem_t table;
int res[3];

void *Service(void *threadid) {
  int i;
  int random_nr;

  for (i = 0; i < 1000; i++) {
    sem_wait(&finished);
    random_nr = rand();
    if (random_nr < RAND_MAX / 3) {
      printf("random_nr: %d -> 0,1 \n", random_nr);
      sem_post(&resource[0]);
      res[0]++;
      sem_post(&resource[1]);
      res[1]++;
    }
    if ((random_nr >= RAND_MAX / 3) && (random_nr < 2 * (RAND_MAX / 3))) {
      printf("random_nr: %d -> 0,2 \n", random_nr);
      sem_post(&resource[0]);
      res[0]++;
      sem_post(&resource[2]);
      res[2]++;
    }
    if (random_nr >= 2 * (RAND_MAX / 3)) {
      printf("random_nr: %d -> 1,2 \n", random_nr);
      sem_post(&resource[1]);
      res[1]++;
      sem_post(&resource[2]);
      res[2]++;
    }
    sem_post(&table);
  }
  pthread_exit(NULL);
}

void *Smoker(void *threadid) {
  int i;

  for (i = 0; i < 333; i++) {
    printf("Smoker %d waits\n", *((int *)threadid));
    if (*((int *)threadid) == 1) {
      //      sem_wait (&resource[0]); printf ("Smoker %d got 0 \n", *((int *)
      //      threadid)); sem_wait (&resource[1]); printf ("Smoker %d got 1 \n",
      //      *((int *) threadid));
      sem_wait(&res_0_1);
      printf("Smoker %d got 0 and 1 \n", *((int *)threadid));
    }
    if (*((int *)threadid) == 2) {
      //      sem_wait (&resource[1]); printf ("Smoker %d got 1 \n", *((int *)
      //      threadid)); sem_wait (&resource[2]); printf ("Smoker %d got 2 \n",
      //      *((int *) threadid));
      sem_wait(&res_1_2);
      printf("Smoker %d got 1 and 2 \n", *((int *)threadid));
    }
    if (*((int *)threadid) == 3) {
      //      sem_wait (&resource[0]); printf ("Smoker %d got 1 \n", *((int *)
      //      threadid)); sem_wait (&resource[2]); printf ("Smoker %d got 2 \n",
      //      *((int *) threadid));
      sem_wait(&res_0_2);
      printf("Smoker %d got 0 and 2 \n", *((int *)threadid));
    }

    printf("Smoker %d is smoking\n", *((int *)threadid));
    sem_post(&finished);
  }
  pthread_exit(NULL);
}

void *Help(void *threadid) {
  int i;

  for (i = 0; i < 1000; i++) {
    sem_wait(&table);
    if ((res[0] > 0) && (res[1] > 0)) {
      res[0]--;
      res[1]--;
      sem_post(&res_0_1);
    }
    if ((res[0] > 0) && (res[2] > 0)) {
      res[0]--;
      res[2]--;
      sem_post(&res_0_2);
    }
    if ((res[1] > 0) && (res[2] > 0)) {
      res[1]--;
      res[2]--;
      sem_post(&res_1_2);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  //  int rc;
  int i;

  sem_init(&finished, 0, 1); // all smokers finished
  sem_init(&resource[0], 0, 0);
  sem_init(&resource[1], 0, 0);
  sem_init(&resource[2], 0, 0);
  sem_init(&res_0_1, 0, 0);
  sem_init(&res_1_2, 0, 0);
  sem_init(&res_0_2, 0, 0);
  sem_init(&table, 0, 0);

  res[0] = res[1] = res[2] = 0;

  srand((unsigned)time(NULL));

  for (i = 0; i < NUM_THREADS; i++) {
    tids[i] = i;
    if (i == 0)
      pthread_create(&threads[i], NULL, Service, (void *)&tids[i]);
    if ((i > 0) && (i < NUM_THREADS - 1))
      pthread_create(&threads[i], NULL, Smoker, (void *)&tids[i]);
    if (i == NUM_THREADS - 1)
      pthread_create(&threads[i], NULL, Help, (void *)&tids[i]);
  }

  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&table);
  sem_destroy(&res_0_2);
  sem_destroy(&res_1_2);
  sem_destroy(&res_0_1);
  sem_destroy(&resource[0]);
  sem_destroy(&resource[1]);
  sem_destroy(&resource[2]);
  sem_destroy(&finished);

  pthread_exit(NULL);
  return 0;
}
