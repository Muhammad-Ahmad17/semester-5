# Operating Systems Exam - Solutions

## Question 2 [5 Marks]
**Given the following data:**
- Number of processes (n) = 10
- CPU utilization = 60% (0.6)

**Calculate the I/O Utilization assuming that the CPU and I/O operations are independent.**

### Solution:

**Formula:**
When CPU and I/O operations are independent, we use the following relationship:

$$\text{CPU Utilization} = 1 - (\text{I/O Utilization})^n$$

Where:
- CPU Utilization = 0.6
- n = 10 (number of processes)
- I/O Utilization = p (what we need to find)

**Derivation:**
- If each process waits for I/O with probability p, then it's using CPU with probability (1-p)
- All n processes wait for I/O simultaneously with probability $p^n$
- CPU is being utilized when at least one process is NOT waiting for I/O
- Therefore: $\text{CPU Utilization} = 1 - p^n$

**Calculation:**

$$0.6 = 1 - p^{10}$$

$$p^{10} = 1 - 0.6 = 0.4$$

$$p = 0.4^{1/10}$$

$$p = 0.4^{0.1}$$

$$p \approx 0.8926$$

**Or expressed as percentage:**
$$p \approx 89.26\%$$

**Answer: I/O Utilization ≈ 0.8926 or 89.26%**

---

## Question 3 [5 x 2 = 10 Marks]

**Problem:** The following C code demonstrates a multi-threaded program with a shared resource. Identify the problem in this code related to thread synchronization. Modify the code by using a mutex to ensure proper synchronization.

### Original Code (Problem Identified):

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 1000; i++) {
        counter++;  // RACE CONDITION - CRITICAL SECTION
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Counter: %d\n", counter);
    return 0;
}
```

### Identified Issue:

**Race Condition on `counter++`**

The operation `counter++` is NOT atomic. It consists of three steps:

1. **Load:** Read counter value from memory into CPU register
2. **Increment:** Add 1 to the register
3. **Store:** Write the new value back to memory

**Problematic Execution Scenario:**

```
Time    Thread 1                    Thread 2              counter
─────────────────────────────────────────────────────────────────
  0     Load: reg1 = 5                                   5
  1                                 Load: reg2 = 5       5
  2     Inc: reg1 = 6                                    5
  3                                 Inc: reg2 = 6        5
  4     Store: counter = 6                               6
  5                                 Store: counter = 6   6
  6                                                      6
```

**Expected:** counter = 7 (after two increments from 5)
**Actual:** counter = 6 (one increment lost!)

### Why It's a Problem:

- Both threads read the same value (5)
- Both increment to 6
- Both write 6 back
- Expected result: 2000 (after 1000 + 1000 increments from 0)
- Actual result: Random value between 1000-2000 (depending on timing)

---

### Solution: Using Mutex

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void* arg) {
    for (int i = 0; i < 1000; i++) {
        pthread_mutex_lock(&mutex);      // ENTRY SECTION
        
        counter++;                        // CRITICAL SECTION (protected)
        
        pthread_mutex_unlock(&mutex);    // EXIT SECTION
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Counter: %d\n", counter);
    return 0;
}
```

### How the Solution Works:

**Mutex (Mutual Exclusion Lock):**

1. **pthread_mutex_lock(&mutex):**
   - If mutex is unlocked, lock it and continue
   - If mutex is locked, thread blocks until it's available
   - Only one thread can hold the lock at any time

2. **Critical Section (counter++):**
   - Now only one thread executes this at a time
   - No race condition possible

3. **pthread_mutex_unlock(&mutex):**
   - Release the lock
   - Allows other waiting threads to acquire it

**Correct Execution Trace:**

```
Time    Thread 1                          Thread 2                counter
─────────────────────────────────────────────────────────────────────────
  0     Lock acquired                                              0
  1     Load: reg1 = 0
  2     Inc: reg1 = 1
  3     Store: counter = 1                                        1
  4     Unlock
  5                                       Lock acquired           1
  6                                       Load: reg2 = 1
  7                                       Inc: reg2 = 2
  8                                       Store: counter = 2      2
  9                                       Unlock
 10     Lock acquired                                             2
 11     ... (continues properly)                                  ...
```

