#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

volatile bool lock = false;

bool test_and_set(bool *target) {
    bool rv = *target;   // Read the current value of `target` (lock)
    *target = true;      // Set `target` (lock) to TRUE
    return rv;           // Return the previous value of `target` (lock)
}

void* thread_function(void* arg) {
    while (true) {
        while (test_and_set(&lock))
            ; /* do nothing */

        // Critical section
        printf("Thread %ld in critical section\n", (long)arg);
        sleep(1);

        lock = false;

        // Remainder section
        printf("Thread %ld in remainder section\n", (long)arg);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t threads[2];

    // Create two threads
    for (int i = 0; i < 2; ++i) {
        pthread_create(&threads[i], NULL, thread_function, (void*)(long)i);
    }

    // Join threads
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
