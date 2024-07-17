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

### How to Execute Another Program in C (using exec)

To execute another program from within your C program, you can use the **exec** family of functions. These functions are standard in Linux and have an emulation in Windows, providing similar functionality.

#### Step-by-Step Guide:

1. **Include the Header File**:
   - For Windows: `#include <process.h>`
   - For Linux: `#include <unistd.h>`

2. **Choose the Exec Function**:
   - `exec` functions come in several variants:
     - `execl`, `execv`: Takes a list or vector of arguments.
     - `execle`, `execve`: Include an environment variable.
     - `execlp`, `execvp`: Uses the system’s PATH environment variable.

3. **Example Using `execlp`**:
   - This variant allows using the system's PATH variable.
   - Example: Executing the `ping` command.
     ```c
     execlp("ping", "ping", "google.com", NULL);
     ```

   - Parameters breakdown:
     - First `"ping"`: Command name or program to execute.
     - Second `"ping"`: Argument passed to the program.
     - Third `google.com`: Additional arguments.
     - Fourth `NULL`: End of argument list.

4. **Understanding Execution Flow**:
   - After calling an `exec` function, the current process is replaced by the new process (e.g., `ping` in this case).
   - Any code following the `exec` call will not execute unless the `exec` call fails.

5. **Handling Errors**:
   - Check the return value of `exec` functions (typically `-1` on failure).
   - Use `errno` to diagnose specific errors.
     ```c
     #include <errno.h>
     if (execvp("ping", args) == -1) {
         perror("execvp failed");
         exit(EXIT_FAILURE);
     }
     ```

6. **Code Example**:
   - Combining all the above concepts:
     ```c
     #include <stdio.h>
     #include <stdlib.h>
     #include <unistd.h>
     #include <errno.h>
     int main() {
         printf("Executing ping google.com...\n");
         if (execlp("ping", "ping", "google.com", NULL) == -1) {
             perror("execvp failed");
             exit(EXIT_FAILURE);
         }
         printf("Program finished executing.\n");
         return 0;
     }
     ```

### Conclusion

Using the **exec** functions allows your C program to execute external programs efficiently. Ensure to handle errors properly using `errno` and consider which `exec` variant suits your needs based on argument passing and environment requirements. These functions are essential for integrating system commands into your C applications seamlessly.

#### Generated Code Example:
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    printf("Executing ping google.com...\n");
    if (execlp("ping", "ping", "google.com", NULL) == -1) {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    printf("Program finished executing.\n");
    return 0;
}
```

This code snippet demonstrates executing the `ping` command with `google.com` as an argument using `execlp`. Ensure to compile and run this on a system where `ping` is available in the PATH variable for successful execution.

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

## Understanding the `mmap` System Call

### What is `mmap`?
The `mmap` (memory map) system call is used to map files or devices into memory. It provides a way for applications to access file contents by mapping them into the virtual address space of the process.

### Why Use `mmap`?
1. **Efficient File Access**: Allows applications to read and write files as if they were in memory, which can be faster than traditional I/O operations.
2. **Memory Sharing**: Can be used to share memory between processes.
3. **Memory-Mapped Files**: Useful for applications that need to access large files, like databases.

### Basic Concepts
- **Memory Mapping**: Associating a file or device with a range of addresses in the application's address space.
- **Virtual Address Space**: The range of addresses that an application can use, which is managed by the operating system.

### Syntax of `mmap`
Here's the basic syntax of the `mmap` system call in C:
```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

### Parameters
- `addr`: Starting address for the new mapping. `NULL` lets the system choose the address.
- `length`: Number of bytes to be mapped.
- `prot`: Memory protection of the mapping. Common options include:
  - `PROT_READ`: Pages can be read.
  - `PROT_WRITE`: Pages can be written.
  - `PROT_EXEC`: Pages can be executed.
  - `PROT_NONE`: Pages cannot be accessed.
- `flags`: Determines the nature of the mapping. Common options include:
  - `MAP_SHARED`: Changes are shared.
  - `MAP_PRIVATE`: Changes are private (copy-on-write).
- `fd`: File descriptor of the file to be mapped.
- `offset`: Offset in the file where the mapping starts.

### Example Code
Here's a simple example to illustrate how `mmap` can be used to map a file into memory and read its contents.

