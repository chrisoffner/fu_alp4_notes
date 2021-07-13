// C program to demonstrate working of Semaphores
// https://www.geeksforgeeks.org/use-posix-semaphores-c/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t mutex;

void *thread(void *arg) {
  sem_wait(&mutex); // wait
  printf("\nEntered..\n");

  sleep(4); // critical section

  printf("\nJust Exiting...\n");
  sem_post(&mutex); // signal
}

int main() {
  pthread_t t1, t2;

  sem_init(&mutex, 0, 1);
  pthread_create(&t1, NULL, thread, NULL);
  sleep(2);
  pthread_create(&t2, NULL, thread, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  sem_destroy(&mutex);
  return 0;
}
