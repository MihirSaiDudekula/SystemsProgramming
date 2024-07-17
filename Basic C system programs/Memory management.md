### Basics
# Stack vs Heap Memory - Simple Explanation

https://www.youtube.com/watch?v=5OJRqkYbK-4&ab_channel=AlexHyett
https://www.youtube.com/watch?v=_8-ht2AKyH4&ab_channel=mycodeschool

## Overview
This guide provides a comprehensive understanding of stack and heap memory, crucial for understanding how your application stores and accesses variables during runtime. 

### **Memory in Applications**
Memory in an application is divided into three main parts:
1. **Machine Code**: The instructions your computer executes.
2. **Stack**: Manages method calls and local variables.
3. **Heap**: Manages dynamically allocated memory.

### **The Stack**
- **Stack Data Structure**: Operates like a stack of books; you can only add or remove items from the top.
- **Call Stack**: Manages method execution and local variables.
  - When a method is called, it is added to the call stack.
  - Once the method finishes, it is removed from the stack.

### **Local Variables on the Stack**
- **Local Variables**: Stored on the call stack.
  - Accessible only within the method they are declared.
  - Removed once the method execution is completed.

### **The Heap**
- **Heap Memory**: Allows storage and access of items in any order.
  - **Dynamic Memory Allocation**: Items can be added or removed from anywhere, but this adds overhead.
- **Long-Lived Data**: Data that needs to persist across method calls or needs to be accessible after method execution is stored on the heap.

### **Variable Storage Rules**
- **Value Types**: Store actual values.
  - **Stack or Heap**: Depending on the declaration context.
  - Example: `int`, `float`.
- **Reference Types**: Store a pointer to a value.
  - **Always on the Heap**: The pointer may be on the stack.
  - Example: `class`, `array`.

### **Global Variables**
- **Global Variables**: Always stored on the heap.
  - Accessible from different parts of the code.

### **Garbage Collection**
- **Garbage Collector**: Cleans up unused memory on the heap.
  - Removes data without references (pointers) after method execution.

### **Special Cases**
- **Static Variables**: Always on the heap, accessible throughout the application.
- **Anonymous Functions**: 
  - Access variables declared in the calling method.
  - Variables are temporarily stored on the heap to maintain accessibility.
- **Asynchronous Methods**: 
  - Run on different threads with their own call stacks.
  - Results stored on the heap to ensure accessibility once the asynchronous operation completes.

# Stack vs Heap

## Introduction to Memory Sections
The computer memory is divided into three major sections:
1. **Code Section**
2. **Stack**
3. **Heap**

### Code Section
- **Function**: Stores the machine code of the program.
- **Loading Process**: The machine code of a program is loaded into the code section when the program is run. This process is called **loading**.
- **Execution**: The CPU executes the code in the code section by taking one instruction at a time.

### Stack
- **Function**: Allocates memory for the variables declared inside the program.
- **Memory Allocation**: Memory allocation for variables happens at runtime, but the amount of memory required is determined at compile time.
- **Access**: The stack memory can be accessed directly by the program using addresses.
- **Activation Record (Stack Frame)**: The block of memory allocated for a function is called an **activation record** or **stack frame**.
- **Characteristics**:
  - **Direct Access**: Stack memory is directly accessible.
  - **Static Size**: The size of the stack memory is determined at compile time.

### Heap
- **Function**: Allocates memory dynamically during the runtime of the program.
- **Memory Allocation**: Memory is allocated as needed during the program's execution, which is not predetermined at compile time.
- **Access**: Heap memory is accessed indirectly through pointers.
- **Usage**:
  - To use heap memory, a programmer must declare a pointer.
  - **Example**:
    ```c
    int *p;
    p = (int*) malloc(10 * sizeof(int)); // Allocates 10 bytes of memory and assigns the address to pointer p
    ```
- **Characteristics**:
  - **Indirect Access**: Heap memory is accessed through pointers.
  - **Dynamic Size**: The size of heap memory is determined at runtime.

### Key Differences Between Stack and Heap
1. **Accessibility**:
   - **Stack**: Directly accessible.
   - **Heap**: Indirectly accessible through pointers.

2. **Memory Allocation**:
   - **Stack**: Memory size is determined at compile time (static).
   - **Heap**: Memory size is determined at runtime (dynamic).

3. **Usage**:
   - **Stack**: For variables declared in the program.
   - **Heap**: For dynamically allocating memory using functions like `malloc`.

