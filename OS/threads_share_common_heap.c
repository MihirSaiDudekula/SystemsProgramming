#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>  // Include <stdlib.h> for malloc

int* x;  // Declare x as an int pointer

void *routine1() {
    (*x)++;  // Increment the value pointed to by x
    sleep(2);
    printf("\nValue of x = %d \n", *x);  // Print the dereferenced value of x
    return NULL;
}

void *routine2() {
    sleep(2);
    printf("\nValue of x = %d \n", *x);  // Print the dereferenced value of x
    return NULL;
}

int main() {
    x = (int*)malloc(sizeof(int));  // Allocate memory for x
    *x = 0;  // Initialize the value of x to 0

    pthread_t t1, t2;
    pthread_create(&t1, NULL, routine1, NULL);
    pthread_create(&t2, NULL, routine2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    free(x);  // Free the allocated memory for x after use
    return 0;
}
