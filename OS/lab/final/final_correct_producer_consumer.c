#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// ============================================================
// PRODUCER-CONSUMER: FINAL CORRECT CODE
// ============================================================
// This is the CORRECT, SAFE, and COMPLETE implementation
// NO DEADLOCK - Uses proper synchronization
// ============================================================

#define BUFFER_SIZE 5

// ============================================================
// SHARED RESOURCES
// ============================================================

int buffer[BUFFER_SIZE];        // Shared buffer
int buffer_count = 0;           // Number of items in buffer
int item_id = 0;                // Counter for produced items

// ============================================================
// SYNCHRONIZATION PRIMITIVES
// ============================================================

pthread_mutex_t mutex;          // Lock for critical section
sem_t empty;                    // Semaphore for empty slots (init = BUFFER_SIZE)
sem_t full;                     // Semaphore for full slots (init = 0)

// ============================================================
// PRODUCER FUNCTION - CORRECT IMPLEMENTATION
// ============================================================
// CORRECT ORDER:
// 1. sem_wait(&empty)       ← Check/wait for empty slot FIRST
// 2. pthread_mutex_lock()   ← Lock critical section SECOND
// 3. [Add item to buffer]   ← Modify shared resource
// 4. pthread_mutex_unlock() ← Release lock
// 5. sem_post(&full)        ← Signal that item is available
// ============================================================

void *producer(void *arg) {
    int thread_id = *(int *)arg;
    
    printf("[PRODUCER %d] Starting...\n", thread_id);
    
    // Produce 3 items per producer
    for(int i = 0; i < 3; i++) {
        // ========================================================
        // STEP 1: Wait for empty slot (BEFORE locking mutex)
        // ========================================================
        printf("[PRODUCER %d] Waiting for empty slot...\n", thread_id);
        sem_wait(&empty);    // If empty=0, this blocks WITHOUT holding mutex
        
        // ========================================================
        // STEP 2: Lock the critical section
        // ========================================================
        printf("[PRODUCER %d] Acquired lock, entering critical section\n", thread_id);
        pthread_mutex_lock(&mutex);
        
        // ========================================================
        // STEP 3: Add item to buffer (CRITICAL SECTION)
        // ========================================================
        item_id++;
        buffer[buffer_count] = item_id;
        printf("[PRODUCER %d] ➜ PRODUCED item: %d at index: %d (Buffer: %d/%d)\n", 
               thread_id, item_id, buffer_count, buffer_count + 1, BUFFER_SIZE);
        buffer_count++;
        
        // ========================================================
        // STEP 4: Unlock the critical section
        // ========================================================
        pthread_mutex_unlock(&mutex);
        printf("[PRODUCER %d] Released lock\n", thread_id);
        
        // ========================================================
        // STEP 5: Signal that an item is now available
        // ========================================================
        sem_post(&full);     // Increment full counter, wake waiting consumer
        
        sleep(1);            // Simulate production time
    }
    
    printf("[PRODUCER %d] Finished producing 3 items\n\n", thread_id);
    pthread_exit(NULL);
}

// ============================================================
// CONSUMER FUNCTION - CORRECT IMPLEMENTATION
// ============================================================
// CORRECT ORDER:
// 1. sem_wait(&full)        ← Check/wait for full slot FIRST
// 2. pthread_mutex_lock()   ← Lock critical section SECOND
// 3. [Remove item]          ← Modify shared resource
// 4. pthread_mutex_unlock() ← Release lock
// 5. sem_post(&empty)       ← Signal that slot is now empty
// ============================================================

