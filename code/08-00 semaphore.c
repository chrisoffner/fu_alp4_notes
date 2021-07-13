struct semaphore {
  int count;    // count of available resources
  Queue *queue; // count >= 1: free, count <= 0: occupied
};              // if count < 0: |count| is the number of waiting threads

void init(struct semaphore *s, int i) {
  s->count = i;    // set i = 1 for mutual exclusion
  s->queue = NULL; // start with empty queue
}
void wait(struct semaphore *s) {
  s->count -= 1; // decrement count of available resources
  if (s->count < 0) {
    block(s->queue); // enqueue thread
  }
}
void signal(struct semaphore *s) {
  s->count += 1; // increment count of available resources
  if (s->count <= 0) {
    unblock(s->queue); // unblock first of queue
  }
}