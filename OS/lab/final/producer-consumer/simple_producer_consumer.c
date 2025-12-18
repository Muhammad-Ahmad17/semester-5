#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Buffer configuration
#define BUFFER_SIZE 5

// Shared buffer
int buffer[BUFFER_SIZE];
int buffer_index = 0;

// Synchronization primitives
pthread_mutex_t mutex;     // Protects critical section
sem_t empty;               // Counts empty slots (starts at BUFFER_SIZE)
sem_t full;                // Counts full slots (starts at 0)

// Producer function
void *producer(void *arg) {
    int item = *(int *)arg;
    
    // Wait for empty slot
    sem_wait(&empty);
    
    // Lock the mutex
    pthread_mutex_lock(&mutex);
    
    // Add item to buffer
    buffer[buffer_index] = item;
    printf("[PRODUCER] Produced item: %d at index %d\n", item, buffer_index);
    buffer_index++;
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    
    // Signal that buffer has item
    sem_post(&full);
    
    return NULL;
}

// Consumer function
void *consumer(void *arg) {
    int item;
    
    // Wait for full slot
    sem_wait(&full);
    
    // Lock the mutex
    pthread_mutex_lock(&mutex);
    
    // Remove item from buffer
    buffer_index--;
    item = buffer[buffer_index];
    printf("[CONSUMER] Consumed item: %d from index %d\n", item, buffer_index);
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    
    // Signal that buffer has empty slot
    sem_post(&empty);
    
    return NULL;
}

int main() {
    pthread_t tid[6];  // Thread IDs
    int items[3] = {10, 20, 30};
    
    printf("========================================\n");
    printf("Producer-Consumer Using Threads & Semaphore\n");
    printf("========================================\n");
    printf("Buffer Size: %d\n", BUFFER_SIZE);
    printf("Mutex: Protects buffer access\n");
    printf("Semaphore 'empty': %d (empty slots)\n", BUFFER_SIZE);
    printf("Semaphore 'full': 0 (full slots)\n");
    printf("========================================\n\n");
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Initialize semaphores
    // empty: starts at BUFFER_SIZE (all slots empty)
    // full: starts at 0 (no items initially)
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    
    // Create 3 producer threads
    printf("Creating 3 PRODUCER threads...\n");
    for(int i = 0; i < 3; i++) {
        pthread_create(&tid[i], NULL, producer, &items[i]);
    }
    
    // Small delay to see producers work first
    sleep(1);
    
    // Create 3 consumer threads
    printf("\nCreating 3 CONSUMER threads...\n\n");
    for(int i = 0; i < 3; i++) {
        pthread_create(&tid[3 + i], NULL, consumer, NULL);
    }
    
    // Wait for all threads to complete
    for(int i = 0; i < 6; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printf("\n========================================\n");
    printf("All threads completed successfully!\n");
    printf("========================================\n");
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    
    return 0;
}
