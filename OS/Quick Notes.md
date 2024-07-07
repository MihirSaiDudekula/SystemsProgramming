### What is a Process?
A process is an instance of a program that is being executed. It is a fundamental concept in an operating system that helps in managing and executing programs. A process can be in one of several states, such as running, waiting, or terminated.

### What Does a Process Contain?
A process contains several components necessary for executing a program. These include:

1. **Program Code (Text Section)**:
   - The executable instructions of the program.

2. **Program Counter**:
   - A register that holds the address of the next instruction to be executed.

3. **Process Stack**:
   - Contains temporary data such as function parameters, return addresses, and local variables.

4. **Heap**:
   - Dynamically allocated memory during the process's runtime.

5. **Data Section**:
   - Contains global variables.

6. **Process Control Block (PCB)**:
   - A data structure in the operating system kernel that contains information needed to manage the process, including:
     - Process state
     - Process ID (PID)
     - Program counter
     - CPU registers
     - Memory management information
     - Accounting information
     - I/O status information

### How Can Two Processes Share a Resource/Common Memory?
Two processes can share resources or memory in several ways:

1. **Shared Memory**:
   - A portion of memory that is accessible by multiple processes.
   - The processes can read from and write to this shared memory region, allowing communication and data exchange.

   Example (POSIX shared memory):
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <sys/mman.h>
   #include <sys/stat.h>
   #include <fcntl.h>
   #include <unistd.h>

   int main() {
       const char *name = "shared_memory";
       const size_t size = 4096;

       int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
       ftruncate(shm_fd, size);

       void *ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
       sprintf(ptr, "Hello, World!");

       return 0;
   }
   ```

2. **Message Passing**:
   - Processes communicate by sending messages to each other through the operating system's message passing facilities.

3. **Pipes**:
   - A unidirectional communication channel that can be used between related processes.

   Example:
   ```c
   #include <stdio.h>
   #include <unistd.h>

   int main() {
       int fd[2];
       pipe(fd);

       if (fork() == 0) {
           // Child process
           close(fd[1]); // Close write end
           char buffer[100];
           read(fd[0], buffer, sizeof(buffer));
           printf("Child received: %s\n", buffer);
           close(fd[0]);
       } else {
           // Parent process
           close(fd[0]); // Close read end
           write(fd[1], "Hello from parent", 17);
           close(fd[1]);
       }

       return 0;
   }
   ```

4. **Memory-Mapped Files**:
   - A file is mapped into the address space of a process, allowing the file to be accessed as if it were part of the process's memory.

### What is a Forked Process?
A forked process is created by the `fork()` system call in Unix-like operating systems. The `fork()` system call creates a new process by duplicating the calling process. The new process is called the child process, and the original process is called the parent process.

- **fork()**:
  - `fork()` creates a new process.
  - The child process is an exact copy of the parent process, except for some differences (e.g., different PID).
  - Both processes continue executing from the point where `fork()` was called.

Example of `fork()`:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        printf("This is the child process\n");
        printf("Child PID: %d\n", getpid());
    } else {
        printf("This is the parent process\n");
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid);
    }

    return 0;
}
```

### Summary
- **Process**: An instance of a running program containing code, data, heap, stack, and PCB.
- **Shared Resources**: Processes can share resources using shared memory, message passing, pipes, or memory-mapped files.
- **Forked Process**: A new process created using the `fork()` system call, duplicating the parent process.

### Creating new processes with `fork()`

#### Introduction
- **Programs vs Processes**: Programs are code you write, whereas processes are instances of running programs. Multiple processes can stem from a single program.
- **Object-Oriented Analogy**: Similar to how a class can instantiate multiple objects, a program can spawn multiple processes.

#### How Processes are Created
- **Program Launcher**: Typically, processes are created by the command shell or program launcher. But how does one process create another?

#### `fork()` Function
- **UNIX and `fork()`**: In UNIX-based systems (Linux, macOS, FreeBSD), processes are created using `fork()`.
- **What is `fork()`?**: `fork()` is a system call that creates a new process (child process) which is a copy of the current process (parent process).

#### Understanding `fork()`
- **Process Tree**: Processes form a tree structure, starting from an initial process (often called `init` or `kernel task`).
- **Cloning Process**: When `fork()` is called, a new child process is created, initially identical to the parent process.

