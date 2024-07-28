### Notes on "Short Introduction to Threads (pthreads)"

**1. Introduction to Threads**
- Threads in Linux are managed using **pthreads** (POSIX threads), facilitating concurrent execution.
- Threads allow programs to perform multiple tasks simultaneously, enhancing efficiency.

understanding through analogy
-
single thread - work that is assigned is performed one after the other
; its like a chef who cooks the first dish for the first customer, and then the next dish only after the first dish is finished for the second customer

multiple threads - work that is assigned is split between threads
;procedure:
- create 2 threads
- give them work to do - first threas does the first work and the other 
does the other 

ex:
- 2 chefs, is chef cooks for customer 1
- second chef cooks for customer 2
- both are working simultaneously
- so customer2 doesnt have to wait for customer1's dish to be finished

https://www.youtube.com/watch?v=mvZKu0DfFLQ

**2. Setting Up**
- Include the pthread header file: `#include <pthread.h>`
- Specify the linker option `-pthread` during compilation to enable thread support.

**3. Creating Threads**
- **pthread_create** function initializes a thread:
  ```c
  pthread_t t1; // Declare pthread variable
  pthread_create(&t1, NULL, routine, NULL);
  ```
  - **Parameters**:
    - `&t1`: Pointer to thread variable.
    - `NULL`: Default thread attributes.
    - `routine`: Function executed by the thread.
    - `NULL`: Arguments passed to the function (none in this case).

**4. Defining the Routine**
- Example routine:
  ```c
  void *routine(void *arg) {
      printf("Test from thread\n");
      // Additional functionality can be added here
      return NULL;
  }
  ```

**5. Synchronizing Threads**
- **pthread_join** waits for a thread to complete execution:
  ```c
  pthread_join(t1, NULL);
  ```
  - Ensures main program waits for thread completion before continuing.
  - the second argument is used to store some incoming result, but since there is no such result, we simply pass NULL

**6. Creating Multiple Threads**
- Additional threads can be created similarly:
  ```c
  pthread_t t2;
  pthread_create(&t2, NULL, routine, NULL);
  pthread_join(t2, NULL);
  ```
  - Allows concurrent execution of multiple routines.

**7. Ensuring Error Handling**
- **pthread_create** and related functions should be checked for errors:
  ```c
  if (pthread_create(&t1, NULL, routine, NULL) != 0) {
      perror("pthread_create");
      return 1; // Error handling
  }
  ```

**8. Parallelism Verification**
- Use of `printf` and `sleep` to verify parallel execution:
  ```c
  printf("Test from thread\n");
  sleep(3); // Simulate thread activity
  printf("Ending thread\n");
  ```