### Example Code

#### Stack Allocation Example
```c
void function() {
    int a; // Memory for 'a' is allocated on the stack
    int b; // Memory for 'b' is allocated on the stack
}
```

#### Heap Allocation Example
```c
#include <stdlib.h>

int main() {
    int *p;
    p = (int*) malloc(10 * sizeof(int)); // Allocates 10 bytes of memory on the heap
    // Use the allocated memory
    free(p); // Deallocates the memory
    return 0;
}
```

## Conclusion
- **Stack**: Memory for variables is allocated at runtime but with a predetermined size, directly accessible.
- **Heap**: Memory is allocated dynamically during program execution, indirectly accessible via pointers.

# Notes: Pointers and Dynamic Memory - Stack vs Heap

## **Introduction**
Memory is a critical resource in computing. Understanding how memory is managed and accessed is essential for effective programming. This lesson covers the concept of dynamic memory in C and C++, focusing on the stack and heap memory segments.

## **Memory Segments**
1. **Text Segment**:
   - Stores the instructions that need to be executed.

2. **Global Variable Segment**:
   - Stores static or global variables that have a lifetime equal to the program's runtime. These variables are accessible throughout the entire lifecycle of the application.

3. **Stack**:
   - Used to store information about function calls and local variables. Each function call creates a stack frame that includes:
     - Local variables
     - Function arguments
     - Return address
   - The stack frame size is determined at compile time.
   - The stack operates on a last-in, first-out (LIFO) basis. When a function finishes execution, its stack frame is removed.

4. **Heap**:
   - Used for dynamic memory allocation. The size of the heap can vary during the application’s lifetime.
   - Managed manually by the programmer using functions like `malloc`, `calloc`, `realloc`, and `free` in C, and `new` and `delete` operators in C++.

## **Stack Memory**
- **Fixed Size**: The stack size is determined at the start of the program and does not grow during runtime.
- **Automatic Deallocation**: Memory is automatically allocated and deallocated as functions are called and return.
- **Stack Overflow**: Occurs when the stack memory is exhausted, often due to deep or infinite recursion.

### **Example Program Explanation**
```c
#include <stdio.h>

int square(int n) {
    return n * n;
}

int squareOfSum(int x, int y) {
    int sum = x + y;
    return square(sum);
}

int main() {
    int a = 5, b = 10;
    int total = squareOfSum(a, b);
    printf("Total: %d\n", total);
    return 0;
}
```
- **Global Variable**: `total` is defined globally.
- **Stack Frames**:
  - `main` function: Allocates memory for `a`, `b`, and `total`.
  - `squareOfSum` function: Allocates memory for `x`, `y`, and `sum`.
  - `square` function: Allocates memory for `n`.

## **Heap Memory**
- **Dynamic Allocation**: The heap allows for flexible memory allocation and deallocation during runtime.
- **Manual Management**: Programmers must manually allocate (`malloc`, `calloc`, `realloc`) and deallocate (`free`) memory.
- **Use Cases**: Suitable for large data structures or when the size of data structures cannot be determined at compile time.

### **Dynamic Memory Allocation in C**
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *p = (int *)malloc(sizeof(int)); // Allocate memory for an integer
    if (p == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    *p = 10; // Assign value to allocated memory
    printf("Value: %d\n", *p);
    free(p); // Deallocate memory

    int *arr = (int *)malloc(20 * sizeof(int)); // Allocate memory for an array of 20 integers
    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < 20; i++) {
        arr[i] = i; // Assign values to the array
    }
    for (int i = 0; i < 20; i++) {
        printf("%d ", arr[i]); // Print array values
    }
    free(arr); // Deallocate memory
    return 0;
}
```

### **Dynamic Memory Allocation in C++**
```cpp
#include <iostream>