#### Code Example
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;
    pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child process: Hello!\n");
    } else if (pid > 0) {
        // Parent process
        printf("Parent process: Hello!\n");
    } else {
        // Fork failed
        fprintf(stderr, "Fork failed!\n");
        return 1;
    }
    
    return 0;
}
```

#### Understanding `fork()` Return Values
- **Return Values**:
  - **Child Process**: Returns `0`.
  - **Parent Process**: Returns the PID (Process ID) of the child.
  - **Error**: Returns `-1` if `fork()` fails.

#### Differentiating Parent and Child Processes
- **Different Tasks**: After `fork()`, parent and child processes can execute different code paths based on the return value of `fork()`.

#### Conclusion
- **Expanding Utility**: While `fork()` allows creating processes that perform different tasks, limitations arise when attempting to run entirely different programs without their source code.

### Notes on the fork() function in C

#### Introduction to Processes and Threads
- **Processes vs Threads**:
  - Processes are independent execution units with their own memory space.
  - Threads are lighter units within a process, sharing memory space.

#### Understanding fork() Function
- **Overview**:
  - `fork()` function creates a new process (child process) identical to the parent process.
  - Located in `<unistd.h>` (Unix Standard Library).

- **Basic Usage**:
  - Example:
    ```c
    #include <unistd.h>
    #include <stdio.h>

    int main() {
        printf("Hello world\n");
        fork();
        printf("Hello world\n");
        return 0;
    }
    ```
  - Output: "Hello world" is printed twice due to `fork()`.

- **Behavior**:
  - **Execution Split**: `fork()` splits the execution of the program into two paths.
  - **Return Values**: 
    - Returns `0` to the child process.
    - Returns child process ID to the parent process.
    - Returns `-1` on failure.

- **Managing Processes**:
  - **Conditional Execution**: Differentiates between parent and child processes using the return value of `fork()`.
    ```c
    int pid = fork();
    if (pid == 0) {
        // Child process
    } else {
        // Parent process
    }
    ```

- **Creating Multiple Processes**:
  - **Example**:
    ```c
    int main() {
        fork();
        fork();
        printf("Hello world\n");
        return 0;
    }
    ```
  - Output: "Hello world" is printed four times.

- **Controlling Process Creation**:
  - **Limiting Processes**: Use conditions to control `fork()` calls to limit the number of child processes created.

- **Process IDs**:
  - Each process has a unique process ID (`pid_t`), retrievable from `fork()`.

#### Conclusion
- **Summary**:
  - `fork()` creates new processes, dividing execution paths.
  - Understanding return values helps manage parent-child processes effectively.
  - Controlling `fork()` calls regulates the number of child processes created.

#### Code Example:
```c
#include <unistd.h>
#include <stdio.h>

int main() {
    int id = fork();
    if (id == 0) {
        printf("Hello from child process\n");
    } else {
        printf("Hello from parent process\n");
    }
    return 0;
}
```

This summarizes the basics of `fork()` function in C for process management and execution control. Adjust `fork()` calls based on project requirements to effectively manage processes.

### Advanced Notes on "Making forked clones more useful with exec()!"

**Introduction to Processes and Forking**
- Processes are fundamental units of execution in an operating system (*processes*). Last time, we discussed how to create processes using the **fork** system call. Forking allows creating a new process that is a clone of the parent process, essentially making a copy (*fork*).

**Limitations of Forking**
- Using **fork** alone only allows us to create multiple instances of the same program (*fork*). However, we are restricted to running variations of the same source code.

**Introducing exec()**
- **exec** is a family of functions available on POSIX compliant operating systems (*exec*).
- It allows replacing the current process with a new program, enabling a child process to execute a different program (*exec*).

**Understanding exec()**
- When **exec** is called, it doesn't create a new child process; rather, it replaces the current process with the specified program (*exec*).
- The original program ceases to exist unless **exec** fails, in which case it returns an error (*exec*).

**Variants of exec()**
- **exec** has several variants:
  - **execl**, **execv**: Execute a program with a list or array of arguments (*execl*, *execv*).
  - **execle**, **execve**: Execute with an added environment (*execle*, *execve*).
  - **execlp**, **execvp**: Search for the program in PATH environment variable (*execlp*, *execvp*).

**Key Considerations**
- Arguments passed to **exec** functions must be null-terminated (*exec*).
- Choosing between **l** and **v** determines whether arguments are passed as a list or vector (*exec*).
- Including **p** allows searching for the program in PATH, making execution more flexible (*exec*).

**Example Application**
- Suppose we use **fork** to create a child process. Adding an **exec** call allows this child process to execute a different program, such as **ls**, to list directories (*fork*, *exec*).

**Conclusion**
- **exec** is crucial for creating versatile applications where one process can dynamically execute different programs (*exec*).
- Understanding its variants and usage is essential for efficient process management in POSIX systems (*exec*).

**Code Example:**
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        execlp("ls", "ls", NULL);
        // This line will only execute if execlp fails
        perror("exec failed");
    } else if (pid > 0) {
        // Parent process
        wait(NULL); // Wait for child to complete
        printf("Child process finished.\n");
    } else {
        perror("fork failed");
        return 1;
    }

    return 0;
}
```

