// Reader Writer with monitor (by Alex Korzec)

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 12
#define NUM_WRITERS 3

int num_reader = 0;
int read_request = 0;
int num_writer = 0;
int write_request = 0;
int buffer = 0;

pthread_mutex_t mutex;
pthread_cond_t reader_cond;
pthread_cond_t writer_cond;

void writer_begin() {
  pthread_mutex_lock(&mutex);
  ++write_request;
  while (num_writer > 0 || num_reader > 0) {
    pthread_cond_wait(&writer_cond, &mutex);
  }
  --write_request;
  ++num_writer;
  pthread_mutex_unlock(&mutex);
}

void writer_end() {
  pthread_mutex_lock(&mutex);
  --num_writer;
  if (read_request > 0) {
    pthread_cond_signal(&reader_cond);
  } else {
    pthread_cond_signal(&writer_cond);
  }
  pthread_mutex_unlock(&mutex);
}

void reader_begin() {
  pthread_mutex_lock(&mutex);
  ++read_request;
  while (num_writer > 0 || write_request > 0) {
    pthread_cond_wait(&reader_cond, &mutex);
  }
  --read_request;
  ++num_reader;
  pthread_cond_signal(&reader_cond);
  pthread_mutex_unlock(&mutex);
}

void reader_end() {
  pthread_mutex_lock(&mutex);
  --num_reader;
  if (num_reader == 0) {
    pthread_cond_signal(&writer_cond);
  }
  pthread_mutex_unlock(&mutex);
}

void *Writer(void *threadid) {
  int i, j;
  for (i = 0; i < 1000; i++) {
    writer_begin();
    ++buffer;
    printf("Writer %d writes %d into buffer \n", (long)threadid, buffer);
    writer_end();
    sleep(rand() % 15);
  }
  pthread_exit(NULL);
}

void *Reader(void *threadid) {
  while (1) {
    reader_begin();
    printf("Reader %d reads %d \n", (long)threadid, buffer);
    fflush(stdout);
    reader_end();
    sleep(rand() % 7);
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
