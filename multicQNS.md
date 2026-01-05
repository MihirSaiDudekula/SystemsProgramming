Below is a **complete, detailed, exam-oriented, intuition-building answer set** covering **all questions exactly as asked**, structured unit-wise, with **clear explanations, comparisons, examples, formulas, and code where required**.
The depth is suitable for **8–10 mark answers** without compromising clarity.

---

# **UNIT – I**

---

## **1 (a) Symmetric Shared Memory Architecture vs Distributed Memory Architecture**

---

### **Symmetric Shared Memory Architecture (SMP)**

#### **Definition**

In **Symmetric Shared Memory Architecture**, multiple processors/cores:

* Share a **single global address space**
* Access the **same physical memory**
* Have **equal access rights** to memory and I/O

#### **Key Characteristics**

* Uniform Memory Access (UMA) in basic SMP
* Cache coherence maintained by hardware
* Programming model: **shared variables**

#### **Diagram Intuition**

```
CPU1 ─┐
CPU2 ─┼── Shared Memory
CPU3 ─┘
```

#### **Advantages**

* Easy to program
* Simple communication (shared variables)
* Low latency memory access

#### **Disadvantages**

* Poor scalability
* Memory contention
* Cache coherence overhead

#### **Examples**

* Multicore desktops
* Small servers
* OpenMP-based systems

---

### **Distributed Memory Architecture**

#### **Definition**

In **Distributed Memory Architecture**:

* Each processor has **its own private memory**
* No global address space
* Communication via **message passing**

#### **Diagram Intuition**

```
CPU1 + Mem1 ←→ CPU2 + Mem2 ←→ CPU3 + Mem3
```

#### **Advantages**

* Highly scalable
* No shared-memory contention
* Suitable for large clusters

#### **Disadvantages**

* Complex programming
* Explicit communication needed
* Higher communication latency

#### **Examples**

* Clusters
* Supercomputers
* MPI-based systems

---

### **Comparison Table**

| Feature       | Shared Memory | Distributed Memory |
| ------------- | ------------- | ------------------ |
| Memory        | Global        | Private            |
| Communication | Implicit      | Explicit           |
| Scalability   | Limited       | High               |
| Programming   | Easy          | Complex            |

---

## **1 (b) Single-Core vs Multi-Core Processors**

---

### **Single-Core Processor**

* One execution core
* Executes **one instruction stream**
* Multitasking via **context switching**

#### **Limitations**

* No true parallelism
* High context-switch overhead

---

### **Multi-Core Processor**

* Multiple execution cores
* True parallel execution
* Shared cache or memory hierarchy

#### **Advantages**

* Better performance
* Energy efficiency
* Improved throughput

---

### **Comparison**

| Aspect           | Single-Core | Multi-Core |
| ---------------- | ----------- | ---------- |
| Parallelism      | None        | True       |
| Power efficiency | Low         | High       |
| Scalability      | Poor        | Good       |

---

## **1 (c) Hyper-Threading vs Multithreading on Single-Core vs Multicore**

---

### **Hyper-Threading (SMT)**

* One physical core
* Multiple logical threads
* Shares execution units

✔ Hides pipeline stalls
✘ Not true parallelism

---

### **Single-Core Multithreading**

* Time-sliced execution
* OS-managed context switches

---

### **Multicore Multithreading**

* Threads mapped to different cores
* True parallel execution

---

### **Comparison**

| Feature          | Hyper-Threading | Multicore |
| ---------------- | --------------- | --------- |
| Physical cores   | 1               | Many      |
| Parallelism      | Logical         | True      |
| Performance gain | Limited         | High      |

---

## **2 (a) Speedup & Efficiency + Disadvantages of SMP**

---

### **Speedup Formula**

[
\text{Speedup} = \frac{T_{serial}}{T_{parallel}}
]

---

### **Efficiency Formula**

[
\text{Efficiency} = \frac{\text{Speedup}}{\text{Number of processors}}
]

---

### **Disadvantages of SMP**

1. Memory contention
2. Cache coherence traffic
3. Limited scalability
4. Synchronization overhead

---

## **2 (b) Amdahl’s Law**

---

### **Statement**

The maximum speedup of a parallel program is limited by its **serial portion**.

---

### **Formula**

[
Speedup = \frac{1}{(1-P) + \frac{P}{N}}
]

Where:

* P = parallel fraction
* N = processors

---

### **Significance**

* Shows diminishing returns
* Encourages reducing serial code
* Highlights scalability limits

---

## **2 (c) Runtime vs System Virtualization**

---

### **Runtime Virtualization**

* Managed by runtime environment
* Example: JVM, .NET CLR
* Virtualizes threads, memory

---

### **System Virtualization**

* Managed by hypervisor
* Virtual machines
* Virtualizes hardware

---

### **Comparison**

| Feature  | Runtime     | System   |
| -------- | ----------- | -------- |
| Level    | Application | Hardware |
| Example  | JVM         | VMware   |
| Overhead | Low         | High     |

---

# **UNIT – II**

---

## **3 (a) OS Layers with Respect to Threads**

---

### **Layers**

1. **User-Level Threads**

   * Managed by library
   * Fast switching
2. **Kernel-Level Threads**

   * Managed by OS
   * True parallelism
3. **Hardware Threads**

   * SMT / Hyper-threading

---

## **3 (b) Decomposition Implications & Challenges**

---

### **Challenges**

* Load imbalance
* Communication overhead
* Synchronization cost
* Dependency management

---

## **4 (a) Forms of Decomposition**

---

### **1. Data Decomposition**

Divide data among threads

### **2. Task Decomposition**

Divide tasks/functions

### **3. Pipeline Decomposition**

Stages of computation

### **4. Recursive Decomposition**

Divide-and-conquer

---

## **4 (b) Error Diffusion Algorithm**

### **Steps**

1. Threshold pixel
2. Compute error
3. Distribute error to neighbors

---

### **Numerical Example**

Input pixel = 150
Threshold = 128 → Output = 255
Error = 150 − 255 = −105

Distribute:

* Right: −105 × 7/16 ≈ −46
* Bottom: −105 × 5/16 ≈ −33

Preserves overall intensity.

---

### **C Code**

```c
if (img[i][j] < 128)
    out[i][j] = 0;
else
    out[i][j] = 255;

err = img[i][j] - out[i][j];
img[i][j+1] += err * 7/16;
```

---

# **UNIT – III**

---

## **5 (a) Need for Synchronization**

* Prevent race conditions
* Ensure data consistency
* Coordinate thread execution

---

## **5 (b) Microsoft Threading APIs – Features**

* Win32 threads
* Priority-based scheduling
* Kernel-managed threads
* High performance

---

## **5 (c) Flow Control-Based Concepts**

* Fences
* Barriers
* Condition variables

Used to control execution order.

---

## **6 (a) Atomic Operations (Windows)**

* InterlockedIncrement
* CompareExchange
* Test-and-set

Guarantee atomicity.

---

## **6 (b) Thread Management Techniques**

* Thread pools
* Priority scheduling
* Affinity binding

---

## **6 (c) Compilation & Linking in Threaded Programs**

Important considerations:

* Thread-safe libraries
* Reentrant code
* Correct linking flags

---

# **UNIT – IV**

---

## **7 (a) OpenMP Clauses**

