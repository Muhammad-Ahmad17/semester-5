# Producer-Consumer & Deadlock: Complete Guide

## What is Deadlock?

A **deadlock** occurs when two or more threads are blocked forever, waiting for each other to release resources. Each thread holds a resource and waits for another resource held by another thread.

---

## Producer-Consumer Deadlock Scenarios

### SCENARIO 1: Incorrect Semaphore Order (DEADLOCK!)

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int buffer_index = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

// WRONG PRODUCER - LEADS TO DEADLOCK
void *producer_WRONG(void *arg) {
    int item = *(int *)arg;
    
    // MISTAKE: Locking BEFORE checking semaphore
    pthread_mutex_lock(&mutex);    // LOCKS FIRST
    
    sem_wait(&empty);              // WAITS SECOND (if empty=0, deadlock!)
    
    buffer[buffer_index] = item;
    printf("[PRODUCER] Produced: %d\n", item);
    buffer_index++;
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    
    return NULL;
}

// WRONG CONSUMER - LEADS TO DEADLOCK
void *consumer_WRONG(void *arg) {
    int item;
    
    // MISTAKE: Locking BEFORE checking semaphore
    pthread_mutex_lock(&mutex);    // LOCKS FIRST
    
    sem_wait(&full);               // WAITS SECOND (if full=0, deadlock!)
    
    buffer_index--;
    item = buffer[buffer_index];
    printf("[CONSUMER] Consumed: %d\n", item);
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    
    return NULL;
}
```

### Why This Causes Deadlock?

```
DEADLOCK SCENARIO:

Buffer is EMPTY (full = 0, empty = 5)

Consumer Thread:
  1. pthread_mutex_lock(&mutex) → LOCKED ✓
  2. sem_wait(&full)             → BLOCKS (full=0, holds MUTEX!)
     
Producer Thread:
  1. Tries pthread_mutex_lock(&mutex) → WAITS (mutex held by Consumer)
  2. Never reaches sem_wait(&empty)
  