**10. Code Sample**
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *routine(void *arg) {
    printf("Test from thread\n");
    sleep(3); // Simulate thread activity
    printf("Ending thread\n");
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    if (pthread_create(&t1, NULL, routine, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    if (pthread_create(&t2, NULL, routine, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    return 0;
}
```

**Summary:**
- Understanding pthreads in C under Linux involves including `pthread.h`, setting linker options, creating threads with `pthread_create`, synchronizing with `pthread_join`, and ensuring error handling for robust application design.

### Overview of `pthread_t`

**`pthread_t`** is a data type used in the POSIX threads (pthreads) library, which provides a way to handle threads in a program. This data type is crucial for thread management in multi-threaded applications. 

#### **Definition and Usage**

- **Data Type**: `pthread_t` is an opaque data type, which means its internal representation is not specified. This allows different implementations to use different underlying data structures.
- **Common Representation**: In many implementations, `pthread_t` is represented as an `unsigned long`, but this is not guaranteed. 

#### **Printing `pthread_t` Values**

- **Printing**: While `pthread_t` may often be printed as an `unsigned long`, this practice is not recommended for production code due to its opaque nature.
  ```c
  printf("%lu", (unsigned long)thread_id);
  ```
- **Function to Get Current Thread ID**: Use `pthread_self()` to obtain the `pthread_t` of the current thread.
  ```c
  pthread_t current_thread = pthread_self();
  printf("%lu", (unsigned long)current_thread);
  ```

#### **Thread Creation and Identification**

- **Thread Creation**: When you create threads using `pthread_create()`, the `pthread_t` value for each thread is set by the system.
  ```c
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, thread_function, NULL);
  pthread_create(&thread2, NULL, thread_function, NULL);
  ```
- **Thread Identification**: The `pthread_join()` function uses `pthread_t` to identify and manage threads.
  ```c
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ```

#### **Linux-Specific Thread ID**

- **`gettid()` Function**: In Linux, you can use `syscall(SYS_gettid)` to obtain the thread ID, which is managed by the Linux kernel. This ID is different from `pthread_t` and is specific to the operating system.
  ```c
  #include <sys/syscall.h>
  #include <unistd.h>
  pid_t linux_thread_id = syscall(SYS_gettid);
  printf("%d", linux_thread_id);
  ```

#### **Differences Between `pthread_t` and OS Thread IDs**

- **`pthread_t`**: Managed at the pthread library level, used for thread management within the program.
- **OS Thread ID**: Managed by the operating system, specific to Linux, and provides a way to interact with kernel-level thread management.

#### **Key Points**

- **Opaque Data Type**: Do not assume `pthread_t` is an `unsigned long` or any specific type.
- **Different Numbers**: `pthread_t` and OS-specific thread IDs may differ; they are not equivalent.
- **Documentation**: Consult the pthread and system documentation for details on thread management and identification.

### Example Code

Below is an example demonstrating how to print both `pthread_t` and Linux thread ID:

```c
#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

void* thread_function(void* arg) {
    pthread_t self = pthread_self();
    pid_t linux_thread_id = syscall(SYS_gettid);
    
    printf("pthread_t: %lu\n", (unsigned long)self);
    printf("Linux thread ID: %d\n", linux_thread_id);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
```

### Summary

- **`pthread_t`** is used for thread management within the pthread library.
- **Linux thread IDs** can be retrieved using `syscall(SYS_gettid)` and represent the kernel-level thread ID.
- **Do not** assume the internal representation of `pthread_t` and use it as an opaque handle for thread operations.

Feel free to ask any questions or request further clarifications if needed!

### Title: What is a mutex in C? (pthread_mutex)

#### Overview
In this lecture, we delve into the concept of **mutex** in C programming, specifically using **pthread_mutex** from the POSIX thread API. Mutexes are essential for managing concurrent access to shared resources and preventing race conditions in multithreaded environments.

---

#### Understanding the Problem: Race Conditions
- **Race Condition**: Occurs when multiple threads access and manipulate shared data concurrently, leading to unpredictable outcomes.
- Example: Incrementing a variable `males` in multiple threads without synchronization can yield incorrect results due to simultaneous reads and writes.

---

#### Introduction to Mutex
- **Mutex**: Short for "mutual exclusion", it is a synchronization mechanism used to ensure that only one thread can access a shared resource at a time.
- **Purpose**: Prevents multiple threads from executing a critical section of code simultaneously, thus ensuring data integrity.

---

#### Implementation Strategy
1. **Mutex Initialization**:
   - Use `pthread_mutex_init` to initialize a mutex object.
   - Syntax: `pthread_mutex_init(&mutex, NULL);`
   - This prepares the mutex for use and sets initial attributes to default.

2. **Using the Mutex**:
   - **Locking**: Use `pthread_mutex_lock` to acquire the mutex before accessing critical sections.
     - Syntax: `pthread_mutex_lock(&mutex);`
   - **Unlocking**: Use `pthread_mutex_unlock` to release the mutex after finishing critical operations.
     - Syntax: `pthread_mutex_unlock(&mutex);`

3. **Example Scenario**:
   - Before accessing shared variable `males`, lock the mutex.
   - Perform operations on `males`.
   - Unlock the mutex once operations are complete.

---

#### Benefits of Mutex
- **Ensures Data Integrity**: Prevents simultaneous access by multiple threads, avoiding race conditions.
- **Thread Safety**: Guarantees that critical sections execute atomically and in the correct order.

---

#### Practical Application
- **Example**: Protecting increment operations on `males` with mutex ensures correct results even with multiple threads.
- **Performance Consideration**: Although mutexes introduce overhead due to thread synchronization, they are crucial for maintaining program correctness in multithreaded environments.

---

#### Conclusion
- **Importance of Mutex**: Vital for managing shared resources in multithreaded programs to prevent race conditions.
- **Best Practices**: Always use mutexes when accessing shared data across threads to ensure program correctness.
- **Future Considerations**: Understanding when and why to use mutexes optimally enhances program efficiency and reliability.

---

#### Code Example
```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;

void *thread_function(void *arg) {
    // Critical section
    pthread_mutex_lock(&mutex);
    // Operations on shared resource
    // Example: Incrementing a shared variable
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);

    // Create threads and perform operations

    pthread_mutex_destroy(&mutex);
    return 0;
}
```

This code snippet demonstrates the basic usage of pthread mutex for ensuring thread-safe access to shared resources.

### How to Create Threads in a Loop (pthread_create)

**Introduction**

In this tutorial, we'll explore how to efficiently create and manage threads using `pthread_create` in C programming. Often, developers face the challenge of spawning multiple threads in a loop without duplicating code unnecessarily.

**Understanding the Issue**

Previously, we encountered situations where individual `pthread_create` and `pthread_join` calls were repeated multiple times in the main function, leading to duplicated code. This approach isn't scalable when we need to create a variable number of threads dynamically.

**Optimizing with Arrays**

To address this, we can utilize arrays to store thread handles (`pthread_t`). This allows us to manage multiple threads more efficiently in a loop.

```c
pthread_t th[4]; // Array to store thread handles
```

**Creating Threads in a Loop**

Instead of manually creating threads like `pthread_create(&p1, NULL, routine, NULL);`, we can loop through an array and create threads using a single loop:

```c
for (int i = 0; i < 4; ++i) {
    pthread_create(&th[i], NULL, routine, NULL);
}
```

This approach eliminates code duplication and simplifies the process of thread creation.

**Joining Threads**

Similarly, to wait for all threads to complete their execution, we can use another loop to join each thread:

```c
for (int i = 0; i < 4; ++i) {
    pthread_join(th[i], NULL);
}
```

By separating the creation and joining of threads into distinct loops, we ensure that threads can execute concurrently, achieving true parallelism.

**Execution Order Observation**

It's important to note that threads may not finish execution in the order they were created. This is due to the nature of thread scheduling and execution times, which can vary.

**Example Output**

After implementing this approach, executing the program will demonstrate threads starting and finishing execution in parallel, utilizing the system's capabilities effectively.

```c
printf("Thread %d has started execution.\n", i);
printf("Thread %d has finished execution.\n", i);
```

**Conclusion**

By adopting this method, developers can enhance the efficiency of multi-threaded programs, leveraging hardware capabilities to maximize performance.

---

**Source Code**

Here is the complete source code for creating and joining threads in a loop:

```c
#include <pthread.h>
#include <stdio.h>

void *routine(void *arg) {
    // Thread routine implementation
    return NULL;
}

int main() {
    pthread_t th[4]; // Array to store thread handles

    // Creating threads
    for (int i = 0; i < 4; ++i) {
        pthread_create(&th[i], NULL, routine, NULL);
        printf("Thread %d has started execution.\n", i);
    }

    // Joining threads
    for (int i = 0; i < 4; ++i) {
        pthread_join(th[i], NULL);
        printf("Thread %d has finished execution.\n", i);
    }

    return 0;
}
```

This code efficiently creates and manages threads in a loop, ensuring parallel execution and optimal resource utilization.

### Title: Get return value from a thread (*pthread_join*)

In this lecture, we delve into the process of retrieving return values from threads using *pthread_join*. Let's break down the key concepts and steps involved:

#### Creating and Using Threads

To begin, we create a thread using *pthread_t*. Our goal is to roll a dice and return the result:

```c
pthread_t th;

// Function to simulate rolling a dice
void* roll_dice(void* arg) {
    int* result = malloc(sizeof(int));  // Allocate memory for the result

    // Simulate rolling a dice
    *result = (rand() % 6) + 1;

    // Return the result
    return result;
}
```

#### Generating Random Numbers

Before rolling the dice, we need to initialize the random number generator:

```c
#include <stdlib.h>
#include <time.h>

// Initialize the random number generator
srand(time(NULL));
```

#### Obtaining and Printing Results

In the main function, after creating the thread, we use *pthread_join* to retrieve the result:

```c
int* res;  // Pointer to store result

// Create thread to roll dice
pthread_create(&th, NULL, roll_dice, NULL);

// Join the thread and get the result
pthread_join(th, (void**)&res);

// Print the result
printf("Result: %d\n", *res);

// Free allocated memory
free(res);
```

#### Explanation of *pthread_join*

- *pthread_join* allows the main thread to wait for the completion of the specified thread (*th*) and retrieve its return value.
- It takes two arguments: the thread identifier (*th*) and a double pointer to void. This pointer will be set to the return value of the thread function (*roll_dice*).

#### Handling Return Values

- Since *pthread_join* expects a void pointer as the return type, we cast the integer pointer (*int\***) returned from *roll_dice* appropriately.
- Memory allocation (*malloc*) ensures that the returned pointer (*result*) remains valid even after the thread exits.

#### Memory Management

- It's crucial to manage memory properly by deallocating dynamically allocated memory (*free(res)*) once it's no longer needed in the main function.

#### Conclusion and Further Exploration

This method allows us to effectively retrieve results from threads in C. For larger applications, consider architectural implications and ensure proper memory management practices.

---

#### Generated Code:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void* roll_dice(void* arg) {
    int* result = malloc(sizeof(int));
    *result = (rand() % 6) + 1;
    return result;
}

int main() {
    pthread_t th;
    srand(time(NULL));

    int* res;
    pthread_create(&th, NULL, roll_dice, NULL);
    pthread_join(th, (void**)&res);
    printf("Result: %d\n", *res);
    free(res);

    return 0;
}
```
### Notes on "How to Pass Arguments to Threads in C"

**Introduction to Thread Argument Passing**
- Passing values to threads in C involves utilizing the **pthread** API.
- This enables each thread to receive unique data necessary for its execution.

**Defining the Problem**
- Scenario: Need to print prime numbers using multiple threads.
- Utilize a global array **primes** containing prime numbers.

**Creating Threads**
- Use **pthread_t** array `th` of size 10 for 10 threads.
- Loop through 0 to 9 to create threads using `pthread_create`.
  ```c
  for (int i = 0; i < 10; ++i) {
      if (pthread_create(&th[i], NULL, routine_function, &i) != 0) {
          perror("Failed to create thread");
      }
  }
  ```

**Routine Function Definition**
- Function signature:
  ```c
  void *routine_function(void *arg);
  ```
- Accepts a **void pointer** argument to receive thread-specific data.

**Passing Arguments to Threads**
- Utilize the address of the index (`&i`) to pass unique values to each thread.
- Cast the void pointer back to an integer pointer within the routine function:
  ```c
  int index = *((int *)arg);
  ```
- Access the `primes` array using the index to print the corresponding prime number.

**Thread Execution Issues**
- **Concurrency Issue:** Threads may start execution with incorrect or duplicate data due to shared memory.
- Issue is exacerbated when threads are created faster than they start executing.
- Solution involves dynamically allocating memory for each thread's index to ensure uniqueness.

**Dynamic Memory Allocation**
- Allocate memory for `int` pointer `a` inside the loop before creating each thread:
  ```c
  int *a = malloc(sizeof(int));
  *a = i;
  ```
- Pass `a` to `pthread_create` instead of `&i`.
- Free the allocated memory within each thread's routine function:
  ```c
  free(arg);
  ```

**Conclusion**
- Implementing dynamic memory allocation ensures each thread receives the intended unique value without interference.
- Achieves correct functionality in a multi-threaded environment.

**Summary**
- The process involves creating threads with unique arguments using pthreads in C.
- Dynamic memory allocation resolves concurrency issues associated with shared memory.

**Additional Insights**
- Further optimizations can involve exploring simpler methods to avoid shared memory pitfalls in multi-threaded environments.

**Sample Code**
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void *routine_function(void *arg) {
    int index = *((int *)arg);
    printf("%d ", primes[index]);
    free(arg);
    return NULL;
}

int main() {
    pthread_t th[10];
    for (int i = 0; i < 10; ++i) {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, routine_function, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < 10; ++i) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    return 0;
}
```

This setup ensures each thread prints exactly one prime number from the `primes` array, demonstrating effective thread argument passing in C.

### Explanation of the Problem without `malloc`

When creating threads in a loop and passing the loop index as an argument, if we do not use `malloc`, we might encounter issues due to the shared memory problem. Let's see an example of this problem:

#### Code without `malloc`:
```c
#include <stdio.h>
#include <pthread.h>

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// Thread routine function
void *routine_function(void *arg) {
    int index = *(int *)arg;
    printf("%d ", primes[index]);
    return NULL;
}

int main() {
    pthread_t th[10];
    int index[10]; // Array to hold indices

    for (int i = 0; i < 10; ++i) {
        index[i] = i;
        if (pthread_create(&th[i], NULL, routine_function, &index[i]) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < 10; ++i) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    return 0;
}
```

### Explanation of the Problem:
- **Issue**: The `index` array is shared among all threads. By the time a thread starts execution, the loop might have advanced, changing the value at `index[i]`.
- **Result**: Threads may read incorrect or unexpected values because the loop might have modified `index[i]` before the thread uses it.

### Fixing the Problem using `malloc`

To fix this problem, we can allocate memory dynamically for each thread's argument. This ensures that each thread gets its own copy of the argument, avoiding the shared memory issue.

#### Code with `malloc`:
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

// Thread routine function
void *routine_function(void *arg) {
    int index = *(int *)arg;
    printf("%d ", primes[index]);
    free(arg); // Free the allocated memory to prevent memory leaks
    return NULL;
}

int main() {
    pthread_t th[10];

    for (int i = 0; i < 10; ++i) {
        int *a = (int *)malloc(sizeof(int)); // Allocate memory for the thread argument
        if (a == NULL) { // Check if memory allocation was successful
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        *a = i; // Set the value of the allocated memory to the current index

        if (pthread_create(&th[i], NULL, routine_function, a) != 0) {
            perror("Failed to create thread");
            free(a); // Free allocated memory if thread creation fails
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < 10; ++i) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
```

### Explanation of the Fix:
1. **Dynamic Memory Allocation**:
   - `int *a = (int *)malloc(sizeof(int));`: Allocates memory for each thread's argument.
   - `*a = i;`: Sets the value of the allocated memory to the current loop index.

2. **Thread Creation**:
   - `pthread_create(&th[i], NULL, routine_function, a);`: Creates a new thread with the dynamically allocated argument.

3. **Memory Management**:
   - `free(arg);`: Frees the allocated memory inside the thread routine after it is no longer needed.

4. **Error Handling**:
   - Check if `malloc` is successful and handle errors properly.
   - If `pthread_create` fails, free the allocated memory to prevent memory leaks.

By using `malloc`, each thread gets a separate copy of the argument, ensuring that there is no shared memory issue, and each thread works with the correct index value.

### Notes on "Difference between trylock and lock in C"

**Overview:**
The discussion focuses on the differences between `pthread_mutex_lock` and `pthread_mutex_trylock` functions in C, specifically regarding their behavior in managing mutexes.

**pthread_mutex_lock:**
- Used to acquire a mutex lock.
- **Blocks** the calling thread if the mutex is already locked by another thread.
- Ensures mutual exclusion.
- Example:
  ```c
  pthread_mutex_lock(&mutex);
  // Critical section
  pthread_mutex_unlock(&mutex);
  ```

**Behavior Example:**
- Creates four threads, each attempting to lock the mutex.
- Only one thread successfully acquires the lock at a time.
- Others wait until the mutex is unlocked.
- Shows sequential execution of "got lock" messages.

**pthread_mutex_trylock:**
- Attempts to acquire a mutex lock **without blocking**.
- Returns immediately:
  - **Returns 0** if the lock is acquired successfully.
  - **Returns EBUSY (16)** if the mutex is already locked.
- Requires checking the return value to determine success.
- Example:
  ```c
  if (pthread_mutex_trylock(&mutex) == 0) {
      // Critical section
      pthread_mutex_unlock(&mutex);
  } else {
      printf("Didn't get the lock\n");
  }
  ```

**Comparison:**
- **pthread_mutex_lock** waits until the lock is acquired.
- **pthread_mutex_trylock** returns immediately with a result indicating success or failure.
- Suitable for scenarios where immediate action is needed or handling alternatives if lock is unavailable.

**Conclusion:**
- `pthread_mutex_lock` guarantees lock acquisition but may block.
- `pthread_mutex_trylock` attempts immediate lock acquisition and returns a result indicating success or failure.

**Example Implementation:**
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg) {
    if (pthread_mutex_trylock(&mutex) == 0) {
        printf("Got the lock\n");
        // Simulate time-consuming task
        sleep(1);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Didn't get the lock\n");
    }
    return NULL;
}

int main() {
    pthread_t threads[4];
    int i;
    for (i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }
    for (i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
```

**Analysis:**
- `pthread_mutex_lock` ensures synchronized access but may delay execution.
- `pthread_mutex_trylock` provides immediate feedback, useful for non-blocking scenarios.

**Additional Notes:**
- Understanding these functions is crucial for efficient resource management in concurrent programming.

### Advanced Notes on Condition Variables in C

**Introduction to Condition Variables:**

Condition variables in C, part of the pthread API, facilitate synchronization between threads based on certain conditions rather than simple locks.

**Scenario Introduction:**

Imagine a gas station simulation where one thread fuels the station (`fuel_filling`), and another thread waits to fuel its car (`car_waiting`). The goal is to coordinate these actions using condition variables.

**Initial Setup:**

- **Global Variables:**
  - `fuel`: Initialized to 0, representing the amount of fuel available.
  
- **Mutex Initialization:**
  - A mutex (`mutex_fuel`) is created to ensure thread-safe access to `fuel`.

```c
pthread_mutex_t mutex_fuel = PTHREAD_MUTEX_INITIALIZER;
int fuel = 0;
```

**Thread Creation:**

Two threads are created using `pthread_create`:
- One for `fuel_filling`
- Another for `car_waiting`

```c
pthread_t threads[2];

for (int i = 0; i < 2; ++i) {
    if (i == 0)
        pthread_create(&threads[i], NULL, fuel_filling, NULL);
    else
        pthread_create(&threads[i], NULL, car_waiting, NULL);
}

for (int i = 0; i < 2; ++i) {
    pthread_join(threads[i], NULL);
}
```

**Fuel Filling Thread (`fuel_filling`):**

This thread increments `fuel` by 15 units, simulating fueling.

```c
void *fuel_filling(void *arg) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&mutex_fuel);
        fuel += 15;
        printf("Filling fuel: %d\n", fuel);
        pthread_mutex_unlock(&mutex_fuel);
        sleep(1); // Simulating a break after each fill
    }
    return NULL;
}
```

**Car Waiting Thread (`car_waiting`):**

This thread decrements `fuel` by 40 units when there's enough fuel available.

```c
void *car_waiting(void *arg) {
    while (fuel < 40) {
        printf("No fuel, waiting...\n");
        sleep(1);
    }
    pthread_mutex_lock(&mutex_fuel);
    fuel -= 40;
    printf("Got fuel, remaining: %d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);
    return NULL;
}
```

**Issue with Current Implementation:**

- The `car_waiting` thread enters a busy wait (`while` loop), which is inefficient and blocks the CPU.

**Introducing Condition Variables:**

Condition variables (`cond_fuel`) help optimize thread synchronization without busy waiting.

- **Condition Variable Initialization:**

```c
pthread_cond_t cond_fuel = PTHREAD_COND_INITIALIZER;
```

**Modifying `car_waiting` with Condition Variables:**

Replace the busy wait with `pthread_cond_wait`:
- This allows the thread to wait efficiently until signaled by `fuel_filling`.

```c
void *car_waiting(void *arg) {
    pthread_mutex_lock(&mutex_fuel);
    while (fuel < 40) {
        printf("No fuel, waiting...\n");
        pthread_cond_wait(&cond_fuel, &mutex_fuel);
    }
    fuel -= 40;
    printf("Got fuel, remaining: %d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);
    return NULL;
}
```

**Signaling the Condition:**

After each fuel increment in `fuel_filling`, signal waiting threads to check the condition again.

```c
void *fuel_filling(void *arg) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&mutex_fuel);
        fuel += 15;
        printf("Filling fuel: %d\n", fuel);
        pthread_cond_signal(&cond_fuel);
        pthread_mutex_unlock(&mutex_fuel);
        sleep(1); // Simulating a break after each fill
    }
    return NULL;
}
```

**Conclusion:**

Condition variables provide a mechanism for threads to efficiently wait for specific conditions rather than busy-waiting, enhancing synchronization in multi-threaded programming.

---

**Code Implementation:**

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_fuel = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_fuel = PTHREAD_COND_INITIALIZER;
int fuel = 0;

void *fuel_filling(void *arg) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&mutex_fuel);
        fuel += 15;
        printf("Filling fuel: %d\n", fuel);
        pthread_cond_signal(&cond_fuel);
        pthread_mutex_unlock(&mutex_fuel);
        sleep(1); // Simulating a break after each fill
    }
    return NULL;
}

void *car_waiting(void *arg) {
    pthread_mutex_lock(&mutex_fuel);
    while (fuel < 40) {
        printf("No fuel, waiting...\n");
        pthread_cond_wait(&cond_fuel, &mutex_fuel);
    }
    fuel -= 40;
    printf("Got fuel, remaining: %d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);
    return NULL;
}

int main() {
    pthread_t threads[2];

    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            pthread_create(&threads[i], NULL, fuel_filling, NULL);
        else
            pthread_create(&threads[i], NULL, car_waiting, NULL);
    }

    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```

**Summary:**

Condition variables provide a powerful tool for managing synchronization in multi-threaded applications, allowing threads to efficiently wait for specific conditions to be met without wasting CPU cycles. Understanding their usage is crucial for effective concurrent programming in C.

### Understanding `pthread_exit` in POSIX Threads

In this discussion, we explore the `pthread_exit` function in the context of POSIX threads, comparing its usage with traditional `return` statements in thread functions and the `exit` function in the main thread.

#### Key Concepts

1. **POSIX Threads (pthreads):**
   - **POSIX Threads** are a standard set of threading APIs defined by the POSIX (Portable Operating System Interface) specification. They allow for the creation and management of threads in a C program.

2. **`pthread_exit`:**
   - The function `**pthread_exit**` is used to terminate a thread and optionally provide a return value. It allows a thread to end its execution and return a value to any thread that may join it.

3. **Thread Function Return Value:**
   - Threads typically return a value using the `return` statement in their function. This value is often accessed by the thread that joins (waits for) the terminated thread.
   - Example:
     ```c
     void* thread_func(void* arg) {
         int result = rand() % 6 + 1;
         pthread_exit((void*)(intptr_t)result);
     }
     ```

4. **Comparison with `return` and `exit`:**
   - **`return`:** In a thread function, using `return` ends the thread and can return a value to the thread that joins it.
   - **`exit`:** Calling `exit` in the main function terminates the entire process, including all its threads. This might not be desirable if threads need to finish their work.

#### Practical Differences and Usage

1. **Using `pthread_exit` in Thread Functions:**
   - Allows the thread to exit and optionally pass a return value to a thread that joins it.
   - Example:
     ```c
     void* thread_func(void* arg) {
         int result = rand() % 6 + 1;
         pthread_exit((void*)(intptr_t)result);
     }
     ```

2. **Using `pthread_exit` in the Main Function:**
   - When `pthread_exit` is called in the main function, it prevents the main thread from exiting immediately, allowing other threads to continue executing.
   - This is useful when you want to let the threads complete their tasks before the entire process terminates.
   - Example:
     ```c
     int main() {
         pthread_t thread;
         pthread_create(&thread, NULL, thread_func, NULL);
         pthread_exit(NULL);  // Waits for all threads to finish
         return 0;  // This line is never reached
     }
     ```

3. **Impact of `return` and `exit` in the Main Function:**
   - **`return` 0:** Exiting the main function with `return` terminates the entire process, causing all threads to stop executing.
   - **`exit(0)`:** Similar to `return`, `exit` ends the entire process immediately, which is not suitable if threads are still running and need to complete their tasks.

4. **Handling Memory Leaks:**
   - When using `pthread_exit`, ensure proper memory management. In the provided example, a memory leak might occur if allocated memory is not freed.
   - Example of memory management:
     ```c
     void* thread_func(void* arg) {
         int* result = malloc(sizeof(int));
         if (result == NULL) return NULL;
         *result = rand() % 6 + 1;
         pthread_exit((void*)result);
     }
     ```

#### Example Code

Here’s a complete example demonstrating the use of `pthread_exit`:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread_func(void* arg) {
    int* result = malloc(sizeof(int));
    if (result == NULL) return NULL;
    *result = rand() % 6 + 1;
    pthread_exit((void*)result);
}

int main() {
    pthread_t thread;
    void* status;

    pthread_create(&thread, NULL, thread_func, NULL);
    
    // Wait for the thread to finish and retrieve the result
    pthread_join(thread, &status);

    // Print the result from the thread
    printf("Thread result: %d\n", (int)(intptr_t)status);

    // Free the allocated memory
    free(status);

    return 0;
}
```

#### Summary

- **`pthread_exit`** allows a thread to exit and optionally return a value without terminating the entire process.
- **`return`** and **`exit`** are used to terminate the main function but also terminate all threads if not handled properly.
- Proper memory management is crucial when using `pthread_exit` to avoid memory leaks. 

Feel free to use `pthread_exit` when you need to manage the lifecycle of threads more precisely without affecting the main process's termination.

## Introduction to Barriers (`pthread_barrier`)

### **Overview**

A **barrier** in threading is a synchronization tool used to ensure that multiple threads reach a certain point in their execution before any of them continue. This is useful when you need threads to work in tandem, ensuring all threads are at a specific state before proceeding.

### **Concept of Barriers**

- **Barrier Purpose**: To make threads wait until a specific number of them have reached the barrier before allowing any of them to proceed.
- **How It Works**: Threads call a **barrier wait** function and block until the designated number of threads have also called this function.

### **Barrier Example**

Consider a scenario with multiple threads and a barrier set to a certain count. Threads will wait at the barrier until the required number of threads (e.g., three) are all waiting. Only then will all threads pass through the barrier and continue execution.

### **Code Implementation**

Here's a step-by-step guide on how to implement barriers using the **pthread** library in C.

#### **1. Define and Initialize the Barrier**

```c
#include <pthread.h>
#include <stdio.h>

// Define a barrier
pthread_barrier_t barrier;

void* threadFunction(void* arg) {
    printf("Thread waiting at the barrier\n");
    pthread_barrier_wait(&barrier);
    printf("Thread passed the barrier\n");
    return NULL;
}

int main() {
    // Number of threads
    const int num_threads = 3;

    // Initialize the barrier with the number of threads
    pthread_barrier_init(&barrier, NULL, num_threads);

    pthread_t threads[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, threadFunction, NULL);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the barrier
    pthread_barrier_destroy(&barrier);

    return 0;
}
```

#### **2. Explanation of Code**

- **Define Barrier**: `pthread_barrier_t barrier;` creates a barrier variable.
- **Initialize Barrier**: `pthread_barrier_init(&barrier, NULL, num_threads);` initializes the barrier. The `num_threads` parameter specifies the count of threads that must wait at the barrier.
- **Barrier Wait**: `pthread_barrier_wait(&barrier);` is used by threads to wait at the barrier. Once the specified number of threads has called this function, they all proceed.
- **Destroy Barrier**: `pthread_barrier_destroy(&barrier);` cleans up the barrier when it's no longer needed.

### **Considerations**

- **Thread Count**: Ensure the number of threads specified in `pthread_barrier_init` matches the actual number of threads waiting at the barrier to avoid deadlocks.
- **Multiple Waits**: Threads can wait at the same barrier multiple times if designed to do so, often used in repeated synchronization tasks.

### **Advanced Usage**

- **Dynamic Wait Counts**: You can adjust the number of threads required for the barrier dynamically if needed by using barrier attributes (not covered here).
- **Handling Execution Flow**: Use barriers to control execution flow and synchronization in more complex scenarios involving multiple groups of threads.

### **Common Issues**

- **Deadlock**: If the number of threads at the barrier is less than required, threads will wait indefinitely. Ensure the thread count is correctly managed.
- **Intermingled Output**: Output from threads may be interleaved due to concurrent execution. This is normal and does not affect barrier synchronization.

### **Conclusion**

The barrier concept in **pthread** helps synchronize threads, ensuring a defined number of threads reach a synchronization point before proceeding. This is crucial for coordinating complex multi-threaded operations.

Feel free to use this example and expand upon it according to your specific needs and the complexity of your threading tasks.

# Detached Threads in Pthreads

## Introduction
**Detached threads** are a concept in multithreading where a thread is run independently of the main thread, and the main thread does not need to wait for it to finish. This is useful for long-running threads that do not need to be joined or waited on.

## Key Concepts

### **Detaching a Thread**
- **Detaching** a thread means that it runs independently and cannot be joined later. This is achieved using the `pthread_detach()` function.
- **Function Syntax:** `pthread_detach(pthread_t thread);`
  - **Parameter:** `pthread_t thread` - the thread to be detached.
  - **Behavior:** Once detached, the thread’s resources are automatically released when the thread terminates. It is no longer joinable.

### **Implications of Detached Threads**
- **Cannot Join Detached Threads:** Once a thread is detached, you cannot use `pthread_join()` to wait for it to complete.
- **Automatic Resource Cleanup:** Detached threads automatically clean up their resources upon completion, which means you do not need to explicitly join them to avoid resource leaks.
- **Potential Issues:** If the main thread exits before detached threads complete their execution, the process will terminate, and the detached threads will be killed prematurely.

### **Using `pthread_exit()` for Proper Termination**
- To ensure that detached threads complete their execution before the process terminates, use `pthread_exit()` instead of `return` in the `main` function.
- **Function Syntax:** `pthread_exit(void *retval);`
  - **Example:** `pthread_exit(NULL);`
  - **Behavior:** This function blocks the main thread from terminating until all other threads have finished execution.

### **Creating Detached Threads Directly**
- You can create a thread in a detached state directly using thread attributes.
- **Steps:**
  1. Create a `pthread_attr_t` object.
  2. Initialize the attributes using `pthread_attr_init()`.
  3. Set the thread’s detach state using `pthread_attr_setdetachstate()`.
  4. Create the thread with the attributes.
  5. Destroy the attribute object.

#### **Code Example: Creating a Detached Thread**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_function(void *arg) {
    sleep(1);
    printf("Thread finished execution\n");
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_attr_t attr;

    // Initialize the attribute object
    pthread_attr_init(&attr);
    
    // Set the thread to be detached
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    // Create the thread with the specified attributes
    pthread_create(&thread, &attr, thread_function, NULL);

    // Destroy the attribute object
    pthread_attr_destroy(&attr);
    
    // Wait for the detached thread to finish
    pthread_exit(NULL);

    return 0;
}
```

### **When to Use Detached Threads**
- **Long-Running Background Tasks:** Use detached threads for tasks that run independently and whose completion is not required by the main thread.
- **Resource Management:** Helps in managing resources efficiently without the need for explicit joins.

### **Setting Thread Attributes**
- **`pthread_attr_t`**: Allows for specifying thread attributes like stack size, scheduling policy, and detach state.
- **Common Attributes:**
  - **Stack Size:** `pthread_attr_setstacksize()`
  - **Scheduling Policy:** `pthread_attr_setschedpolicy()`
  - **Detach State:** `pthread_attr_setdetachstate()`

## Summary
Detached threads are an effective way to manage threads that do not require synchronization with the main thread. By using `pthread_detach()` or setting attributes during thread creation, you can avoid having to join threads and ensure that resources are cleaned up properly.

For a more thorough understanding, exploring other thread attributes and their impacts on thread behavior can be beneficial.

Static Initializers in the PTHREAD API
======================================

Overview
--------

Static initializers in the PTHREAD API simplify the initialization of certain threading objects. This can enhance performance and reduce the amount of code needed for creating and managing threads and synchronization primitives.

Mutex Initialization
--------------------

### Traditional Method

1.  **Define the Mutex**:
    
    ```
    c

    

    `pthread_mutex_t mutex;`
    ```

2.  **Initialize the Mutex**:
    
    ```
    c

    

    `pthread_mutex_init(&mutex, NULL);`
    ```

3.  **Destroy the Mutex**:
    
    ```
    c

    

    `pthread_mutex_destroy(&mutex);`
    ```

    -   The `pthread_mutex_destroy()` function is good practice to call, but not always mandatory as many systems automatically clean up resources.

### Improved Method with Static Initializers

1.  **Static Initialization**:
    
    ```
    c

    

    `pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;`
    ```

    -   **`PTHREAD_MUTEX_INITIALIZER`** is a macro that provides default values for the mutex. This avoids the need to call `pthread_mutex_init()`.
2.  **Destruction**:

    -   While not always necessary to call `pthread_mutex_destroy()`, it is considered good practice to do so if dynamically allocating resources.

### Initializing an Array of Mutexes

-   **Static Initializer Limitation**:

    -   **`PTHREAD_MUTEX_INITIALIZER`** cannot be used to initialize an array of mutexes directly. Instead, iterate and initialize each mutex individually:

    ```
    c

    

    `pthread_mutex_t mutex_array[THREAD_NUM];
    ```
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_mutex_init(&mutex_array[i], NULL);
    }`

Condition Variables Initialization
----------------------------------

### Traditional Method

1.  **Define the Condition Variable**:
    
    ```c   
    pthread_cond_t cond;
    ```

2.  **Initialize the Condition Variable**:
    
    ```c   
    pthread_cond_init(&cond, NULL);
    ```

3.  **Destroy the Condition Variable**:
    
    ```c   
    pthread_cond_destroy(&cond);
    ```

### Improved Method with Static Initializers

1.  **Static Initialization**:
    
    ```c
        pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    ```

    -   **`PTHREAD_COND_INITIALIZER`** is a macro for condition variables that avoids the need to call `pthread_cond_init()`.

Barriers and Other Complex PTHREAD Objects
------------------------------------------

-   **No Static Initializers**:
    -   For complex objects like barriers, static initializers are not available. This is due to the need to pass additional parameters (e.g., count for barriers), making static initialization infeasible.

Summary
-------

-   **Mutexes and Condition Variables**: Use static initializers (`PTHREAD_MUTEX_INITIALIZER` and `PTHREAD_COND_INITIALIZER`) for simpler and more efficient code when not dealing with arrays or dynamic allocations.
-   **Complex Objects**: For barriers and other complex synchronization objects, use the traditional initialization methods as static initializers are not provided.

# Deadlocks in C

## Introduction to Deadlocks

**Deadlocks** are a critical issue in multithreading environments where two or more threads are waiting indefinitely for resources that the other threads hold. This often results in the entire system becoming unresponsive.

## Example of Basic Deadlock

### Setup

1. **Mutex Initialization**
   ```c
   pthread_mutex_t mutex_fuel;
   pthread_mutex_init(&mutex_fuel, NULL);
   ```

2. **Thread Routine**
   In this scenario, eight threads are created, each modifying a shared resource (`fuel`). The mutex `mutex_fuel` is used to ensure that the modification of `fuel` is thread-safe.
   ```c
   void* thread_routine(void* arg) {
       pthread_mutex_lock(&mutex_fuel);
       fuel += 50;
       pthread_mutex_unlock(&mutex_fuel);
       return NULL;
   }
   ```

3. **Main Thread**
   After all threads are joined, the final value of `fuel` is printed.
   ```c
   int main() {
       pthread_t threads[8];
       for (int i = 0; i < 8; i++) {
           pthread_create(&threads[i], NULL, thread_routine, NULL);
       }
       for (int i = 0; i < 8; i++) {
           pthread_join(threads[i], NULL);
       }
       printf("Fuel: %d\n", fuel);
       return 0;
   }
   ```

### Issue: Locking Mutex Twice

If a thread tries to lock `mutex_fuel` twice without unlocking it in between, the thread will be stuck waiting for itself to release the mutex, resulting in a deadlock.

## Deadlock with Multiple Mutexes

### Setup

1. **Additional Mutex**
   ```c
   pthread_mutex_t mutex_water;
   pthread_mutex_init(&mutex_water, NULL);
   ```

2. **Thread Routine with Multiple Locks**
   ```c
   void* thread_routine(void* arg) {
       pthread_mutex_lock(&mutex_fuel);
       pthread_mutex_lock(&mutex_water);
       fuel += 50;
       water = fuel;
       pthread_mutex_unlock(&mutex_water);
       pthread_mutex_unlock(&mutex_fuel);
       return NULL;
   }
   ```

### Deadlock Scenario

When multiple mutexes are involved, deadlock can occur if different threads acquire the mutexes in different orders. For instance, if one thread locks `mutex_fuel` first and `mutex_water` second, and another thread locks `mutex_water` first and `mutex_fuel` second, a circular wait condition can occur:

1. **Thread 1** locks `mutex_fuel` and waits for `mutex_water`.
2. **Thread 2** locks `mutex_water` and waits for `mutex_fuel`.

### Visualization of Deadlock

- **Thread 1**: Locks `mutex_fuel` → Waits for `mutex_water`.
- **Thread 2**: Locks `mutex_water` → Waits for `mutex_fuel`.

Both threads are waiting for each other to release the mutex they need, leading to a deadlock.

## Code to Simulate Deadlock

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_fuel;
pthread_mutex_t mutex_water;
int fuel = 50;
int water = 10;

void* thread_routine(void* arg) {
    int rand_num = rand() % 2;
    
    if (rand_num == 0) {
        pthread_mutex_lock(&mutex_fuel);
        sleep(1); // Simulate some work
        pthread_mutex_lock(&mutex_water);
    } else {
        pthread_mutex_lock(&mutex_water);
        sleep(1); // Simulate some work
        pthread_mutex_lock(&mutex_fuel);
    }
    
    fuel += 50;
    water = fuel;
    
    pthread_mutex_unlock(&mutex_water);
    pthread_mutex_unlock(&mutex_fuel);
    return NULL;
}

int main() {
    pthread_t threads[8];
    pthread_mutex_init(&mutex_fuel, NULL);
    pthread_mutex_init(&mutex_water, NULL);
    
    for (int i = 0; i < 8; i++) {
        pthread_create(&threads[i], NULL, thread_routine, NULL);
    }
    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final Fuel: %d\n", fuel);
    printf("Final Water: %d\n", water);
    
    pthread_mutex_destroy(&mutex_fuel);
    pthread_mutex_destroy(&mutex_water);
    return 0;
}
```

## Conclusion

**Deadlocks** occur when multiple threads acquire multiple locks in different orders, leading to a situation where no thread can proceed because each thread is holding a resource that the other needs. To avoid deadlocks:
- **Ensure consistent locking order** across all threads.
- **Minimize the time locks are held**.
- **Use timeout mechanisms** if appropriate.

Understanding and avoiding deadlocks are crucial for developing robust multithreaded applications.

### Introduction to Semaphores in C

**Semaphores** are synchronization tools used to control access to a shared resource by multiple threads or processes in a concurrent system. This note will cover the basics of semaphores in C, including their initialization, usage, and the differences between semaphores and mutexes.

#### 1. **What is a Semaphore?**

- A **semaphore** is a variable or abstract data type used to control access to a common resource by multiple threads or processes.
- It has an internal **integer value** which helps in managing the access control.

#### 2. **Including Semaphores in C**

To use semaphores in C, include the following header:

```c
#include <semaphore.h>
```

#### 3. **Defining and Initializing Semaphores**

- Define a semaphore variable of type **`sem_t`**:

  ```c
  sem_t semaphore;
  ```

- Initialize the semaphore using **`sem_init`**:

  ```c
  sem_init(&semaphore, 0, 1);
  ```

  **Parameters:**
  - `&semaphore`: Reference to the semaphore variable.
  - `0`: Indicates semaphore is used for threads within the same process (use `1` for inter-process synchronization).
  - `1`: Initial value of the semaphore. It determines how many threads can access the critical section concurrently.

#### 4. **Destroying Semaphores**

- Once done, destroy the semaphore to free resources using **`sem_destroy`**:

  ```c
  sem_destroy(&semaphore);
  ```

#### 5. **Operations on Semaphores**

- **Wait Operation** (`sem_wait`): This operation decreases the semaphore value. If the semaphore value is zero, the thread will block until the semaphore value is greater than zero.

  ```c
  sem_wait(&semaphore);
  ```

- **Post Operation** (`sem_post`): This operation increases the semaphore value, potentially unblocking one of the waiting threads.

  ```c
  sem_post(&semaphore);
  ```

#### 6. **Example Code**

Here is a simple example demonstrating the use of semaphores with threads:

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;

void* thread_routine(void* arg) {
    int id = *((int*)arg);
    
    sem_wait(&semaphore); // Wait (decrement the semaphore)
    
    printf("Hello from thread %d\n", id);
    sleep(1); // Simulate some work
    
    sem_post(&semaphore); // Post (increment the semaphore)
    
    return NULL;
}

int main() {
    pthread_t threads[4];
    int ids[4];
    
    // Initialize semaphore with value 1
    sem_init(&semaphore, 0, 1);
    
    // Create threads
    for (int i = 0; i < 4; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_routine, &ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Destroy semaphore
    sem_destroy(&semaphore);
    
    return 0;
}
```

**Explanation:**
- The semaphore is initialized with a value of 1, allowing only one thread to enter the critical section at a time.
- Each thread waits (`sem_wait`) to enter the critical section and posts (`sem_post`) when it exits.
- This ensures that only one thread can print and sleep at a time, effectively managing the access to the critical section.

#### 7. **Differences Between Semaphores and Mutexes**

- **Mutex**: A synchronization primitive that allows only one thread to access a resource at a time. Mutexes are binary (0 or 1) and do not allow multiple threads to access the resource simultaneously.
- **Semaphore**: Can have a count greater than one, allowing multiple threads to access a set number of resources concurrently. It acts like a generalized mutex with a counter.

**Key Points:**
- **Mutexes** are used for mutual exclusion in single-threaded contexts.
- **Semaphores** are used for managing access to a pool of resources or for controlling concurrent access in a more flexible way.

#### 8. **Advanced Usage**

- By changing the initial value of the semaphore, you can allow multiple threads to access the critical section concurrently. For example:
  
  ```c
  sem_init(&semaphore, 0, 2); // Allows up to 2 threads to access the critical section simultaneously
  ```

**Example Outcomes:**
- If the semaphore is initialized to 2, then up to 2 threads can execute the critical section code concurrently.
- If the semaphore is initialized to 3, then up to 3 threads can access the critical section concurrently, and so forth.

#### Conclusion

Semaphores are a powerful tool for controlling access to shared resources in concurrent programming. Understanding their basic operations and how they differ from mutexes is crucial for effective synchronization in multi-threaded applications.

## Producer-Consumer Problem in Multi-Threading

### Introduction

The **Producer-Consumer Problem** is a classic synchronization problem in computer science where a shared buffer is used to coordinate the interaction between multiple producer and consumer threads. Producers generate data and place it into a shared buffer, while consumers take data from this buffer and process it. The main challenge is to manage the shared buffer to ensure that it is accessed in a thread-safe manner and to handle scenarios where the buffer is either full or empty.

### Problem Description

#### Shared Buffer

- **Shared Buffer**: A data structure (e.g., an array) that holds items produced by producer threads and consumed by consumer threads. It is crucial that the access to this buffer is properly synchronized to prevent data corruption.

#### Producers and Consumers

- **Producers**: Threads that generate data and place it into the shared buffer.
- **Consumers**: Threads that take data from the buffer and process it.

#### Key Problems

1. **Shared Memory Access**:
   - **Race Conditions**: When multiple threads access the buffer simultaneously, there's a risk of data corruption. Using a **mutex** (short for mutual exclusion) can help prevent this by ensuring that only one thread can access the buffer at a time.

2. **Buffer Full**:
   - When the buffer is full, producers need to wait until there is space available. Implementing this efficiently is crucial to avoid busy-waiting.

3. **Buffer Empty**:
   - When the buffer is empty, consumers need to wait until there is data available. Again, this needs to be managed to avoid inefficient busy-waiting.

### Solution Approach

#### Basic Implementation

1. **Buffer Definition**:
   ```c
   #define BUFFER_SIZE 10
   int buffer[BUFFER_SIZE];
   int count = 0;  // Number of items currently in the buffer
   ```

2. **Producer Function**:
   ```c
   void* producer(void* arg) {
       while (1) {
           int x = rand() % 100;  // Produce a random number
           pthread_mutex_lock(&mutex);
           if (count < BUFFER_SIZE) {
               buffer[count++] = x;  // Add item to the buffer
           }
           pthread_mutex_unlock(&mutex);
           sleep(1);  // Simulate time taken to produce an item
       }
       return NULL;
   }
   ```

3. **Consumer Function**:
   ```c
   void* consumer(void* arg) {
       while (1) {
           pthread_mutex_lock(&mutex);
           if (count > 0) {
               int y = buffer[--count];  // Remove item from the buffer
               printf("Got number: %d\n", y);
           }
           pthread_mutex_unlock(&mutex);
           sleep(1);  // Simulate time taken to consume an item
       }
       return NULL;
   }
   ```

4. **Main Function**:
   ```c
   int main() {
       pthread_t prod, cons;

       pthread_mutex_init(&mutex, NULL);

       pthread_create(&prod, NULL, producer, NULL);
       pthread_create(&cons, NULL, consumer, NULL);

       pthread_join(prod, NULL);
       pthread_join(cons, NULL);

       pthread_mutex_destroy(&mutex);

       return 0;
   }
   ```

#### Advanced Implementation Using Semaphores

**Semaphores** are used to manage access to the buffer by tracking the number of full and empty slots.

1. **Semaphore Definitions**:
   ```c
   #include <semaphore.h>

   sem_t empty_slots;
   sem_t full_slots;
   ```

2. **Initialization**:
   ```c
   int main() {
       pthread_t prod, cons;

       sem_init(&empty_slots, 0, BUFFER_SIZE);  // Initial empty slots
       sem_init(&full_slots, 0, 0);              // Initial full slots

       pthread_create(&prod, NULL, producer, NULL);
       pthread_create(&cons, NULL, consumer, NULL);

       pthread_join(prod, NULL);
       pthread_join(cons, NULL);

       sem_destroy(&empty_slots);
       sem_destroy(&full_slots);

       return 0;
   }
   ```

3. **Producer Function with Semaphores**:
   ```c
   void* producer(void* arg) {
       while (1) {
           int x = rand() % 100;  // Produce a random number
           sem_wait(&empty_slots);  // Wait for an empty slot
           pthread_mutex_lock(&mutex);
           buffer[count++] = x;  // Add item to the buffer
           pthread_mutex_unlock(&mutex);
           sem_post(&full_slots);  // Signal that there is a new item
           sleep(1);
       }
       return NULL;
   }
   ```

4. **Consumer Function with Semaphores**:
   ```c
   void* consumer(void* arg) {
       while (1) {
           sem_wait(&full_slots);  // Wait for an available item
           pthread_mutex_lock(&mutex);
           int y = buffer[--count];  // Remove item from the buffer
           pthread_mutex_unlock(&mutex);
           sem_post(&empty_slots);  // Signal that there is an empty slot
           printf("Got number: %d\n", y);
           sleep(1);
       }
       return NULL;
   }
   ```
   ### Getting the Value of a Semaphore

#### Introduction
In this tutorial, we'll explore how to retrieve the value of a semaphore in a multithreaded program. Semaphores are synchronization primitives used to control access to shared resources by multiple threads. They maintain a count that is adjusted based on operations such as **wait** and **post**.

#### Basic Semaphore Operations
1. **Creating a Semaphore**:
   ```c
   sem_t semaphore;
   sem_init(&semaphore, 0, 4); // Initialize semaphore with a value of 4
   ```
   - `sem_init()` initializes the semaphore.
   - The second parameter (0) specifies whether the semaphore is shared between processes (0 for threads, non-zero for processes).
   - The third parameter (4) sets the initial value of the semaphore.

2. **Waiting on a Semaphore**:
   ```c
   sem_wait(&semaphore);
   ```
   - This operation decrements the semaphore value. If the value is already zero, the thread blocks until the semaphore becomes greater than zero.

3. **Posting to a Semaphore**:
   ```c
   sem_post(&semaphore);
   ```
   - This operation increments the semaphore value. If there are blocked threads, one of them may be unblocked.

#### Retrieving Semaphore Value
To get the current value of a semaphore, use the **`sem_getvalue()`** function:
   ```c
   int sem_val;
   sem_getvalue(&semaphore, &sem_val);
   ```
   - **`sem_getvalue()`** retrieves the current value of the semaphore and stores it in the variable `sem_val`.

#### Example Code
Here's a code snippet demonstrating how to use the `sem_getvalue()` function:
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 4

sem_t semaphore;

void* thread_func(void* arg) {
    int index = *((int*)arg);
    int sem_val;

    // Wait on the semaphore
    sem_wait(&semaphore);

    // Get and print semaphore value after wait
    sem_getvalue(&semaphore, &sem_val);
    printf("Thread %d: Value after wait = %d\n", index, sem_val);

    // Simulate some work
    sleep(index + 1);

    // Post to the semaphore
    sem_post(&semaphore);

    // Get and print semaphore value after post
    sem_getvalue(&semaphore, &sem_val);
    printf("Thread %d: Value after post = %d\n", index, sem_val);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int indices[NUM_THREADS];
    int i;

    // Initialize semaphore with value of 4
    sem_init(&semaphore, 0, 4);

    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, thread_func, (void*)&indices[i]);
    }

    // Join threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphore
    sem_destroy(&semaphore);

    return 0;
}
```

#### Explanation
1. **Semaphore Initialization**:
   - We initialize the semaphore with a value of 4, which allows up to 4 threads to proceed concurrently.

2. **Thread Operations**:
   - Each thread waits on the semaphore, prints its value, then sleeps for a duration based on its index.
   - After sleeping, each thread posts to the semaphore and prints the semaphore's value again.

3. **Race Conditions**:
   - The output may vary due to race conditions, as the value retrieved by `sem_getvalue()` may not be consistent if multiple threads are interacting with the semaphore concurrently.

4. **Synchronization**:
   - To reduce the impact of race conditions, the `sleep` function is used to delay the execution of threads, helping to illustrate changes in semaphore values more clearly.

#### Conclusion
- The **`sem_getvalue()`** function provides a snapshot of the semaphore's value at a particular moment.
- Semaphore values can change rapidly due to concurrent operations, leading to race conditions.
- For accurate synchronization, ensure proper thread management and consider using additional synchronization mechanisms if needed.

Feel free to experiment with the code and adjust parameters to observe how the semaphore value changes in different scenarios.

# Parallelism vs Concurrency

Understanding the distinction between **parallelism** and **concurrency** is crucial in multithreading and performance optimization. Both concepts are related to the execution of multiple threads but differ significantly in their implementation and impact on performance.

## Parallelism

**Parallelism** involves running multiple threads simultaneously to perform tasks more efficiently. Key characteristics include:

- **Execution of the Same Code on Different Data**: In parallelism, multiple threads execute the same code but operate on different sets of data. For example, if an array with six elements is processed, six threads might be used, each handling one element independently.
  
- **No Shared Memory Access**: Threads in parallelism do not access or modify the same memory location. This minimizes the risk of data conflicts and ensures that threads work independently.
  
- **Efficiency**: Parallelism can significantly enhance performance. For instance, if you have 50 elements and launch 50 threads (one per element), the operation can theoretically be performed 50 times faster, assuming adequate CPU resources and minimal overhead for thread management.

### Example

Consider calculating the sum of the first 10 prime numbers using two threads. Each thread handles half of the array. Since they don’t access the same memory locations, they work in parallel, leading to efficient execution.

## Concurrency

**Concurrency** refers to multiple threads working simultaneously on a shared resource or memory location. Important aspects include:

- **Shared Memory Access**: Threads in a concurrent system often access and modify the same memory location, which can lead to issues if not properly managed.
  
- **Critical Sections**: Threads might need to enter critical sections (sections of code that access shared resources), potentially causing performance bottlenecks. For example, if multiple threads are incrementing a shared counter, they may need to lock the counter to prevent data corruption.

- **Potential Inefficiency**: High levels of concurrency can degrade performance if threads frequently access shared resources, leading to contention and increased waiting times.

### Example

In a scenario where multiple threads are processing mail and updating a shared variable (e.g., a counter of processed mails), the threads might need to synchronize access to this variable. If processing each mail takes a second and the threads are highly concurrent, the performance might not improve compared to a single-threaded approach due to contention and overhead from synchronization mechanisms.

## Parallelism vs Concurrency: Key Differences

- **Degree of Parallelism vs. Concurrency**: Parallelism improves performance by distributing tasks across multiple threads with no shared memory conflicts, while concurrency deals with multiple threads accessing shared resources. The degree of parallelism is usually more beneficial for performance compared to concurrency.

- **Efficiency and Performance**: Higher parallelism generally results in better performance as tasks are performed simultaneously without contention. Conversely, high concurrency with shared resources can lead to inefficiencies due to synchronization overhead.

## Practical Considerations

- **Optimizing Parallelism**: To leverage parallelism effectively, ensure that threads work on independent data and minimize the need for synchronization. For example, splitting an array into segments and processing each segment in parallel can improve performance.

- **Managing Concurrency**: When dealing with concurrency, use synchronization mechanisms like **mutexes** to manage access to shared resources and avoid data races. Be cautious of performance trade-offs introduced by these mechanisms.

## Example Code

### Parallelism Example Code

Here’s a simple example in Python using the `threading` module to compute the sum of an array in parallel:

```python
import threading

