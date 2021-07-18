// Reader Writer with monitor

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 12
#define NUM_WRITERS 8
#define MAX_READERS 4

int read_count = NUM_THREADS - NUM_WRITERS;
int buffer = 0;

pthread_mutex_t mutex;
pthread_cond_t reader_cond;
pthread_cond_t writer_cond;

void *Writer(void *threadid) {
  int i, j;
  for (i = 0; i < 1000; i++) {
    pthread_mutex_lock(&mutex);
    while (read_count > 0) {
      printf("Writer %d wants to write...\n", (long)threadid);
      pthread_cond_wait(&writer_cond, &mutex);
    }
    buffer++;
    printf("Writer %d writes %d into buffer \n", (long)threadid, buffer);
    read_count = NUM_THREADS - NUM_WRITERS;
    pthread_cond_broadcast(&reader_cond);
    pthread_mutex_unlock(&mutex);
    sleep(rand() % 20);
  }
  pthread_exit(NULL);
}

void *Reader(void *threadid) {
  int last_seen = -1;
  while (1) {
    pthread_mutex_lock(&mutex);
    while (last_seen == buffer) {
      printf("Reader %d wants to read...\n", (long)threadid);
      pthread_cond_wait(&reader_cond, &mutex);
    }
    printf("Reader %d reads %d \n", (long)threadid, buffer);
    fflush(stdout);
    --read_count;
    last_seen = buffer;
    sleep(rand() % 10);
    if (read_count == 0) {
      pthread_cond_signal(&writer_cond);
    }
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;
  buffer = 0;

  srand(time(NULL));

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&reader_cond, NULL);
  pthread_cond_init(&writer_cond, NULL);

  for (t = 0; t < NUM_THREADS; t++) {
    printf("In main: creating thread %ld\n", t);
    if (t < NUM_WRITERS) {
      rc = pthread_create(&threads[t], NULL, Writer, (void *)t);
    } else {
      rc = pthread_create(&threads[t], NULL, Reader, (void *)t);
    }
    if (rc) {
      printf("ERROR; return code from pthread_create () is %d\n", rc);
      exit(-1);
    }
  }
  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&reader_cond);
  pthread_cond_destroy(&writer_cond);

  pthread_exit(NULL);
}
