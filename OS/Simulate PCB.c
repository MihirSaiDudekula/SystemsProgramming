#include <stdio.h>
#include <stdlib.h>

// Define process states
typedef enum {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

// Define a structure for PCB
typedef struct {
    int pid;             // Process ID
    State state;         // Process state (e.g., running, waiting, etc.)
    void *program_counter; // Program counter (address of the next instruction to execute)
    int cpu_registers[10]; // Example: CPU registers (could be more or less depending on architecture)
    // Add more fields as needed for memory management, open files, accounting, I/O status, etc.
    // Example fields:
    // Memory management information
    // File descriptors for open files
    // Accounting information (e.g., CPU time used, start time)
    // I/O status information (e.g., pending I/O requests)
} PCB;

// Function to initialize a new PCB
PCB* createPCB(int pid) {
    PCB *new_pcb = (PCB *) malloc(sizeof(PCB));
    if (new_pcb != NULL) {
        new_pcb->pid = pid;
        new_pcb->state = READY; // Initialize state as READY
        new_pcb->program_counter = NULL; // Initialize program counter
        // Initialize other fields as needed
    }
    return new_pcb;
}

// Function to free memory allocated for a PCB
void destroyPCB(PCB *pcb) {
    free(pcb);
}

int main() {
    // Example usage
    PCB *process1 = createPCB(1);
    PCB *process2 = createPCB(2);

    // Modify PCB fields as necessary
    process1->state = RUNNING;
    process1->cpu_registers[0] = 100; // Example: setting a CPU register

    // Display information (for demonstration purposes)
    printf("Process 1: PID %d, State %d\n", process1->pid, process1->state);
    printf("Process 2: PID %d, State %d\n", process2->pid, process2->state);

    // Clean up
    destroyPCB(process1);
    destroyPCB(process2);

    return 0;
}
