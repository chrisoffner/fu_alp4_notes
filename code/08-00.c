struct semaphore {
  int count; // thread counter
  Queue *wt; // count>=1: free, count<=0: occupied
} 						// if count<0 : |count| is the 	
						// number of waiting threads
void init (semaphore *s, int i) {
  s->count = i; // set i=1 for mutual exclusion
  s->wt = NULL;
}
void P(semaphore *s) {
  s->count--;
  if (s->count < 0)
    block(s->wt); // enqueue thread
}
void V(semaphore *s) {
  s->count++;
  if (s->count <= 0)
    deblock(s->wt); // deblock first of queue
}