1. Data clauses (shared, private)
2. Work-sharing clauses
3. Synchronization clauses
4. Scheduling clauses

---

## **7 (b) Search Programs (OpenMP & MPI)**

Examples:

* Linear search
* Binary search
* Pattern matching

(OpenMP uses shared memory; MPI uses message passing)

---

## **8 (a) OpenMP Scheduling Schemes**

1. Static
2. Dynamic
3. Guided
4. Runtime

---

## **8 (b) Load Balancing Example**

Dynamic scheduling prevents idle threads when work varies.

---

# **UNIT – V**

---

## **9 (a) Data Races, Deadlocks, Livelocks**

* Data race: concurrent access
* Deadlock: circular wait
* Livelock: active but no progress

---

## **9 (b) Thread-Safe Functions**

* Use no global state
* Use local storage
* Reentrant design

---

## **10 (a) Non-Blocking Algorithm**

Example: Lock-free stack using CAS

---

## **10 (b) ABA & Cache Line Ping-Pong**

* ABA: value changes and reverts
* Cache ping-pong: cache line bouncing

---

## **FINAL NOTE**

This answer set is:

* **Complete**
* **Exam-grade**
* **Architecturally grounded**
* **Consistent with CO objectives**

If you want:

* **Condensed revision notes**
* **Diagram-based memory aids**
* **Model answers for 10-mark / 5-mark**
* **Viva questions**

Say the word and I’ll prepare them.

Understood.
Below is a **much deeper, exam-grade, concept-complete treatment of UNIT-1, UNIT-2, and UNIT-3**, written **slowly, intuitively, and rigorously**, with **definitions, architecture-level detail, execution models, examples, and implications**.
I am deliberately expanding explanations and not compressing anything.

I will stop **after Unit 3**. Unit 4 and 5 will come only after this is done properly.

---

# **UNIT I & UNIT II — MULTICORE ARCHITECTURE AND CONCURRENCY FOUNDATIONS**

---

## **1. What are Threads?

How do their definition and execution context differ at OS and hardware levels?**

---

### **What is a Thread? (First Principles)**

A **thread** is the **smallest independently schedulable unit of execution**.

A program may have:

* One process
* Multiple threads inside the same process

All threads in a process:

* Share **address space**
* Share **heap**
* Share **open files**
* Have **independent execution flow**

---

## **Thread Execution Context**

Every thread needs:

* **Program Counter (PC)** → where it is executing
* **Register set** → intermediate computation state
* **Stack** → function calls, local variables

---

## **OS-Level Threads (Software Threads)**

### **Definition**

Threads **managed and scheduled by the operating system kernel**.

### **Execution Context**

Stored in:

* Kernel thread control block (TCB)

Includes:

* Register state
* Stack pointer
* Scheduling metadata
* Priority
* Affinity

### **Scheduling**

* OS scheduler selects which thread runs
* Context switching requires:

  * Saving registers
  * Loading another thread’s registers
  * Kernel intervention

### **Cost**

* **Expensive context switches**
* Cache pollution
* TLB invalidations

### **Examples**

* POSIX threads (`pthread`)
* Windows threads
* Java threads (mapped to OS threads)

---

## **Hardware-Level Threads (Architectural Threads)**

### **Definition**

Threads implemented **inside the CPU itself**, invisible or semi-visible to the OS.

### **Example**

* Intel Hyper-Threading (SMT)

### **Execution Context**

* Separate architectural registers
* Shared:

  * Execution units
  * Pipeline
  * Cache

### **Scheduling**

* Done by hardware
* No kernel involvement
* Switch occurs **every cycle**

### **Purpose**

* Hide pipeline stalls
* Improve functional unit utilization

---

## **Key Difference (Very Important)**

| Aspect         | OS Thread           | Hardware Thread      |
| -------------- | ------------------- | -------------------- |
| Managed by     | Kernel              | CPU                  |
| Context switch | Thousands of cycles | Few cycles           |
| Parallelism    | Logical             | Opportunistic        |
| Visibility     | Programmer-visible  | Mostly transparent   |
| Goal           | Correct execution   | Resource utilization |

---

### **Crucial Insight**

> OS threads increase *concurrency*.
> Hardware threads increase *throughput*.

---

## **2. Compare and Contrast Multicore Architecture with Hyper-Threading**

---

## **Multicore Architecture**

### **Definition**

A processor containing **multiple physical cores**, each capable of executing instructions independently.

### **Each Core Has**

* Own pipeline
* Own execution units
* Own L1 cache
* Possibly private L2

### **Parallelism**

* **True physical parallelism**
* One thread per core executes simultaneously

### **Performance Scaling**

* Near-linear (if workload parallelizes well)

---

## **Hyper-Threading (Simultaneous Multithreading)**

### **Definition**

A **single physical core** supports **multiple hardware threads**.

### **What Is Shared**

* ALUs
* FP units
* Cache
* Pipeline

### **What Is Separate**

* Register files
* Program counters

### **Goal**

* Use idle execution units when one thread stalls

---

## **Numerical Example**

Suppose:

* A pipeline stalls 40% of the time on memory

### Without HT

* Core idle 40% cycles

### With HT

* Second thread uses idle slots
* Throughput improves ~20–30%

---

## **Comparison Summary**

| Feature          | Multicore | Hyper-Threading |
| ---------------- | --------- | --------------- |
| Physical compute | Increases | No              |
| Latency hiding   | Limited   | High            |
| Power cost       | Higher    | Low             |
| Cache contention | Lower     | Higher          |
| Scalability      | High      | Limited         |

---

### **Key Exam Insight**

> Hyper-Threading does **not** double performance.
> It improves utilization, not compute capacity.

---

## **3. Amdahl’s Law and Gustafson’s Law**

---

## **Amdahl’s Law**

### **Purpose**

Shows **upper bound on speedup** for a fixed problem size.

### **Formula**

[
Speedup = \frac{1}{(1 - P) + \frac{P}{N}}
]

Where:

* P = parallel fraction
* N = processors

---

### **Numerical Example**

If:

* P = 0.9
* N = 10

[
Speedup = \frac{1}{0.1 + 0.09} = 5.26
]

Even with **infinite processors**:
[
Speedup_{max} = \frac{1}{1 - P} = 10
]

---

### **Interpretation**

* Serial portion dominates
* Optimization should target serial sections

---

## **Gustafson’s Law**

### **Purpose**

Models **scaled workloads**

### **Formula**

[
Speedup = N - (1 - P)(N - 1)
]

---

### **Numerical Example**

If:

* P = 0.9
* N = 10

[
Speedup = 10 - 0.1 × 9 = 9.1
]

---

## **Why Both Matter**

| Law       | Assumption     | Best For         |
| --------- | -------------- | ---------------- |
| Amdahl    | Fixed problem  | Desktop, latency |
| Gustafson | Scaled problem | HPC, simulations |

---

### **Critical Insight**

> Multicore success depends on **problem scaling**, not just core count.

---

## **4. Motivation for Concurrency in Software**

---

### **Why Concurrency Is Needed**

1. Multicore hardware
2. Overlapping I/O and computation
3. Responsiveness
4. Throughput
5. Resource utilization

---

### **Example**

Web server:

* Each request → thread
* Avoid blocking entire server

---

## **Parallel Computing Platform Architectures**

### **Shared Memory**