### Understanding Zombie Processes

**Introduction to Processes:**
- Processes in computing are instances of executing programs. They can be created using system calls like **fork** and **exec**, which allow programs to spawn new processes.

**Parent-Child Relationship:**
- **Parent processes** create **child processes** to perform tasks on their behalf, often waiting for them to complete before continuing execution.

**Zombie Processes Explained:**
- **Zombie processes** occur when a child process completes execution but remains in the process table because the parent process hasn't yet called **wait** or **waitpid** to retrieve its exit status.
- These processes are "dead" but their metadata remains until the parent retrieves the exit status.
- They can be viewed using commands like `ps` or system monitor programs.

**Implications of Zombie Processes:**
- While a single zombie process might not be problematic, multiple zombies can accumulate over time.
- They occupy system resources and can potentially slow down the system if not managed properly.

**How to Prevent Zombie Processes:**
- **Reaping Processes:** Ensure that parent processes call **wait** or **waitpid** to reap their children after they have finished execution.
- This frees up resources associated with the process and removes it from the process table.

**Handling Zombie Processes:**
- If dealing with legacy code or external programs generating zombies:
  - Terminating the parent process responsible for the zombies will cause the operating system to clean up all associated zombie processes.
  - This is an effective way to handle zombie accumulation when direct control over the parent process isn't feasible.

**Conclusion:**
- Zombie processes are a natural part of process management in Unix-like systems but can impact system performance if left unchecked.
- By understanding how to reap child processes, developers can ensure efficient resource management and prevent potential slowdowns.

**Code Example:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;

    // Create a child process
    child_pid = fork();

    if (child_pid > 0) {
        // Parent process
        sleep(10);  // Parent sleeps for 10 seconds
        printf("Parent process finished sleeping.\n");
        // Parent doesn't call wait to reap the child
    } else if (child_pid == 0) {
        // Child process
        printf("Child process executing...\n");
        sleep(5);  // Child sleeps for 5 seconds
        printf("Child process finished.\n");
    } else {
        // Error forking
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}

