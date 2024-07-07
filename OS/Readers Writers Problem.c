#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared data structures
sem_t mutex; // Semaphore for mutual exclusion when updating readcount
sem_t wrt;   // Semaphore for writers and first/last reader
int readcount = 0; // Number of readers currently in the critical section

// Function declarations
void* writer(void* arg);
void* reader(void* arg);

int main() {
    pthread_t r1, r2, r3, w1, w2; // Reader and Writer threads

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    // Create reader and writer threads
    pthread_create(&r1, NULL, reader, (void*)1);
    pthread_create(&r2, NULL, reader, (void*)2);
    pthread_create(&r3, NULL, reader, (void*)3);
    pthread_create(&w1, NULL, writer, (void*)1);
    pthread_create(&w2, NULL, writer, (void*)2);

    // Join threads
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}

// Writer function
void* writer(void* arg) {
    int id = (int)arg;
    while (1) {
        sem_wait(&wrt); // Wait to enter critical section

        // Writing is performed
        printf("Writer %d is writing\n", id);
        sleep(1); // Simulate writing time

        sem_post(&wrt); // Signal to leave critical section

        sleep(1); // Simulate time outside critical section
    }
}

// Reader function
void* reader(void* arg) {
    int id = (int)arg;
    while (1) {
        sem_wait(&mutex); // Wait to enter critical section for updating readcount
        readcount++; // Increment readcount

        if (readcount == 1) // If this is the first reader
            sem_wait(&wrt); // Wait to enter critical section

        sem_post(&mutex); // Signal to leave critical section for updating readcount

        // Reading is performed
        printf("Reader %d is reading\n", id);
        sleep(1); // Simulate reading time

        sem_wait(&mutex); // Wait to enter critical section for updating readcount
        readcount--; // Decrement readcount

        if (readcount == 0) // If this is the last reader
            sem_post(&wrt); // Signal to leave critical section

        sem_post(&mutex); // Signal to leave critical section for updating readcount

        sleep(1); // Simulate time outside critical section
    }
}