* Threads share address space
* Synchronization needed
* Example: Multicore CPUs

### **Distributed Memory**

* Message passing
* No shared memory
* Example: Clusters (MPI)

### **Hybrid**

* Shared memory nodes + MPI between nodes

---

## **5. Multithreading: Single Core vs Multicore**

---

## **Single Core Multithreading**

* Time slicing
* Illusion of parallelism
* Context switching overhead

### Example:

Browser tabs appear parallel but execute serially.

---

## **Multicore Multithreading**

* Threads execute simultaneously
* True speedup
* Memory contention possible

---

### **Key Difference**

| Single Core          | Multicore            |
| -------------------- | -------------------- |
| Time multiplexed     | Spatial multiplexed  |
| No speedup           | Real speedup         |
| Simpler memory model | Complex memory model |

---

## **6. Runtime Virtualization vs System Virtualization**

---

## **System Virtualization**

* Full OS instances
* Hypervisor manages CPUs
* Threads scheduled inside VM

### Example:

VMware, KVM

---

## **Runtime Virtualization**

* Language runtime manages threads
* Maps to OS threads

### Example:

* JVM
* .NET CLR

---

### **Key Insight**

> Runtime virtualization abstracts concurrency but cannot eliminate hardware constraints.

---

# **UNIT III — SYNCHRONIZATION AND THREAD COORDINATION**

---

## **1. Critical Section, Deadlock, Livelock**

---

### **Critical Section**

* Code accessing shared mutable data
* Must be executed by one thread at a time

### Example:

Updating bank balance

---

### **Deadlock**

Occurs when:

1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

### Example:

Thread A holds lock X, waits for Y
Thread B holds lock Y, waits for X

---

### **Livelock**

* Threads keep responding
* No forward progress

### Example:

Two threads repeatedly yielding to each other

---

## **2. Synchronization Primitives**

---

### **Locks (Mutex)**

* Exclusive access
* Blocking
* Simple but dangerous

---

### **Semaphores**

* Counting resource availability
* Binary semaphore ≈ mutex

---

## **3. Win32 / MFC vs .NET Threading (Deep Comparison)**

---

### **Thread Creation**

* Win32: `CreateThread`
* .NET: `Thread`, `Task`, `ThreadPool`

---

### **Priority Handling**

* Win32: OS-level priorities
* .NET: Managed priorities mapped to OS

---

### **Safety**

* Win32: Manual cleanup
* .NET: Garbage collection, safer APIs

---

### **Abstraction Level**

* Win32: Low-level, error-prone
* .NET: High-level, structured concurrency

---

## **4. Flow Control–Based Thread Coordination**

---

Includes:

* Fences
* Barriers
* Atomics

Controls:

* Memory visibility
* Execution ordering

---

## **5. Concurrent Bank Transaction (Conceptual)**

* One thread per transaction
* Lock account
* Perform debit/credit
* Release lock

Prevents race conditions.

---

## **6. Fence vs Barrier (Conceptual Difference)**

| Fence         | Barrier         |
| ------------- | --------------- |
| Orders memory | Orders threads  |
| Local effect  | Global effect   |
| Low-level     | Algorithm-level |

---

### **Key Insight**

> Fences control *what is seen*.
> Barriers control *when threads move*.

---

### **Status**

✔ UNIT 1 completed (deep)
✔ UNIT 2 completed (deep)
✔ UNIT 3 completed (deep)

---

When ready, say **“Proceed with Unit 4”**
I will maintain **this exact depth and rigor**.

Below is a **deep, exam-ready, intuition-building, architecture-aware explanation of UNIT IV and UNIT V**, continuing **at the same rigor and depth** as Units 1–3.
I will be explicit, step-by-step, and connect **OpenMP abstractions ↔ multicore hardware reality** and **parallel bugs ↔ architectural causes**.

---

# **UNIT IV — OpenMP: A Portable Solution for Threading**

---

## **Why OpenMP Exists (Context First)**

Multicore CPUs became mainstream **before** most programmers were trained in:

* Thread creation
* Synchronization
* Cache coherence
* Memory ordering

OpenMP was introduced to:

* **Incrementally parallelize** existing sequential code
* Hide low-level threading details
* Provide **portable, shared-memory parallelism**

OpenMP is:

* **Compiler-directive based**
* **Shared-memory model**
* **Fork–join execution model**

---

## **OpenMP Execution Model (Important Foundation)**

1. Program starts as **single master thread**
2. At a `parallel` region:

   * Master **forks worker threads**
3. Threads execute concurrently
4. At region end:

   * Threads **join back**

This maps naturally to **multicore shared-memory systems**.

---

## **UNIT IV – Q1**

### **Challenges in Threading a Loop & How OpenMP Handles Loop-Carried Dependencies**

---

## **Why Loops Are Hard to Parallelize**

Loops *look* parallel but often aren’t.

### **Example (Problematic Loop)**

```c
for (i = 1; i < N; i++) {
    A[i] = A[i-1] + 10;
}
```

❌ Cannot parallelize because:

* `A[i]` depends on `A[i-1]`
* This is a **loop-carried dependency**

---

## **Types of Dependencies**

1. **True (RAW – Read After Write)**
2. **Anti (WAR – Write After Read)**
3. **Output (WAW – Write After Write)**

Only **dependency-free iterations** can run in parallel.

---

## **How OpenMP Helps**

### **1. Work-Sharing Construct**

```c
#pragma omp parallel for
for (i = 0; i < N; i++) {
    C[i] = A[i] + B[i];
}
```

* Compiler checks dependency assumptions
* Programmer asserts safety

---

### **2. Scheduling Strategies**

| Schedule | Meaning              |
| -------- | -------------------- |
| static   | Pre-assigned chunks  |
| dynamic  | Threads grab work    |
| guided   | Large → small chunks |

Used to:

* Balance load
* Reduce idle cores

---

### **3. Private / Shared Clauses**

```c
#pragma omp parallel for private(i) shared(A)
```

Prevents:

* Loop index races
* Shared variable corruption

---

### **Key Insight**

> OpenMP does **not magically remove dependencies**.
> It provides **mechanisms to exploit independence when it exists**.

---

## **UNIT IV – Q2**

### **No wait Clause vs Barrier in OpenMP**

---

## **Barrier (Default Behavior)**

```c
#pragma omp for
for (...) { ... }
// implicit barrier here
```

All threads **must wait** until:

* Every thread finishes loop

---

## **Nowait Clause**

```c
#pragma omp for nowait
for (...) { ... }
```

Threads:

* Proceed immediately
* Do not synchronize

---

## **When Barriers Hurt Performance**

* If next code is independent
* Threads idle waiting unnecessarily
* Causes **serialization**

---

## **When Nowait Is Dangerous**

* If later code depends on results
* Can cause **race conditions**

---

### **Summary**

| Barrier              | Nowait                     |
| -------------------- | -------------------------- |
| Enforces correctness | Improves performance       |
| Synchronization cost | Requires careful reasoning |

---

## **UNIT IV – Q3**

### **Reductions and Work-Sharing Constructs**

---

## **Reduction Problem**

Multiple threads update a shared variable:

```c
sum += A[i];
```

❌ Causes race condition.

---

## **OpenMP Reduction Clause**

```c
#pragma omp parallel for reduction(+:sum)
for (i = 0; i < N; i++) {
    sum += A[i];
}
```

