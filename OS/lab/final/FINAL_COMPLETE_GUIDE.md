# FINAL COMPLETE CORRECT PRODUCER-CONSUMER CODE

## Quick Start

```bash
cd /home/muhammad-ahmad/Desktop/semester-5/OS/lab/final

# Compile
gcc -pthread -o pc final_correct_producer_consumer.c

# Run
./pc
```

---

## Code Overview

This is the **FINAL, CORRECT, COMPLETE** implementation of Producer-Consumer with:
- ✅ **No Deadlock** (proper lock ordering)
- ✅ **No Race Conditions** (mutex protection)
- ✅ **Clear Comments** (explained every step)
- ✅ **Easy to Understand** (simple logic)
- ✅ **Production Ready** (proper cleanup)

---

## Program Structure

### Shared Resources
```c
int buffer[BUFFER_SIZE];     // What they share
int buffer_count;            // How many items
int item_id;                 // Item counter
```

### Synchronization Objects
```c
pthread_mutex_t mutex;       // Lock (1 thread at a time)
sem_t empty;                 // Empty slots (starts at 5)
sem_t full;                  // Full slots (starts at 0)
```

### Thread Functions
```c
void *producer(void *arg)    // Produces items
void *consumer(void *arg)    // Consumes items
```

---

## The CORRECT Order (KEY POINT!)

### Producer (Step by Step)

```
STEP 1: sem_wait(&empty)
        ↓
        "Is there empty space?"
        If YES: Continue
        If NO: Wait (WITHOUT holding mutex)

STEP 2: pthread_mutex_lock(&mutex)
        ↓
        Lock the door (exclusive access)

STEP 3: buffer[buffer_count] = item_id++
        ↓
        Add item to buffer
        (Critical section - only I can do this)

STEP 4: pthread_mutex_unlock(&mutex)
        ↓
        Unlock the door (others can enter)

STEP 5: sem_post(&full)
        ↓
        Tell consumer "I added an item!"
        Wake up any waiting consumer
```

### Consumer (Step by Step)

```
STEP 1: sem_wait(&full)
        ↓
        "Is there an item?"
        If YES: Continue
        If NO: Wait (WITHOUT holding mutex)

STEP 2: pthread_mutex_lock(&mutex)
        ↓
        Lock the door (exclusive access)

STEP 3: item = buffer[--buffer_count]
        ↓
        Take item from buffer
        (Critical section - only I can do this)

STEP 4: pthread_mutex_unlock(&mutex)
        ↓
        Unlock the door (others can enter)

STEP 5: sem_post(&empty)
        ↓
        Tell producer "I freed a slot!"
        Wake up any waiting producer
```

---

## Why This Works (NO DEADLOCK)

### Scenario: Buffer Full

```
Buffer is FULL (empty = 0)

Producer calls sem_wait(&empty)
  ↓
Semaphore counter = 0, so PRODUCER BLOCKS
  ↓
BUT: Producer does NOT hold mutex!
  ↓
Consumer can:
  1. Acquire mutex
  2. Remove item from buffer
  3. Release mutex
  4. Call sem_post(&empty)
  ↓
PRODUCER WAKES UP!
  ↓
RESULT: NO DEADLOCK ✓
```

### Scenario: Buffer Empty

```
Buffer is EMPTY (full = 0)

Consumer calls sem_wait(&full)
  ↓
Semaphore counter = 0, so CONSUMER BLOCKS
  ↓
BUT: Consumer does NOT hold mutex!
  ↓
Producer can:
  1. Acquire mutex
  2. Add item to buffer
  3. Release mutex
  4. Call sem_post(&full)
  ↓
CONSUMER WAKES UP!
  ↓
RESULT: NO DEADLOCK ✓
```

---

## Execution Timeline Example