int main() {
    int *p = new int; // Allocate memory for an integer
    *p = 10; // Assign value to allocated memory
    std::cout << "Value: " << *p << std::endl;
    delete p; // Deallocate memory

    int *arr = new int[20]; // Allocate memory for an array of 20 integers
    for (int i = 0; i < 20; i++) {
        arr[i] = i; // Assign values to the array
    }
    for (int i = 0; i < 20; i++) {
        std::cout << arr[i] << " "; // Print array values
    }
    delete[] arr; // Deallocate memory
    return 0;
}
```

## **Key Concepts**
- **Stack**:
  - Fixed size, automatic deallocation.
  - Used for local variables and function calls.
  - Prone to stack overflow with deep recursion or excessive allocation.

- **Heap**:
  - Dynamic size, manual management.
  - Suitable for large or dynamically-sized data structures.
  - Requires careful allocation and deallocation to avoid memory leaks.



### How to Properly Deal with Dynamically Allocated Memory

Dynamic memory allocation is a fundamental concept in programming, allowing efficient use of memory during runtime. Here's a comprehensive overview of the key aspects and best practices:

#### Introduction to Dynamically Allocated Memory

Dynamic memory allocation allows programs to request memory at runtime, which is essential for handling varying data sizes and structures efficiently.

#### Three Main Functions for Memory Allocation

1. **malloc**
   - Allocates a block of memory of specified size.
   - Example: `int *K = malloc(sizeof(int));`

2. **calloc**
   - Allocates memory for an array of elements, initializing them to zero.
   - Example: `int *array = calloc(64, sizeof(int));`

3. **realloc**
   - Adjusts the size of previously allocated memory blocks.
   - Example: `array = realloc(array, 64 * sizeof(int));`

#### Risks and Best Practices

- **Null Pointer Check**: Always check if memory allocation succeeds.
  - Example: `if (K == NULL) { /* handle error */ }`

- **Memory Fragmentation**: Continuous allocation and deallocation can lead to fragmented memory, affecting performance.
  - Solution: Use memory efficiently and avoid excessive fragmentation.

#### Dealing with Garbage Values

- **memset Function**: Clears allocated memory to zero.
  - Example: `memset(array, 0, 64 * sizeof(int));`

- **calloc vs. malloc**: Choose based on whether initialization to zero is required.

#### Memory Reallocation

- **realloc Function**: Resizes allocated memory blocks.
  - Example: `array = realloc(array, 64 * sizeof(int));`

- **Handling realloc**: Update pointers correctly as realloc may return a new address.

#### Conclusion

Proper management of dynamically allocated memory involves understanding the functions and their implications. Always ensure null pointer checks, handle memory fragmentation, and utilize functions like `memset` or `calloc` appropriately to manage memory efficiently.

#### Example Program

Here's an example program demonstrating dynamic memory allocation and deallocation:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Example of dynamic memory allocation and initialization
    int *K = malloc(sizeof(int));
    if (K == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    *K = 17;
    printf("Value at K: %d\n", *K);
    
    // Example of allocating an array and using memset
    int *array = calloc(64, sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }
    // Print array values
    for (int i = 0; i < 64; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Example of reallocating memory
    array = realloc(array, 32 * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Memory reallocation failed.\n");
        return 1;
    }
    
    // Print reallocated array values
    for (int i = 0; i < 32; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Free allocated memory
    free(K);
    free(array);
    
    return 0;
}
```

This program demonstrates basic principles of memory allocation, reallocation, and deallocation in C.

### Notes on "How to Use Dynamically Allocated Arrays"

**1. Introduction to Dynamically Allocated Arrays**
- Dynamically allocated array: Can be accessed in any function; allocation controlled by programmer.

**2. When to Use Global Arrays**
- Use global arrays if array is needed in multiple places within project.
- Example: `int an_array[256];` allocates 256 integers globally.
- Global arrays are automatically initialized to 0.

**3. Using Static Allocation for Known Maximum Size**
- Use static allocation if maximum elements known and not excessively large.
- Example: `int an_array[256];`
- Efficient in memory usage and known size.

**4. Advantages of Dynamically Allocated Arrays**
- Use when array size varies and needs optimization.
- Example: Use `malloc` to allocate memory dynamically:
  ```c
  int *array;
  array = malloc(256 * sizeof(int));
  ```

**5. Managing Dynamically Allocated Memory**
- Deallocation is necessary after use to avoid memory leaks.
- Use `free(array);` at the end of usage.

**6. Handling Array Size**
- Store array size separately to manage dynamic changes.
- Example:
  ```c
  size_t array_size = 256;
  ```

**7. Resizing Dynamically Allocated Arrays**
- Use `realloc` to resize arrays:
  ```c
  array_size *= 2;
  array = realloc(array, array_size * sizeof(int));
  ```

**8. Error Handling**
- Check for null pointers after `malloc` or `realloc`.
- Example:
  ```c
  if (array == NULL) {
      fprintf(stderr, "Array not allocated.\n");
      return 1;
  }
  ```