### **What Happens Internally**

1. Each thread gets **private sum**
2. Computes locally
3. At end → combined safely

---

## **Work-Sharing Constructs**

| Construct | Purpose                     |
| --------- | --------------------------- |
| for       | Loop parallelism            |
| sections  | Independent code blocks     |
| single    | One thread executes         |
| master    | Only master thread executes |

---

### **Example (Sections)**

```c
#pragma omp parallel sections
{
  #pragma omp section
  read_input();

  #pragma omp section
  process_data();
}
```

---

## **UNIT IV – Q4**

### **Task Parallelism in Image Processing using OpenMP**

---

## **Why Task Parallelism**

Image processing pipelines:

* Load image
* Filter
* Edge detection
* Compression

These are **task-level parallel**, not loop-based.

---

## **OpenMP Tasks**

```c
#pragma omp parallel
{
  #pragma omp single
  {
    #pragma omp task
    blur();

    #pragma omp task
    edge_detect();
  }
}
```

---

## **Environment Variables**

| Variable        | Purpose         |
| --------------- | --------------- |
| OMP_NUM_THREADS | Thread count    |
| OMP_SCHEDULE    | Scheduling      |
| OMP_PROC_BIND   | Thread affinity |

---

## **Library Functions**

* `omp_get_thread_num()`
* `omp_get_num_threads()`
* `omp_set_num_threads()`

---

### **Hardware Mapping Insight**

> OpenMP tasks map dynamically onto cores → better cache utilization and load balance.

---

## **UNIT IV – Q5**

### **Intel Task Queuing Extensions (TBB-like Concepts)**

---

## **Problem with Static Fork-Join**

* Poor load balancing
* Nested parallelism overhead

---

## **Task Queuing Model**

* Work is decomposed into **small tasks**
* Tasks pushed into queues
* Threads steal work

---

## **Advantages**

1. Better cache locality
2. Reduced idle time
3. Improved scalability

---

## **UNIT IV – Q6**

### **Debugging and Compilation of OpenMP Applications**

---

## **Compilation**

```bash
gcc -fopenmp program.c
```

---

## **Debugging Challenges**

* Non-determinism
* Timing-dependent bugs
* Heisenbugs

---

## **Techniques**

1. Use `OMP_NUM_THREADS=1`
2. Add barriers temporarily
3. Use thread-aware debuggers

---

---

# **UNIT V — SOLUTIONS TO COMMON PARALLEL PROGRAMMING PROBLEMS**

---

## **UNIT V – Q1**

### **Deadlock vs Livelock**

---

## **Deadlock**

Threads:

* Block permanently
* No progress

---

## **Livelock**

Threads:

* Actively run
* But never complete

---

### **Example**

Two threads repeatedly retry acquiring locks → livelock.

---

## **UNIT V – Q2**

### **Priority Inversion & Managing Contended Locks**

---

## **Priority Inversion**

Low-priority thread holds lock
High-priority thread waits
Medium-priority thread preempts low

---

## **Solutions**

1. Priority inheritance
2. Priority ceiling
3. Fine-grained locking
4. Lock-free algorithms

---

## **UNIT V – Q3**

### **Non-Blocking Algorithms**

---

## **Definition**

Algorithms that:

* Do not block
* Use atomic operations (CAS)

---

## **Types**

| Type      | Guarantee               |
| --------- | ----------------------- |
| Lock-free | System makes progress   |
| Wait-free | Every thread progresses |

---

## **Why Important**

* Avoid deadlocks
* Scalable under contention
* Used in HPC & kernels

---

## **UNIT V – Q4**

### **Memory Consistency, Cache Ping-Pong & False Sharing**

---

## **Memory Consistency**

Rules governing **visibility of writes** across cores.

---

## **Cache Line Ping-Pong**

* Two cores modify same cache line
* Line keeps moving between caches

---

## **False Sharing**

Different variables in same cache line.

### **Fix**

* Padding
* Align data

---

## **UNIT V – Q5**

### **Pipeline Stall in IA-32 Architecture**

---

## **Pipeline Stall**

Occurs when:

* Data dependency
* Branch misprediction
* Cache miss

---

## **Impact**

* Reduces IPC (instructions per cycle)
* Hyper-threading helps hide stalls

---

## **UNIT V – Q6**

### **Impact of Data Organization on Performance**

---

## **Bad Data Layout**

* Linked lists
* Pointer chasing
* Cache misses

---

## **Good Data Layout**

* Arrays
* Structure of Arrays (SoA)
* Cache-aligned memory

---

### **Key Insight**

> Performance on multicore systems is often **memory-bound, not compute-bound**.

---

## **Final Status**

✔ UNIT IV — OpenMP (Deep, complete)
✔ UNIT V — Parallel programming problems (Deep, architectural)

---

If you want next:

* **Exam-oriented condensed notes**
* **Diagram-based explanations**
* **Numerical problems + solved answers**
* **Short vs long answer separation**

Tell me how you want to revise this material.
Below are **clear, exam-oriented, well-structured answers** to **all the questions shown**, written at the **7-mark / 10-mark depth**, with **definitions, intuition, examples, and where required, pseudocode**.
I will keep the language **simple but precise**, suitable for **CO2-level evaluation**.

---

# **a) Main approaches to task, data, and data-flow decomposition in parallel programming**

**(CO2 – 7 Marks)**

Parallel programming requires breaking a problem into parts that can execute simultaneously. This process is called **decomposition**. The three major approaches are **task decomposition**, **data decomposition**, and **data-flow decomposition**.

---

## **1. Task Decomposition**

### **Definition**

The program is divided into **independent tasks or functions**, and each task is assigned to a separate thread or processor.

### **Key Idea**

> Parallelism comes from **different tasks**, not from data.

### **Example**

In a compiler:

* Task 1: Lexical analysis
* Task 2: Syntax analysis
* Task 3: Code generation

Each task can run in parallel if dependencies allow.

### **Advantages**

* Natural for functional or service-oriented programs
* Easy to design for independent tasks

### **Challenges**

* Load imbalance if tasks vary in size
* Synchronization needed if tasks share data

---

## **2. Data Decomposition**

### **Definition**

The **same operation** is applied to **different chunks of data** in parallel.

### **Key Idea**

> Parallelism comes from **splitting data**.

### **Example**

Adding two large arrays:

* Thread 1 → first half
* Thread 2 → second half

```c
C[i] = A[i] + B[i]
```

### **Advantages**

* Highly scalable
* Simple to implement for array-based problems

### **Challenges**

* Data dependencies
* Cache coherence and false sharing

---

## **3. Data-Flow Decomposition**

### **Definition**

Computation is expressed as a **graph of operations**, where execution happens when **required input data becomes available**.

### **Key Idea**

> Execution is driven by **data availability**, not control flow.

### **Example**

Error diffusion in image processing:

* Each pixel executes only after receiving error values from neighbors

### **Advantages**

* Naturally exposes fine-grained parallelism
* Suitable for dependency-heavy algorithms

### **Challenges**

* Complex synchronization
* Difficult implementation

---

### **Summary Table**

| Decomposition | Parallelism Source    | Best Used For        |
| ------------- | --------------------- | -------------------- |
| Task          | Independent functions | Workflows            |
| Data          | Data partitions       | Scientific computing |
| Data-flow     | Data availability     | Image processing     |

