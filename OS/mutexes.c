#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
int sharedvar = 0;

void *thread_function(void *arg) {
    // Critical section
    pthread_mutex_lock(&mutex);
    // Operations on shared resource
    // Example: Incrementing a shared variable
    sharedvar++;
    printf("\nValue of shared variable = %d \n",sharedvar);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);

    // Create threads and perform operations
    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}