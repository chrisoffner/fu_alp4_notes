int currentSum = 0;

pthread_mutex_t mutex;
pthread_cond_t condition_var;

void begin_read(int tid) {
    pthread_mutex_lock(&mutex);
    while(currentSum + tid > N) {
        pthread_cond_wait(&condition_var, &mutex);
    }
    currentSum += tid;
    pthread_mutex_unlock(&mutex);
}

void end_read(int tid) {
    pthread_mutex_lock(&mutex);
    currentSum -= tid;
    pthread_cond_signal(&condition_var);
    pthread_mutex_unlock(&mutex);
}

void *readData(void *t) {
    //...
    for(...) {
        begin_read(tid);
        read(file, tid);
        end_read(tid);
    }
    pthread_exit(NULL);
}