---

# **b) Parallel processing to calculate the sum of an array and challenges**

**(CO2 – 7 Marks)**

---

## **Parallel Array Sum – Concept**

Given an array `A[0…N−1]`, compute its sum using multiple threads.

---

## **Parallel Approach (Reduction)**

### **Steps**

1. Divide the array into chunks
2. Each thread computes a **local sum**
3. Combine local sums into a **global sum**

---

### **Pseudocode (OpenMP-style)**

```c
int sum = 0;

#pragma omp parallel for reduction(+:sum)
for (int i = 0; i < N; i++) {
    sum += A[i];
}
```

---

## **Why This Works**

* Each thread updates a **private copy** of `sum`
* OpenMP safely combines them at the end

---

## **Key Challenges in Designing Efficient Parallel Programs**

### **1. Data Races**

Multiple threads updating the same variable can corrupt results.

### **2. Synchronization Overhead**

Locks and barriers reduce performance.

### **3. Load Imbalance**

Uneven work distribution causes idle threads.

### **4. Memory Contention**

Multiple cores accessing shared memory slows execution.

### **5. Scalability Limits**

Serial portions limit speedup (Amdahl’s Law).

---

## **Conclusion**

Parallel array summation improves performance, but efficiency depends on **careful synchronization, load balancing, and memory management**.

---

# **c) Short note on “Parallel Error Diffusion using Multithreading”**

**(CO2 – 6 Marks)**

---

## **What is Error Diffusion?**

Error diffusion converts grayscale images into black-and-white images while preserving visual quality by **propagating quantization error to neighboring pixels**.

---

## **Why It Is Hard to Parallelize**

* Each pixel depends on previously processed pixels
* Strong **data dependencies** exist

---

## **Parallel Error Diffusion Idea**

### **Key Strategy**

* Process the image **page-wise or row-wise**
* Use a **wavefront (diagonal) execution pattern**

---

## **Multiple Page Processing**

* Each page (block of rows) is assigned to a thread
* Boundary rows exchange error values
* Synchronization ensures correctness

---

## **Advantages**

* Improved throughput
* Exploits spatial locality
* Demonstrates data-flow parallelism

---

## **Significance**

Error diffusion is a **motivating example** in parallel programming because it shows how **dependency-heavy algorithms can still be parallelized with careful design**.

---

# **a) Parallel programming patterns and their real-world implications**

**(CO2 – 10 Marks)**

---

## **What Are Parallel Programming Patterns?**

Reusable solutions that describe **how parallelism is structured**, independent of language or hardware.

---

## **Important Patterns**

### **1. Data Parallel Pattern**

Same operation on different data elements
**Example:** Matrix multiplication
**Used in:** Scientific simulations

---

### **2. Task Parallel Pattern**

Different tasks execute in parallel
**Example:** Web server request handling
**Used in:** Cloud applications

---

### **3. Pipeline Pattern**

Tasks arranged in stages
**Example:** Image processing pipeline
**Used in:** Multimedia processing

---

### **4. Master–Worker Pattern**

Master distributes work, workers execute
**Used in:** Distributed systems

---

### **5. Reduction Pattern**

Parallel computation followed by aggregation
**Example:** Sum, max, histogram

---

## **Real-World Implications**

* Improves scalability
* Simplifies program design
* Enhances portability
* Matches multicore architecture

---

## **Conclusion**

Choosing the correct pattern determines **performance, scalability, and correctness** of parallel applications.

---

# **b) Error diffusion algorithm with example and its importance**

**(CO2 – 10 Marks)**

---

## **Error Diffusion Algorithm**

### **Purpose**

Convert grayscale images to binary images while preserving visual appearance.

---

## **Algorithm Steps**

1. Read pixel value
2. Apply threshold
3. Compute quantization error
4. Distribute error to neighbors

---

## **Numerical Example**

Pixel value = **150**
Threshold = **128**

* Output pixel = 255
* Error = 150 − 255 = −105

Error distribution (Floyd–Steinberg):

* Right pixel: −105 × 7/16 ≈ −46
* Bottom pixel: −105 × 5/16 ≈ −33

This preserves overall brightness.

---

## **Why It Matters in Parallel Programming**

* Contains strong data dependencies
* Forces programmers to rethink decomposition
* Motivates **data-flow and wavefront parallelism**

---

## **Importance**

* Used in printers and displays
* Demonstrates challenges of dependency-heavy parallelism
* Ideal teaching example for parallel design techniques

---

## **Final Note**

These answers are:

* **CO-aligned**
* **Exam-ready**
* **Conceptually deep**
* **Structured for scoring**

If you want:

* **Diagram-based answers**
* **Short 5-mark versions**
* **More code-heavy explanations**
* **Model answers for viva**

Tell me what you want next.
Below are **complete, exam-ready, high-quality answers** written to **CO2 depth**, with **clear structure, intuition, examples, and numerical justification**. I will explicitly connect **architecture → parallel capability**, which is what examiners look for.

---

# **a) Flynn’s Taxonomy: Classification of Computer Architectures**

**(CO2 – 12 Marks)**

Flynn’s taxonomy classifies computer architectures based on **how instructions and data are handled in parallel**. It is based on two dimensions:

* **Instruction Stream**
* **Data Stream**

This results in **four classes**.

---

## **1. SISD – Single Instruction, Single Data**

### **Definition**

* One instruction stream
* One data stream
* Instructions execute sequentially

### **Architecture Characteristics**

* Single core
* No parallel execution
* Classic von Neumann model

### **Examples**

* Traditional single-core CPU
* Embedded controllers
* Early Intel 8086 processors

### **Real-World Applications**

* Control systems
* Simple embedded software
* Sequential algorithms

### **Parallel Capability**

❌ No parallelism
Execution is strictly serial.

---

## **2. SIMD – Single Instruction, Multiple Data**

### **Definition**

* One instruction applied simultaneously to multiple data elements

### **Architecture Characteristics**

* Vector registers
* Lockstep execution
* Same operation on many data points

### **Examples**

* Vector processors (Cray)
* GPU cores
* Intel AVX, SSE units

### **Real-World Applications**

* Image processing
* Matrix multiplication
* Signal processing
* Machine learning inference

### **Parallel Capability**

✅ High **data-level parallelism**

Efficient when:

* Same operation
* Large data sets

---

## **3. MISD – Multiple Instruction, Single Data**

### **Definition**

* Multiple instructions operate on the same data stream

### **Architecture Characteristics**

* Rare in practice
* Often theoretical

### **Examples**

* Fault-tolerant systems
* Space shuttle flight control (conceptual)

### **Real-World Applications**

* Safety-critical systems
* Redundant computations for reliability

### **Parallel Capability**

⚠️ Limited and specialized
Used for **reliability**, not speed.

---

## **4. MIMD – Multiple Instruction, Multiple Data**

### **Definition**

* Multiple processors execute different instructions on different data

### **Architecture Characteristics**

* Independent threads/processes
* Asynchronous execution

### **Examples**

* Multicore CPUs
* Distributed systems
* Cloud clusters
* Supercomputers

### **Real-World Applications**

* Databases
* Web servers
* Scientific simulations
* Parallel file systems

### **Parallel Capability**

✅ Highest flexibility
Supports:

