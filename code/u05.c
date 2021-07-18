// simple producer consumer example with semaphores 

#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

#define NUM_PLACES 10
#define NUM_CONSUMER 5
#define NUM_PRODUCER 4
#define N 2

sem_t empty;
sem_t mutex;

int elementCounter = 0;

struct Node {
    int content;
    struct Node* next;    
};

struct Node* first;

void printBuffer() {
    if(first->content == -1) {
        printf("EMPTY BUFFER\n");
    } else {
        printf("[ ");
        struct Node* current = first;
        while(current->content != -1) {
            printf("%d ", current->content);
            current = current->next;
        }
        printf("]\n");
    }
}

void* Producer (void* threadid) {
    int* tid = (int*) threadid;
    for(int i = 0; i < NUM_CONSUMER * N; i++) {
        sem_wait(&mutex);
        struct Node* newNode = malloc(sizeof(struct Node));
        newNode->content = elementCounter;
        newNode->next = first;
        first = newNode;
        printf("Producer %d puts %d into buffer\n", *tid, elementCounter); fflush (stdout);
        elementCounter++;
        printBuffer();
        sem_post(&mutex); 
        sem_post(&empty); 
    }
    free(tid);
    pthread_exit (NULL);
} 


void* Consumer (void* threadid) {
    int* tid = (int*) threadid;
    for(int i = 0; i < NUM_PRODUCER * N; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        printf ("Consumer %d takes %d \n", *tid, first->content); fflush (stdout);
        struct Node* oldNode = first;
        first = first->next;
        free(oldNode);
        printBuffer();
        sem_post(&mutex);
    }
    free(tid);
    pthread_exit (NULL);
}

int main (int argc, char *argv[]) { 
    pthread_t threads[NUM_PRODUCER + NUM_CONSUMER];
    int rc;
 
    sem_init(&empty, 0, 0); // amount data in buffer
    sem_init(&mutex, 0, 1); // critical section to deal with buffer
    
    first = malloc(sizeof(struct Node));
    first->content = -1;
    
    for(int i = 0; i < NUM_PRODUCER + NUM_CONSUMER; i++) {
        printf ("In main: creating thread %d\n", i);
        int *tid = malloc(sizeof(*tid));
        *tid = i;
        if (i < NUM_PRODUCER) {
            rc = pthread_create (&threads[i], NULL, Producer, tid);
        } else {
            rc = pthread_create (&threads[i], NULL, Consumer, tid);
        }

        if (rc) {
            printf ("ERROR; return code from pthread_create () is %d\n", rc);
            exit(-1);
        }
    }

    for(int i = 0; i < NUM_PRODUCER + NUM_CONSUMER; i++) {
        pthread_join (threads[i], NULL);
    }
    
    free(first);

    sem_destroy(&mutex);
    sem_destroy(&empty);
  
    pthread_exit(NULL);
} 

