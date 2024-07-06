#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITERATIONS 10

volatile bool flag[2];
volatile int turn;

void* process_0(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        flag[0] = true;
        turn = 1;
        while (flag[1] && turn == 1);
        
        // Critical section
        printf("Process 0 in critical section\n");
        sleep(1);
        
        flag[0] = false;
        
        // Remainder section
        printf("Process 0 in remainder section\n");
        sleep(1);
    }
    return NULL;
}

void* process_1(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        flag[1] = true;
        turn = 0;
        while (flag[0] && turn == 0);
        
        // Critical section
        printf("Process 1 in critical section\n");
        sleep(1);
        
        flag[1] = false;
        
        // Remainder section
        printf("Process 1 in remainder section\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread0, thread1;

    flag[0] = flag[1] = false;
    turn = 0;

    pthread_create(&thread0, NULL, process_0, NULL);
    pthread_create(&thread1, NULL, process_1, NULL);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);

    return 0;
}