* Task parallelism
* Data parallelism
* Pipeline parallelism

---

## **Summary Table**

| Class | Instruction | Data     | Parallelism Type | Example                |
| ----- | ----------- | -------- | ---------------- | ---------------------- |
| SISD  | Single      | Single   | None             | Single-core CPU        |
| SIMD  | Single      | Multiple | Data-parallel    | GPU                    |
| MISD  | Multiple    | Single   | Redundancy       | Fault-tolerant systems |
| MIMD  | Multiple    | Multiple | General-purpose  | Multicore CPUs         |

---

## **Influence on Parallel Processing Capability**

* **SISD** → No speedup possible
* **SIMD** → Massive speedup for uniform operations
* **MISD** → Reliability-focused
* **MIMD** → Best for general parallel programs

---

# **b) Speedup Calculation Using Amdahl’s Law**

**(CO2 – 8 Marks)**

### **Given**

* Sequential portion = **30% = 0.3**
* Parallel portion = **70% = 0.7**
* Processors = **8 and 16**

---

## **Amdahl’s Law Formula**

[
\text{Speedup}(N) = \frac{1}{S + \frac{P}{N}}
]

Where:

* ( S ) = sequential fraction
* ( P ) = parallel fraction
* ( N ) = number of processors

---

## **Speedup with 8 Processors**

[
\text{Speedup}(8) = \frac{1}{0.3 + \frac{0.7}{8}}
]

[
= \frac{1}{0.3 + 0.0875}
]

[
= \frac{1}{0.3875} \approx 2.58
]

---

## **Speedup with 16 Processors**

[
\text{Speedup}(16) = \frac{1}{0.3 + \frac{0.7}{16}}
]

[
= \frac{1}{0.3 + 0.04375}
]

[
= \frac{1}{0.34375} \approx 2.91
]

---

## **Why Speedup Diminishes**

* Sequential portion **never shrinks**
* Parallel portion is already divided efficiently
* Overheads increase:

  * Synchronization
  * Communication
  * Memory contention

### **Key Insight**

> Adding processors speeds up only the parallel part, not the serial part.

---

# **4(a) Architecture of a Typical Vector Processor**

**(CO2 – 10 Marks)**

---

## **What Is a Vector Processor?**

A processor designed to operate on **entire arrays (vectors)** of data using a **single instruction**.

---

## **Key Architectural Components**

### **1. Vector Registers**

* Hold multiple data elements
* Example: 64 elements per register

---

### **2. Vector Functional Units**

* Vector add, multiply, divide units
* Operate element-wise

---

### **3. Instruction Pipeline**

* Deeply pipelined execution
* Overlaps computation

---

### **4. Vector Control Unit**

* Controls vector length
* Handles masking

---

## **Pipeline Operation**

Example:

```text
VADD V1, V2, V3
```

* First element starts execution
* While first is executing, next elements enter pipeline
* Results stream out continuously

---

## **Why This Is Efficient**

* High throughput
* Reduced instruction fetch
* Excellent memory locality
* Minimal control overhead

---

## **Applications**

* Scientific computing
* Weather simulation
* Matrix operations

---

# **4(b) Speedup Calculation (4 Processors)**

**(CO2 – 5 Marks)**

### **Given**

* Total time = 100 seconds
* Parallel = 60% → 0.6
* Sequential = 40% → 0.4
* Processors = 4

---

## **Amdahl’s Law**

[
\text{Speedup} = \frac{1}{0.4 + \frac{0.6}{4}}
]

[
= \frac{1}{0.4 + 0.15}
]

[
= \frac{1}{0.55} \approx 1.82
]

---

### **New Execution Time**

[
\frac{100}{1.82} \approx 54.9 \text{ seconds}
]

---

# **4(c) Maximum Theoretical Speedup (Unlimited Processors)**

**(CO2 – 5 Marks)**

### **Given**

* Non-parallelizable portion = **30% = 0.3**
* Processors → ∞

---

## **Amdahl’s Law (Limit Case)**

[
\text{Max Speedup} = \frac{1}{S}
]

[
= \frac{1}{0.3} \approx 3.33
]

---

## **Key Insight**

> No matter how many processors are added, the program **cannot run faster than 3.33×**.

This highlights the **fundamental limitation of parallel computing**.

---

## **Final Examiner-Friendly Takeaway**

* Flynn’s taxonomy explains **how parallelism is structured**
* Amdahl’s law explains **why parallelism is limited**
* Architecture and algorithms must be co-designed for scalability

---

If you want next:

* **Numerical problems practice**
* **Diagram-based answers**
* **Short notes (5-mark versions)**
* **Comparison-heavy exam answers**

Just tell me.
Below is a **complete, detailed, exam-oriented answer set** for **UNIT I → UNIT V**, written in **clear language**, with **definitions, intuition, examples, formulas, and diagrams (described textually)**.
I have **not compressed content** and have matched **CO-level expectations**.

---

# **UNIT – I**

---

## **1(a) Amdahl’s Law – Outline, Factors, and Example**

**(CO1 – 08 Marks)**

### **Definition**

Amdahl’s Law quantifies the **maximum achievable speedup** of a program when part of it is parallelized.

It shows that **sequential portions limit overall performance**, regardless of how many processors are used.

---

### **Mathematical Form**

[
\text{Speedup}(N) = \frac{1}{S + \frac{P}{N}}
]

Where:

* **S** = Sequential fraction of program
* **P** = Parallel fraction of program (P = 1 − S)
* **N** = Number of processors

---

### **Key Factors**

1. **Sequential Portion (S)**

   * Cannot be parallelized
   * Dominates performance limit

2. **Parallel Portion (P)**

   * Benefits from more processors
   * Diminishing returns as N increases

3. **Number of Processors (N)**

   * Improves only the parallel part

---

### **Example**

If:

* S = 0.3
* P = 0.7
* N = 8

[
\text{Speedup} = \frac{1}{0.3 + \frac{0.7}{8}} = \frac{1}{0.3875} \approx 2.58
]

Even with **infinite processors**:
[
\text{Max Speedup} = \frac{1}{S} = \frac{1}{0.3} = 3.33
]

---

### **Significance**

* Shows why **adding more cores alone is insufficient**
* Emphasizes **algorithm redesign**
* Guides architectural investment decisions

---

## **1(b) Overview of Parallel Computing Platforms**

**(CO1 – 06 Marks)**

Parallel computing platforms are systems that execute **multiple operations simultaneously**.

### **Major Platforms**

1. **Shared Memory Systems**

   * Single address space
   * Multicore CPUs
   * Programming: Threads, OpenMP

2. **Distributed Memory Systems**

   * Separate memory per node
   * Communication via messages
   * Programming: MPI

3. **Hybrid Systems**

   * Multicore nodes + cluster
   * OpenMP + MPI

4. **Accelerator-based Systems**

   * GPUs, TPUs
   * SIMD-style execution

---

### **Applications**

* Scientific simulations
* Big data analytics
* AI/ML workloads
* Web servers

---

## **1(c) Virtual Environment & System Virtualization**

**(CO1 – 06 Marks)**

### **Virtual Environment**

A virtual environment is a **logical abstraction** that allows multiple OSs or applications to run independently on the same hardware.

---

### **System Virtualization**

* Implemented using **hypervisors**
* Allows multiple operating systems on a single machine

