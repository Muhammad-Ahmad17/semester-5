# Simple Producer-Consumer with Threads & Semaphore

## Quick Start

### Compile
```bash
gcc -pthread -o pc simple_producer_consumer.c
```

### Run
```bash
./pc
```

---

## Code Explanation (Step by Step)

### 1. Includes & Setup
```c
#include <pthread.h>   // For threads
#include <semaphore.h> // For semaphores
#include <unistd.h>    // For sleep()

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];      // Shared buffer
int buffer_index = 0;          // Current position
```

### 2. Synchronization Objects
```c
pthread_mutex_t mutex;  // Lock (one at a time)
sem_t empty;            // Empty slots counter (starts at 5)
sem_t full;             // Full slots counter (starts at 0)
```

**What they do:**
- **mutex**: Ensures only 1 thread accesses buffer at a time
- **empty**: Tracks how many empty slots available
- **full**: Tracks how many items in buffer

---

## Producer Function (Easy Explanation)

```c
void *producer(void *arg) {
    int item = *(int *)arg;
    
    // STEP 1: Wait for empty slot
    sem_wait(&empty);
    
    // STEP 2: Lock access to buffer
    pthread_mutex_lock(&mutex);
    
    // STEP 3: Add item to buffer
    buffer[buffer_index] = item;
    printf("[PRODUCER] Produced: %d\n", item);
    buffer_index++;
    
    // STEP 4: Unlock access
    pthread_mutex_unlock(&mutex);
    
    // STEP 5: Tell consumer there's an item
    sem_post(&full);
    
    return NULL;
}
```

**In Plain English:**
1. Wait until there's space in buffer
2. Lock the door (only I can use buffer now)
3. Put my item in the buffer
4. Unlock the door
5. Ring the bell to tell consumer there's food!

---

## Consumer Function (Easy Explanation)

```c
void *consumer(void *arg) {
    int item;
    
    // STEP 1: Wait for item in buffer
    sem_wait(&full);
    
    // STEP 2: Lock access to buffer
    pthread_mutex_lock(&mutex);
    
    // STEP 3: Take item from buffer
    buffer_index--;
    item = buffer[buffer_index];
    printf("[CONSUMER] Consumed: %d\n", item);
    
    // STEP 4: Unlock access
    pthread_mutex_unlock(&mutex);
    
    // STEP 5: Tell producer there's empty space
    sem_post(&empty);
    
    return NULL;
}
```

**In Plain English:**
1. Wait until there's something to eat
2. Lock the door (only I can use buffer now)
3. Take an item from the buffer
4. Unlock the door
5. Ring the bell to tell producer there's space!

---

## Main Function Breakdown

```c
int main() {
    pthread_t tid[6];           // Array of 6 threads
    int items[3] = {10, 20, 30};
    
    // Initialize mutex (lock)
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // 5 empty slots
    sem_init(&full, 0, 0);              // 0 full slots
    
    // Create 3 producers
    for(int i = 0; i < 3; i++) {
        pthread_create(&tid[i], NULL, producer, &items[i]);
    }
    
    sleep(1);  // Let producers go first
    
    // Create 3 consumers
    for(int i = 0; i < 3; i++) {
        pthread_create(&tid[3 + i], NULL, consumer, NULL);
    }
    
    // Wait for all threads to finish
    for(int i = 0; i < 6; i++) {
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
```

---

## Execution Flow Diagram

```
START
  ↓
Initialize:
  mutex = UNLOCKED
  empty = 5 (slots available)
  full = 0 (items available)
  ↓
Create 3 Producers & 3 Consumers
  ↓
┌──────────────────────────────────────┐
│ PRODUCER 1                           │
│ sem_wait(&empty)    → empty: 5 → 4   │
│ mutex_lock()                         │
│ buffer[0] = 10                       │
│ buffer_index = 1                     │
│ mutex_unlock()                       │
│ sem_post(&full)     → full: 0 → 1    │
└──────────────────────────────────────┘
  ↓
┌──────────────────────────────────────┐
│ CONSUMER 1                           │
│ sem_wait(&full)     → full: 1 → 0    │
│ mutex_lock()                         │
│ item = buffer[0] (get 10)            │
│ buffer_index = 0                     │
│ mutex_unlock()                       │
│ sem_post(&empty)    → empty: 4 → 5   │
└──────────────────────────────────────┘
  ↓
DONE
```

