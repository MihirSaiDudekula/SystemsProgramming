#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Number of philosophers

sem_t chopstick[N]; // Semaphore array for chopsticks

void* philosopher(void* num);
void pick_chopsticks(int philosopher_number);
void put_chopsticks(int philosopher_number);
void think(int philosopher_number);
void eat(int philosopher_number);

int main() {
    pthread_t philosophers[N];
    int philosopher_numbers[N];

    // Initialize semaphores for chopsticks
    for (int i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    // Wait for philosopher threads to complete (they won't in this infinite loop example)
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopstick[i]);
    }

    return 0;
}

void* philosopher(void* num) {
    int philosopher_number = *(int*)num;
    while (1) {
        think(philosopher_number);
        pick_chopsticks(philosopher_number);
        eat(philosopher_number);
        put_chopsticks(philosopher_number);
    }
}

void pick_chopsticks(int philosopher_number) {
    if (philosopher_number % 2 == 0) {
        // Even philosopher
        sem_wait(&chopstick[(philosopher_number + 1) % N]);
        sem_wait(&chopstick[philosopher_number]);
    } else {
        // Odd philosopher
        sem_wait(&chopstick[philosopher_number]);
        sem_wait(&chopstick[(philosopher_number + 1) % N]);
    }
}

void put_chopsticks(int philosopher_number) {
    sem_post(&chopstick[philosopher_number]);
    sem_post(&chopstick[(philosopher_number + 1) % N]);
}

void think(int philosopher_number) {
    printf("Philosopher %d is thinking.\n", philosopher_number);
    sleep(rand() % 3 + 1); // Random thinking time
}

void eat(int philosopher_number) {
    printf("Philosopher %d is eating.\n", philosopher_number);
    sleep(rand() % 3 + 1); // Random eating time
}