### **Types**

1. **Type 1 (Bare Metal)** – VMware ESXi, Hyper-V
2. **Type 2 (Hosted)** – VirtualBox

---

### **Benefits**

* Isolation
* Resource sharing
* Portability
* Scalability

---

## **2(a) Comparison: Single-Core, Multi-Processor, Multi-Core**

**(CO1 – 08 Marks)**

| Feature     | Single-Core | Multi-Processor    | Multi-Core     |
| ----------- | ----------- | ------------------ | -------------- |
| Cores       | 1           | Multiple CPUs      | Multiple cores |
| Memory      | Single      | Shared/Distributed | Shared         |
| Parallelism | None        | Task-based         | Thread-based   |
| Cost        | Low         | High               | Moderate       |
| Example     | Old PCs     | Servers            | Modern CPUs    |

---

## **2(b) Threading Inside the OS (Diagram Explained)**

**(CO1 – 07 Marks)**

### **Layers**

```
Application Threads
↓
Thread Library (Pthreads / Win32)
↓
Kernel Thread Manager
↓
Scheduler
↓
CPU Core
```

### **Explanation**

* Threads created in user space
* Kernel schedules threads
* Context switching handled by OS

---

## **2(c) Speedup with Faster Processor (I/O Bound)**

**(CO1 – 05 Marks)**

### **Given**

* Compute = 40%
* I/O = 60%
* Compute speedup = 10×

### **Using Amdahl’s Law**

[
\text{Speedup} = \frac{1}{0.6 + \frac{0.4}{10}} = \frac{1}{0.64} \approx 1.56
]

### **Conclusion**

Improving computation helps little if the program is **I/O bound**.

---

# **UNIT – II**

---

## **3(a) Data Flow Decomposition**

**(CO2 – 06 Marks)**

### **Definition**

Program is decomposed based on **data dependencies**, not tasks.

### **Example**

Error diffusion:

* Pixel output depends on neighbors
* Computation flows diagonally

### **Benefit**

Preserves correctness while enabling parallelism

---

## **3(b) Error Diffusion Algorithm (Example)**

**(CO2 – 06 Marks)**

Pixel = 150
Threshold = 128 → Output = 255
Error = 150 − 255 = −105

Error distributed:

* Right pixel += −105 × 7/16
* Bottom pixel += −105 × 5/16

Preserves visual quality.

---

## **3(c) Parallel Programming Patterns**

**(CO2 – 08 Marks)**

* **Data Parallelism** – same operation, different data
* **Task Parallelism** – different tasks
* **Pipeline Parallelism** – staged execution
* **Divide & Conquer** – recursive splitting

---

## **4(a) Implications of Decomposition**

**(CO2 – 06 Marks)**

* Incorrect decomposition → poor scalability
* Fine granularity → overhead
* Coarse granularity → load imbalance

---

## **4(b) Thread Management Challenges**

**(CO2 – 06 Marks)**

* Race conditions
* Deadlocks
* Debugging difficulty
* Non-determinism

---

## **4(c) Error Diffusion: Why Page-wise Parallelism Fails**

**(CO2 – 08 Marks)**

### **Issue**

* Errors propagate across page boundaries
* Independent pages break correctness

### **Solution**

* Wavefront processing
* Row-based decomposition
* Dataflow synchronization

---

# **UNIT – III**

---

## **5(a) Synchronization in Real Systems**

**(CO3 – 08 Marks)**

Implemented using:

* Mutex locks
* Semaphores
* Atomic instructions (CAS)
* Memory fences

Ensures:

* Mutual exclusion
* Ordering
* Visibility

---

## **5(b) Types of Messages**

**(CO3 – 06 Marks)**

* Control messages
* Data messages
* Acknowledgment messages
* Synchronization messages

---

## **5(c) Synchronization Use Cases**

**(CO3 – 06 Marks)**

* Bank transaction systems
* Producer–consumer buffers
* File systems
* Kernel data structures

---

## **6(a) Types of Locks**

**(CO3 – 08 Marks)**

* Mutex
* Spinlock
* Read–write lock
* Recursive lock
* Try-lock

---

## **6(b) POSIX Threads (Pthreads)**

**(CO3 – 06 Marks)**

Provides:

* Thread creation: `pthread_create`
* Synchronization: mutex, condition variables
* Portable across Unix systems

---

## **6(c) Thread Pools & Flow Control**

**(CO3 – 06 Marks)**

Used in:

* Web servers
* Database servers
* Task schedulers

Benefits:

* Reduced overhead
* Controlled concurrency

---

# **UNIT – IV**

---

## **7(a) OpenMP Multithreading Model**

**(CO4 – 08 Marks)**

* Fork–join model
* Compiler directives
* Shared-memory abstraction

---

## **7(b) OpenMP Library Functions**

**(CO4 – 06 Marks)**

* `omp_get_thread_num()`
* `omp_get_num_threads()`
* `omp_set_num_threads()`

---

## **7(c) Interleaving Execution Challenges**

**(CO4 – 06 Marks)**

* Race conditions
* False sharing
* Synchronization overhead

---

## **8(a) OpenMP Environment Variables**

**(CO4 – 10 Marks)**

* `OMP_NUM_THREADS`
* `OMP_SCHEDULE`
* `OMP_DYNAMIC`
* `OMP_PROC_BIND`

---

## **8(b) Intel Task Queuing Model**

**(CO4 – 10 Marks)**

* Task creation
* Task stealing
* Dynamic load balancing

---

# **UNIT – V**

---

## **9(a) Try-lock with Code**

**(CO5 – 06 Marks)**

```c
if (pthread_mutex_trylock(&lock) == 0) {
    // critical section
    pthread_mutex_unlock(&lock);
}
```

Avoids blocking but risks starvation.

---

## **9(b) ABA Problem**

**(CO5 – 07 Marks)**

### **Problem**

CAS sees A → B → A as unchanged

### **Solution**

* Version tagging
* Hazard pointers

---

## **9(c) Data Organization Impact**

**(CO5 – 07 Marks)**

* Cache locality
* False sharing
* Cache line alignment

---

## **10(a) Memory Bottleneck in Multicore**

**(CO5 – 06 Marks)**

* Shared memory bus
* Cache coherence traffic
* NUMA latency

---

## **10(b) Thread-safe Functions**

**(CO5 – 06 Marks)**

Example:

* `strtok_r()` vs `strtok()`

---

## **10(c) Cache-related Issues**

**(CO5 – 08 Marks)**

* False sharing
* Cache ping-pong
* Coherence overhead

---

### **Final Note**

These answers are **exam-complete**, **conceptually deep**, and **aligned with standard multicore textbooks** (Hennessy, Intel Multicore Programming).

If you want:

* **Diagrams drawn**
* **Code-heavy versions**
* **10/15-mark expansions**
* **MCQ-focused summaries**

Tell me what you want next.
Below is a **structured, exam-ready, and detailed answer set**, written with **clear separation of concepts**, **architectural clarity**, and **illustrative examples**, aligned to **CO1–CO5 expectations**.
I will follow **unit-wise organization**, maintain **depth**, and avoid superficial explanations.

---

# UNIT – I

---

## 1(a) Relationship between Processors, Processes, and Threads

**CO1 (08)**

### 1. Processor (CPU)

