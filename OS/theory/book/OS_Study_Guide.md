# Operating Systems Study Guide

## Table of Contents
1. [Inter-Process Communication (IPC)](#inter-process-communication-ipc)
2. [Threads](#threads)
3. [Producer-Consumer Problem](#producer-consumer-problem)
4. [Critical Sections](#critical-sections)
5. [Semaphores](#semaphores)

---

## Inter-Process Communication (IPC)

Inter-Process Communication is a mechanism that allows processes to communicate and synchronize their actions. IPC can be categorized based on persistence and implementation method.

### Types of IPC Mechanisms

#### 1. **Unnamed Pipes (Anonymous Pipes)**

**Purpose:** Temporary inter-process communication

**Characteristics:**
- Exist only during the execution of the processes
- Unidirectional (one-way communication)
- Parent-child process relationship typically required
- Created using the `pipe()` system call
- Disappear when processes terminate

**Key Operations:**
- `pipe()` - Creates a pipe
- `fork()` - Typically used with pipes for process creation

**Use Cases:**
- Connecting output of one process to input of another
- Shell command piping: `command1 | command2`

---

#### 2. **Named Pipes (FIFO - First In First Out)**

**Purpose:** Persistent inter-process communication via the filesystem

**Characteristics:**
- Exist in the filesystem as special files
- Persist even after processes terminate
- Persist until explicitly deleted
- Can be used by unrelated processes
- Bidirectional (can be opened for reading and writing)
- Created using `mkfifo()` command

**Key Features:**
- Named in the filesystem (like regular files)
- Can be manipulated like files (permissions, ownership)
- Accessible by multiple processes simultaneously

**Use Cases:**
- Client-server communication
- Process synchronization via filesystem
- Decoupling process lifetimes

---

#### 3. **Data Structure for Open Files**

**File Descriptor Table (Process-level):**
```
Process
├── File Descriptor Table (per process)
│   ├── FD 0 → stdin
│   ├── FD 1 → stdout
│   ├── FD 2 → stderr
│   ├── FD 3 → File pointer to open file/pipe
│   └── FD n → ...
└── File Table Entries (system-wide)
    └── Points to inode
```

**Components:**
- **File Descriptor (FD):** Integer index in per-process file descriptor table
- **File Table Entry:** System-wide table containing:
  - File offset (current position)
  - Access mode (read/write/both)
  - Reference count
  - Pointer to inode
- **Inode:** Filesystem metadata (permissions, size, ownership, etc.)

---

#### 4. **File Descriptor Duplication: `dup` and `dup2`**

**`dup(int oldfd)`**

Purpose: Creates a duplicate of a file descriptor

**Behavior:**
- Returns the lowest numbered available file descriptor
- New FD points to the same open file description as oldfd
- Shares file offset with original
- Both descriptors share reference count
- Useful for redirecting I/O

**Example:**
```c
int fd = open("file.txt", O_RDONLY);
int fd_copy = dup(fd);  // fd_copy points to same file as fd
```

**`dup2(int oldfd, int newfd)`**

Purpose: Duplicates oldfd to a specific file descriptor number

**Behavior:**
- If newfd is already open, it's closed first
- Returns newfd (now pointing to the same file as oldfd)
- If oldfd == newfd, function returns newfd unchanged
- Commonly used for I/O redirection

**Example:**
```c
int fd = open("output.txt", O_WRONLY | O_CREAT);
dup2(fd, STDOUT_FILENO);  // Redirect stdout to output.txt
```

---

#### 5. **Shared Memory**

**Purpose:** Direct memory sharing between processes (fastest IPC)

**Characteristics:**
- Multiple processes access the same memory region
- No data copying required (unlike pipes)
- Very fast communication
- Requires synchronization mechanisms (mutexes, semaphores)
- Survives process termination (unless explicitly removed)

**Key System Calls:**
- `shmget()` - Create/get shared memory segment
- `shmat()` - Attach shared memory to process address space
- `shmdt()` - Detach shared memory
- `shmctl()` - Control shared memory segments

**Advantages:**
- Minimal overhead
- Direct memory access
- High performance

**Disadvantages:**
- Requires careful synchronization
- Complex to debug
- Multiple processes modifying same data is risky

---

#### 6. **Sockets**

**Purpose:** Network-based and local IPC (can work across networks)

**Characteristics:**
- Endpoint of communication
- Can work locally (Unix domain sockets) or over networks (TCP/IP sockets)
- Bidirectional communication
- Connection-oriented (TCP) or connectionless (UDP)
- Identified by IP address, port number, and protocol

**Types:**
- **Unix Domain Sockets:** Local communication only, fast
- **Internet Sockets (TCP/IP):** Network communication

**Key Operations:**
- `socket()` - Create a socket
- `bind()` - Bind socket to address
- `listen()` - Listen for connections
- `accept()` - Accept incoming connection
- `connect()` - Connect to remote socket
- `send()/recv()` - Data transmission

---

#### 7. **Redirection (File-Process Connection)**

**Purpose:** Connecting processes to files (file descriptors to file table entries)

**Types:**
- **Input Redirection:** `command < input.txt`
- **Output Redirection:** `command > output.txt`
- **Append Redirection:** `command >> output.txt`
- **Error Redirection:** `command 2> error.txt`

**Implementation (in C):**
```c
// Redirect stdout to file
int fd = open("output.txt", O_WRONLY | O_CREAT, 0644);
dup2(fd, STDOUT_FILENO);  // File descriptor 1 (stdout) → output.txt
close(fd);
```

**Key Concept:** Redirection changes which file a file descriptor points to, without changing the program's code.

---

### IPC Comparison Summary

| Feature | Unnamed Pipes | Named Pipes | Shared Memory | Sockets |
|---------|---------------|------------|---------------|---------|
| **Persistence** | Temporary | Persistent | Persistent | Depends |
| **Directionality** | Unidirectional | Bidirectional | N/A | Bidirectional |
| **Related Processes** | Required | Not required | Not required | Not required |
| **Speed** | Medium | Medium | Very Fast | Slow (Network) |
| **Synchronization** | Built-in (blocking) | Built-in (blocking) | Manual | Manual |
| **Network Support** | No | No | No | Yes |

---

## Threads

A thread is the **smallest unit of execution** within a process. Unlike separate processes, threads within the same process share resources.

### Thread vs Process

**Process:**
- Independent execution context
- Separate memory space
- Own file descriptors
- Own page tables
- Expensive to create and context switch

**Thread:**
- Shares memory with other threads in same process
- Shares file descriptors
- Shares heap and data segments
- Each has own stack and program counter (PC)
- Lightweight, cheaper to create

### Shared Thread Resources (within same process)

- **Code segment** (program instructions)
- **Data segment** (global/static variables)
- **Heap** (dynamically allocated memory)
- **File descriptors** (open files)
- **Signal handlers**

### Per-Thread Resources

- **Stack** (local variables, function calls)
- **Program Counter (PC)** (current instruction)
- **Registers** (CPU state)
- **Thread-local storage** (TLS)

---

### Thread Attributes

Thread attributes define the characteristics and behavior of a thread:

1. **Detach State**
   - Joinable (default): Parent thread can wait for thread completion
   - Detached: Thread resources freed automatically upon termination

2. **Scheduling Policy**
   - SCHED_FIFO (First In First Out)
   - SCHED_RR (Round Robin)
   - SCHED_OTHER (Default)

3. **Scheduling Priority**
   - Integer value (higher = higher priority)
   - Range depends on scheduling policy

4. **Stack Size**
   - Minimum size for thread stack
   - Default typically 1-8 MB

5. **Scope**
   - Process scope: Threads compete within process
   - System scope: Threads compete system-wide

6. **Affinity**
   - CPU core binding
   - Bind thread to specific processor

---

### Thread Lifecycle

Threads go through several states during their lifetime:

```
┌─────────────────────────────────────────────────────┐
│              Thread Lifecycle States                 │
└─────────────────────────────────────────────────────┘

    ┌──────────┐
    │   NEW    │  (Thread created but not started)
    └────┬─────┘
         │
         │ start()
         ▼
    ┌──────────┐
    │  READY   │  (Ready to run, waiting for CPU)
    └────┬─────┘
         │
         │ CPU assigned
         ▼
    ┌──────────┐
    │ RUNNING  │  (Executing on CPU)
    └────┬─────┘
         │
         ├─────────────────────────────┐
         │                             │
         │ I/O wait/lock wait          │ Preemption/
         │ Sleep/Yield                 │ Time slice
         ▼                             ▼
    ┌──────────┐                  ┌──────────┐
    │ WAITING  │◄────────────────►│  READY   │
    └────┬─────┘                  └────┬─────┘
         │ Resource available          │
         │ Timeout expires             │ CPU assigned
         └──────────────┬──────────────┘
                        │
                        │ Return to READY
                        │ (unless CPU assigned directly)
                        ▼
                    ┌──────────────┐
                    │ TERMINATED   │  (Thread finished)
                    └──────────────┘
```

**State Descriptions:**

- **NEW:** Thread object created, but thread not yet started
- **READY:** Thread ready to execute, waiting for scheduler to allocate CPU
- **RUNNING:** Currently executing on a CPU core
- **WAITING:** Thread blocked (I/O operation, lock, sleep)
- **TERMINATED:** Thread execution completed

---

### Priority Donation (Priority Inheritance)

**Problem:** Priority Inversion

When a high-priority thread waits for a resource held by a low-priority thread, the system behaves unexpectedly.

**Example:**
```
High Priority Thread (HP):   Waiting for Lock
↓
Medium Priority Thread (MP): Running (preempts LP)
↓
Low Priority Thread (LP):    Holding Lock
```

**Solution:** Priority Donation/Inheritance

When LP holds a lock needed by HP:
1. LP's priority is temporarily raised to HP's priority
2. LP can complete its critical section quickly
3. LP releases lock
4. LP's priority returns to normal
5. HP can now acquire lock

**Implementation:**
- When thread waits on lock, donate its priority to lock holder
- Restore original priority when lock is released
- Prevents priority inversion situations

---

### User-Level Threads vs System-Level Threads

#### User-Level Threads

**Implementation:**
- Managed by user-level thread library (not OS kernel)
- OS kernel unaware of threads
- Scheduling done by library runtime

**Advantages:**
- Fast thread creation and context switching
- Can implement custom scheduling
- Portable across OS platforms
- No kernel involvement overhead

**Disadvantages:**
- Cannot utilize multiple processors (single kernel thread per process)
- If one thread blocks, entire process blocks
- OS sees only single process, not individual threads
- Blocked I/O blocks all threads

**Example:** GNU Portable Threads, Python greenlets

```
┌─────────────────────────────────┐
│         Process                 │
├─────────────────────────────────┤
│  ┌────────────────────────────┐ │
│  │   User-Level Thread Lib    │ │
│  │  ┌─────┐ ┌─────┐ ┌─────┐  │ │
│  │  │Thread│ │Thread│ │Thread│ │ │
│  │  └─────┘ └─────┘ └─────┘  │ │
│  └────────────────────────────┘ │
└─────────────────────────────────┘
         │
         │ Single Kernel Thread
         ▼
    ┌─────────────┐
    │  OS Kernel  │
    └─────────────┘
```

#### System-Level Threads

**Implementation:**
- Managed directly by OS kernel
- OS kernel creates and schedules threads
- Each thread gets own kernel scheduling structure

**Advantages:**
- Can run on multiple processors simultaneously
- One thread's I/O blocking doesn't block others
- Fine-grained scheduling control
- OS provides proper resource management

**Disadvantages:**
- Slower thread creation (kernel involvement)
- Expensive context switching
- Kernel overhead for each thread
- Limited by system resources

**Example:** POSIX pthreads, Java threads, Windows threads

```
┌─────────────────────────────────┐
│         Process                 │
├─────────────────────────────────┤
│  ┌─────┐  ┌─────┐  ┌─────┐     │
│  │Thread│ │Thread│ │Thread│    │
│  └─────┘  └─────┘  └─────┘     │
└─────────────────────────────────┘
     │         │         │
     │ Kernel threads (separate)
     ▼         ▼         ▼
┌─────────────────────────────┐
│       OS Kernel             │
│  ┌─────┐ ┌─────┐ ┌─────┐   │
│  │ KT1 │ │ KT2 │ │ KT3 │   │ (Each can run on different CPUs)
│  └─────┘ └─────┘ └─────┘   │
└─────────────────────────────┘
```

---

### Thread Synchronization

#### 1. **Race Condition**

**Definition:** A situation where two or more threads access shared data, and the final result depends on the timing/ordering of thread execution.

**Example:**
```c
// Shared variable
int counter = 0;

// Thread 1:
counter++;  // Read 0 → Write 1

// Thread 2:
counter++;  // Read 0 → Write 1

// Expected: counter = 2
// Actual: counter = 1 (both read 0 before either writes)
```

**Problem:** The operation `counter++` is NOT atomic. It's actually:
1. Read current value from memory into register
2. Add 1 to register
3. Write new value back to memory

If threads interleave, both might read 0 before either writes back 1.

---

#### 2. **Critical Section**

**Definition:** A code segment where shared data is accessed. Only one thread can execute a critical section at a time.

**Properties (Mutex Requirements):**

1. **Mutual Exclusion:** Only one thread in critical section at a time
2. **Progress:** If no thread in critical section, waiting thread should enter
3. **Bounded Waiting:** Thread shouldn't wait indefinitely
4. **No assumption on thread speeds:** One slow thread shouldn't block others

**General Structure:**
```c
do {
    // Entry section (acquire lock/semaphore)
    ENTRY_SECTION();
    
    // Critical Section
    // ... access shared data ...
    
    // Exit section (release lock/semaphore)
    EXIT_SECTION();
    
    // Remainder section
    // ... do other work ...
} while (TRUE);
```

---

#### 3. **Synchronization Primitives**

##### **Mutex (Mutual Exclusion Lock)**

A binary lock that is either locked or unlocked.

**Operations:**
- `lock()` - Acquire mutex (block if unavailable)
- `unlock()` - Release mutex
- Atomic operations guaranteed by kernel/hardware

**Characteristics:**
- Simplest synchronization primitive
- Binary state: locked/unlocked
- Ownership concept (the thread that locked must unlock)

**Implementation Patterns:**
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Critical section
pthread_mutex_lock(&mutex);
// ... shared data access ...
pthread_mutex_unlock(&mutex);
```

---

##### **Semaphores**

A counter-based synchronization primitive (discussed in detail below)

**Types:**
- **Binary Semaphore:** Counter = {0, 1} (similar to mutex)
- **Counting Semaphore:** Counter = {0, 1, 2, ...}

**Operations:**
- `wait()` - Decrement counter (block if counter < 0)
- `signal()` - Increment counter (wakeup one waiting thread)

---

#### 4. **Priority Inversion (and Its Solution)**

**Problem:** Priority Inversion

A high-priority thread waits for a resource held by low-priority thread.

**Example Scenario:**

```
Time    HP (High Priority)      MP (Medium)    LP (Low Priority)
─────────────────────────────────────────────────────────────
  0     Waiting for Lock                       Running, Has Lock
  
  1     Waiting (blocked)       Preempts LP    Preempted
  
  2     Waiting (blocked)       Running         Blocked (can't continue)
  
  3     Waiting (blocked)       Running         Blocked
  
  4     Waiting (blocked)       Finishes       Resumes
  
  5     Waiting for Lock        (in queue)     Running, Has Lock
  
  6     Waiting (now acquired)  Running        Released Lock
  
  7     Now running!             Running       Waiting
```

**Problem:** HP (highest priority) is delayed by MP (medium priority), which is unfair to priority system.

**Solution:** Priority Donation/Inheritance

1. When LP holds lock and HP waits for it, raise LP's priority to HP's level
2. LP executes at HP's priority, completes critical section quickly
3. LP releases lock, priority returns to original level
4. HP acquires lock and runs

**Result:** No medium-priority thread can preempt LP while it holds the lock needed by HP.

---

## Producer-Consumer Problem

The **Producer-Consumer problem** (also called **Bounded Buffer Problem**) is a classic synchronization problem demonstrating concurrent data structures and mutual exclusion.

### Problem Setup

**Scenario:**
- Producer: Generates items and puts them in a buffer
- Consumer: Takes items from buffer
- Buffer: Bounded size (limited capacity)

**Buffer Structure:**
```c
#define BUFFER_SIZE 10

typedef struct {
    DATA data;
} item;

item buffer[BUFFER_SIZE];      // Circular buffer
int in = 0;                     // Next insertion position
int out = 0;                    // Next removal position
int counter = 0;                // Number of items in buffer
```

### Visual Representation

```
Buffer State (BUFFER_SIZE = 10):

Empty Buffer:
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│  │  │  │  │  │  │  │  │  │  │
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
in=0, out=0, counter=0

Partially Full:
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│X │X │X │  │  │  │Y │Y │  │  │
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
in=3, out=6, counter=5

Full Buffer:
┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
│X │X │X │X │X │X │X │X │X │X │
└──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
in=0, out=0, counter=10 (after wraparound)
```

### Producer Code (Naive - Problematic)

```c
item nextProduced;

while (TRUE) {
    // Produce item
    
    // Wait while buffer full
    while (counter == BUFFER_SIZE)
        ;  // Busy wait (spin lock)
    
    // Add to buffer
    buffer[in] = nextProduced;
    in = (in + 1) % BUFFER_SIZE;
    counter++;
}
```

### Consumer Code (Naive - Problematic)

```c
item nextConsumed;

while (TRUE) {
    // Wait while buffer empty
    while (counter == 0)
        ;  // Busy wait (spin lock)
    
    // Remove from buffer
    nextConsumed = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    counter--;
}
```

---

### The Critical Problem: `counter++` and `counter--`

**WARNING:** The operations `counter++` and `counter--` are **NOT atomic!**

#### What `counter++` Really Is:

```
counter++;
```

is actually executed as:

```assembly
register = counter        ; Load counter into register
register = register + 1   ; Increment register
counter = register        ; Store back to counter
```

#### Race Condition Example

**Initial State:** `counter = 5`

**Scenario:** Producer increments (+1) and Consumer decrements (-1) simultaneously

```
Time    Producer                    Consumer                  counter
────────────────────────────────────────────────────────────────────
  0                                                           5
  1     Load: reg_p = 5                                       5
  2                                   Load: reg_c = 5         5
  3     Inc:  reg_p = 6                                       5
  4                                   Dec:  reg_c = 4         5
  5     Store: counter = 6                                    6
  6                                   Store: counter = 4      4
  7                                                           4
```

**Result:** 
- Started with counter = 5
- One increment (+1) and one decrement (-1) should leave counter = 5
- **Actual result: counter = 4** (WRONG!)

**Why?** Both threads read the old value (5) before either wrote back, so one update was lost.

---

### Synchronization Issues

**Problems with naive implementation:**

1. **Race Condition:** `counter++` and `counter--` not atomic
2. **Lost Updates:** Updates can be lost due to interleaving
3. **Busy Waiting:** Inefficient CPU usage (spinning on condition)
4. **No Blocking:** Threads waste CPU cycles checking conditions

---

### Correct Solution using Semaphores

```c
#define BUFFER_SIZE 10

typedef struct {
    DATA data;
} item;

item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Semaphores for synchronization
semaphore mutex = 1;           // Binary semaphore (mutual exclusion)
semaphore full = 0;            // Counts full slots
semaphore empty = BUFFER_SIZE; // Counts empty slots

// PRODUCER
item nextProduced;

while (TRUE) {
    // Produce item
    
    wait(empty);      // Wait until empty slot available
    wait(mutex);      // Enter critical section
    
    buffer[in] = nextProduced;
    in = (in + 1) % BUFFER_SIZE;
    
    signal(mutex);    // Exit critical section
    signal(full);     // Signal that slot is full
}

// CONSUMER
item nextConsumed;

while (TRUE) {
    wait(full);       // Wait until item available
    wait(mutex);      // Enter critical section
    
    nextConsumed = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    
    signal(mutex);    // Exit critical section
    signal(empty);    // Signal that slot is empty
}
```

**How it works:**
- `empty` semaphore: Tracks available slots (initially BUFFER_SIZE)
- `full` semaphore: Tracks filled items (initially 0)
- `mutex`: Ensures only one thread modifies buffer/counters
- Producer waits on `empty`, signals `full`
- Consumer waits on `full`, signals `empty`
- No race condition, no busy waiting

---

## Critical Sections

A critical section is a code segment where shared resources are accessed. The goal is to allow only one thread to execute in a critical section at any time.

### Requirements for Critical Section (Mutual Exclusion)

1. **Mutual Exclusion:** At most one process in critical section at any time
2. **Progress:** Processes not in remainder section shouldn't block others from entering
3. **Bounded Waiting:** Process shouldn't wait indefinitely
4. **Bounded Resources:** No assumption about relative speeds of processes

### General Template

```c
do {
    ENTRY_SECTION();       // Acquire lock/permission
    
    /* CRITICAL SECTION */
    // Access shared data here
    
    EXIT_SECTION();        // Release lock
    
    /* REMAINDER SECTION */
    // Do other work
} while (TRUE);
```

---

### Solution 1: Toggle Access (Turn-Based)

**Concept:** Only one process allowed in at a time, must alternate.

```c
// Shared variable
int turn = 0;  // Initially, process 0's turn

// Process i (where j is the other process):
do {
    // Entry section - wait for our turn
    while (turn != i)
        ;  // Busy wait
    
    /* CRITICAL SECTION */
    
    // Exit section - give turn to other process
    turn = j;
    
    /* REMAINDER SECTION */
} while (TRUE);
```

**Advantages:**
- Simple to understand
- Guarantees mutual exclusion

**Disadvantages:**
- **Strict alternation required:** If process 0 finishes early, it must wait for process 1 even if 1 is in remainder section
- **Progress violation:** If process 1 is slow or blocked, process 0 cannot enter
- **Violates bounded waiting indirectly:** Process forced to wait even if other doesn't need section

---

### Solution 2: Flag for Each Process (Intent Flags)

**Concept:** Each process signals intent to enter critical section.

```c
// Shared variables
boolean flag[2];  // flag[i] = TRUE means process i wants to enter CS

// Initially: flag[0] = FALSE, flag[1] = FALSE

// Process i:
do {
    // Entry section
    flag[i] = TRUE;
    
    // Wait while other process wants entry
    while (flag[j])
        ;  // Busy wait
    
    /* CRITICAL SECTION */
    
    // Exit section
    flag[i] = FALSE;
    
    /* REMAINDER SECTION */
} while (TRUE);
```

**Example (Race Condition):**

```
Time    Process 0                   Process 1               flag[0] flag[1]
──────────────────────────────────────────────────────────────────────────
  0     flag[0] = TRUE                                      T       F
  1                                  flag[1] = TRUE         T       T
  2     Check flag[1] (TRUE)                                T       T
  3                                  Check flag[0] (TRUE)   T       T
  4     Busy wait                                           T       T
  5     Busy wait                   Busy wait               T       T
  6     ...                          ...                    T       T
  ∞     DEADLOCK!                                           T       T
```

**Problems:**
- **Deadlock:** Both set flag to TRUE, both see other's flag, both wait forever
- **Not safe for mutual exclusion** in this form

---

### Solution 3: Peterson's Algorithm (Flag + Turn)

**Concept:** Combine intent flags with turn-taking for safety.

```c
// Shared variables
boolean flag[2];
int turn;

// Initially: flag[0] = FALSE, flag[1] = FALSE, turn = any value

// Process i (j is the other process):
do {
    // Entry section
    flag[i] = TRUE;        // Express intention
    turn = j;              // Allow other process priority
    
    // Wait while other wants entry AND it's their turn
    while (flag[j] && turn == j)
        ;  // Busy wait
    
    /* CRITICAL SECTION */
    
    // Exit section
    flag[i] = FALSE;
    
    /* REMAINDER SECTION */
} while (TRUE);
```

**How it prevents problems:**

1. **Process sets flag[i] = TRUE:** Signals intent to enter
2. **Process sets turn = j:** Gives priority to other process
3. **Check condition:** Only enters if:
   - Other process doesn't want entry (flag[j] == FALSE), OR
   - It's our turn (turn == i)

**Trace (No Deadlock):**

```
Time    Process 0                 Process 1              flag  turn
──────────────────────────────────────────────────────────────────
  0     flag[0] = TRUE                                   [T,F]  0
  1     turn = 1                                         [T,F]  1
  2     Check: flag[1]=F && turn=1?                      [T,F]  1
  3     YES, both false→ ENTER CS                        [T,F]  1
  4                              flag[1] = TRUE          [T,T]  1
  5                              turn = 0                [T,T]  0
  6                              Check: flag[0]=T && turn=0?
  7                              YES, both true→ WAIT    [T,T]  0
  8     Execute CS                                       [T,T]  0
  9     ...                                              [T,T]  0
 10     flag[0] = FALSE                                  [F,T]  0
 11     ...                                              [F,T]  0
 12                              Waiting (flag[0] now F)
 13                              Check: flag[0]=F?→YES   [F,T]  0
 14                              ENTER CS                [F,T]  0
```

**Properties (Peterson's Algorithm):**
- ✓ **Mutual Exclusion:** Only one process in CS at a time
- ✓ **Progress:** Process not in remainder won't block others
- ✓ **Bounded Waiting:** Wait is bounded (at most one other process enters before this one)

---

### Solution 4: Hardware Solution

**Problem:** Software solutions may fail on multiprocessor systems with caches.

#### Why Software Solutions Fail on Cached Multiprocessors

**Scenario:**

```
Initially:  location_a = A0, location_b = B0

Processor 1:
  a) Writes A1 to location_a
  b) Sets location_b = B1 (indicating data is valid)

Processor 2:
  c) Waits in loop until location_b == B1
  d) Reads value from location_a
```

**Problem with Caches:**

```
Processor 1 Cache              Processor 2 Cache       Main Memory
───────────────────────────────────────────────────────────────
a=A0                           a=A0                    a=A0, b=B0
b=B0                           b=B0

Step 1: P1 writes A1 to a
a=A1 (P1 cache only!)          a=A0                    a=A0 (not written yet)
                               b=B0                    b=B0

Step 2: P1 sets b=B1
a=A1                           a=A0                    a=A0
b=B1 (P1 cache)                b=B0                    b=B0

Step 3: P2 reads b (waits for B1)
P1's writes haven't propagated!
a=A1                           a=A0                    a=A0
b=B1                           b=B0 (still old!)       b=B0

P2 still sees b=B0, keeps waiting!
```

**Issues:**
1. **Write-Through vs Write-Back:** Cache writes might not be immediately visible
2. **Sequential Consistency Violation:** Write ordering not preserved
3. **Cache Coherency:** Different processors see different values

#### Hardware Requirements for Critical Sections

**1. Write-Through Cache:**
- Every write immediately visible to all processors
- On write, invalidate in other caches or update immediately
- Ensures first write visible before second

**2. Sequential Consistency:**
- If P1 writes to location A before P2 writes to location B, then:
  - ALL processors see A's write before B's write
  - Requires either:
    - Write barriers (flush writes)
    - Shared memory (no private caches)
    - Cache coherency protocol

**3. Atomic Operations:**
- Hardware instructions that cannot be interrupted:
  - Test-and-set (TAS)
  - Compare-and-swap (CAS)
  - Fetch-and-add

#### Hardware Solution Implementations

**Atomic Test-and-Set (TAS):**

```c
// Hardware atomic operation
bool TestAndSet(bool *lock) {
    bool original = *lock;
    *lock = TRUE;
    return original;  // Atomically
}

// Using TAS for mutual exclusion
do {
    // Entry section
    while (TestAndSet(&lock))
        ;  // Spin until lock acquired
    
    /* CRITICAL SECTION */
    
    // Exit section
    lock = FALSE;
    
    /* REMAINDER SECTION */
} while (TRUE);
```

**Atomic Compare-and-Swap (CAS):**

```c
// Hardware atomic operation
bool CompareAndSwap(int *value, int expected, int new) {
    if (*value == expected) {
        *value = new;
        return TRUE;
    }
    return FALSE;
}

// Using CAS for mutual exclusion
do {
    // Entry section
    while (!CompareAndSwap(&lock, 0, 1))
        ;  // Spin until lock acquired (0→1)
    
    /* CRITICAL SECTION */
    
    // Exit section
    lock = 0;
    
    /* REMAINDER SECTION */
} while (TRUE);
```

---

## Semaphores

A **semaphore** is a synchronization primitive consisting of a counter and an associated wait queue. It's designed to manage access to a shared resource and control synchronization.

### Semaphore Concept

A semaphore is a protected counter that supports only two atomic operations:
- **wait():** Decrement counter; block if negative
- **signal():** Increment counter; wake up waiting process

**Key Principle:** Semaphores prevent busy waiting by blocking processes instead of spinning.

---

### Semaphore Data Structure

```c
typedef struct {
    int value;                  // Counter (can be negative)
    struct process *list;       // Queue of processes waiting on semaphore
} SEMAPHORE;
```

**Components:**
- `value`: Integer counter
  - Positive: Number of available resources
  - Zero: No available resources (but none waiting)
  - Negative: Number of processes waiting
- `list`: Linked list of processes blocked on this semaphore

---

### Wait and Signal Operations

#### wait() Operation

```c
SEMAPHORE s;

wait(s) {
    s.value = s.value - 1;
    if (s.value < 0) {
        add this process to s.list;
        block();  // Suspend this process
    }
}
```

**Atomic Execution:**
```
wait(semaphore s):
  1. Decrement s.value
  2. IF s.value < 0:
       a. Add current process to s.list
       b. Block current process (yield CPU)
     ELSE:
       c. Continue execution
```

**Behavior:**
- If resources available (value ≥ 0): Decrement and continue
- If no resources (value < 0): Block and add to queue

**Example Trace:**
```
Initial: s.value = 2, s.list = []

Thread A: wait(s)
  s.value = 1, continue

Thread B: wait(s)
  s.value = 0, continue

Thread C: wait(s)
  s.value = -1, block! (added to s.list)

Thread D: wait(s)
  s.value = -2, block! (added to s.list)

State after: s.value = -2, s.list = [C, D]
```

#### signal() Operation

```c
SEMAPHORE s;

signal(s) {
    s.value = s.value + 1;
    if (s.value <= 0) {
        remove a process P from s.list;
        wakeup(P);  // Resume this process
    }
}
```

**Atomic Execution:**
```
signal(semaphore s):
  1. Increment s.value
  2. IF s.value <= 0:
       a. Remove one process P from s.list
       b. Wakeup process P (add to ready queue)
     ELSE:
       c. No blocked processes, just return
```

**Why `<= 0` and not `< 0`?**
- When value becomes 0: Resources just exhausted, no wait yet
- When value < 0: |value| processes are waiting
- Signal checks: if value ≤ 0, there might be waiting processes

**Example Trace (continuing above):**
```
State: s.value = -2, s.list = [C, D]

Thread A: signal(s)
  s.value = -1, value ≤ 0!
  Remove C from s.list, wakeup(C)
  State: s.value = -1, s.list = [D]

State: s.value = -1, s.list = [D]

Thread B: signal(s)
  s.value = 0, value ≤ 0!
  Remove D from s.list, wakeup(D)
  State: s.value = 0, s.list = []

State: s.value = 0, s.list = []

Thread E: signal(s)
  s.value = 1, value > 0
  No one waiting, just return
  State: s.value = 1, s.list = []
```

---

### Semaphore Types

#### 1. Binary Semaphore

**Range:** {0, 1}
**Usage:** Mutual exclusion (like a mutex)

```c
semaphore mutex = 1;

// Critical section
wait(mutex);
/* CRITICAL SECTION */
signal(mutex);
```

**Behavior:**
- Initially 1 (unlocked)
- wait() decrements to 0 (locked)
- Others trying to wait() get value = -1 and block
- signal() increments back to 1, wakes one waiting process

#### 2. Counting Semaphore

**Range:** {0, 1, 2, ...}
**Usage:** Manage multiple identical resources

```c
semaphore available = N;  // N identical resources

wait(available);           // Acquire resource
/* Use resource */
signal(available);         // Release resource
```

**Example:** Limiting concurrent database connections

```c
#define MAX_CONNECTIONS 5
semaphore db_conn = MAX_CONNECTIONS;

// In each thread:
wait(db_conn);             // Get a connection (or wait)
// ... use connection ...
signal(db_conn);           // Return connection
```

---

### Semaphore Usage Patterns

#### Pattern 1: Mutual Exclusion

```c
semaphore mutex = 1;

wait(mutex);
/* CRITICAL SECTION - shared data access */
signal(mutex);
```

#### Pattern 2: Synchronization (Producer-Consumer)

```c
semaphore full = 0;    // Count of full slots
semaphore empty = N;   // Count of empty slots
semaphore mutex = 1;   // Mutual exclusion

// Producer
wait(empty);    // Wait for empty slot
wait(mutex);    // Enter critical section
/* add item */
signal(mutex);  // Exit critical section
signal(full);   // Signal item available

// Consumer
wait(full);     // Wait for item
wait(mutex);    // Enter critical section
/* remove item */
signal(mutex);  // Exit critical section
signal(empty);  // Signal empty slot available
```

#### Pattern 3: Precedence (Synchronization Points)

Ensure process P1 executes before P2:

```c
semaphore synch = 0;  // Initially blocked

// P1
statement1;
signal(synch);        // P1 done, wake P2

// P2
wait(synch);          // Wait for P1 to signal
statement2;
```

**Execution Guarantee:**
- Even if P2 starts first, it blocks at wait(synch)
- P1 executes statement1, then signals
- P2 continues after signal

---

### Comparison: Busy Wait vs Blocking Semaphores

#### Busy Wait (Spinning) - BAD

```c
int lock = 0;

while (lock == 0)
    ;  // Spin, waste CPU cycles!
lock = 1;

/* CRITICAL SECTION */

lock = 0;
```

**Problems:**
- Wastes CPU resources
- High power consumption
- Lower priority thread may never run (starvation)
- Inefficient on single-CPU systems

#### Blocking Semaphore - GOOD

```c
semaphore s = 1;

wait(s);   // Block if unavailable (yield CPU)

/* CRITICAL SECTION */

signal(s); // Wake one waiting process
```

**Advantages:**
- CPU not wasted
- Process yields CPU to others
- Fair scheduling
- Efficient resource usage

---

### Semaphore vs Mutex

| Feature | Semaphore | Mutex |
|---------|-----------|-------|
| **Type** | Counter-based | Binary lock |
| **Ownership** | No ownership | Thread ownership |
| **Can signal multiple times?** | Yes | No (must be locked first) |
| **Can be released by any thread?** | Yes | Typically no (error if not owner) |
| **Flexibility** | More flexible | More restricted |
| **Error detection** | Less robust | More robust |
| **Typical use** | Synchronization, resource counting | Mutual exclusion |

---

### Semaphore Potential Issues

#### 1. Deadlock

```c
semaphore s1 = 1, s2 = 1;

// Thread 1                // Thread 2
wait(s1);                  wait(s2);
wait(s2);                  wait(s1);
/* work */                 /* work */
signal(s2);                signal(s1);
signal(s1);                signal(s2);
```

If both threads execute first wait() line:
- Thread 1: Holds s1, waits for s2
- Thread 2: Holds s2, waits for s1
- **DEADLOCK!**

**Solution:** Always acquire semaphores in same order across all threads.

#### 2. Starvation

Low-priority processes never acquire semaphore if higher-priority processes keep signaling.

**Solution:** FIFO queue instead of arbitrary selection.

#### 3. Incorrect Usage

```c
wait(s);
wait(s);  // Deadlock with binary semaphore!
signal(s);
signal(s);
```

Forgot to signal the same number of times as wait.

---

### Complete Example: Dining Philosophers

```c
#define N 5

semaphore chopstick[N];  // Each initially 1
semaphore mutex = 1;    // Protect state variable

for (int i = 0; i < N; i++)
    chopstick[i] = 1;

philosopher(int i) {
    while (TRUE) {
        // Thinking
        think();
        
        // Hungry - try to get chopsticks
        wait(mutex);                           // Enter critical section
        state[i] = HUNGRY;
        test(i);                               // Try to acquire chopsticks
        signal(mutex);                         // Exit critical section
        
        wait(self[i]);                         // Wait if couldn't get chopsticks
        
        // Eating
        eat();
        
        // Finished - release chopsticks
        wait(mutex);                           // Enter critical section
        state[i] = THINKING;
        test((i + 1) % N);                     // Check right neighbor
        test((i - 1 + N) % N);                 // Check left neighbor
        signal(mutex);                         // Exit critical section
    }
}

test(int i) {
    if (state[i] == HUNGRY && 
        state[(i + 1) % N] != EATING && 
        state[(i - 1 + N) % N] != EATING) {
        state[i] = EATING;
        signal(self[i]);                       // Wake philosopher i
    }
}
```

---

## Summary Table

### Synchronization Primitives

| Primitive | Purpose | Atomic | Complexity | Use Case |
|-----------|---------|--------|-----------|----------|
| **Mutex** | Mutual exclusion | Yes | Simple | Critical sections |
| **Binary Semaphore** | Mutual exclusion | Yes | Simple | Binary control |
| **Counting Semaphore** | Resource counting | Yes | Medium | Resource pools |
| **Condition Variable** | Wait for condition | With lock | Medium | Wait/notify pattern |
| **Monitor** | High-level sync | Language-level | Complex | Object-oriented |

### IPC Methods Comparison

| Method | Speed | Persistence | Network | Sync |
|--------|-------|-------------|---------|------|
| **Unnamed Pipe** | Medium | No | No | Built-in |
| **Named Pipe** | Medium | Yes | No | Built-in |
| **Shared Memory** | Very Fast | Yes | No | Manual |
| **Socket** | Slow | Depends | Yes | Manual |