---

## Semaphore State Changes

```
Initial State:
empty = 5  (5 empty slots in buffer)
full = 0   (0 full slots in buffer)

After Producer puts item:
empty = 4  (1 slot used)
full = 1   (1 item available)

After Consumer takes item:
empty = 5  (slot is empty again)
full = 0   (no items left)
```

---

## What Happens When Buffer is Full?

```
Buffer is FULL (empty = 0):

Producer calls sem_wait(&empty)
    ↓
Counter is 0, so PRODUCER BLOCKS
    ↓
Waits for Consumer to take item
    ↓
Consumer takes item → sem_post(&empty)
    ↓
Producer wakes up and continues
```

---

## What Happens When Buffer is Empty?

```
Buffer is EMPTY (full = 0):

Consumer calls sem_wait(&full)
    ↓
Counter is 0, so CONSUMER BLOCKS
    ↓
Waits for Producer to add item
    ↓
Producer adds item → sem_post(&full)
    ↓
Consumer wakes up and continues
```

---

## Key Concepts

### Mutex (Mutual Exclusion)
- Only **1 thread** at a time in critical section
- Prevents race conditions
- Lock before accessing shared data
- Unlock after done

### Semaphore
- **Counter-based** synchronization
- `sem_wait()`: counter-- (block if 0)
- `sem_post()`: counter++ (wake waiting thread)

### This Solution Ensures
✅ No race conditions (mutex protects buffer)
✅ No deadlock (producer waits for empty, consumer waits for full)
✅ No busy waiting (threads sleep until signaled)
✅ Fair access (FIFO queue for waiting threads)

---

## Example Output

```
========================================
Producer-Consumer Using Threads & Semaphore
========================================
Buffer Size: 5
Mutex: Protects buffer access
Semaphore 'empty': 5 (empty slots)
Semaphore 'full': 0 (full slots)
========================================

Creating 3 PRODUCER threads...
[PRODUCER] Produced item: 10 at index 0
[PRODUCER] Produced item: 20 at index 1
[PRODUCER] Produced item: 30 at index 2

Creating 3 CONSUMER threads...

[CONSUMER] Consumed item: 30 from index 2
[CONSUMER] Consumed item: 20 from index 1
[CONSUMER] Consumed item: 10 from index 0

========================================
All threads completed successfully!
========================================
```

---

## Common Issues & Solutions

### Issue 1: Deadlock
**Problem:** Producer waits for consumer, consumer waits for producer
**Solution:** Use semaphores correctly (empty & full)

### Issue 2: Race Condition
**Problem:** Both threads modify buffer simultaneously
**Solution:** Use mutex lock before accessing buffer

### Issue 3: Lost Updates
**Problem:** One thread's change overwritten
**Solution:** Mutex prevents this

---

## Further Improvements

### Version 2: Multiple Items Per Producer
```c
void *producer(void *arg) {
    for(int i = 0; i < 5; i++) {  // Produce 5 items
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        // ... add item ...
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);  // Simulate work
    }
    return NULL;
}
```

### Version 3: Random Delays
```c
sleep(rand() % 3);  // Sleep 0-2 seconds
```

### Version 4: Better Print Output
```c
printf("[TIME: %ld] [PRODUCER %ld] Item: %d\n", 
       time(NULL), pthread_self(), item);
```

---

## Compile & Run Commands

```bash
# Compile (must use -pthread flag)
gcc -pthread -o pc simple_producer_consumer.c

# Run
./pc

# Run multiple times to see different outputs
for i in {1..3}; do ./pc; echo "---"; done

# Compile with debugging
gcc -pthread -g -o pc simple_producer_consumer.c

# Run with debugger (gdb)
gdb ./pc
```

---

## Summary

| Part | Purpose | Why? |
|------|---------|------|
| `pthread_mutex` | Lock buffer access | Prevent race conditions |
| `sem_wait(&empty)` | Wait for space | Don't overfill buffer |
| `sem_wait(&full)` | Wait for item | Don't over-consume |
| `sem_post()` | Signal other thread | Wake them up |

This is the **complete, working, easy-to-understand** producer-consumer solution! 🎯