**Result:** counter = 2000 (CORRECT!)

---

### Key Changes:

| Aspect | Original | Fixed |
|--------|----------|-------|
| **Mutex** | None (missing) | `pthread_mutex_t mutex` |
| **Initialization** | N/A | `PTHREAD_MUTEX_INITIALIZER` |
| **Lock** | None | `pthread_mutex_lock()` before critical section |
| **Unlock** | None | `pthread_mutex_unlock()` after critical section |
| **Race Condition** | YES (present) | NO (eliminated) |
| **Correctness** | NO (wrong result) | YES (correct result) |

---

## Question 4 [4 x 4 + 4 = 20 Marks]

**Consider the following scenario:**

| Process ID | Arrival Time (ms) | CPU Burst Time (ms) |
|-----------|------------------|-------------------|
| P1 | 0 | 8 |
| P2 | 1 | 4 |
| P3 | 2 | 2 |
| P4 | 3 | 4 |
| P5 | 5 | 3 |

### Question 4.1: Calculate average waiting time and turnaround time for each of the following scheduling algorithms:

1. **First-Come, First-Served (FCFS)**
2. **Shortest Job First (SJF)**
3. **Shortest Remaining Time First (SRTF)**
4. **Round Robin (RR)** with time quantum = 2ms

---

### 1. FIRST-COME, FIRST-SERVED (FCFS)

**Algorithm:** Processes execute in order of arrival, no preemption.

**Gantt Chart:**
```
│ P1      │ P2  │ P3 │ P4  │ P5  │
├─────────┼─────┼────┼─────┼─────┤
0         8     12   14    18    21
```

**Timeline:**
- P1: Arrives at 0, starts at 0, finishes at 8
- P2: Arrives at 1, starts at 8, finishes at 12
- P3: Arrives at 2, starts at 12, finishes at 14
- P4: Arrives at 3, starts at 14, finishes at 18
- P5: Arrives at 5, starts at 18, finishes at 21

**Calculations:**

| Process | Arrival | CPU Burst | Start | Completion | Waiting Time | Turnaround Time |
|---------|---------|-----------|-------|------------|--------------|-----------------|
| P1 | 0 | 8 | 0 | 8 | 0 - 0 = 0 | 8 - 0 = 8 |
| P2 | 1 | 4 | 8 | 12 | 8 - 1 = 7 | 12 - 1 = 11 |
| P3 | 2 | 2 | 12 | 14 | 12 - 2 = 10 | 14 - 2 = 12 |
| P4 | 3 | 4 | 14 | 18 | 14 - 3 = 11 | 18 - 3 = 15 |
| P5 | 5 | 3 | 18 | 21 | 18 - 5 = 13 | 21 - 5 = 16 |

**Average Waiting Time (FCFS):**
$$AWL = \frac{0 + 7 + 10 + 11 + 13}{5} = \frac{41}{5} = 8.2 \text{ ms}$$

**Average Turnaround Time (FCFS):**
$$ATT = \frac{8 + 11 + 12 + 15 + 16}{5} = \frac{62}{5} = 12.4 \text{ ms}$$

---

### 2. SHORTEST JOB FIRST (SJF) - Non-preemptive

**Algorithm:** Process with shortest CPU burst time executes next. No preemption (process runs to completion).

**Processing Order by CPU Burst:**
- P3: 2ms (arrives 2)
- P5: 3ms (arrives 5)
- P2: 4ms (arrives 1)
- P4: 4ms (arrives 3)
- P1: 8ms (arrives 0)

**Gantt Chart:**
```
│ P1 │ P2  │ P3 │ P5  │ P4  │
├────┼─────┼────┼─────┼─────┤
0    8     12   14    17    21
```