```
TIME | PRODUCER 1        | PRODUCER 2        | CONSUMER 1        | CONSUMER 2
─────┼──────────────────┼──────────────────┼──────────────────┼──────────────
T0   | sem_wait(empty)  |                  |                  |
     | ✓ (empty=5)      |                  |                  |
     | mutex_lock()     |                  |                  |
─────┼──────────────────┼──────────────────┼──────────────────┼──────────────
T1   | Add item 1       | sem_wait(empty)  |                  |
     |                  | ✓ (empty=4)      |                  |
     | mutex_unlock()   |                  |                  |
     | sem_post(full)   |                  |                  |
─────┼──────────────────┼──────────────────┼──────────────────┼──────────────
T2   | sleep            | mutex_lock()     | sem_wait(full)   |
     |                  | Add item 2       | ✓ (full=1)       |
     |                  |                  | mutex_lock() ✓   |
─────┼──────────────────┼──────────────────┼──────────────────┼──────────────
T3   |                  | mutex_unlock()   | Remove item 1    |
     |                  | sem_post(full)   |                  |
     |                  |                  | mutex_unlock()   |
     |                  |                  | sem_post(empty)  |
─────┴──────────────────┴──────────────────┴──────────────────┴──────────────

Key: ✓ = Operation successful
     Blocked = Thread waiting
```

---

## Expected Output

```
╔═══════════════════════════════════════════════════════════════════╗
║     PRODUCER-CONSUMER SYNCHRONIZATION (CORRECT IMPLEMENTATION)     ║
╚═══════════════════════════════════════════════════════════════════╝

Configuration:
  • Buffer Size: 5
  • Producers: 2 (produce 3 items each = 6 total)
  • Consumers: 2 (consume 3 items each = 6 total)
  • Mutex: Protects buffer access
  • Semaphore 'empty': Tracks empty slots (initial = 5)
  • Semaphore 'full': Tracks full slots (initial = 0)

Synchronization Order:
  PRODUCER:  sem_wait(empty) → mutex_lock → add → mutex_unlock → sem_post(full)
  CONSUMER:  sem_wait(full)  → mutex_lock → remove → mutex_unlock → sem_post(empty)

═══════════════════════════════════════════════════════════════════

✓ Mutex initialized (unlocked)
✓ Semaphore 'empty' initialized (value = 5)
✓ Semaphore 'full' initialized (value = 0)

Creating 2 PRODUCER threads...
✓ Producer threads created

Creating 2 CONSUMER threads...
✓ Consumer threads created

Waiting for all threads to complete...

[PRODUCER 1] Starting...
[PRODUCER 1] Waiting for empty slot...
[PRODUCER 1] Acquired lock, entering critical section
[PRODUCER 1] ➜ PRODUCED item: 1 at index: 0 (Buffer: 1/5)
[PRODUCER 1] Released lock
[PRODUCER 2] Starting...
[PRODUCER 2] Waiting for empty slot...
[PRODUCER 2] Acquired lock, entering critical section
[PRODUCER 2] ➜ PRODUCED item: 2 at index: 1 (Buffer: 2/5)
[PRODUCER 2] Released lock
[CONSUMER 1] Starting...
[CONSUMER 1] Waiting for item...
[CONSUMER 1] Acquired lock, entering critical section
[CONSUMER 1] ➜ CONSUMED item: 2 from index: 1 (Buffer: 1/5)
[CONSUMER 1] Released lock
[CONSUMER 2] Starting...
[CONSUMER 2] Waiting for item...
[CONSUMER 2] Acquired lock, entering critical section
[CONSUMER 2] ➜ CONSUMED item: 1 from index: 0 (Buffer: 0/5)
[CONSUMER 2] Released lock

... (continues for all items) ...

[PRODUCER 1] Finished producing 3 items
[PRODUCER 2] Finished producing 3 items
[CONSUMER 1] Finished consuming 3 items
[CONSUMER 2] Finished consuming 3 items

═══════════════════════════════════════════════════════════════════
║ EXECUTION COMPLETED SUCCESSFULLY                                 ║
╠═══════════════════════════════════════════════════════════════════╣
║ Final Buffer State:                                               ║
║   • Buffer count: 0 (should be 0)                                 ║
║   • All items produced: 6                                         ║
║   • All items consumed: 6                                         ║
║   • NO DEADLOCK ✓                                                 ║
║   • NO RACE CONDITIONS ✓                                          ║
║   • PROPER SYNCHRONIZATION ✓                                      ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

## Critical Section Diagram

```
┌─────────────────────────────────────────────────────────┐
│ CRITICAL SECTION (Only 1 thread at a time)             │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Thread A:                    Thread B:                │
│  ┌──────────────────┐        ┌──────────────────┐     │
│  │ pthread_mutex    │        │ WAIT              │     │
│  │ _lock()          │ ──┐    │ (cannot enter)   │     │
│  │                  │   │    │                  │     │
│  │ buffer[i] = x    │   │    │                  │     │
│  │                  │   │    │                  │     │
│  │ pthread_mutex    │   │    │                  │     │
│  │ _unlock()        │ ◄─┘    │                  │     │
│  └──────────────────┘        │                  │     │
│                              │ mutex_lock()     │     │
│                              │ ...              │     │
│                              └──────────────────┘     │
│                                                         │
└─────────────────────────────────────────────────────────┘