```

Explanation:
fork(): This system call creates a new process. After fork(), there are two processes - the parent and the child.

Parent Process: The parent process continues after fork() and sleeps for 10 seconds. It does not call wait() to reap the child process.

Child Process: The child process executes immediately after fork() and sleeps for 5 seconds before printing that it has finished.

Zombie Process: If the parent process does not call wait() to reap the child process, the child process becomes a zombie after it finishes its execution but before it is reaped by the parent.

Preventing Zombies: To prevent zombie processes, the parent process should call wait() or waitpid() to reap the child process after it finishes its work.

### Advanced Notes: Waiting for Processes to Finish (Using the wait function) in C

**Overview:**
In this lecture, we explore the usage of the `wait` function in C to manage the execution order of processes created using `fork()`.

**1. Problem Introduction:**
- Using `fork()` to create two processes: one child and one parent.
- Goal: Print numbers 1 through 10 with the child process printing numbers 1-5 and the parent process printing numbers 6-10.

**2. Implementation Steps:**

**2.1. Determine Process ID (PID):**
- Use `getpid()` to determine if the process is the child or the parent.
- `fork()` returns 0 to the child process and the child's PID to the parent.

```c
if (pid == 0) {
    // Child process
} else {
    // Parent process
}
```

**2.2. Control Number Printing:**
- Set initial number (`n`) based on process type:
  - Child starts at 1.
  - Parent starts at 6.

```c
if (pid == 0) {
    n = 1;  // Child process starts at 1
} else {
    n = 6;  // Parent process starts at 6
}
```

**2.3. Print Numbers:**
- Use a loop to print numbers from `n` to `n + 4`.
- Flush output using `fflush(stdout)` after each print to ensure immediate display.

```c
for (int i = n; i <= n + 4; i++) {
    printf("%d ", i);
    fflush(stdout);  // Ensure immediate output
}
```

**3. Execution Order Issue:**
- Without synchronization, outputs from child and parent processes may interleave due to OS scheduling.

```plaintext
Example output without synchronization:
Child: 1 2 3 4 5
Parent: 6 7 8 9 10
```

**4. Introducing `wait` Function:**
- `wait()` pauses the parent process until the child process completes, ensuring sequential execution.

```c
if (pid != 0) {
    wait(NULL);  // Parent waits for any child process to finish
}
```

**5. Final Output:**
- With `wait()` implemented, the output ensures child process completes first, followed by the parent process.

```plaintext
Example output with `wait`:
Child: 1 2 3 4 5
Parent: 6 7 8 9 10
```

**6. Conclusion:**
- The `wait` function is essential for synchronizing processes in C.
- Ensures predictable execution order when dealing with multiple processes.
- Useful in scenarios where processes need to coordinate outputs sequentially.

**7. Further Considerations:**
- Handling scenarios where child processes themselves create additional child processes requires nested synchronization.

**Code Generated:**

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int pid;
    int n;

    pid = fork();

    if (pid == 0) {
        // Child process
        n = 1;
    } else {
        // Parent process
        n = 6;
    }

    for (int i = n; i <= n + 4; i++) {
        printf("%d ", i);
        fflush(stdout);  // Ensure immediate output
    }

    if (pid != 0) {
        wait(NULL);  // Parent waits for any child process to finish
    }

    return 0;
}
```

**Analysis and Notes:**
- The `wait` function is crucial for ensuring the correct sequence of process execution.
- Understanding process IDs (`pid`) and their implications on process behavior is fundamental.
- Proper output flushing (`fflush(stdout)`) ensures immediate display of printed numbers.

These notes provide a comprehensive overview of using `wait` in C to manage process synchronization and ensure ordered execution of tasks.

### Notes on Process IDs in C

#### Introduction to Process IDs
- **Definition**: Process IDs (**PIDs**) are unique identification numbers assigned to each process in Linux.
- **Uniqueness**: Every process, including child and parent processes, has a distinct PID.

#### Retrieving Process IDs
- **getpid() Function**:
  - Retrieves the PID of the current process.
  - Syntax: `#include <unistd.h>`
  - Example:
    ```c
    #include <stdio.h>
    #include <unistd.h>

    int main() {
        pid_t pid = getpid();
        printf("Current PID: %d\n", pid);
        return 0;
    }
    ```
- **getppid() Function**:
  - Retrieves the parent PID of the current process.
  - Example:
    ```c
    #include <stdio.h>
    #include <unistd.h>

    int main() {
        pid_t pid = getpid();
        pid_t ppid = getppid();
        printf("Current PID: %d, Parent PID: %d\n", pid, ppid);
        return 0;
    }
    ```

#### Example Usage
- **Child and Parent Processes**:
  - When creating multiple processes using `fork()`, each process receives a unique PID.
  - Parent process manages child processes and waits for them to terminate using `wait()`.

#### Handling Child Processes
- **Wait Functionality**:
  - Ensures proper termination sequence: child processes should terminate before parent process.
  - Example:
    ```c
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/wait.h>

    int main() {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            sleep(1); // Simulate some work
            printf("Child process completed.\n");
        } else {
            // Parent process
            wait(NULL);
            printf("Parent process completed.\n");
        }
        return 0;
    }
    ```

#### Conclusion
- **Summary**:
  - Understanding and managing process IDs is crucial for process management in Linux.
  - Always ensure child processes are properly managed to prevent issues like zombie processes.