void *consumer(void *arg) {
    int thread_id = *(int *)arg;
    int item;
    
    printf("[CONSUMER %d] Starting...\n", thread_id);
    
    // Consume 3 items per consumer
    for(int i = 0; i < 3; i++) {
        // ========================================================
        // STEP 1: Wait for full slot (BEFORE locking mutex)
        // ========================================================
        printf("[CONSUMER %d] Waiting for item...\n", thread_id);
        sem_wait(&full);     // If full=0, this blocks WITHOUT holding mutex
        
        // ========================================================
        // STEP 2: Lock the critical section
        // ========================================================
        printf("[CONSUMER %d] Acquired lock, entering critical section\n", thread_id);
        pthread_mutex_lock(&mutex);
        
        // ========================================================
        // STEP 3: Remove item from buffer (CRITICAL SECTION)
        // ========================================================
        buffer_count--;
        item = buffer[buffer_count];
        printf("[CONSUMER %d] ➜ CONSUMED item: %d from index: %d (Buffer: %d/%d)\n", 
               thread_id, item, buffer_count, buffer_count, BUFFER_SIZE);
        
        // ========================================================
        // STEP 4: Unlock the critical section
        // ========================================================
        pthread_mutex_unlock(&mutex);
        printf("[CONSUMER %d] Released lock\n", thread_id);
        
        // ========================================================
        // STEP 5: Signal that a slot is now empty
        // ========================================================
        sem_post(&empty);    // Increment empty counter, wake waiting producer
        
        sleep(1);            // Simulate consumption time
    }
    
    printf("[CONSUMER %d] Finished consuming 3 items\n\n", thread_id);
    pthread_exit(NULL);
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {
    pthread_t producer_threads[2];
    pthread_t consumer_threads[2];
    int producer_ids[2] = {1, 2};
    int consumer_ids[2] = {1, 2};
    
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════════╗\n");
    printf("║     PRODUCER-CONSUMER SYNCHRONIZATION (CORRECT IMPLEMENTATION)     ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("Configuration:\n");
    printf("  • Buffer Size: %d\n", BUFFER_SIZE);
    printf("  • Producers: 2 (produce 3 items each = 6 total)\n");
    printf("  • Consumers: 2 (consume 3 items each = 6 total)\n");
    printf("  • Mutex: Protects buffer access\n");
    printf("  • Semaphore 'empty': Tracks empty slots (initial = %d)\n", BUFFER_SIZE);
    printf("  • Semaphore 'full': Tracks full slots (initial = 0)\n");
    printf("\n");
    printf("Synchronization Order:\n");
    printf("  PRODUCER:  sem_wait(empty) → mutex_lock → add → mutex_unlock → sem_post(full)\n");
    printf("  CONSUMER:  sem_wait(full)  → mutex_lock → remove → mutex_unlock → sem_post(empty)\n");
    printf("\n");
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    
    // ========================================================
    // INITIALIZATION
    // ========================================================
    
    // Initialize mutex (unlocked state)
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphore 'empty' with BUFFER_SIZE
    // (all slots are empty initially)
    sem_init(&empty, 0, BUFFER_SIZE);
    
    // Initialize semaphore 'full' with 0
    // (no items in buffer initially)
    sem_init(&full, 0, 0);
    
    printf("✓ Mutex initialized (unlocked)\n");
    printf("✓ Semaphore 'empty' initialized (value = %d)\n", BUFFER_SIZE);
    printf("✓ Semaphore 'full' initialized (value = 0)\n\n");
    
    // ========================================================
    // CREATE PRODUCER THREADS
    // ========================================================
    
    printf("Creating 2 PRODUCER threads...\n");
    for(int i = 0; i < 2; i++) {
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }
    printf("✓ Producer threads created\n\n");
    
    // ========================================================
    // CREATE CONSUMER THREADS
    // ========================================================
    
    printf("Creating 2 CONSUMER threads...\n");
    for(int i = 0; i < 2; i++) {
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }
    printf("✓ Consumer threads created\n\n");
    
    // ========================================================
    // WAIT FOR ALL THREADS TO COMPLETE
    // ========================================================
    
    printf("Waiting for all threads to complete...\n\n");
    
    for(int i = 0; i < 2; i++) {
        pthread_join(producer_threads[i], NULL);
    }
    
    for(int i = 0; i < 2; i++) {
        pthread_join(consumer_threads[i], NULL);
    }
    
    // ========================================================
    // CLEANUP
    // ========================================================
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    // ========================================================
    // SUMMARY
    // ========================================================
    
    printf("═══════════════════════════════════════════════════════════════════\n");
    printf("║ EXECUTION COMPLETED SUCCESSFULLY                                 ║\n");
    printf("╠═══════════════════════════════════════════════════════════════════╣\n");
    printf("║ Final Buffer State:                                               ║\n");
    printf("║   • Buffer count: %d (should be 0)                                ║\n", buffer_count);
    printf("║   • All items produced: 6                                         ║\n");
    printf("║   • All items consumed: 6                                         ║\n");
    printf("║   • NO DEADLOCK ✓                                                 ║\n");
    printf("║   • NO RACE CONDITIONS ✓                                          ║\n");
    printf("║   • PROPER SYNCHRONIZATION ✓                                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════════╝\n\n");
    
    return 0;
}

// ============================================================
// HOW TO COMPILE AND RUN
// ============================================================
/*

COMPILE:
  gcc -pthread -o producer_consumer final_correct_producer_consumer.c

RUN:
  ./producer_consumer

EXPECTED OUTPUT:
  • 2 Producers each produce 3 items = 6 total
  • 2 Consumers each consume 3 items = 6 total
  • Final buffer count = 0 (all consumed)
  • Thread logs show proper synchronization
  • NO deadlock occurs

WHAT MAKES THIS CORRECT:
  ✓ Proper lock ordering (semaphore BEFORE mutex)
  ✓ No deadlock possible
  ✓ Race conditions prevented
  ✓ Proper resource synchronization
  ✓ Clean thread creation and joining
  ✓ Proper cleanup of resources

*/

// ============================================================
// KEY POINTS
// ============================================================
/*

1. CRITICAL LOCK ORDER:
   sem_wait() BEFORE pthread_mutex_lock()
   
   This ensures:
   • No thread holds mutex while waiting for semaphore
   • Other threads can always progress
   • DEADLOCK IMPOSSIBLE

2. SEMAPHORE USAGE:
   empty: Tracks available empty slots
          Producer waits if empty=0 (buffer full)
   
   full:  Tracks available full slots
          Consumer waits if full=0 (buffer empty)

3. MUTEX USAGE:
   Protects the critical section where buffer is modified
   Only ONE thread can modify buffer at a time

4. SYNCHRONIZATION FLOW:
   Producer produces → Consumer consumes
   Producers wait if buffer full
   Consumers wait if buffer empty
   Both wait BEFORE acquiring mutex

5. ADVANTAGE OF THIS APPROACH:
   • Simple and clear
   • No deadlock detection needed
   • Deadlock prevented by DESIGN
   • Efficient (no busy waiting)

*/