A **processor** is a **hardware execution engine** that performs instruction execution. Modern processors may contain:

* Multiple **cores**
* Hardware support for **Simultaneous Multithreading (SMT / Hyper-Threading)**

A processor executes instructions issued by threads.

---

### 2. Process

A **process** is an **OS-level abstraction** representing a running program.

**Characteristics:**

* Has **independent virtual address space**
* Owns resources:

  * Code segment
  * Heap
  * File descriptors
  * I/O handles
* Provides **isolation** from other processes

Processes **do not execute directly**—they contain threads.

---

### 3. Thread

A **thread** is the **smallest schedulable unit of execution**.

**Characteristics:**

* Shares process resources (memory, files)
* Has its own:

  * Program Counter (PC)
  * Stack
  * Registers
  * Thread state

---

### Relationship Summary

| Entity    | Nature         | Owns Memory | Schedulable | Executes Instructions |
| --------- | -------------- | ----------- | ----------- | --------------------- |
| Processor | Hardware       | No          | No          | Yes                   |
| Process   | OS abstraction | Yes         | Indirectly  | No                    |
| Thread    | Execution unit | No (shared) | Yes         | Yes                   |

➡ **Threads run on processors, processes host threads**

---

## 1(b) Multithreading on Single-Core vs Multicore Platforms

**CO1 (06)**

### Single-Core Multithreading

* Only **one core**
* Threads execute via **time slicing**
* OS performs **context switching**
* No true parallelism

**Execution:**

```
T1 → T2 → T3 → T1 → ...
```

**Use Case:**

* Responsiveness (UI + background work)

---

### Multicore Multithreading

* Multiple cores
* Threads execute **simultaneously**
* True parallelism

**Execution:**

```
Core 1 → T1
Core 2 → T2
Core 3 → T3
```

---

### Comparison Table

| Aspect            | Single-Core | Multicore |
| ----------------- | ----------- | --------- |
| Parallelism       | No          | Yes       |
| Throughput        | Limited     | High      |
| Context Switching | Heavy       | Reduced   |
| Scalability       | Poor        | Excellent |

---

## 1(c) Process of Thread Creation

**CO1 (06)**

### Steps in Thread Creation

1. **Thread Request**

   * Application calls API:

     * POSIX: `pthread_create()`
     * Windows: `CreateThread()`

2. **Thread Control Block (TCB) Allocation**

   * OS allocates metadata:

     * Thread ID
     * State
     * Priority

3. **Stack Allocation**

   * Private stack created

4. **Register Context Initialization**

   * PC set to thread start function

5. **Scheduler Registration**

   * Thread enters **Ready Queue**

6. **Execution**

   * Scheduler dispatches thread to CPU

---

# UNIT – II

---

## 3(a) Major Forms of Decomposition

**CO2 (08)**

### 1. Data Decomposition

Data is partitioned across threads.

**Example:** Matrix multiplication

* Each thread computes a subset of rows

**Advantages:**

* High scalability
* Minimal synchronization

---

### 2. Task Decomposition

Different tasks executed in parallel.

**Example:** Web server

* Request parsing
* Authentication
* Response generation

---

## 3(b) Error Diffusion in Parallel Programming

**CO2 (06)**

### Error Diffusion Algorithm

Used in **image halftoning**.

**Key Property:**

* Output of pixel `i` affects pixels `i+1`, `i+2`

### Parallel Challenge

* **Data dependency**
* Cannot process pixels independently

### Justification for Parallel Analysis

* Requires:

  * Wavefront parallelism
  * Tiling strategies
  * Synchronization between rows

---

## 3(c) Key Considerations in Thread Design

**CO2 (06)**

1. **Granularity**

   * Too fine → overhead
   * Too coarse → underutilization

2. **Synchronization Cost**

   * Minimize locks

3. **Load Balancing**

   * Equal work distribution

4. **Memory Locality**

   * Avoid cache contention

---

# UNIT – III

---

## 5(a) Need for Synchronization

**CO3 (08)**

Synchronization ensures:

* **Correctness**
* **Data consistency**
* **Ordering guarantees**

### Without Synchronization

* Data races
* Corruption
* Non-deterministic output

---

### Synchronization Mechanisms

* Mutex
* Semaphore
* Atomic operations
* Barriers

---

## 5(b) Microsoft Threading APIs

**CO3 (06)**

### Features

* Native Windows kernel threads
* Fine-grained priority control
* Scalable thread pools

### APIs

* `CreateThread()`
* `WaitForSingleObject()`
* `InterlockedIncrement()`

### Advantages

* High performance
* Tight OS integration

---

## 5(c) Flow Control-Based Concepts

**CO3 (06)**

### Definition

Control thread execution based on:

* Resource availability
* Buffer capacity
* Event completion

### Examples

* Producer–Consumer
* Bounded buffers
* Thread pools

---

# UNIT – IV

---

## 7(a) Challenges in Threading a Loop

**CO4 (10)**

1. Loop-carried dependencies
2. Load imbalance
3. False sharing
4. Reduction variables
5. Synchronization overhead

**Example:** Prefix sum loop

---

## 7(b) Intel Task Queuing Execution Model

**CO4 (10)**

### Model Overview

* Tasks placed in a **work queue**
* Worker threads dynamically steal tasks

**Advantages:**

* Load balancing
* Irregular workloads
* Better cache usage

---

## 8(a) OpenMP Reductions

**CO4 (10)**

### Purpose

Combine private thread results into shared result.

```c
#pragma omp parallel for reduction(+:sum)
for(int i=0;i<n;i++)
    sum += a[i];
```

### Benefits

* Avoids locks
* High performance

---

## 8(b) OpenMP Data Copy Clauses

**CO4 (06)**

| Clause         | Purpose             |
| -------------- | ------------------- |
| `shared`       | Shared memory       |
| `private`      | Thread local        |
| `firstprivate` | Initialized private |
| `lastprivate`  | Copy back           |

---

## 8(c) Barrier vs Nowait

**CO4 (04)**

| Barrier       | Nowait               |
| ------------- | -------------------- |
| Forces sync   | Skips sync           |
| Ensures order | Improves performance |
| Safe          | Risky if misused     |

---

# UNIT – V

---

## 9(a) Data Races

**CO5 (10)**

### Definition

Two threads:

* Access same memory
* At least one writes
* No synchronization

### Consequences

* Incorrect results
* Hard-to-debug bugs

### Prevention

* Locks
* Atomic variables
* Immutability

---

## 9(b) Contended Locks & Priority Inversion

**CO5 (10)**

### Problems

* Starvation
* Performance collapse

### Solutions

* Lock striping
* Priority inheritance
* Lock-free algorithms

---

## 10(a) Thread-Safe Design Principles

**CO5 (10)**

1. Avoid global state
2. Use immutable objects
3. Minimize lock scope
4. Prefer atomic operations
5. Document synchronization contracts

---

## 10(b) Cache-Efficient Memory Strategies

**CO5 (10)**

1. Data alignment
2. Structure padding
3. NUMA-aware allocation
4. Avoid false sharing
5. Batch memory access

---

### Final Note

If you want, I can:

* Convert this into **handwritten-style exam notes**
* Add **diagrams**
* Provide **code-centric explanations**
* Reduce it to **7/10/15-mark answer formats**

Just tell me the format you need.