**Timeline:**
- P1: Arrives at 0, starts at 0 (only one in queue), finishes at 8
- P2: Arrives at 1, starts at 8 (shortest among waiting), finishes at 12
- P3: Arrives at 2, starts at 12 (after P2), finishes at 14
- P5: Arrives at 5, starts at 14 (after P3), finishes at 17
- P4: Arrives at 3, starts at 17 (after P5), finishes at 21

**Calculations:**

| Process | Arrival | CPU Burst | Start | Completion | Waiting Time | Turnaround Time |
|---------|---------|-----------|-------|------------|--------------|-----------------|
| P1 | 0 | 8 | 0 | 8 | 0 - 0 = 0 | 8 - 0 = 8 |
| P2 | 1 | 4 | 8 | 12 | 8 - 1 = 7 | 12 - 1 = 11 |
| P3 | 2 | 2 | 12 | 14 | 12 - 2 = 10 | 14 - 2 = 12 |
| P5 | 5 | 3 | 14 | 17 | 14 - 5 = 9 | 17 - 5 = 12 |
| P4 | 3 | 4 | 17 | 21 | 17 - 3 = 14 | 21 - 3 = 18 |

**Average Waiting Time (SJF):**
$$AWL = \frac{0 + 7 + 10 + 9 + 14}{5} = \frac{40}{5} = 8.0 \text{ ms}$$

**Average Turnaround Time (SJF):**
$$ATT = \frac{8 + 11 + 12 + 12 + 18}{5} = \frac{61}{5} = 12.2 \text{ ms}$$

---

### 3. SHORTEST REMAINING TIME FIRST (SRTF) - Preemptive

**Algorithm:** At each scheduling decision, choose process with shortest remaining CPU burst. Can preempt currently running process.

**Processing:**

| Time | Ready Queue | Action | Running |
|------|-------------|--------|---------|
| 0 | [P1(8)] | Start P1 | P1 |
| 1 | [P2(4), P1(7)] | P2 shorter, preempt | P2 |
| 2 | [P3(2), P1(7), P2(3)] | P3 shorter, preempt | P3 |
| 3 | [P4(4), P1(7), P2(3), P3(1)] | P3 continues (1 < 4,3,7) | P3 |
| 4 | [P4(4), P1(7), P2(3), P5(3)] | P2 or P5 (both 3), P2 first | P2 |
| 5 | [P5(3), P1(7), P4(4), P2(2)] | P2 shorter (2) | P2 |
| 6 | [P5(3), P1(7), P4(4)] | P5 shorter (3) | P5 |
| 7 | [P1(7), P4(4), P5(2)] | P5 shorter (2) | P5 |
| 8 | [P1(7), P4(4)] | P4 shorter (4) | P4 |
| 9 | [P1(7), P4(3)] | P4 continues | P4 |
| 10 | [P1(7), P4(2)] | P4 continues | P4 |
| 11 | [P1(7), P4(1)] | P4 continues | P4 |
| 12 | [P1(7)] | Start P1 | P1 |
| 19 | [] | Done | - |

**Gantt Chart:**
```
│P1 │P2 │P3 │P2 │P5 │P4  │P1      │
├───┼───┼───┼───┼───┼────┼────────┤
0   1   2   4   6   8    12       19
    1   2   4   5   8    12
```

**Breakdown:**
- P1: 0-1 (1ms), P2: 1-2 (1ms), P3: 2-4 (2ms), P2: 4-6 (2ms), P5: 6-8 (2ms), P4: 8-12 (4ms), P1: 12-19 (7ms)

**Wait for Completion Times:**
- P1: 0-1 (1ms running), 12-19 (7ms running) = Total 19ms completion
- P2: 1-2 (1ms), 4-6 (2ms running) = Total 6ms completion
- P3: 2-4 (2ms running) = Total 4ms completion
- P4: 8-12 (4ms running) = Total 12ms completion
- P5: 6-8 (2ms running) = Total 8ms completion

**Calculations:**

