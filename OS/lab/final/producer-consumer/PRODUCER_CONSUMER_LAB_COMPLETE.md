# LAB #7: Producer-Consumer Synchronization

## Objectives
- Demonstrate the working of producer and consumer functions
- Create a producer thread and consumer thread to perform producer and consumer routines
- Show synchronization among the producer and consumer threads using Semaphores

---

## Pre-Lab Theory: Producer-Consumer Problem

### Concept
The producer-consumer problem is a classic synchronization problem where:
- A **fixed-size buffer** stores items
- A **producer process** creates items and enters them into the buffer
- A **consumer process** removes items from the buffer and consumes them

### Problem
If both processes execute without proper synchronization:
- Race conditions occur
- Buffer size becomes incorrect
- Data integrity is compromised

### Solution
Use **Semaphores** and **Mutex locks** for synchronization.

---

## TASK 1: Consumer Function Implementation

### Given Code (Producer Function)
```c
#include <stdio.h>
#include <stdlib.h>

// Initialize a mutex to 1 
int mutex = 1; 

// Number of full slots as 0 
int full = 0; 

// Number of empty slots as size of buffer 
int empty = 10, x = 0; 

// Function to produce an item and add it to the buffer 
void producer() 
{ 
    // Decrease mutex value by 1 
    --mutex; 

    // Increase the number of full slots by 1 
    ++full; 

    // Decrease the number of empty slots by 1 
    --empty; 

    // Item produced 
    x++; 
    printf("\nProducer produces item %d", x); 

    // Increase mutex value by 1 
    ++mutex; 
}
```

### Consumer Function (to be implemented)
```c
void consumer() 
{ 
    // Decrease mutex value by 1 
    --mutex; 

    // Decrease the number of full slots by 1 
    --full; 

    // Increase the number of empty slots by 1 
    ++empty; 

    // Item consumed 
    x--; 
    printf("\nConsumer consumes item %d", x); 

    // Increase mutex value by 1 
    ++mutex; 
}
```

---

## TASK 2: Driver Code with Menu

### Complete Code: task2_producer_consumer.c
```c
#include <stdio.h>
#include <stdlib.h>

// Initialize a mutex to 1 
int mutex = 1; 

// Number of full slots as 0 
int full = 0; 

// Number of empty slots as size of buffer 
int empty = 10, x = 0; 

// Function to produce an item and add it to the buffer 
void producer() 
{ 
    // Decrease mutex value by 1 
    --mutex; 

    // Increase the number of full slots by 1 
    ++full; 

    // Decrease the number of empty slots by 1 
    --empty; 

    // Item produced 
    x++; 
    printf("\nProducer produces item %d", x); 

    // Increase mutex value by 1 
    ++mutex; 
}

void consumer() 
{ 
    // Decrease mutex value by 1 
    --mutex; 

    // Decrease the number of full slots by 1 
    --full; 

    // Increase the number of empty slots by 1 
    ++empty; 

    // Item consumed 
    x--; 
    printf("\nConsumer consumes item %d", x); 

    // Increase mutex value by 1 
    ++mutex; 
}

int main() 
{ 
    int choice; 

    printf("\n=== Producer-Consumer Problem ===\n");
    
    while(1) 
    { 
        printf("\nEnter the Choice:\n"); 
        printf("1. Producer\n"); 
        printf("2. Consumer\n"); 
        printf("3. Exit\n"); 
        scanf("%d", &choice); 

        switch(choice) 
        { 
            case 1: 
                // Condition for Producer
                if ((mutex == 1) && (empty != 0)) 
                { 
                    producer(); 
                } 
                else 
                { 
                    printf("\nBuffer is FULL!!"); 
                } 
                break; 

            case 2: 
                // Condition for Consumer
                if ((mutex == 1) && (full != 0)) 
                { 
                    consumer(); 
                } 
                else 
                { 
                    printf("\nBuffer is EMPTY!!"); 
                } 
                break; 

            case 3: 
                exit(0); 
                break; 

            default: 
                printf("\nInvalid Choice!"); 
        } 
    } 

    return 0; 
}
```

### Expected Output
```
=== Producer-Consumer Problem ===

Enter the Choice:
1. Producer
2. Consumer
3. Exit
1

Producer produces item 1
Enter the Choice:
1. Producer
2. Consumer
3. Exit
2

Consumer consumes item 1
Enter the Choice:
1. Producer
2. Consumer
3. Exit
1

Producer produces item 2
Enter the Choice:
1. Producer
2. Consumer
3. Exit
3
```

---

## TASK 3: Circular Buffer Implementation

### Given Consumer Function
```c
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer
int in = 0;     // Index for producer to insert
int out = 0;    // Index for consumer to remove

void consumer() 
{ 
    // Remove item from buffer 
    int item = buffer[out]; 
    out = (out + 1) % BUFFER_SIZE; 
    count--; 
    printf("Consumed item %d\n", item); 
}
```