RESULT: DEADLOCK
  - Consumer blocked on semaphore (holds mutex)
  - Producer waiting for mutex (can't proceed)
  - Both threads STUCK FOREVER! ☠️
```

---

### SCENARIO 2: Correct Order (NO DEADLOCK)

```c
// CORRECT PRODUCER - NO DEADLOCK
void *producer_CORRECT(void *arg) {
    int item = *(int *)arg;
    
    // CORRECT: Check semaphore BEFORE locking
    sem_wait(&empty);              // WAIT FIRST (no mutex held)
    
    pthread_mutex_lock(&mutex);    // LOCK SECOND
    
    buffer[buffer_index] = item;
    printf("[PRODUCER] Produced: %d\n", item);
    buffer_index++;
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    
    return NULL;
}

// CORRECT CONSUMER - NO DEADLOCK
void *consumer_CORRECT(void *arg) {
    int item;
    
    // CORRECT: Check semaphore BEFORE locking
    sem_wait(&full);               // WAIT FIRST (no mutex held)
    
    pthread_mutex_lock(&mutex);    // LOCK SECOND
    
    buffer_index--;
    item = buffer[buffer_index];
    printf("[CONSUMER] Consumed: %d\n", item);
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    
    return NULL;
}
```

### Why This Works (NO Deadlock)?

```
SAFE SCENARIO:

Buffer is EMPTY (full = 0, empty = 5)

Consumer Thread:
  1. sem_wait(&full)           → BLOCKS (no mutex held!)
     
Producer Thread:
  1. sem_wait(&empty)          → SUCCEEDS (empty=5)
  2. pthread_mutex_lock()      → SUCCEEDS
  3. Add item to buffer
  4. pthread_mutex_unlock()    → Releases mutex
  5. sem_post(&full)           → WAKES UP CONSUMER!
  
Consumer Thread:
  6. Wakes up from sem_wait(&full)
  7. pthread_mutex_lock()      → SUCCEEDS (not held now)
  8. Remove item from buffer
  9. pthread_mutex_unlock()    → Releases mutex
  10. sem_post(&empty)         → Signals producer
  
RESULT: NO DEADLOCK ✓
```

---

## EXAM QUESTIONS & ANSWERS

### Q1: Why is Deadlock Possible in Producer-Consumer?

**Answer:**
```
Deadlock occurs when:

1. Lock (mutex) is acquired BEFORE checking resource availability (semaphore)
2. If resource unavailable, thread blocks WHILE HOLDING the lock
3. Other thread cannot acquire lock to produce/release resource
4. Both threads wait forever

Example:
Thread A: Lock mutex → wait on empty semaphore (blocked, holds lock)
Thread B: Try to lock mutex → blocked (Thread A has it)
         Cannot signal to wake Thread A
Result: DEADLOCK
```

---

### Q2: What is the Deadlock Condition in Producer-Consumer?

**Answer:**
```
FOUR CONDITIONS NEEDED FOR DEADLOCK:

1. Mutual Exclusion
   - Mutex lock prevents simultaneous buffer access ✓

2. Hold and Wait
   - Thread holds mutex while waiting for semaphore ✓
   - WRONG ORDER: mutex first, then semaphore

3. No Preemption
   - Thread cannot be forcibly removed from resources ✓

4. Circular Wait
   - Producer waits for Consumer (via full semaphore)
   - Consumer waits for mutex (held by Producer)
   - CIRCULAR DEPENDENCY! ✓

IF ANY CONDITION IS REMOVED → NO DEADLOCK
```

---

### Q3: How to Prevent Deadlock in Producer-Consumer?

**Answer:**
```
PREVENTION TECHNIQUES:

1. CORRECT LOCK ORDERING (MOST IMPORTANT)
   ✓ Wait on semaphore BEFORE acquiring mutex
   ✗ Don't lock mutex before checking resource availability

2. HOLD TIME MINIMIZATION
   ✓ Release locks quickly
   ✓ Do work outside critical section

3. RESOURCE ORDERING
   ✓ Always acquire resources in same order
   ✓ Producer: empty → mutex
   ✓ Consumer: full → mutex

4. TIMEOUTS
   ✓ Use sem_timedwait() instead of sem_wait()
   ✓ Thread wakes up after timeout, can retry

FORMULA FOR NO DEADLOCK:
sem_wait() → pthread_mutex_lock() → (critical section) 
→ pthread_mutex_unlock() → sem_post()
```

---

### Q4: Explain the Deadlock Scenario Step-by-Step

**Answer:**
```
SCENARIO: 2 Producers, 2 Consumers, Buffer Size = 1

DEADLOCK SITUATION:

Time 1: Producer1 locks mutex, waits on empty
        (Buffer full, empty = 0)
        Hold: mutex
        Wants: empty

Time 2: Consumer1 locks mutex... BLOCKED
        (Producer1 has mutex)
        Wants: mutex

Time 3: Producer1 cannot proceed
        Consumer1 cannot proceed

Time 4: DEADLOCK!
        Producer1: Holds mutex, wants empty
        Consumer1: Wants mutex, blocked forever

WHY NO ONE HELPS?
- Producer1 cannot signal 'full' (has not released mutex)
- Consumer1 cannot acquire mutex (Producer1 blocks it)
- No one can advance → DEADLOCK ☠️
```

---

### Q5: Write Code That Causes Deadlock (Common Exam Question)

**Answer:**
```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 2

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;
sem_t full;

void *producer(void *arg) {
    pthread_mutex_lock(&mutex);    // LOCK FIRST ← MISTAKE!
    
    sem_wait(&full);               // WAIT SECOND (while holding lock)
    // If full = 0 (buffer empty), THIS BLOCKS WITH LOCK HELD!
    
    buffer[count++] = 1;
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *consumer(void *arg) {
    pthread_mutex_lock(&mutex);    // Tries to lock...
    // But Producer holds it while blocked on semaphore
    // CONSUMER DEADLOCKED!
    
    count--;
    buffer[count] = 0;
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}

// RESULT: DEADLOCK! Both threads stuck forever.
```

---

### Q6: Correct Version Without Deadlock

**Answer:**
```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 2

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex;
sem_t empty, full;

void *producer(void *arg) {
    sem_wait(&empty);              // WAIT FIRST ✓
    
    pthread_mutex_lock(&mutex);    // LOCK SECOND ✓
    
    buffer[count++] = 1;
    printf("[PRODUCER] Produced\n");
    
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    
    return NULL;
}

void *consumer(void *arg) {
    sem_wait(&full);               // WAIT FIRST ✓
    
    pthread_mutex_lock(&mutex);    // LOCK SECOND ✓
    
    count--;
    printf("[CONSUMER] Consumed\n");
    
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    
    return NULL;
}

// RESULT: NO DEADLOCK! Works perfectly ✓
```

---

### Q7: What Happens if Both Threads Use Same Semaphore for Lock? (Deadlock)

**Answer:**
```c
// WRONG: Using one semaphore for both protection
sem_t resource;  // Initialized to 1 (binary semaphore)

void *producer(void *arg) {
    sem_wait(&resource);   // Acquire
    // Produce item
    sem_post(&resource);   // Release
}

void *consumer(void *arg) {
    sem_wait(&resource);   // Try to acquire
    // If producer has it, consumer blocks
    // If they're both waiting on SAME resource...
    // Can cause synchronization issues but NOT typical deadlock
    // (because semaphore wakes up waiting thread)
    
    // However, if CIRCULAR dependency:
    // Thread A waits for B, Thread B waits for A → DEADLOCK
}
```

---

### Q8: True/False Questions for Exams

**Q: "Deadlock cannot occur if semaphore is used instead of mutex."**
```
FALSE ✗

Reason: Deadlock depends on the USAGE pattern, not just the tool.
- Semaphore misuse can cause deadlock
- Mutex misuse can cause deadlock
- Correct usage of both prevents deadlock
```

**Q: "Deadlock occurs in producer-consumer if mutex is locked before semaphore."**
```
TRUE ✓

Reason: If thread blocks on semaphore while holding mutex,
other thread cannot acquire mutex to help, causing deadlock.
```

**Q: "Using sem_wait before pthread_mutex_lock always prevents deadlock."**
```
TRUE ✓ (in producer-consumer context)

Reason: Thread doesn't hold mutex while waiting,
so other threads can proceed and signal semaphores.
```

---

## Deadlock vs Livelock vs Starvation

| Issue | Definition | Example |
|-------|-----------|---------|
| **Deadlock** | Threads blocked forever, waiting for each other | Producer holds mutex, waits on semaphore; Consumer can't get mutex |
| **Livelock** | Threads keep running but make no progress | Threads keep retrying but always conflict |
| **Starvation** | One thread never gets resource | High priority thread always preempts low priority |

---

## Summary Table

| Scenario | Deadlock? | Why |
|----------|-----------|-----|
| sem_wait() → mutex_lock() | NO ✓ | No circular wait |
| mutex_lock() → sem_wait() | YES ☠️ | Circular wait possible |
| Correct lock ordering | NO ✓ | Resources acquired in order |
| Timeout on sem_wait() | NO ✓ | Can recover from wait |
| Multiple mutexes, wrong order | YES ☠️ | Circular dependency |

---

## Key Points to Remember for Exam

```
1. DEADLOCK REQUIRES ALL FOUR CONDITIONS:
   ✓ Mutual Exclusion
   ✓ Hold and Wait
   ✓ No Preemption
   ✓ Circular Wait

2. PRODUCER-CONSUMER DEADLOCK CAUSE:
   ✗ Lock mutex BEFORE checking semaphore
   ✓ Check semaphore BEFORE locking mutex

3. PREVENTION:
   ✓ Always acquire resources in same order
   ✓ Don't hold locks while waiting
   ✓ Use timeouts
   ✓ Detect and recover from deadlock

4. IN EXAMS:
   - Identify if code can deadlock
   - Explain why it deadlocks
   - Provide correct solution
   - Use diagrams showing resource holding
```

---

## Most Common Exam Questions

### Type 1: "Will This Code Deadlock?"
```
Given code snippet → Analyze lock ordering → Say YES/NO → Explain

Answer format:
"YES/NO, because [explain resource holding and waiting]"
```

### Type 2: "What is the Deadlock Condition?"
```
List and explain the 4 conditions:
1. Mutual Exclusion - (explain how)
2. Hold and Wait - (explain how)
3. No Preemption - (explain how)
4. Circular Wait - (explain how)
```

### Type 3: "Correct the Deadlock Code"
```
Given wrong code → Reorder operations → Show corrected code
```

### Type 4: "Trace Execution and Find Deadlock Point"
```
Show timeline of thread execution → Mark where deadlock occurs
```

---

## Practice Problems

**Problem 1:** Two producers, two consumers, buffer size 1. Will it deadlock if:
```c
pthread_mutex_lock(&mutex);
sem_wait(&empty);  // or sem_wait(&full) for consumer
```
**Answer:** YES, deadlock possible. Correct order: sem_wait before mutex_lock.

**Problem 2:** Does this pattern prevent deadlock?
```c
sem_wait(&empty);
sem_wait(&full);   // Both semaphores before mutex
pthread_mutex_lock(&mutex);
```
**Answer:** YES, no deadlock. Both semaphores checked before lock acquired.

**Problem 3:** Multiple semaphores in wrong order:
```c
sem_wait(&sem1);
sem_wait(&sem2);
while(sem2 < sem1) sem_wait(&sem1);  // Circular!
```
**Answer:** POSSIBLE DEADLOCK due to circular wait.