1. **Include Necessary Headers**
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <fcntl.h>
   #include <sys/mman.h>
   #include <sys/stat.h>
   #include <unistd.h>
   ```

2. **Example Function Using `mmap`**
   ```c
   int main() {
       // Open the file
       int fd = open("example.txt", O_RDONLY);
       if (fd == -1) {
           perror("Error opening file");
           exit(EXIT_FAILURE);
       }

       // Get the size of the file
       struct stat st;
       if (fstat(fd, &st) == -1) {
           perror("Error getting file size");
           close(fd);
           exit(EXIT_FAILURE);
       }
       size_t filesize = st.st_size;

       // Map the file into memory
       char *mapped = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
       if (mapped == MAP_FAILED) {
           perror("Error mapping file");
           close(fd);
           exit(EXIT_FAILURE);
       }

       // Close the file descriptor
       close(fd);

       // Use the mapped memory (e.g., print the file contents)
       for (size_t i = 0; i < filesize; ++i) {
           putchar(mapped[i]);
       }

       // Unmap the file
       if (munmap(mapped, filesize) == -1) {
           perror("Error unmapping file");
           exit(EXIT_FAILURE);
       }

       return 0;
   }
   ```

### Explanation of the Example
1. **Opening the File**: The file `example.txt` is opened using the `open` system call with read-only permissions (`O_RDONLY`).
2. **Getting File Size**: The `fstat` function retrieves the size of the file.
3. **Mapping the File**: The `mmap` function maps the file into memory with read-only access (`PROT_READ`) and private mapping (`MAP_PRIVATE`).
4. **Using the Mapped Memory**: The contents of the file are printed using `putchar`.
5. **Unmapping the File**: The `munmap` function unmaps the file from memory.

### Summary
- **`mmap`** maps files or devices into memory, providing an efficient way to read and write files.
- The **parameters** of `mmap` control the starting address, length, protection, flags, file descriptor, and offset.
- The **example** demonstrates opening a file, mapping it into memory, reading its contents, and unmapping it.

This explanation should help an absolute beginner understand the basics of the `mmap` system call and how to use it in a simple program.

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

## Communication Between Two Processes Using `pipe()` in C

### Introduction
**Pipes** are a form of inter-process communication (IPC) that allow data to be passed from one process to another. The `pipe()` system call in C creates a unidirectional data channel that can be used for this purpose.

### Key Concepts
- **Pipe**: A pipe is a conduit for data flow between processes. Data written to the pipe by one process can be read by another process.
- **Unidirectional**: Data flows in only one direction, from the write end to the read end.

### Syntax of `pipe()`
```c
int pipe(int pipefd[2]);
```
- `pipefd`: An array of two integers. `pipefd[0]` is the read end and `pipefd[1]` is the write end of the pipe.

### Example Program
Here's a simple example to illustrate how two processes can communicate using a pipe in C.

1. **Include Necessary Headers**
   ```c
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include <string.h>
   ```

2. **Example Function Using `pipe()`**
   ```c
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
   ```

### Explanation of the Example
1. **Create the Pipe**: The `pipe(pipefd)` function creates a pipe. `pipefd[0]` is the read end, and `pipefd[1]` is the write end.
2. **Fork a New Process**: The `fork()` function creates a new process. The return value of `fork()` helps determine whether the current process is the parent or the child.
3. **Parent Process**:
   - Closes the read end of the pipe (`close(pipefd[0])`).
   - Writes a message to the pipe (`write(pipefd[1], write_msg, strlen(write_msg) + 1)`).
   - Closes the write end of the pipe after writing (`close(pipefd[1])`).
4. **Child Process**:
   - Closes the write end of the pipe (`close(pipefd[1])`).
   - Reads the message from the pipe (`read(pipefd[0], read_msg, sizeof(read_msg))`).
   - Prints the message read from the pipe (`printf("Child process read: %s\n", read_msg)`).
   - Closes the read end of the pipe after reading (`close(pipefd[0])`).

### Summary
- **Pipes** are used for unidirectional communication between processes.
- The `pipe()` system call creates a pipe, and `fork()` creates a new process.
- Data is written to the pipe by the parent process and read by the child process.

This example demonstrates basic inter-process communication using a pipe in C. It shows how to set up the pipe, handle the fork, and manage the read and write operations in both the parent and child processes.

### Notes on Getting Exit Status Code in C

**Overview**

This lecture discusses how to obtain and interpret the exit status code of a program executed using `exec` in a C program, specifically focusing on scenarios involving the `ping` program.

**Using Fork to Execute Programs**

- **Fork Mechanism**: Used to create a child process which can execute the `ping` program while the parent process waits.
- **Execution**: Child process executes `ping` program, parent process waits for completion.

**Handling Exit Status**

- **Understanding `exec` Return Value**:
  - `exec` function returns `-1` if it fails to find the program to execute (`ping` in this case).
  - Parent process can check this return value to handle errors.

```c
int error = exec(...);
if (error == -1) {
    printf("Could not find program to execute.\n");
    return 0;
}
```

- **Differentiating Errors**:
  - If `ping` program does not exist, `exec` returns `-1`.
  - If `ping` program executes but encounters an error (like incorrect domain), parent process still continues.

**Using `wait` Function**

- **Waiting for Child Process**:
  - `wait` function returns status information in `WSTATUS` parameter.

```c
int wstatus;
wait(&wstatus);
```

- **Macros for Checking Status**:
  - `WIFEXITED`: Checks if child process terminated normally.
  - `WEXITSTATUS`: Retrieves the exit status code of the child process.

```c
if (WIFEXITED(wstatus)) {
    int status_code = WEXITSTATUS(wstatus);
    if (status_code == 0) {
        printf("Success: Ping executed properly.\n");
    } else {
        printf("Failure: Ping did not execute properly. Status code: %d\n", status_code);
    }
}
```

**Conclusion**

- **Interpreting Status Codes**:
  - Status code `0` indicates successful execution.
  - Non-zero status codes signify errors or specific conditions depending on the program.

**Example Code**

```c
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
        int wstatus;
        wait(&wstatus);

        if (WIFEXITED(wstatus)) {
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
```

This code demonstrates how to execute the `ping` program and retrieve its exit status in a parent process using `fork`, `exec`, and `wait` functions in C.


The `wstatus` variable, when used with the `wait` function in Unix-like operating systems, can have various possible values depending on the state and outcome of the child process. Here are the possible values and their meanings:

1. **Normal Termination**:
   - If the child process terminates normally (i.e., it calls `exit()` or returns from `main()`), `wait` sets `wstatus` to a value that indicates normal termination.
   - `WIFEXITED(wstatus)` will return true, indicating that the child process exited normally.
   - `WEXITSTATUS(wstatus)` will return the exit status code of the child process. This is typically a value between 0 and 255, where 0 usually indicates success.

2. **Termination Due to Signal**:
   - If the child process terminates due to receiving a signal (such as `SIGTERM`, `SIGSEGV`, etc.), `wait` sets `wstatus` to indicate termination by signal.
   - `WIFSIGNALED(wstatus)` will return true, indicating that the child process terminated due to a signal.
   - `WTERMSIG(wstatus)` will return the number of the signal that caused the termination.

3. **Stopped or Continued State**:
   - If the child process is stopped by a signal (`SIGSTOP`, `SIGTSTP`, etc.) or continued (`SIGCONT`), `wait` can set `wstatus` to indicate these states.
   - `WIFSTOPPED(wstatus)` will return true if the child process is currently stopped.
   - `WSTOPSIG(wstatus)` will return the number of the signal that caused the process to stop.
   - `WIFCONTINUED(wstatus)` will return true if the child process was continued after being stopped.

These macros (`WIFEXITED`, `WEXITSTATUS`, `WIFSIGNALED`, `WTERMSIG`, `WIFSTOPPED`, `WSTOPSIG`, `WIFCONTINUED`) are defined in `<sys/wait.h>` and are used to interpret the value of `wstatus` returned by `wait`.

**Summary of Possible Values**:
- If `WIFEXITED(wstatus)` is true, `wstatus` contains `WEXITSTATUS(wstatus)`.
- If `WIFSIGNALED(wstatus)` is true, `wstatus` contains `WTERMSIG(wstatus)`.
- If `WIFSTOPPED(wstatus)` is true, `wstatus` contains `WSTOPSIG(wstatus)`.
- `WIFCONTINUED(wstatus)` indicates if the child process was continued after being stopped.

These values help in determining the exact condition under which the child process terminated or its current state, providing valuable information for handling processes in Unix-based systems.


### Understanding Standard Input and Standard Output in C

In C programming, **standard input** (`stdin`) and **standard output** (`stdout`) are fundamental concepts for handling input from the user and outputting data to the screen or other devices. These are provided by the operating system and are crucial for interacting with a program during its execution.

#### 1. Standard Input (`stdin`)
- `stdin` is a standard input stream where a program reads input from the user or from another program.

##### Key Points:
- By default, `stdin` is typically associated with the keyboard input.
- It is represented by the macro `stdin` in C.
- It is buffered, meaning input is read in chunks rather than one character at a time.

Buffering: In the context of stdin, buffering means that input is collected in memory in chunks rather than being read character by character. This improves efficiency by reducing the number of system calls and speeding up input operations.

Line Buffering: By default, stdin is typically line-buffered, meaning input is processed line by line. This means that input is stored in a buffer until a newline character (\n) is encountered, at which point the entire line is passed to the program.

##### Example:
```c
#include <stdio.h>

int main() {
    char input[100];

    printf("Enter your name: ");
    fgets(input, sizeof(input), stdin); // Read input from stdin

    printf("Hello, %s", input);

    return 0;
}
```
- **Explanation**:
  - `fgets(input, sizeof(input), stdin);` reads a line of input from `stdin` (keyboard) into the `input` array.
  - The `fgets` function reads until a newline (`\n`) or EOF (End of File) is encountered, storing the input in `input`.

#### 2. Standard Output (`stdout`)
- `stdout` is a standard output stream where a program writes its output, typically to the console or terminal.

##### Key Points:
- By default, `stdout` is associated with the screen.
- It is represented by the macro `stdout` in C.
- It is also buffered, meaning output is collected in chunks and displayed.

##### Example:
```c
#include <stdio.h>

int main() {
    int number = 42;

    printf("The answer is: %d\n", number); // Output to stdout

    return 0;
}
```
- **Explanation**:
  - `printf("The answer is: %d\n", number);` outputs the formatted string and value of `number` to `stdout`.

#### File Descriptors and Standard Streams
- In C, standard input (`stdin`), standard output (`stdout`), and standard error (`stderr`) are represented by file descriptors:
  - `STDIN_FILENO` (0): Standard input file descriptor.
  - `STDOUT_FILENO` (1): Standard output file descriptor.
  - `STDERR_FILENO` (2): Standard error output file descriptor.

##### Example of using File Descriptors:
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char buffer[100];
    ssize_t bytes_read;

    // Read from stdin using file descriptor
    bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytes_read == -1) {
        perror("Error reading from stdin");
        return 1;
    }

    // Write to stdout using file descriptor
    ssize_t bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
    if (bytes_written == -1) {
        perror("Error writing to stdout");
        return 1;
    }

    return 0;
}
```
- **Explanation**:
  - `read(STDIN_FILENO, buffer, sizeof(buffer));` reads input directly from `stdin` using its file descriptor.
  - `write(STDOUT_FILENO, buffer, bytes_read);` writes the contents of `buffer` (read from `stdin`) directly to `stdout`.

#### Conclusion
Understanding `stdin` and `stdout` is crucial for handling input and output operations in C. These standard streams provide a simple yet powerful way to interact with users and display information, making C programs versatile in their ability to handle various input and output scenarios.


### Title: Redirecting standard output in C

#### Overview
In this tutorial, we continue with our previous program, focusing on redirecting the standard output (**stdout**) of a program in C to a file. This technique allows us to capture program output into a designated file instead of displaying it on the terminal.

#### Problem Statement
Currently, when we execute our program, the output appears directly on the terminal. However, we only need to know if the operation was successful, without displaying specific messages.

#### Solution Approach
To redirect the output to a file in Linux, we follow these steps:

1. **Create a File**: Use the `open()` function to create or open a file. The file will store the redirected output.
   ```c
   #include <fcntl.h>
   int file = open("ping_results.txt", O_WRONLY | O_CREAT, 0777);
   ```

   - **Explanation**: 
     - `O_WRONLY`: Opens the file for writing only.
     - `O_CREAT`: Creates the file if it doesn't exist.
     - `0777`: Sets permissions for the file (read, write, execute for all).

2. **Error Handling**: Check if the file descriptor is valid (`file == -1` indicates an error).

3. **Redirect Output**: Use the `dup2()` function to redirect `stdout` to the file descriptor obtained from `open()`.
   ```c
   dup2(file, STDOUT_FILENO);
   ```

   - **Explanation**:
     - `dup2()` duplicates the file descriptor (`file`) onto `STDOUT_FILENO` (standard output).

4. **Close Unused File Descriptor**: Close the original file descriptor if it's not needed anymore.
   ```c
   close(file);
   ```

#### File Descriptor Basics
- **File Descriptor**: A unique identifier for an open file within a process.
- **Standard File Descriptors in Linux**:
  - `STDIN_FILENO`: Standard input (0)
  - `STDOUT_FILENO`: Standard output (1)
  - `STDERR_FILENO`: Standard error (2)

#### Conclusion
By redirecting standard output to a file in C, we can control where program output is displayed, facilitating cleaner program execution without unnecessary terminal output.

#### Example Code
```c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // Open or create ping_results.txt for writing
    int file = open("ping_results.txt", O_WRONLY | O_CREAT, 0777);

    // Check for errors in opening file
    if (file == -1) {
        perror("Error opening file");
        return 1;
    }

    // Redirect stdout to the file descriptor 'file'
    dup2(file, STDOUT_FILENO);

    // Close the original file descriptor (optional)
    close(file);

    // Now stdout is redirected to ping_results.txt
    // Execute your program here

    return 0;
}
```

This approach ensures that all output generated by the program is directed into `ping_results.txt` instead of being displayed on the terminal, achieving our goal of controlling program output location.

### Explanation of `dup()` and `dup2()` in C

In C programming, `dup()` and `dup2()` are functions used to duplicate file descriptors. They are particularly useful for redirecting input and output streams in Unix-like operating systems. Here's a simplified explanation of each function with example code.

#### 1. `dup()`

- **Purpose**: `dup()` duplicates an existing file descriptor and returns a new file descriptor that refers to the same file or device.

- **Prototype**: `int dup(int oldfd);`

- **Parameters**:
  - `oldfd`: The file descriptor to be duplicated.

- **Return Value**:
  - Returns a new file descriptor if successful (which is the lowest available descriptor number greater than or equal to `0`).
  - `-1` if an error occurs.

- **Example**:
  
  ```c
  #include <stdio.h>
  #include <unistd.h>
  #include <fcntl.h>

  int main() {
      int fd = open("output.txt", O_WRONLY | O_CREAT, 0644);
      if (fd == -1) {
          perror("Error opening file");
          return 1;
      }

      // Duplicate the file descriptor 'fd'
      int new_fd = dup(fd);
      if (new_fd == -1) {
          perror("Error duplicating file descriptor");
          return 1;
      }

      // Now both 'fd' and 'new_fd' refer to the same file

      // Write to the original file descriptor
      dprintf(fd, "Hello from original fd!\n");

      // Write to the duplicated file descriptor
      dprintf(new_fd, "Hello from duplicated fd!\n");

      // Close both file descriptors
      close(fd);
      close(new_fd);

      return 0;
  }
  ```

  - **Explanation**: 
    - `open("output.txt", O_WRONLY | O_CREAT, 0644);` opens a file for writing (`O_WRONLY`) and creates it if it doesn't exist (`O_CREAT`).
    - `dup(fd);` duplicates the file descriptor `fd`, creating `new_fd`.
    - `dprintf(fd, ...);` and `dprintf(new_fd, ...);` both write to the same file (`output.txt`) because they refer to the same underlying file descriptor.
    - Closing both file descriptors (`close(fd);` and `close(new_fd);`) ensures proper cleanup.

#### 2. `dup2()`

- **Purpose**: `dup2()` duplicates an existing file descriptor to a specified file descriptor number, allowing redirection of file descriptors.

- **Prototype**: `int dup2(int oldfd, int newfd);`

- **Parameters**:
  - `oldfd`: The file descriptor to be duplicated.
  - `newfd`: The file descriptor number to which `oldfd` should be duplicated.

- **Return Value**:
  - Returns `newfd` if successful.
  - `-1` if an error occurs.

- **Example**:
  
  ```c
  #include <stdio.h>
  #include <unistd.h>
  #include <fcntl.h>

  int main() {
      int fd = open("output.txt", O_WRONLY | O_CREAT, 0644);
      if (fd == -1) {
          perror("Error opening file");
          return 1;
      }

      // Duplicate 'fd' to file descriptor 1 (STDOUT_FILENO)
      int result = dup2(fd, STDOUT_FILENO);
      if (result == -1) {
          perror("Error duplicating file descriptor");
          return 1;
      }

      // Now STDOUT_FILENO (1) points to 'output.txt'

      // Write to STDOUT_FILENO (which now points to 'output.txt')
      printf("Hello from stdout redirected to output.txt\n");

      // Close the original file descriptor
      close(fd);

      return 0;
  }
  ```

  - **Explanation**:
    - `open("output.txt", O_WRONLY | O_CREAT, 0644);` opens `output.txt` for writing.
    - `dup2(fd, STDOUT_FILENO);` duplicates `fd` (output file descriptor) to `STDOUT_FILENO` (standard output).
    - `printf(...);` writes output to `STDOUT_FILENO`, which is redirected to `output.txt`.
    - Closing the original file descriptor (`close(fd);`) ensures proper cleanup.

#### Conclusion

`dup()` and `dup2()` are powerful functions in C for manipulating file descriptors, allowing programs to manage input and output redirection efficiently. They are essential for tasks such as implementing shell pipelines, redirecting standard input/output/error, and handling file operations in Unix-like systems. Understanding their usage is fundamental for advanced C programming, especially in contexts involving system-level programming and file management.