### Producer Function (Same Pattern)
```c
void producer(int item) 
{ 
    // Add item to buffer 
    buffer[in] = item; 
    in = (in + 1) % BUFFER_SIZE; 
    count++; 
    printf("Produced item %d\n", item); 
}
```

---

## TASK 4: Driver Code for Circular Buffer

### Complete Code: task4_circular_buffer.c
```c
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer
int in = 0;     // Index for producer to insert
int out = 0;    // Index for consumer to remove
int item_number = 0; // To track item numbers

void producer() 
{ 
    // Add item to buffer 
    item_number++;
    buffer[in] = item_number; 
    printf("Produced item %d at index %d\n", item_number, in);
    in = (in + 1) % BUFFER_SIZE; 
    count++; 
}

void consumer() 
{ 
    // Remove item from buffer 
    int item = buffer[out]; 
    printf("Consumed item %d from index %d\n", item, out);
    out = (out + 1) % BUFFER_SIZE; 
    count--; 
}

int main()
{
    int choice;

    printf("\n=== Circular Buffer Producer-Consumer ===\n");

    while(1)
    {
        printf("\nBuffer Status: count=%d, in=%d, out=%d\n", count, in, out);
        printf("Enter the Choice:\n");
        printf("1. Producer\n");
        printf("2. Consumer\n");
        printf("3. Display Buffer\n");
        printf("4. Exit\n");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                if(count < BUFFER_SIZE)
                {
                    producer();
                }
                else
                {
                    printf("Buffer is FULL!!\n");
                }
                break;

            case 2:
                if(count > 0)
                {
                    consumer();
                }
                else
                {
                    printf("Buffer is EMPTY!!\n");
                }
                break;

            case 3:
                printf("Buffer contents: ");
                for(int i = 0; i < BUFFER_SIZE; i++)
                {
                    printf("%d ", buffer[i]);
                }
                printf("\n");
                break;

            case 4:
                exit(0);
                break;

            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}
```

### Expected Output
```
=== Circular Buffer Producer-Consumer ===

Buffer Status: count=0, in=0, out=0
Enter the Choice:
1. Producer
2. Consumer
3. Display Buffer
4. Exit
1
Produced item 1 at index 0

Buffer Status: count=1, in=1, out=0
Enter the Choice:
1. Producer
2. Consumer
3. Display Buffer
4. Exit
1
Produced item 2 at index 1

Buffer Status: count=2, in=2, out=0
Enter the Choice:
1. Producer
2. Consumer
3. Display Buffer
4. Exit
2
Consumed item 1 from index 0

Buffer Status: count=1, in=2, out=1
Enter the Choice:
1. Producer
2. Consumer
3. Display Buffer
4. Exit
3
Buffer contents: 1 2 0 0 0
```

---

## TASK 5: Threading Implementation (Without Synchronization)

### Complete Code: task5_threading.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;
int item_number = 0;

// Modified signature for POSIX thread routine
void *producer(void *param)
{
    int num_items = *(int *)param;
    
    for(int i = 0; i < num_items; i++)
    {
        if(count < BUFFER_SIZE)
        {
            item_number++;
            buffer[in] = item_number;
            printf("[PRODUCER] Produced item %d at index %d (count=%d)\n", 
                   item_number, in, count);
            in = (in + 1) % BUFFER_SIZE;
            count++;
        }
        else
        {
            printf("[PRODUCER] Buffer FULL! Waiting...\n");
        }
        sleep(1); // Simulate work
    }
    
    pthread_exit(NULL);
}

