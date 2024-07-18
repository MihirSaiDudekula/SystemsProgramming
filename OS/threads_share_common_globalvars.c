#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int x =2; //global var

void *routine1() {
    x++;
    sleep(2);
    printf("\nValue of x = %d \n",x);
    return NULL;
}

void *routine2() {
    //increment done in routine 1 is displayed here
    sleep(2);
    printf("\nValue of x = %d \n",x);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, routine1, NULL);
    pthread_create(&thread2, NULL, routine2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}