**9. Choosing Between `calloc` and `malloc`**
- Use `calloc` to initialize memory to zero; use `malloc` for faster allocation.
- Example:
  ```c
  array = calloc(256, sizeof(int));
  ```

**Conclusion**
- Dynamically allocated arrays are flexible but require careful management.
- Consider usage scenarios and memory requirements before choosing.

**Code Example:**
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *array;
    size_t array_size = 256;

    // Allocate memory
    array = malloc(array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Array not allocated.\n");
        return 1;
    }

    // Use array
    array[10] = 17;
    printf("%d\n", array[10]);

    // Resize array
    array_size *= 2;
    array = realloc(array, array_size * sizeof(int));
    if (array == NULL) {
        fprintf(stderr, "Array not reallocated.\n");
        return 1;
    }

    // Free memory
    free(array);

    return 0;
}
```

These notes provide a comprehensive overview of dynamically allocated arrays, covering initialization, usage, resizing, and memory management, along with practical examples and error handling strategies.

**Title: When to Free Memory in C**

### Introduction
In this topic, we delve into understanding the crucial aspects of memory management in C, specifically focusing on when and how to free dynamically allocated memory.

### Memory Allocation and Deallocation
Memory in C can be allocated dynamically using functions like **malloc**, **calloc**, and **realloc**. It's imperative to understand that dynamically allocated memory must be explicitly freed when it's no longer needed to prevent memory leaks.

#### Example 1: Basic Memory Allocation
When using **malloc** to allocate memory, as shown in the example:
```c
int *P = (int *)malloc(sizeof(int));
```
The memory allocated here must be freed using **free(P)** after its use to avoid memory leaks.

#### Example 2: Multiple Pointers to the Same Memory
If multiple pointers point to the same dynamically allocated memory block, as in:
```c
int *P = (int *)malloc(sizeof(int));
int *Q = P;
```
You only need to free the memory once, either with **free(P)** or **free(Q)**, since they both point to the same block of memory.

#### Example 3: Strings in Memory
Strings allocated using methods like `**char *STR = "test"** do not require explicit deallocation because they are managed by the runtime and not stored in the stack or heap.`

#### Example 4: Local Variables
Local variables are automatically deallocated when the function exits because they are stored on the stack. Therefore, no explicit freeing is necessary.

#### Example 5: Global Variables
Global variables, residing in global memory, are not deallocated until the program terminates. Hence, they do not require manual freeing during program execution.

### Conclusion
In conclusion, **free** should always accompany **malloc**, **calloc**, or **realloc** calls to prevent memory leaks in C. Understanding the type of memory (heap, stack, or special runtime-managed) helps determine whether explicit deallocation is necessary.

### Additional Considerations
- Ensure not to free memory more than once (double-freeing) as it leads to runtime errors.
- Libraries may require specific memory management procedures, such as calling a custom free function provided by the library.

By following these guidelines, efficient memory management in C programs can be ensured, mitigating potential memory leaks and errors.

### Code from Lecture:
```c
#include <stdlib.h>
#include <stdio.h>

int main() {
    int *P = (int *)malloc(sizeof(int));
    int *Q = P;
    
    // Example of freeing memory
    free(P);
    
    return 0;
}
```

### Good Practice for Freeing Memory in C

When working with dynamically allocated memory in C, it's crucial to follow good practices to avoid issues like double freeing memory, which can lead to runtime errors and crashes.

#### Understanding Double Freeing

Double freeing occurs when the same memory address is freed more than once. This commonly happens when:
- A pointer to dynamically allocated memory is freed in one part of the code.
- The same pointer or another pointer to the same memory is freed again elsewhere, inadvertently.

#### Example Scenario

Consider the following scenario:
```c
int *array = malloc(10 * sizeof(int));
// Assume some processing or function calls
free(array);  // Freeing the dynamically allocated memory
// More code...
free(array);  // Accidentally freeing 'array' again
```
Here, `array` is freed twice, leading to undefined behavior and potential crashes.

#### Correct Practice: Nullifying Pointers After Freeing

To prevent double freeing issues, **nullify pointers** after freeing them:
```c
int *array = malloc(10 * sizeof(int));
// Assume some processing or function calls
free(array);
array = NULL;  // Nullify the pointer after freeing
// More code...
free(array);  // Now, even if accidentally called again, it's safe
```

#### Why Nullify Pointers?

- Setting a pointer to `NULL` after freeing ensures that subsequent attempts to free it again (whether intentionally or unintentionally) will have no effect. The `free()` function does nothing when called with a `NULL` pointer, avoiding runtime errors.

#### Summary

- Always assign `NULL` to pointers after freeing dynamically allocated memory to prevent accidental double freeing.
- This practice simplifies memory management and reduces the risk of runtime errors in C programs.

#### Example Code Analysis

In the lecture, the speaker demonstrated this practice with an example where:
```c
int *array = malloc(10 * sizeof(int));
// Process array
free(array);
array = NULL;
// More code...
free(array);  // Even if called again, no issues due to the pointer being NULL
```
This approach ensures robust memory management and enhances program reliability.

#### Conclusion

Implementing the practice of nullifying pointers after freeing memory is essential for writing reliable and error-free C programs. By adopting this habit, developers can avoid common pitfalls such as double freeing and simplify memory management tasks.

---

**Generated Code:**

```c
#include <stdlib.h>