void *consumer(void *param)
{
    int num_items = *(int *)param;
    
    for(int i = 0; i < num_items; i++)
    {
        if(count > 0)
        {
            int item = buffer[out];
            printf("[CONSUMER] Consumed item %d from index %d (count=%d)\n", 
                   item, out, count);
            out = (out + 1) % BUFFER_SIZE;
            count--;
        }
        else
        {
            printf("[CONSUMER] Buffer EMPTY! Waiting...\n");
        }
        sleep(1); // Simulate work
    }
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t producer_tid, consumer_tid;
    pthread_attr_t attr;
    int num_items = 10;

    printf("=== Threading without Synchronization ===\n");
    printf("Note: Race conditions may occur!\n\n");

    // Get the default thread attributes
    pthread_attr_init(&attr);

    // Create producer thread
    pthread_create(&producer_tid, &attr, producer, (void *)&num_items);

    // Create consumer thread
    pthread_create(&consumer_tid, &attr, consumer, (void *)&num_items);

    // Wait for threads to complete
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    printf("\n=== Program Completed ===\n");
    printf("Final buffer count: %d\n", count);

    return 0;
}
```

### Problems Highlighted
**Race Condition Issues:**
1. **Multiple threads access `count` simultaneously** → Inconsistent value
2. **Buffer index corruption** → Both threads may access same location
3. **Lost updates** → One thread's modification overwritten by another
4. **Incorrect final count** → Should be 0 (all produced = all consumed), but may not be

### Example Problematic Scenario
```
Thread P: read count=0
Thread C: read count=0
Thread P: write count=1
Thread C: write count=-1  // WRONG! Should be 0
```

---

## TASK 6: Remove Item Function

### Given Insert Item Function
```c
/* Add an item to the buffer */ 
int insert_item(int item) 
{ 
    /* When the buffer is not full add the item 
       and increment the counter*/ 
    if(counter < BUFFER_SIZE) 
    { 
        buffer[counter] = item; 
        counter++; 
        return 0; 
    } 
    else 
    { /* Error the buffer is full */ 
        return -1; 
    } 
}
```

### Remove Item Function (Same Pattern)
```c
/* Remove an item from the buffer */ 
int remove_item(int *item) 
{ 
    /* When the buffer is not empty remove the item 
       and decrement the counter*/ 
    if(counter > 0) 
    { 
        counter--; 
        *item = buffer[counter]; 
        return 0; 
    } 
    else 
    { /* Error the buffer is empty */ 
        return -1; 
    } 
}
```

---

## TASK 6+: Complete Synchronized Producer-Consumer with Semaphores

### Complete Code: task6_synchronized.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define RAND_DIVISOR 100000000
#define TRUE 1

// Buffer structure
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];

// Counter for buffer items
int counter = 0;

// Thread control variables
pthread_t tid;
pthread_attr_t attr;

// Synchronization primitives
pthread_mutex_t mutex;  // Mutex lock
sem_t full;             // Semaphore for full slots (initialized to 0)
sem_t empty;            // Semaphore for empty slots (initialized to BUFFER_SIZE)

// Function prototypes
void *producer(void *param);
void *consumer(void *param);
int insert_item(int item);
int remove_item(int *item);

// Initialize all data structures
void initializeData() 
{
    // Create the mutex lock
    pthread_mutex_init(&mutex, NULL);

    // Create the full semaphore and initialize to 0
    // (0 items in buffer initially)
    sem_init(&full, 0, 0);

    // Create the empty semaphore and initialize to BUFFER_SIZE
    // (all slots empty initially)
    sem_init(&empty, 0, BUFFER_SIZE);

    // Get the default thread attributes
    pthread_attr_init(&attr);

    // Initialize buffer counter
    counter = 0;
    
    printf("=== Data Structures Initialized ===\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Mutex created\n");
    printf("Semaphores initialized\n\n");
}

// Add an item to the buffer
int insert_item(int item) 
{ 
    /* When the buffer is not full add the item 
       and increment the counter*/ 
    if(counter < BUFFER_SIZE) 
    { 
        buffer[counter] = item; 
        counter++; 
        return 0; 
    } 
    else 
    { /* Error the buffer is full */ 
        return -1; 
    } 
}

// Remove an item from the buffer
int remove_item(int *item) 
{ 
    /* When the buffer is not empty remove the item 
       and decrement the counter*/ 
    if(counter > 0) 
    { 
        counter--; 
        *item = buffer[counter]; 
        return 0; 
    } 
    else 
    { /* Error the buffer is empty */ 
        return -1; 
    } 
}

// Producer thread routine
void *producer(void *param)
{
    int num_items = *(int *)param;
    buffer_item item;

    for(int i = 0; i < num_items; i++)
    {
        // Sleep for random time (simulate production)
        sleep(1);

        // Generate item
        item = rand() % RAND_DIVISOR;

        // Acquire empty semaphore (wait if buffer is full)
        sem_wait(&empty);

        // Acquire mutex lock
        pthread_mutex_lock(&mutex);

        // Insert item into buffer
        printf("[PRODUCER %ld] Producing item %d (Buffer: %d/%d)\n", 
               pthread_self(), item, counter, BUFFER_SIZE);
        insert_item(item);

        // Release mutex lock
        pthread_mutex_unlock(&mutex);

        // Signal full semaphore (increment count)
        sem_post(&full);
    }

    printf("[PRODUCER %ld] Completed\n", pthread_self());
    pthread_exit(NULL);
}

// Consumer thread routine
void *consumer(void *param)
{
    int num_items = *(int *)param;
    buffer_item item;

    for(int i = 0; i < num_items; i++)
    {
        // Acquire full semaphore (wait if buffer is empty)
        sem_wait(&full);

        // Acquire mutex lock
        pthread_mutex_lock(&mutex);

        // Remove item from buffer
        remove_item(&item);
        printf("[CONSUMER %ld] Consuming item %d (Buffer: %d/%d)\n", 
               pthread_self(), item, counter, BUFFER_SIZE);

        // Release mutex lock
        pthread_mutex_unlock(&mutex);

        // Signal empty semaphore (increment count)
        sem_post(&empty);

        // Sleep for random time (simulate consumption)
        sleep(1);
    }

    printf("[CONSUMER %ld] Completed\n", pthread_self());
    pthread_exit(NULL);
}

int main()
{
    int num_producers = 2;
    int num_consumers = 2;
    int num_items = 4;  // Each producer produces this many

    // Initialize data structures
    initializeData();

    // Create producer threads
    printf("Creating %d Producer threads...\n", num_producers);
    for(int i = 0; i < num_producers; i++)
    {
        pthread_create(&tid, &attr, producer, (void *)&num_items);
    }

    // Create consumer threads
    printf("Creating %d Consumer threads...\n\n", num_consumers);
    for(int i = 0; i < num_consumers; i++)
    {
        pthread_create(&tid, &attr, consumer, (void *)&num_items);
    }

    // Sleep to allow all threads to complete
    sleep(15);

    printf("\n=== Program Completed ===\n");
    printf("Final buffer counter: %d (should be 0)\n", counter);

    return 0;
}
```