#### Additional Considerations
- **Error Handling**:
  - `wait()` function returns `-1` if there are no child processes to wait for.
  - Useful for handling scenarios where no child processes exist to be waited upon.

  ### Memory Management in C: `mmap` and `brk`

Memory management is a critical part of working with processes in operating systems. Two common system calls used for memory management in Unix-like systems are `mmap` and `brk`. Let's break down these concepts and their usage in simple terms, along with examples.

#### `brk` and `sbrk`

The `brk` and `sbrk` system calls are used to manage the program's heap memory. The heap is a region of a process's memory that can be dynamically expanded or contracted at runtime.

- **`brk`**: Sets the end of the heap to a specified value.
- **`sbrk`**: Increments the program's data space by a specified amount.

##### Syntax

- `int brk(void *end_data_segment);`
- `void *sbrk(intptr_t increment);`

##### Example Using `sbrk`

Here is a simple example demonstrating how `sbrk` can be used to allocate and deallocate memory:

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    // Allocate memory
    void *initial_brk = sbrk(0); // Get current program break
    printf("Initial program break: %p\n", initial_brk);

    void *new_brk = sbrk(1024); // Allocate 1024 bytes
    printf("New program break after allocation: %p\n", new_brk);

    // Use the allocated memory
    int *array = (int *)new_brk;
    for (int i = 0; i < 256; i++) {
        array[i] = i;
    }

    // Deallocate memory
    sbrk(-1024); // Deallocate 1024 bytes
    void *final_brk = sbrk(0); // Get current program break
    printf("Final program break after deallocation: %p\n", final_brk);

    return 0;
}
```

#### `mmap`

The `mmap` system call maps files or devices into memory. It can be used to allocate memory by mapping anonymous pages (i.e., pages not associated with any file).

##### Syntax

```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);
```

- **`addr`**: Starting address for the new mapping. `NULL` means the kernel chooses the address.
- **`length`**: Length of the mapping.
- **`prot`**: Desired memory protection (e.g., `PROT_READ`, `PROT_WRITE`).
- **`flags`**: Determines the nature of the mapping (e.g., `MAP_PRIVATE`, `MAP_ANONYMOUS`).
- **`fd`**: File descriptor. `-1` for anonymous mapping.
- **`offset`**: Offset in the file. Must be zero for anonymous mapping.

##### Example Using `mmap`

Here is an example demonstrating how `mmap` can be used to allocate and deallocate memory:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    // Allocate memory
    size_t length = 1024;
    void *addr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Memory allocated at: %p\n", addr);

    // Use the allocated memory
    int *array = (int *)addr;
    for (int i = 0; i < 256; i++) {
        array[i] = i;
    }

    // Deallocate memory
    if (munmap(addr, length) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    printf("Memory deallocated.\n");

    return 0;
}
```

### Summary

- **`brk` and `sbrk`**: Used to adjust the program's heap size.
  - `sbrk` increments the data space by a specified amount.
  - Example: `sbrk(1024)` allocates 1024 bytes on the heap.
- **`mmap`**: Maps files or devices into memory, also used for anonymous memory allocation.
  - `mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)` allocates 1024 bytes of memory.
  - `munmap(addr, length)` deallocates the memory.

Both `brk/sbrk` and `mmap` are powerful tools for dynamic memory management in C, offering different ways to allocate and manage memory in your programs.

### Notes on Communicating Between Processes (Using Pipes) in C

#### Introduction to Pipes
- **Purpose**: Facilitate communication between processes without shared memory.
- **Mechanism**: Acts like an in-memory file with a buffer for data exchange.

#### Creating a Pipe
- **Function**: `pipe()`
- **Parameters**: Takes an array of two integers (*fd[2]*).
- **Output**: File descriptors (*fd[0]* for read end, *fd[1]* for write end).
- **Usage**: Essential for inter-process communication.

#### Forking Processes
- **Process**: Creates a child process from parent process.
- **File Descriptor Inheritance**: Upon forking, child inherits parent's file descriptors.
- **Management**: Each process manages its own set of descriptors.

#### Writing to a Pipe
- **Function**: `write()`
- **Parameters**: File descriptor (*fd[1]*), buffer pointer, number of bytes.
- **Action**: Sends data from child to parent process via write end of the pipe.
- **Closing**: After writing, close the write end using `close(fd[1])`.