| Process | Arrival | CPU Burst | Completion | Turnaround Time | Waiting Time |
|---------|---------|-----------|------------|-----------------|--------------|
| P1 | 0 | 8 | 19 | 19 - 0 = 19 | 19 - 8 = 11 |
| P2 | 1 | 4 | 6 | 6 - 1 = 5 | 5 - 4 = 1 |
| P3 | 2 | 2 | 4 | 4 - 2 = 2 | 2 - 2 = 0 |
| P4 | 3 | 4 | 12 | 12 - 3 = 9 | 9 - 4 = 5 |
| P5 | 5 | 3 | 8 | 8 - 5 = 3 | 3 - 3 = 0 |

**Average Waiting Time (SRTF):**
$$AWL = \frac{11 + 1 + 0 + 5 + 0}{5} = \frac{17}{5} = 3.4 \text{ ms}$$

**Average Turnaround Time (SRTF):**
$$ATT = \frac{19 + 5 + 2 + 9 + 3}{5} = \frac{38}{5} = 7.6 \text{ ms}$$

---

### 4. ROUND ROBIN (RR) - Time Quantum = 2ms

**Algorithm:** Each process gets 2ms time slice. After 2ms, process goes to end of ready queue.

**Processing:**

| Time | Running | Remaining | Action | New Queue |
|------|---------|-----------|--------|-----------|
| 0 | P1 | 8 → 6 | Slice done, go to queue | [P1(6)] |
| 1 | P2 | 4 → 2 | Preempted by arrival | [P1(6), P2(2)] |
| 2 | P1 | 6 → 4 | Slice done | [P2(2), P1(4)] |
| 2 | P3 | 2 | Arrived | [P2(2), P1(4), P3(2)] |
| 3 | P4 | 4 | Arrived | [P2(2), P1(4), P3(2), P4(4)] |
| 4 | P2 | 2 → 0 | Finished! | [P1(4), P3(2), P4(4)] |
| 5 | P5 | 3 | Arrived | [P1(4), P3(2), P4(4), P5(3)] |
| 6 | P1 | 4 → 2 | Slice done | [P3(2), P4(4), P5(3), P1(2)] |
| 8 | P3 | 2 → 0 | Finished! | [P4(4), P5(3), P1(2)] |
| 10 | P4 | 4 → 2 | Slice done | [P5(3), P1(2), P4(2)] |
| 12 | P5 | 3 → 1 | Slice done (1ms only) | [P1(2), P4(2), P5(1)] |
| 14 | P1 | 2 → 0 | Finished! | [P4(2), P5(1)] |
| 14 | P4 | 2 → 0 | Finished! | [P5(1)] |
| 14 | P5 | 1 → 0 | Finished! | [] |

**Gantt Chart:**
```
│P1  │P2  │P1  │P2  │P1  │P3  │P4  │P5  │P1  │P4  │P5  │
├────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┤
0    2    4    6    8   10   12   14   16   18   20   21
```

**Detailed Timeline:**

- P1: Runs 0-2 (2ms), preempted; Runs 2-4 (2ms), preempted; Runs 6-8 (2ms), preempted; Runs 14-16 (2ms), done at 16
  - Completion: 16, Turnaround: 16-0 = 16, Waiting: 16-8 = 8

- P2: Runs 4-6 (2ms), preempted; Runs 6-8 (but only 2ms needed), done at 8
  - Wait: P2 arrives at 1, runs at 4 (wait 3ms), then 6 (total wait before 2nd run = 5-4=1ms, plus waited 3-1 initially = 7ms)
  - Actually: Waiting time = (4-1) + (8-6) = 3 + 2 = 5ms
  - Completion: 8, Turnaround: 8-1 = 7, Waiting: 7-4 = 3

Wait, let me recalculate more carefully:

**Correct RR Calculation:**

```
│P1(0-2)│P2(2-4)│P3(4-6)│P2(6-8)│P4(8-10)│P5(10-12)│P1(12-14)│P4(14-16)│P5(16-17)│
├───────┼───────┼───────┼───────┼────────┼─────────┼─────────┼─────────┼─────────┤
0       2       4       6       8        10        12        14        16        17
```

