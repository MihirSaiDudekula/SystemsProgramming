### Background and Foreground Processes

#### Overview
In computing, managing **background** and **foreground processes** is crucial for efficient task handling in operating systems. This involves controlling whether a process executes in the foreground (actively receiving user input) or background (paused or running independently). Let's delve into the concepts and practicalities of these operations.

#### Creating a Simple Program
To understand these concepts better, let's create a basic program:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int x;
    printf("Input number: ");
    scanf("%d", &x);
    printf("Result: %d * 5 = %d\n", x, x * 5);
    sleep(4);
    return 0;
}
```
This program prompts the user for a number, multiplies it by 5, and then pauses execution for 4 seconds.

#### Moving Processes to Background
**Backgrounding** a process means pausing it without terminating. This can be achieved using the terminal command:
```
$ ./main
```
To background a running process, such as `./main`, use `Ctrl + Z`. This sends a **SIGTSTP** signal, pausing the process but not terminating it.

#### Bringing Processes to Foreground
To bring a background process back to the foreground, use the `fg` command:
```
$ fg
```
This sends a **SIGCONT** signal to resume the paused process, allowing it to continue execution.

#### Signal Handling
- **SIGTSTP (Ctrl + Z)**: Pauses the process and moves it to the background.
- **SIGCONT (fg command)**: Resumes execution of a backgrounded process.

#### Behind the Scenes
When a process is paused (`Ctrl + Z`), it stops at the current line of code, waiting for a signal to continue (`fg`). This distinction is crucial: **stopping** a process (pause) vs. **terminating** it (complete execution).

### Title: Handling Signals

**Overview:**
In this lesson, we delve into the concept of handling signals in Unix-based systems using C programming. Signals are mechanisms used to notify processes of certain events, such as user interrupts like `Ctrl-Z`. We explore how to customize signal handling to control process behavior.

---

**Background:**
Previously, we discussed sending processes to the background (`Ctrl-Z`) and bringing them back to the foreground. Now, we focus on modifying default signal handling behaviors to suit specific requirements.

---

**Understanding Signal Handling:**
Signals are crucial in Unix systems for asynchronous communication between processes and the operating system. They inform processes about events such as termination or interruption requests.

1. **Signal Handling in C:**
   - Signals can be managed using the `signal.h` library in C.
   - The `SIGACTION` structure (`struct sigaction`) is pivotal for defining custom signal handling routines.

2. **Custom Signal Handling Function:**
   - Define a handler function (`SIG Handler`) to manage specific signals.
   - Example:
     ```c
     void sig_handler(int signum) {
         printf("Stopped not allowed\n");
     }
     ```
   - This function executes when a `SIGSTP` signal is received (`Ctrl-Z`).

3. **Implementing Signal Handling:**
   - Initialize a `struct sigaction` (`sa`) to configure signal handling.
   - Example:
     ```c
     struct sigaction sa;
     sa.sa_handler = sig_handler;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = SA_RESTART;
     sigaction(SIGTSTP, &sa, NULL);
     ```
   - `SA_RESTART` ensures system calls interrupted by signals are restarted, maintaining program flow.

4. **Comparison with `signal` function:**
   - The `signal` function is an older approach to signal handling.
   - Use `sigaction` for better portability and additional features like preserving previous handlers.

5. **Special Cases - `SIGCONT`:**
   - Signals like `SIGCONT` (`Ctrl-C`) can be handled differently.
   - Example:
     ```c
     void sig_cont(int signum) {
         printf("Input number: ");
         fflush(stdout);
     }
     sigaction(SIGCONT, &sa, NULL);
     ```
   - This function prompts for user input upon process resumption from background (`fg`).

---

**Conclusion:**
Customizing signal handling in C provides flexibility in managing process behavior upon receiving signals. `sigaction` offers advantages over `signal`, ensuring code portability and enhanced functionality.

**Code Example:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int signum) {
    printf("Stopped not allowed\n");
}

void sig_cont(int signum) {
    printf("Input number: ");
    fflush(stdout);
}

int main() {
    struct sigaction sa_stp, sa_cont;
    
    // Handle SIGTSTP
    sa_stp.sa_handler = sig_handler;
    sigemptyset(&sa_stp.sa_mask);
    sa_stp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_stp, NULL);
    
    // Handle SIGCONT
    sa_cont.sa_handler = sig_cont;
    sigemptyset(&sa_cont.sa_mask);
    sa_cont.sa_flags = SA_RESTART;
    sigaction(SIGCONT, &sa_cont, NULL);
    
    // Example usage
    printf("Enter a number: ");
    int num;
    scanf("%d", &num);
    printf("Result: %d\n", num * 5);
    
    return 0;
}
```

