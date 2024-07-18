#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global array of prime numbers
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// This is the function that each thread will execute
void *routine_function(void *arg) {
    //pthread create always expects the routine parameters to be void pointers and not that of any datatype, so we need to work with it
    // Cast the argument back to an integer pointer
    int index = *((int *)arg);
    
    // Print the prime number corresponding to the index
    printf("%d ", primes[index]);
    
    // Free the memory allocated to the argument (not strictly necessary here)
    free(arg);
    
    // Return NULL to conform to pthread function signature
    return NULL;
}

int main() {
    pthread_t th[10]; // Array to hold thread IDs

    // Create 10 threads
    for (int i = 0; i < 10; ++i) {
        // Allocate memory for an integer
        int *a = malloc(sizeof(int));
        
        // Store the current loop index `i` in the allocated memory
        *a = i;

        // If a were a local variable declared within the loop (int a;), its memory would be allocated on the stack. Accessing such a variable after the loop has finished could lead to undefined behavior since the variable's lifetime would end once the loop iteration completes. By allocating memory on the heap (malloc), we ensure that the memory persists until explicitly freed (free(arg);) even after the loop exits.

        // In a multithreaded environment, if all threads were to receive a pointer to the same stack variable a, they would all potentially see the same value of i as it changes in subsequent iterations of the loop. This could lead to data races and unpredictability in the program's behavior. By allocating memory for each a separately (malloc), each thread receives its own independent copy of i.
        
        // Create a new thread, passing the routine_function and the argument `a`
        if (pthread_create(&th[i], NULL, routine_function, a) != 0) {
            perror("Failed to create thread");
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 10; ++i) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    return 0;
}