**Completion times:**
- P1: 14 (2+2+2+8=14ms total, done at 14)
- P2: 8 (4ms total burst)
- P3: 6 (2ms total burst)
- P4: 16 (4ms total burst)
- P5: 17 (3ms total burst)

Let me recalculate from correct Gantt:

| Process | Arrival | CPU Burst | Completion | Turnaround | Waiting |
|---------|---------|-----------|------------|------------|---------|
| P1 | 0 | 8 | 14 | 14-0=14 | 14-8=6 |
| P2 | 1 | 4 | 8 | 8-1=7 | 7-4=3 |
| P3 | 2 | 2 | 6 | 6-2=4 | 4-2=2 |
| P4 | 3 | 4 | 16 | 16-3=13 | 13-4=9 |
| P5 | 5 | 3 | 17 | 17-5=12 | 12-3=9 |

**Average Waiting Time (RR):**
$$AWL = \frac{6 + 3 + 2 + 9 + 9}{5} = \frac{29}{5} = 5.8 \text{ ms}$$

**Average Turnaround Time (RR):**
$$ATT = \frac{14 + 7 + 4 + 13 + 12}{5} = \frac{50}{5} = 10.0 \text{ ms}$$

---

### Summary Table - Question 4.2

**Comparison of All Scheduling Algorithms:**

| Algorithm | Avg Waiting Time (ms) | Avg Turnaround Time (ms) | Notes |
|-----------|----------------------|-------------------------|-------|
| **FCFS** | 8.2 | 12.4 | Simple but inefficient, long waits |
| **SJF** | 8.0 | 12.2 | Better than FCFS, minimal average wait |
| **SRTF** | **3.4** | **7.6** | **Best performance** (preemptive) |
| **RR (q=2)** | 5.8 | 10.0 | Fair, moderate performance |

---

### Question 4.3: Evaluation & Explanation

**Which scheduling method provides optimal waiting time and turnaround time?**

**Answer: SHORTEST REMAINING TIME FIRST (SRTF)**

**Reasons:**

1. **Lowest Waiting Time (3.4 ms):**
   - SRTF prioritizes short jobs, minimizing idle time waiting for completion
   - Preemption allows better utilization of short bursts
   - Prevents long-running processes from blocking short ones

2. **Lowest Turnaround Time (7.6 ms):**
   - Turnaround = Waiting + CPU Burst Time
   - By minimizing waiting, turnaround is also minimized
   - SRTF gives priority to completing quick tasks first

3. **Efficiency Analysis:**

| Metric | FCFS | SJF | SRTF | RR |
|--------|------|-----|------|-----|
| Waiting Time | 8.2 | 8.0 | **3.4** | 5.8 |
| Turnaround Time | 12.4 | 12.2 | **7.6** | 10.0 |
| Improvement over FCFS | - | 2.4% | 58.5% | 29.3% |

4. **Context:**
   - **SRTF is optimal** for minimizing both waiting and turnaround time
   - However, SRTF requires knowing CPU burst times in advance (impractical)
   - SRTF causes overhead from frequent preemptions
   - In practice, RR offers better real-world performance despite higher theoretical numbers

5. **Trade-offs:**
   - **FCFS:** Simplest to implement, unfair to short processes
   - **SJF:** Good performance but requires burst time prediction
   - **SRTF:** Best theoretical performance but high preemption overhead
   - **RR:** Fair, reasonable performance, suitable for interactive systems

---

## Summary

| Question | Topic | Answer |
|----------|-------|--------|
| **Q2** | I/O Utilization | **89.26%** |
| **Q3** | Mutex Implementation | Code provided with race condition fixed |
| **Q4.1** | Scheduling Algorithms | FCFS, SJF, SRTF, RR calculations shown |
| **Q4.2** | Algorithm Comparison | SRTF optimal with AWL=3.4ms, ATT=7.6ms |
| **Q4.3** | Best Algorithm | **SRTF** for theoretical optimality |