#### Reading from a Pipe
- **Function**: `read()`
- **Parameters**: File descriptor (*fd[0]*), buffer pointer, number of bytes.
- **Action**: Retrieves data from parent by reading from the read end of the pipe.
- **Closing**: After reading, close the read end using `close(fd[0])`.

#### Error Handling
- **Checking**: Verify return values of `pipe()`, `write()`, and `read()`.
- **Conditions**: Handle `-1` for errors (e.g., pipe creation, writing, or reading failures).

#### Example Implementation
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pid_t pid;
    int X, Y;

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        close(fd[0]);  // Close unused read end

        printf("Enter a number: ");
        scanf("%d", &X);

        write(fd[1], &X, sizeof(int));
        close(fd[1]);  // Close write end after writing

        return 0;
    } else { // Parent process
        close(fd[1]);  // Close unused write end

        read(fd[0], &Y, sizeof(int));
        close(fd[0]);  // Close read end after reading

        printf("Received from child process: %d\n", Y);

        return 0;
    }
}
```

Sure! Let's go through the provided C code with detailed comments explaining each part:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int array[] = {1, 2, 3, 4, 1, 3}; // Example array
    int array_size = sizeof(array) / sizeof(int); // Calculate number of elements in array

    int fd[2]; // Array to hold file descriptors for the pipe
    if (pipe(fd) == -1) { // Create the pipe
        perror("Pipe failed"); // Print error message if pipe creation fails
        return 1; // Return 1 to indicate error
    }

    pid_t pid = fork(); // Create a child process
    if (pid < 0) { // Check for fork failure
        perror("Fork failed"); // Print error message if fork fails
        return 2; // Return 2 to indicate fork failure
    }

    if (pid == 0) { // Child process
        close(fd[0]); // Close reading end of the pipe
        int start = 0; // Start index for child process
        int end = array_size / 2; // End index for child process
        int sum = 0; // Variable to hold partial sum
        for (int i = start; i < end; ++i) {
            sum += array[i]; // Calculate partial sum for the child process
        }
        printf("Calculated partial sum (child): %d\n", sum); // Print partial sum for child
        write(fd[1], &sum, sizeof(sum)); // Write partial sum to the pipe
        close(fd[1]); // Close writing end of the pipe after writing
        exit(0); // Exit child process
    } else { // Parent process
        close(fd[1]); // Close writing end of the pipe
        int start = array_size / 2; // Start index for parent process
        int end = array_size; // End index for parent process
        int sum = 0; // Variable to hold partial sum
        for (int i = start; i < end; ++i) {
            sum += array[i]; // Calculate partial sum for the parent process
        }
        printf("Calculated partial sum (parent): %d\n", sum); // Print partial sum for parent

        int sum_from_child; // Variable to store the partial sum received from child
        read(fd[0], &sum_from_child, sizeof(sum_from_child)); // Read from the pipe into sum_from_child
        close(fd[0]); // Close reading end of the pipe after reading

        int total_sum = sum + sum_from_child; // Calculate total sum
        printf("Total sum: %d\n", total_sum); // Print total sum
        wait(NULL); // Wait for child process to finish
    }

    return 0; // Exit main function
}
```

### Explanation with Comments:

1. **Array Initialization and Size Calculation:**
   ```c
   int array[] = {1, 2, 3, 4, 1, 3}; // Example array
   int array_size = sizeof(array) / sizeof(int); // Calculate number of elements in array
   ```

2. **Pipe Creation:**
   ```c
   int fd[2]; // Array to hold file descriptors for the pipe
   if (pipe(fd) == -1) { // Create the pipe
       perror("Pipe failed"); // Print error message if pipe creation fails
       return 1; // Return 1 to indicate error
   }
   ```
   - `fd[0]` is the file descriptor for reading from the pipe, and `fd[1]` is for writing to the pipe.