Only ONE thread can execute critical section at a time!
```

---

## Semaphore State Changes During Execution

```
INITIAL STATE:
  empty = 5  (5 empty slots)
  full = 0   (0 full slots)

AFTER PRODUCER 1:
  empty = 4  (4 empty slots)
  full = 1   (1 full slot)

AFTER PRODUCER 2:
  empty = 3  (3 empty slots)
  full = 2   (2 full slots)

AFTER CONSUMER 1:
  empty = 4  (4 empty slots)
  full = 1   (1 full slot)

FINAL STATE (All produced & consumed):
  empty = 5  (all slots empty again)
  full = 0   (no items)
```

---

## Code Features Explained

### 1. Clear Comments
Every step is commented explaining what happens and why

### 2. Descriptive Output
Shows exactly what each thread is doing at each moment

### 3. Proper Initialization
```c
pthread_mutex_init(&mutex, NULL);
sem_init(&empty, 0, BUFFER_SIZE);
sem_init(&full, 0, 0);
```

### 4. Proper Cleanup
```c
pthread_mutex_destroy(&mutex);
sem_destroy(&empty);
sem_destroy(&full);
```

### 5. Thread Creation
```c
for(int i = 0; i < 2; i++) {
    pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
}
```

### 6. Thread Joining
```c
for(int i = 0; i < 2; i++) {
    pthread_join(producer_threads[i], NULL);
}
```

---

## Common Modifications

### Modify Buffer Size
```c
#define BUFFER_SIZE 10  // Change from 5 to 10
```

### Modify Number of Items
```c
for(int i = 0; i < 5; i++) {  // Change from 3 to 5
    // ... produce/consume ...
}
```

### Modify Number of Threads
```c
int producer_ids[3] = {1, 2, 3};  // 3 producers instead of 2
for(int i = 0; i < 3; i++) {
    pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
}
```

### Add More Realistic Delays
```c
sleep(rand() % 3);  // Random sleep 0-2 seconds
```

---

## What You Learned

| Concept | Meaning |
|---------|---------|
| **Mutex** | Lock that ensures only 1 thread at a time |
| **Semaphore** | Counter that can be > 1, tracks resources |
| **sem_wait()** | Acquire resource (counter--) |
| **sem_post()** | Release resource (counter++) |
| **Critical Section** | Code that only 1 thread should execute |
| **Race Condition** | Bug when multiple threads access shared data |
| **Deadlock** | Threads stuck waiting for each other |
| **Synchronization** | Coordinating threads to prevent conflicts |

---

## Compilation Flags Explained

```bash
gcc -pthread -o pc final_correct_producer_consumer.c
    └─ Link pthread library (required for threads)
```

---

## Summary

This code demonstrates:
1. ✅ Correct synchronization
2. ✅ Proper resource ordering
3. ✅ No deadlock
4. ✅ No race conditions
5. ✅ Clear, easy-to-understand logic
6. ✅ Production-quality implementation

**This is ready for exam, assignment, or production use!** 🎯