This code snippet demonstrates handling `SIGTSTP` to prevent process suspension and `SIGCONT` to prompt user interaction upon process foregrounding.

---

This comprehensive approach equips programmers with the tools to manage signal behavior effectively in Unix environments using C programming. Understanding and customizing signal handling enhances control over process execution and user interaction.

# **What are Race Conditions?**

## **Introduction**
- **Race Condition**: A situation in concurrent programming where the outcome of the execution is dependent on the sequence or timing of uncontrollable events such as thread execution.

A race condition in threading occurs when two or more threads can access shared data and they try to change it at the same time. Since the thread scheduling algorithm can swap between threads at any time, you don’t know the order in which the threads will attempt to access the shared data. This can lead to unpredictable results and bugs that are difficult to reproduce.

## **Example Scenario**
- **Scenario Setup**: 
  - Create two threads that increment a shared variable `mails`.
  - Each thread simulates receiving mails and increments the `mails` variable.

## **Initial Example: Incrementing Once**
1. **Variable Initialization**:
   ```c
   int mails = 0;
   ```
2. **Thread Routine**:
   ```c
   void routine() {
       mails++;
   }
   ```
3. **Expected Outcome**: 
   - Each thread increments `mails` once.
   - Expected result after both threads execute: `2`.

## **Increasing Iterations**
1. **Modify Routine to Increment Multiple Times**:
   ```c
   void routine() {
       for (int i = 0; i < 100; i++) {
           mails++;
       }
   }
   ```
2. **Expected Outcome**: 
   - Each thread increments `mails` 100 times.
   - Expected result: `200`.

3. **Increasing to 1 Million Iterations**:
   - Modify loop to increment `mails` 1 million times.
   - Expected result: `2,000,000`.

## **Unexpected Result**
- When running the program with 1 million iterations, the result is incorrect and inconsistent (e.g., `1,411,192` instead of `2,000,000`).
- This discrepancy is due to **race conditions**.

## **Understanding the Race Condition**
- **Increment Operation**:
  1. **Read the value of `mails`**.
  2. **Increment the value**.
  3. **Write the incremented value back to `mails`**.

- **Single Thread Execution**:
  - One thread reads, increments, and writes back without interference, producing correct results.

- **Multiple Threads Execution**:
  - If two threads execute concurrently, interleaving of the read, increment, and write operations can occur.
  - This interleaving causes incorrect updates and inconsistent results.

## **Detailed Example with Two Threads**
1. **Initial Value**: `mails = 23`.
2. **Thread 1 Execution**:
   - Reads `mails = 23`.
   - Increments to `24`.
   - Writes back `24`.
3. **Thread 2 Execution**:
   - Reads `mails = 23` (before Thread 1 writes back).
   - Increments to `24`.
   - Writes back `24`.
   - Result after both threads: `24` (instead of `25`).

## **Assembly Code Insight**
- **Increment Operation in Assembly**:
  ```assembly
  movl mails, %eax  ; Read `mails` into CPU register
  incl %eax         ; Increment the value in the register
  movl %eax, mails  ; Write the incremented value back to `mails`
  ```
- This assembly code shows the three-step process: read, increment, and write.

## **Frequency of Race Conditions**
- **Low Iterations**: Less likely to experience race conditions due to fast completion of threads.
- **High Iterations**: Increased likelihood of interleaving and race conditions.

## **Significance of Data Integrity**
- **Critical Systems**: In environments like financial systems, race conditions can lead to significant errors and data corruption.
- Ensuring accurate data handling is crucial regardless of the probability of race conditions.

## **Conclusion**
- **Race Conditions**: Highlight the importance of managing concurrent access to shared resources.
- **Next Steps**: Explore solutions to prevent race conditions, such as synchronization mechanisms.

## **Sample Code**
```c
#include <stdio.h>
#include <pthread.h>

int mails = 0;

void* routine() {
    for (int i = 0; i < 1000000; i++) {
        mails++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, routine, NULL);
    pthread_create(&t2, NULL, routine, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Number of mails: %d\n", mails);
    return 0;
}
```

- **Expected Output**: With correct synchronization, the output should be `2000000`, but without it, results will vary due to race conditions.

reason why pthread wants pointers and not direct funcs:

Avoiding Data Copying: Passing pointers rather than actual values to threads avoids the overhead of copying data. When you pass a pointer to a function, you're essentially passing a reference to the data, not the data itself. This is especially beneficial when dealing with large or complex data structures, as copying them would consume more memory and time.

Thread Safety: Since threads typically share the same memory space (in the case of multi-threading within a single process), passing pointers ensures that threads can access and manipulate the same data concurrently. Passing by value would create separate copies for each thread, potentially leading to inconsistencies or synchronization issues.