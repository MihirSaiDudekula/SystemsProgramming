#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main() {
    int pipefd[2]; // Array to hold pipe ends
    pid_t pid;
    char write_msg[] = "Hello from parent process!";
    char read_msg[100];

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process
        close(pipefd[0]); // Close unused read end

        // Write to the pipe
        write(pipefd[1], write_msg, strlen(write_msg) + 1);
        close(pipefd[1]); // Close write end after writing
    } else { // Child process
        close(pipefd[1]); // Close unused write end

        // Read from the pipe
        read(pipefd[0], read_msg, sizeof(read_msg));
        printf("Child process read: %s\n", read_msg);
        close(pipefd[0]); // Close read end after reading
    }

    return 0;
}
