#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to print current process' ID and its parent's process ID
void printProcessInfo(const char *process_type) {
    printf("%s process: PID = %d, Parent PID = %d\n", process_type, getpid(), getppid());
}

int main(int argc, char *argv[]) {
    // Fork a new process
    pid_t pid = fork();

    if (pid == -1) {
        // Handle error
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {
        // Child process
        printProcessInfo("Child");

        // Check if arguments are provided for the command
        // if (argc < 2) {
        //     fprintf(stderr, "Usage: %s <command> [<arg1> <arg2> ...]\n", argv[0]);
        //     exit(EXIT_FAILURE);
        // }

        // // Execute the command specified by the user
        // execvp(argv[1], &argv[1]);

        // Define arguments and environment variables for the ls command
        char *argv[] = { "ls", "-l", NULL };
        char *envp[] = { NULL };

        // Execute the ls command in the child process
        execve("/bin/ls", argv, envp);

        // execve never returns here if it executes successfully
        // If execve returns, it must have failed
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printProcessInfo("Parent");

        int status;
        
        // Wait for the child process to terminate
        pid_t child_pid = waitpid(pid, &status, 0);

        // while pid is the process id of the child process ; &status is a pointer to an integer where the exit status of the child process will be stored after waitpid() returns. The exit status provides information about how the child process terminated (normally or abnormally).

        if (child_pid == -1) {
            // waitpid failed
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        // Check if child process exited normally
        if (WIFEXITED(status)) {
            // Print child process's exit status
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else {
            // Child process did not exit normally
            printf("Child did not exit normally\n");
        }
    }

    return 0;
}
