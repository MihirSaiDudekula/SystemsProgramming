#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        int error = execl("/bin/ping", "ping", "google.com", NULL);
        if (error == -1) {
            printf("Could not find program to execute.\n");
            return 0;
        }
    } else if (pid > 0) {
        // Parent process
        //from here within the parent, we want to know about what happened to child process

        int wstatus;
        wait(&wstatus);
        // Parent process waits for the child process to finish and collects its status into an int variable 

        // however, the specific numerical value of wstatus directly doesn't provide meaningful information unless interpreted with the macros provided (WIFEXITED and WEXITSTATUS).

        if (WIFEXITED(wstatus)) {

            // for normal termination of thr child process:
            // WIFEXITED(wstatus) macro returns true.
            // WEXITSTATUS(wstatus) macro retrieves the exit status code of the child process.

            // remember - there is a difference between successfully exiting and successfully executing and then exiting the process

             // while WIFEXITED(wstatus) helps us distinguish between normal and abnormal termination, checking the exit status code (WEXITSTATUS(wstatus)) allows us to further differentiate between a successful execution (exit status 0) and a failure (non-zero exit status).
            
            int status_code = WEXITSTATUS(wstatus);
            if (status_code == 0) {
                printf("Success: Ping executed properly.\n");
            } else {
                printf("Failure: Ping did not execute properly. Status code: %d\n", status_code);
            }
        }
    } else {
        // Fork failed
        perror("Fork failed");
        return 1;
    }

    return 0;
}