def compute_sum(arr, start, end, result):
    result[0] = sum(arr[start:end])

def parallel_sum(arr, num_threads):
    length = len(arr)
    segment_size = length // num_threads
    threads = []
    results = [0] * num_threads

    for i in range(num_threads):
        start = i * segment_size
        end = (i + 1) * segment_size if i != num_threads - 1 else length
        thread = threading.Thread(target=compute_sum, args=(arr, start, end, results[i:i+1]))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    return sum(results)

# Example usage
array = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
num_threads = 2
print(parallel_sum(array, num_threads))
```

### Concurrency Example Code

Here’s an example illustrating concurrency with a shared counter and a mutex in Python:

```python
import threading
import time

def process_mail(mailbox, lock):
    with lock:
        print(f"Processing mail: {mailbox['count']}")
        time.sleep(1)  # Simulate processing time
        mailbox['count'] += 1

def concurrent_processing(num_threads):
    mailbox = {'count': 0}
    lock = threading.Lock()
    threads = []

    for _ in range(num_threads):
        thread = threading.Thread(target=process_mail, args=(mailbox, lock))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

    print(f"Total mails processed: {mailbox['count']}")

# Example usage
concurrent_processing(5)
```

These examples demonstrate how parallelism and concurrency are handled in programming and their impact on performance.

## Thread Pools in C (using the PTHREAD API)

### Introduction to Thread Pools
- **Thread Pools** are a collection of pre-created threads that wait for tasks to execute, allowing efficient management of tasks in a multi-threaded environment.
- **Goal**: To execute multiple tasks concurrently rather than sequentially, improving the efficiency of programs that need to process many tasks.

### Components of a Thread Pool
1. **Thread Pool**: A set of worker threads that are always running. Each thread is either executing a task or waiting for a new task to arrive.
2. **Task Queue**: A queue where tasks are placed and retrieved in a First-In-First-Out (FIFO) manner.
3. **Main Thread**: The thread or entity that submits tasks to the pool. This could be the primary application thread or a separate thread in a larger system.

### Operations of a Thread Pool
1. **Submit Task**:
   - **Function**: Add a task to the task queue.
   - **Details**: The main thread or other entities submit tasks which are then added to the end of the queue.
2. **Get and Assign Task**:
   - **Function**: Retrieve a task from the queue and assign it to an available thread.
   - **Details**: Threads wait for tasks and pick them up as they become available.
3. **Execute Task**:
   - **Function**: Perform the task assigned to a thread.
   - **Details**: The thread processes the task and completes it, typically involving computation or I/O operations.

### Implementing a Task
- **Task Definition**: Represented as a `struct` with the data needed for execution.
- **Example Task Structure**:
  ```c
  typedef struct {
      int a;
      int b;
  } Task;
  ```
- **Task Execution Function**:
  ```c
  void execute_task(Task* task) {
      int result = task->a + task->b;
      printf("Sum of %d and %d is %d\n", task->a, task->b, result);
  }
  ```

### Task Queue Implementation
- **Queue Structure**: A simple array-based queue with a fixed size and a count of tasks.
- **Example Queue Implementation**:
  ```c
  #define MAX_TASKS 256

  Task task_queue[MAX_TASKS];
  int task_count = 0;
  ```

### Creating Threads and Managing Tasks
- **Thread Function**: Each thread executes this function to process tasks from the queue.
- **Start Thread Function**:
  ```c
  void* start_thread(void* arg) {
      while (1) {
          pthread_mutex_lock(&queue_mutex);

          while (task_count == 0) {
              pthread_cond_wait(&cond_queue, &queue_mutex);
          }

          Task task = task_queue[0];
          for (int i = 0; i < task_count - 1; ++i) {
              task_queue[i] = task_queue[i + 1];
          }
          task_count--;

          pthread_mutex_unlock(&queue_mutex);

          execute_task(&task);
      }
      return NULL;
  }
  ```

### Submitting Tasks
- **Submit Task Function**:
  ```c
  void submit_task(Task task) {
      pthread_mutex_lock(&queue_mutex);

      task_queue[task_count++] = task;
      pthread_cond_signal(&cond_queue);

      pthread_mutex_unlock(&queue_mutex);
  }
  ```

### Handling Synchronization
- **Mutex**: Used to ensure that access to the task queue is thread-safe.
- **Condition Variable**: Used to block threads until tasks are available in the queue.
- **Initialization**:
  ```c
  pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond_queue = PTHREAD_COND_INITIALIZER;
  ```

### Improvements and Considerations
- **CPU Usage**: Without proper synchronization, threads may consume excessive CPU by continuously checking the queue.
- **Condition Variables**: Use condition variables to block threads until tasks are available, improving CPU usage efficiency.
- **Final Thread Function**:
  ```c
  void* start_thread(void* arg) {
      while (1) {
          pthread_mutex_lock(&queue_mutex);

          while (task_count == 0) {
              pthread_cond_wait(&cond_queue, &queue_mutex);
          }

          Task task = task_queue[0];
          for (int i = 0; i < task_count - 1; ++i) {
              task_queue[i] = task_queue[i + 1];
          }
          task_count--;

          pthread_mutex_unlock(&queue_mutex);

          execute_task(&task);
      }
      return NULL;
  }
  ```

### Example Code
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_TASKS 256

typedef struct {
    int a;
    int b;
} Task;

Task task_queue[MAX_TASKS];
int task_count = 0;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_queue = PTHREAD_COND_INITIALIZER;

void execute_task(Task* task) {
    int result = task->a + task->b;
    printf("Sum of %d and %d is %d\n", task->a, task->b, result);
}

void* start_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&queue_mutex);

        while (task_count == 0) {
            pthread_cond_wait(&cond_queue, &queue_mutex);
        }

        Task task = task_queue[0];
        for (int i = 0; i < task_count - 1; ++i) {
            task_queue[i] = task_queue[i + 1];
        }
        task_count--;

        pthread_mutex_unlock(&queue_mutex);

        execute_task(&task);
    }
    return NULL;
}

void submit_task(Task task) {
    pthread_mutex_lock(&queue_mutex);

    task_queue[task_count++] = task;
    pthread_cond_signal(&cond_queue);

    pthread_mutex_unlock(&queue_mutex);
}

int main() {
    pthread_t threads[4];
    for (int i = 0; i < 4; ++i) {
        pthread_create(&threads[i], NULL, start_thread, NULL);
    }

    for (int i = 0; i < 100; ++i) {
        Task task = { rand() % 100, rand() % 100 };
        submit_task(task);
    }

    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
```

### Summary
- **Thread Pools** efficiently manage multiple concurrent tasks using a fixed number of threads.
- **Key Components** include a task queue, threads, and synchronization mechanisms.
- **Synchronization** is critical to prevent race conditions and ensure efficient task execution.
- **Condition Variables** improve CPU efficiency by blocking threads when no tasks are available.