### Compilation and Execution
```bash
gcc -pthread -o synchronized_pc task6_synchronized.c
./synchronized_pc
```

### How Synchronization Works

#### Semaphore Explanation
- **`sem_wait(sem_t *sem)`**: Decrements semaphore. If 0, blocks thread.
- **`sem_post(sem_t *sem)`**: Increments semaphore. Wakes one blocked thread.

#### Synchronization Logic

**Producer:**
1. `sem_wait(&empty)` → Wait until buffer has empty slot
2. `pthread_mutex_lock()` → Lock critical section
3. Insert item, increment counter
4. `pthread_mutex_unlock()` → Release lock
5. `sem_post(&full)` → Signal consumer

**Consumer:**
1. `sem_wait(&full)` → Wait until buffer has item
2. `pthread_mutex_lock()` → Lock critical section
3. Remove item, decrement counter
4. `pthread_mutex_unlock()` → Release lock
5. `sem_post(&empty)` → Signal producer

### Expected Output
```
=== Data Structures Initialized ===
Buffer size: 5
Mutex created
Semaphores initialized

Creating 2 Producer threads...
Creating 2 Consumer threads...

[PRODUCER 140123456789] Producing item 123456 (Buffer: 1/5)
[PRODUCER 140123456790] Producing item 234567 (Buffer: 2/5)
[CONSUMER 140123456791] Consuming item 234567 (Buffer: 1/5)
[CONSUMER 140123456792] Consuming item 123456 (Buffer: 0/5)
[PRODUCER 140123456789] Producing item 345678 (Buffer: 1/5)
[PRODUCER 140123456790] Producing item 456789 (Buffer: 2/5)
[CONSUMER 140123456791] Consuming item 456789 (Buffer: 1/5)
[CONSUMER 140123456792] Consuming item 345678 (Buffer: 0/5)
[PRODUCER 140123456789] Completed
[PRODUCER 140123456790] Completed
[CONSUMER 140123456791] Completed
[CONSUMER 140123456792] Completed

=== Program Completed ===
Final buffer counter: 0 (should be 0)
```

---

## Key Concepts Summary

### Semaphores
| Semaphore | Initial | Purpose |
|-----------|---------|---------|
| `empty` | BUFFER_SIZE | Tracks available empty slots |
| `full` | 0 | Tracks available full slots |

### Mutex
- Ensures only one thread accesses the buffer at a time
- Prevents race conditions in critical section

### Critical Section
- The code between `pthread_mutex_lock()` and `pthread_mutex_unlock()`
- Only one thread can execute this section at a time

### Synchronization Flow
```
Producer                         Consumer
   |                               |
   v                               v
sem_wait(empty) ──────────→ sem_wait(full)
   |                               |
   v                               v
mutex_lock()          mutex_lock()
   |                               |
   v (insert)          v (remove)
mutex_unlock()        mutex_unlock()
   |                               |
   v                               v
sem_post(full)  ──────────→ sem_post(empty)
```

---

## Compilation Commands

```bash
# Task 2: Simple menu-driven
gcc -o task2 task2_producer_consumer.c
./task2

# Task 4: Circular buffer
gcc -o task4 task4_circular_buffer.c
./task4

# Task 5: Threading without sync
gcc -pthread -o task5 task5_threading.c
./task5

# Task 6: With semaphore synchronization
gcc -pthread -o task6 task6_synchronized.c
./task6
```

---

## Conclusion

This lab demonstrates the complete progression from basic producer-consumer to fully synchronized threads using semaphores and mutexes. Proper synchronization ensures data integrity and correct program behavior in concurrent environments.
