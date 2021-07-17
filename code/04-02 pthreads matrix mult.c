// simple matrix mult. with pthreads

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

int ma[3][3];
int mb[3][3];
int result[3][3];

void *matr_mult(void *threadid) {
  long tid;
  int i, j;

  tid = (long)threadid;
  printf("Hello World! It's me, thread #%ld !\n", tid);
  for (i = 0; i < 3; i++) {
    result[tid][i] = 0;
    for (j = 0; j < 3; j++) {
      result[tid][i] += ma[tid][j] * mb[j][i];
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  int i, j;

  // init data
  srand((unsigned)time(NULL));
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      ma[i][j] = (int)(((double)rand() / (RAND_MAX - 1)) * 100);
      printf("%d,%d: %d \n", i, j, ma[i][j]);
    }
  }

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      mb[i][j] = (int)(((double)rand() / (RAND_MAX - 1)) * 100);
      printf("%d,%d: %d \n", i, j, mb[i][j]);
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, matr_mult, (void *)t);
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
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      printf(" %d", result[i][j]);
    }
    printf("\n");
  }

  /* Last thing that main() should do */
  pthread_exit(NULL);
}
