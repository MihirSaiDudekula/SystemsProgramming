#include <stdio.h>   // Standard input/output library
#include <signal.h>  // Signal handling library

// When handling signals in C, the struct sigaction plays a crucial role. It is defined in the <signal.h> header and typically includes the following members relevant to our discussion:

// sa_handler: Specifies the function to be called when the signal occurs.

// sa_mask: Defines the set of signals that should be blocked (masked) while the signal handler runs.

// sa_flags: Controls various aspects of signal handling behavior, such as restarting interrupted system calls.


// Function declaration for signal handler
void handle_SIGSTP(int sig);

int main() {
    struct sigaction sa;  
    // struct sigaction is a structure defined in the signal.h header file. It is used to specify the action to be taken on receipt of a specific signal.
    // sa is an instance of this structure that will hold the configuration details for handling a signal.
    
    sa.sa_handler = &handle_SIGSTP;
    // sa_handler is a member of the struct sigaction structure. It represents the function that will be called when the associated signal (SIGTSTP in this case) is received.

    //note- this is not for calling the function, but this is a pointer to the function, signifying that this is the function that will handle in case of a signal

    
    // The function sigemptyset is used to initialize a signal set to be empty. In other words, it clears any existing signals

    // here, we empty sa.sa_mask, ensuring that no signals are blocked (masked) when the associated signal handler (sa_handler) is executing.

    // If other signals were masked (blocked), they could lead to missed signals or undesirable interactions with the signal handler.
    sigemptyset(&sa.sa_mask);
    
    // Setting additional flags for the handler
    sa.sa_flags = SA_RESTART;
    // When set, SA_RESTART ensures that certain system calls interrupted by the signal (SIGTSTP in this case) will be automatically restarted by the operating system rather than returning an error
    
    // Binding SIGSTP signal to the handler function
    // SIGSTP is the same as Ctrl + z in the terminal
    sigaction(SIGTSTP, &sa, NULL);
    // sigaction is a function used to define the action taken by a process on receipt of a specific signal.
    //the arguments to this are the signal,the struct associated with it, and the  the current action for the specified signal, which is null in this case

    // Main program loop
    while (1) {
        int input;
        
        // Prompting user for input
        printf("Input a number: ");
        scanf("%d", &input);
        
        // Calculating and printing the result
        printf("%d times 5 equals %d\n", input, input * 5);
    }
    
    return 0;
}

// Signal handler function definition
void handle_SIGSTP(int sig) {
    // Handling SIGSTP signal by printing a message
    printf("\nStopping not allowed\n");
}
