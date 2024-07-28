#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10

// Shared buffer and synchronization variables
int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;
sem_t empty_slots;
sem_t full_slots;

// Producer thread function
void* producer(void* arg) {
    while (1) {
        // Generate a random number
        int x = rand() % 100;

        // Wait for an empty slot
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);

        // Add to buffer
        buffer[count++] = x;
        printf("Produced: %d\n", x);

        pthread_mutex_unlock(&mutex);
        sem_post(&full_slots);

        // Simulate production delay
        sleep(1);
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    while (1) {
        // Wait for a full slot
        sem_wait(&full_slots);
        pthread_mutex_lock(&mutex);

        // Remove from buffer
        int y = buffer[--count];
        printf("Consumed: %d\n", y);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);

        // Simulate consumption delay
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish (they actually run indefinitely in this example)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);

    return 0;
}


// ### Explanation

// 1. **Global Variables**
//    - `buffer[BUFFER_SIZE]`: The shared buffer with a fixed size.
//    - `count`: Tracks the number of items in the buffer.

// 2. **Synchronization**
//    - `pthread_mutex_t mutex`: Ensures mutual exclusion when accessing the buffer.
//    - `sem_t empty_slots`: Semaphore to track the number of empty slots in the buffer.
//    - `sem_t full_slots`: Semaphore to track the number of full slots in the buffer.

// 3. **Producer Function**
//    - Generates a random number.
//    - Uses `sem_wait` to wait for an empty slot.
//    - Locks the mutex to safely add the number to the buffer.
//    - Unlocks the mutex and posts to `full_slots` to indicate a new item is available.
//    - Simulates a delay to represent production time.

// 4. **Consumer Function**
//    - Uses `sem_wait` to wait for a full slot.
//    - Locks the mutex to safely remove an item from the buffer.
//    - Unlocks the mutex and posts to `empty_slots` to indicate a slot has been freed.
//    - Simulates a delay to represent consumption time.

// 5. **Main Function**
//    - Initializes the mutex and semaphores.
//    - Creates producer and consumer threads.
//    - Joins the threads (though in this infinite loop example, it will not terminate).
//    - Cleans up by destroying the mutex and semaphores.

// This implementation ensures that the producer and consumer operate correctly with respect to the shared buffer, avoiding race conditions and handling full/empty buffer scenarios efficiently.