void process_array(int **ptr, int num_elements) {
    // Process the array
    free(*ptr);
    *ptr = NULL;  // Nullify the pointer to avoid double freeing
}

int main() {
    int *array = malloc(10 * sizeof(int));
    // Process array
    process_array(&array, 10);
    array = NULL;  // Optional: Nullifying in main
    // More code...
    free(array);  // Safe to call, as 'array' is NULL
    return 0;
}
```

This code snippet demonstrates the implementation of nullifying pointers after freeing memory to prevent double freeing issues in C programs.

### Title: Memory leaks and how to prevent them

**Introduction to Memory Leaks**

Memory leaks are a common issue in programming, particularly in languages like C where memory management is manual. They occur when allocated memory is not properly deallocated after it is no longer needed, leading to wasted memory resources and potential program instability.

**Types of Memory Allocation**

1. **Statically Allocated Variables**:
   - Variables declared within a function with a fixed size.
   - Allocated on the stack.
   - Automatically deallocated when the function exits.

2. **Globally Allocated Variables**:
   - Variables declared outside of any function, accessible throughout the program.
   - Deallocated when the program terminates.

3. **Dynamically Allocated Variables**:
   - Variables allocated using functions like `malloc` in C.
   - Allocated on the heap, not automatically managed.
   - Requires explicit deallocation using `free`.

**Example of Dynamically Allocated Memory**

```c
int *a = malloc(sizeof(int));
*a = 5;
printf("%d\n", *a);
free(a);
```

- Dynamically allocated memory must be freed explicitly using `free()` when it is no longer needed.

**Understanding Memory Leaks**

- **Definition**: Occurs when memory is allocated dynamically but not freed after its use.
- **Example**: Allocating an array dynamically within a function and not freeing it before the function exits.

```c
int* random_array(int n) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100;
    }
    return arr; // Memory leak if not freed
}
```

- **Issue**: Returning a pointer to dynamically allocated memory without freeing it leads to a memory leak.

**Preventing Memory Leaks**

- **Best Practices**:
  - Always pair memory allocation with deallocation in the same function.
  - Prefer passing dynamically allocated memory as parameters instead of returning it.
  - Use global or stack-allocated variables when appropriate to avoid manual memory management.

**Common Pitfalls**

- **Modifying Pointers**: Altering pointers without updating the original reference can lead to invalid memory deallocation.

```c
int *array = malloc(10 * sizeof(int));
int *temp = array; // Create a copy if modification is necessary
array++; // Invalidates original pointer for deallocation
free(temp); // Correct way to deallocate
```

**Conclusion**

Memory leaks in C can be subtle and problematic if not managed properly. By understanding dynamic memory allocation and ensuring proper deallocation practices, developers can minimize memory leaks and enhance program reliability.

**Code Summary**

Below is a summarized version of the example code provided in the lecture:

```c
#include <stdio.h>
#include <stdlib.h>

void random_array(int *array, int n) {
    for (int i = 0; i < n; ++i) {
        array[i] = rand() % 100;
    }
}

int main() {
    int array[15]; // Stack-allocated array
    random_array(array, 15);
    for (int i = 0; i < 15; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
```

This code demonstrates stack allocation and proper usage without memory leaks.

---

This detailed explanation covers the fundamentals of memory leaks in C, how they occur, and best practices to prevent them, summarized from the lecture transcript provided.