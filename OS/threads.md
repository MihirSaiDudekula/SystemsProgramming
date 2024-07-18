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