3. **Forking a Child Process:**
   ```c
   pid_t pid = fork(); // Create a child process
   if (pid < 0) { // Check for fork failure
       perror("Fork failed"); // Print error message if fork fails
       return 2; // Return 2 to indicate fork failure
   }
   ```
   - `fork()` creates a new process. `pid` will be `0` in the child process and a positive value (child's PID) in the parent process.

4. **Child Process Execution:**
   ```c
   if (pid == 0) { // Child process
       close(fd[0]); // Close reading end of the pipe
       // Calculate partial sum for the child
       // Write partial sum to the pipe
       // Close writing end of the pipe after writing
       // Exit child process
   }
   ```

5. **Parent Process Execution:**
   ```c
   else { // Parent process
       close(fd[1]); // Close writing end of the pipe
       // Calculate partial sum for the parent
       // Read from the pipe into sum_from_child
       // Close reading end of the pipe after reading
       // Calculate total sum
       // Print total sum
       wait(NULL); // Wait for child process to finish
   }
   ```
   - The parent process calculates its partial sum, reads the partial sum from the child process via the pipe, calculates the total sum, and waits for the child process to finish (`wait(NULL)`).

### Explanation of Closing File Descriptors:

- **Why Two File Descriptors (fd[0] and fd[1])**: 
  - `fd[0]` is used for reading from the pipe, and `fd[1]` is used for writing to the pipe. This separation ensures that processes can communicate bidirectionally through the pipe without conflicts.
  
- **Closing File Descriptors**:
  - Each process closes the file descriptor it does not need to use:
    - The child process closes `fd[0]` because it only writes to the pipe (`fd[1]`).
    - The parent process closes `fd[1]` because it only reads from the pipe (`fd[0]`).

### Summary:

This C program demonstrates inter-process communication using pipes (`pipe` function) and process creation (`fork` function). The parent and child processes cooperate to calculate partial sums of an array, communicate these sums through a pipe, and then compute and print the total sum. Closing unused file descriptors (`fd[0]` and `fd[1]`) ensures proper resource management and prevents unexpected behavior in inter-process communication.

### Introduction to FIFOs (aka named pipes) in C

#### Overview of Pipes and Their Limitations
- Pipes are communication channels between processes in the same hierarchy.
- **File descriptors (FDs)** are special handles used to access pipes.
- When a process forks, file descriptors are inherited.

#### Introduction to FIFOs
- **FIFOs** (also known as named pipes) extend pipe functionality to non-hierarchical processes.
- They behave like regular files and are accessible to any process.

#### Creating a FIFO in C
To create a FIFO in a C program:

1. **Include Necessary Headers**
   ```c
   #include <stdio.h>
   #include <fcntl.h>
   #include <unistd.h>
   ```

2. **Create the FIFO**
   ```c
   int main() {
       char *fifo_path = "myFIFO1";
       int status = mkfifo(fifo_path, 0777); // Create FIFO with read/write permissions
       
       if (status == -1) {
           printf("Could not create FIFO file\n");
           return 1;
       }
       
       printf("FIFO file created successfully\n");
       return 0;
   }
   ```
   - Uses `mkfifo` function to create a FIFO named `myFIFO1`.
   - Checks status code for error handling.

3. **Open the FIFO for Writing**
   ```c
   int fd = open("myFIFO1", O_WRONLY); // Open FIFO for writing
   
   if (fd == -1) {
       perror("Error opening FIFO");
       return 1;
   }
   
   // Write to the FIFO
   int x = 97;
   write(fd, &x, sizeof(x));
   
   close(fd); // Close the FIFO after writing
   ```

4. **Handling FIFO Behavior**
   - Opening a FIFO for writing blocks until another process opens it for reading.
   - Closing the FIFO after operations are complete is crucial.

#### Understanding FIFO Behavior
- **Blocking Nature**: Opening a FIFO for write waits until another process opens it for read.
- **Demonstration**: Use `cat` to read from a FIFO opened for write to unblock the writing process.

#### Conclusion and Next Steps
- FIFOs are essential for inter-process communication.
- Future topics include bidirectional communication and error handling.

#### Example Code
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char *fifo_path = "myFIFO1";
    int status = mkfifo(fifo_path, 0777); // Create FIFO with read/write permissions
   
    if (status == -1) {
        printf("Could not create FIFO file\n");
        return 1;
    }
   
    printf("FIFO file created successfully\n");
   
    int fd = open("myFIFO1", O_WRONLY); // Open FIFO for writing
   
    if (fd == -1) {
        perror("Error opening FIFO");
        return 1;
    }
   
    int x = 97;
    write(fd, &x, sizeof(x)); // Write to the FIFO
   
    close(fd); // Close the FIFO after writing
   
    return 0;
}
```

This notes summarize the lecture on FIFOs in C, covering creation, opening for writing, blocking behavior, and basic error handling.

### Two-way Communication Between Processes (Using Pipes) in C

**Introduction**
- Two-way communication between processes involves establishing communication channels for data exchange.
- This method uses **pipes** in C to facilitate communication between a parent and child process.

**Problem Scenario**
- The objective is to implement a program where:
  1. The parent process generates a random number from 0 to 9.
  2. This number is sent to the child process via a pipe.
  3. The child process multiplies this number by 4 and sends it back to the parent.
  4. The parent process then prints the final result on the screen.

**Implementation Steps**

**1. Pipe Creation**
- Create a pipe using `pipe()` function to establish a unidirectional communication channel.

```c
int p1[2];  // Pipe 1
int p2[2];  // Pipe 2

