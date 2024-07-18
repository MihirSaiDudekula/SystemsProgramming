#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h> 

int X = 0; // Global variable for answer checking

void handle_sigusr1(int signum) {
    if (X == 0) {
        printf("\nHint: Remember that multiplication is repetitive addition!\n\n");
    }
}

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        sleep(5); // Wait for 5 seconds
        kill(getppid(), SIGUSR1); 
        // Send SIGUSR1 to parent
        // The kill() function in Unix-like operating systems is primarily used to send signals to processes. - not necessarily kill them
        // // it has 2 parameters - pid: Specifies the process ID of the process to which the signal should be sent. sig: Specifies the signal to send.
        // kill() is used in scenarios where a process wants to notify another process about an event or request an action through signal delivery.
        // sigaction() is used to specify how a process should react when it receives a specific signal.
        exit(0);
    } else {
        struct sigaction sa;
        sa.sa_handler = handle_sigusr1;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL); 
        // Parent process
        printf("What is the result of 3 times 5?\n");
        scanf("%d", &X);

        if (X == 15) {
            printf("Right!\n");
        } else {
            printf("Wrong!\n");
        }

        // Wait for child process to finish
        wait(NULL);

    }

    return 0;
}