if (pipe(p1) == -1 || pipe(p2) == -1) {
    perror("Pipe creation failed");
    exit(EXIT_FAILURE);
}
```

**2. Forking Processes**
- Use `fork()` to create a child process. The child process will handle the multiplication task.

```c
pid_t pid = fork();

if (pid < 0) {
    perror("Fork failed");
    exit(EXIT_FAILURE);
}
```

**3. Communication Process in Child**
- In the child process (`pid == 0`):
  - Close unnecessary ends of pipes (`p1` for reading, `p2` for writing).
  - Read the number from `p1`, multiply it by 4, and write the result to `p2`.

```c
if (pid == 0) {
    close(p1[1]);  // Close write end of p1
    close(p2[0]);  // Close read end of p2
    
    int X;
    read(p1[0], &X, sizeof(X));
    X *= 4;
    write(p2[1], &X, sizeof(X));
    
    close(p1[0]);
    close(p2[1]);
    exit(EXIT_SUCCESS);
}
```

**4. Communication Process in Parent**
- In the parent process (`pid > 0`):
  - Close unnecessary ends of pipes (`p1` for writing, `p2` for reading).
  - Generate a random number, write it to `p1`, read the result from `p2`, and print it.

```c
else {
    close(p1[0]);  // Close read end of p1
    close(p2[1]);  // Close write end of p2
    
    srand(time(NULL));
    int Y = rand() % 10;
    
    printf("Generated number: %d\n", Y);
    write(p1[1], &Y, sizeof(Y));
    
    read(p2[0], &Y, sizeof(Y));
    printf("Result received: %d\n", Y);
    
    close(p1[1]);
    close(p2[0]);
    wait(NULL);  // Wait for child process to finish
}
```

**Conclusion**
- By using separate pipes (`p1` for parent-to-child and `p2` for child-to-parent), we avoid issues of pipe blocking and ensure reliable two-way communication between processes.

**Additional Notes**
- **Pipe Mechanism**: Pipes allow communication between processes in a unidirectional manner. Using two pipes facilitates bidirectional communication by separating input and output channels.
- **Error Handling**: Proper error checking ensures robustness in handling pipe creation, forking processes, and reading/writing operations.
  
**Code Implementation**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int p1[2];  // Pipe 1
    int p2[2];  // Pipe 2
    
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {  // Child process
        close(p1[1]);  // Close write end of p1
        close(p2[0]);  // Close read end of p2
        
        int X;
        read(p1[0], &X, sizeof(X));
        X *= 4;
        write(p2[1], &X, sizeof(X));
        
        close(p1[0]);
        close(p2[1]);
        exit(EXIT_SUCCESS);
    }
    else {  // Parent process
        close(p1[0]);  // Close read end of p1
        close(p2[1]);  // Close write end of p2
        
        srand(time(NULL));
        int Y = rand() % 10;
        
        printf("Generated number: %d\n", Y);
        write(p1[1], &Y, sizeof(Y));
        
        read(p2[0], &Y, sizeof(Y));
        printf("Result received: %d\n", Y);
        
        close(p1[1]);
        close(p2[0]);
        wait(NULL);  // Wait for child process to finish
    }
    
    return 0;
}
```

This code effectively demonstrates two-way communication using pipes in C between a parent and child process, ensuring reliable data exchange without blocking issues.

up next - codevault c/os playlist vid 7 onw