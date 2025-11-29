# Operating Systems Concepts Used in EdgeMQ
## Complete Technical Breakdown + Implementation Challenges

---

# 📚 Part 1: OS Concepts & System Calls Used

## 1️⃣ Process Management

### Concepts Used:
- **Process Creation** (`fork()`, `exec()`)
- **Process Termination** (`wait()`, `exit()`)
- **Process IDs** (`getpid()`, `getppid()`)
- **Daemon Processes** (background broker service)

### Where in Project:
```c
// Creating worker processes for handling partitions
pid_t pid = fork();
if (pid == 0) {
    // Child process handles partition 0
    handle_partition(0);
    exit(0);
} else {
    // Parent continues
    wait(NULL);  // Wait for child to finish
}
```

### System Calls:
- `fork()` - Create new process
- `wait()` / `waitpid()` - Wait for child process
- `exec()` family - Execute new program
- `exit()` - Terminate process

---

## 2️⃣ Thread Management (Heavy Usage)

### Concepts Used:
- **POSIX Threads** (`pthread`)
- **Thread Creation & Joining**
- **Thread Pools**
- **Thread-Local Storage**
- **Thread Synchronization**

### Where in Project:
```c
// Week 1: Handle each client in separate thread
pthread_t client_thread;
pthread_create(&client_thread, NULL, handle_client, (void*)client_fd);
pthread_detach(client_thread);

// Week 2: Thread pool for message processing
ThreadPool *pool = threadpool_create(16);  // 16 worker threads
threadpool_add_task(pool, process_message, msg);
```

### System Calls/APIs:
- `pthread_create()` - Create thread
- `pthread_join()` - Wait for thread completion
- `pthread_detach()` - Detach thread
- `pthread_self()` - Get current thread ID
- `pthread_exit()` - Terminate thread

---

## 3️⃣ Synchronization Mechanisms (Critical!)

### A. Mutexes (Mutual Exclusion Locks)

**Concept:** Prevent race conditions when multiple threads access shared data

**Where Used:**
```c
// Protecting topic data structure
pthread_mutex_t topic_mutex;

pthread_mutex_lock(&topic_mutex);
// Critical section: modify topic
topic->message_count++;
pthread_mutex_unlock(&topic_mutex);
```

**System Calls:**
- `pthread_mutex_init()` - Initialize mutex
- `pthread_mutex_lock()` - Acquire lock (blocking)
- `pthread_mutex_trylock()` - Try to acquire lock (non-blocking)
- `pthread_mutex_unlock()` - Release lock
- `pthread_mutex_destroy()` - Destroy mutex

### B. Semaphores

**Concept:** Control access to limited resources

**Where Used:**
```c
// Limit concurrent consumers
sem_t consumer_slots;
sem_init(&consumer_slots, 0, 100);  // Max 100 concurrent consumers

sem_wait(&consumer_slots);     // Decrement (may block)
// ... process consumer ...
sem_post(&consumer_slots);     // Increment
```

**System Calls:**
- `sem_init()` - Initialize semaphore
- `sem_wait()` - Decrement (P operation)
- `sem_post()` - Increment (V operation)
- `sem_destroy()` - Destroy semaphore

### C. Condition Variables

**Concept:** Thread synchronization for waiting on conditions

**Where Used:**
```c
// Wait for new messages
pthread_cond_t message_available;
pthread_mutex_t queue_mutex;

pthread_mutex_lock(&queue_mutex);
while (queue_empty) {
    pthread_cond_wait(&message_available, &queue_mutex);
}
// Process message
pthread_mutex_unlock(&queue_mutex);

// Signal new message
pthread_cond_signal(&message_available);
```

**System Calls:**
- `pthread_cond_init()` - Initialize condition variable
- `pthread_cond_wait()` - Wait for signal
- `pthread_cond_signal()` - Wake one waiting thread
- `pthread_cond_broadcast()` - Wake all waiting threads

### D. Read-Write Locks

**Concept:** Multiple readers, single writer optimization

**Where Used:**
```c
pthread_rwlock_t topic_lock;

// Many consumers can read simultaneously
pthread_rwlock_rdlock(&topic_lock);
read_messages();
pthread_rwlock_unlock(&topic_lock);

// Only one producer can write
pthread_rwlock_wrlock(&topic_lock);
write_message();
pthread_rwlock_unlock(&topic_lock);
```

---

## 4️⃣ File I/O & Management (Heavy Usage)

### Concepts Used:
- **File Descriptors**
- **Buffered vs Unbuffered I/O**
- **File Locking**
- **Memory-Mapped Files** (advanced optimization)
- **Append-Only Logs**

### Where in Project:

#### A. Basic File I/O
```c
// Writing message to partition log
int fd = open("/data/topics/events/partition_0.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
write(fd, message_buffer, message_size);
fsync(fd);  // Force flush to disk
close(fd);
```

**System Calls:**
- `open()` - Open file
- `read()` - Read from file
- `write()` - Write to file
- `close()` - Close file
- `lseek()` - Move file pointer
- `fsync()` - Flush to disk
- `fdatasync()` - Flush data only (faster)

#### B. File Locking
```c
// Prevent concurrent writes
struct flock lock;
lock.l_type = F_WRLCK;    // Write lock
lock.l_whence = SEEK_SET;
lock.l_start = 0;
lock.l_len = 0;           // Lock entire file

fcntl(fd, F_SETLKW, &lock);  // Acquire lock (blocking)
// Write to file
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);   // Release lock
```

**System Calls:**
- `fcntl()` - File control (locking, flags)
- `flock()` - Advisory file locking

#### C. Memory-Mapped Files (Week 2 optimization)
```c
// Map log file to memory for fast access
int fd = open("partition_0.log", O_RDWR);
void *mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

// Direct memory access (much faster than read/write)
memcpy(mapped + offset, message, message_size);

msync(mapped, message_size, MS_SYNC);  // Sync to disk
munmap(mapped, file_size);
```

**System Calls:**
- `mmap()` - Map file to memory
- `munmap()` - Unmap file
- `msync()` - Synchronize memory with disk

#### D. Directory Operations
```c
// Create topic directories
mkdir("/data/topics/events", 0755);
mkdir("/data/topics/events/partition_0", 0755);

// Scan for existing topics on startup
DIR *dir = opendir("/data/topics");
struct dirent *entry;
while ((entry = readdir(dir)) != NULL) {
    // Load topic
}
closedir(dir);
```

**System Calls:**
- `mkdir()` - Create directory
- `opendir()` - Open directory
- `readdir()` - Read directory entry
- `closedir()` - Close directory
- `stat()` / `fstat()` - Get file info

---

## 5️⃣ Inter-Process Communication (IPC)

### Concepts Used:
- **Pipes** (for simple producer-consumer)
- **Named Pipes (FIFOs)**
- **Shared Memory** (System V or POSIX)
- **Message Queues**
- **Sockets** (TCP/IP - main communication)

### Where in Project:

#### A. Pipes (Optional: for internal communication)
```c
int pipefd[2];
pipe(pipefd);

if (fork() == 0) {
    // Child writes
    close(pipefd[0]);
    write(pipefd[1], "message", 7);
    close(pipefd[1]);
} else {
    // Parent reads
    close(pipefd[1]);
    char buf[100];
    read(pipefd[0], buf, 100);
    close(pipefd[0]);
}
```

#### B. Shared Memory (POSIX)
```c
// Create shared memory for inter-broker communication
int shm_fd = shm_open("/edgemq_stats", O_CREAT | O_RDWR, 0666);
ftruncate(shm_fd, sizeof(BrokerStats));

BrokerStats *stats = mmap(NULL, sizeof(BrokerStats), 
                          PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

// Multiple processes can access stats
stats->message_count++;
```

**System Calls:**
- `pipe()` - Create pipe
- `mkfifo()` - Create named pipe
- `shm_open()` - Create/open shared memory (POSIX)
- `shm_unlink()` - Remove shared memory
- `shmget()`, `shmat()`, `shmdt()` - System V shared memory

---

## 6️⃣ Network Programming (Core Component)

### Concepts Used:
- **TCP Sockets** (main broker communication)
- **Socket Options** (SO_REUSEADDR, TCP_NODELAY)
- **Non-blocking I/O**
- **I/O Multiplexing** (`select`, `poll`, `epoll`)
- **Network Byte Order**

### Where in Project:

#### A. Socket Creation & Binding
```c
// Create TCP socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);

// Set socket options
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
setsockopt(server_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));

// Bind to address
struct sockaddr_in address;
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(9092);
bind(server_fd, (struct sockaddr *)&address, sizeof(address));

// Listen for connections
listen(server_fd, 128);  // Backlog of 128

// Accept client
int client_fd = accept(server_fd, NULL, NULL);
```

**System Calls:**
- `socket()` - Create socket
- `bind()` - Bind to address
- `listen()` - Listen for connections
- `accept()` - Accept connection
- `connect()` - Connect to server
- `send()` / `recv()` - Send/receive data
- `shutdown()` - Shutdown connection
- `close()` - Close socket
- `setsockopt()` / `getsockopt()` - Socket options

#### B. I/O Multiplexing with epoll (Week 2)
```c
// Much more efficient than select() for many connections
int epoll_fd = epoll_create1(0);

struct epoll_event ev, events[MAX_EVENTS];
ev.events = EPOLLIN;  // Monitor for input
ev.data.fd = client_fd;
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);

// Wait for events on any socket
while (1) {
    int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++) {
        if (events[i].events & EPOLLIN) {
            // Data available on events[i].data.fd
            handle_client_data(events[i].data.fd);
        }
    }
}
```

**System Calls:**
- `epoll_create()` / `epoll_create1()` - Create epoll instance
- `epoll_ctl()` - Control epoll (add/remove/modify)
- `epoll_wait()` - Wait for events
- `select()` - I/O multiplexing (older, less efficient)
- `poll()` - I/O multiplexing (middle ground)

---

## 7️⃣ Memory Management

### Concepts Used:
- **Dynamic Memory Allocation**
- **Memory Pools**
- **Buffer Management**
- **Memory Leaks Prevention**

### Where in Project:
```c
// Allocate message buffer
Message *msg = malloc(sizeof(Message));
if (msg == NULL) {
    perror("malloc failed");
    return -1;
}

// Use message
strcpy(msg->topic, "events");

// Free when done
free(msg);

// Memory pool for performance (Week 2)
typedef struct {
    void *blocks[1000];
    int free_count;
} MemoryPool;

void* pool_alloc(MemoryPool *pool) {
    if (pool->free_count > 0) {
        return pool->blocks[--pool->free_count];
    }
    return malloc(DEFAULT_SIZE);
}
```

**System Calls:**
- `malloc()` - Allocate memory
- `calloc()` - Allocate and zero memory
- `realloc()` - Resize allocation
- `free()` - Free memory
- `mmap()` - Map memory (also for anonymous mapping)
- `munmap()` - Unmap memory

---

## 8️⃣ Signal Handling

### Concepts Used:
- **Signal Registration**
- **Signal Handlers**
- **Graceful Shutdown**

### Where in Project:
```c
// Handle Ctrl+C gracefully
void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        printf("Shutting down gracefully...\n");
        
        // Flush all logs
        persistence_flush_all();
        
        // Close connections
        broker_stop();
        
        exit(0);
    }
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);  // Ignore broken pipe
    
    // Start broker
}
```

**System Calls:**
- `signal()` - Set signal handler
- `sigaction()` - Advanced signal handling
- `kill()` - Send signal to process
- `raise()` - Send signal to self

---

## 9️⃣ Time Management

### Concepts Used:
- **Timestamps**
- **Timers**
- **Sleep/Delays**
- **Time Measurement**

### Where in Project:
```c
// Add timestamp to message
struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
msg->timestamp = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;  // milliseconds

// Measure operation time
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
// ... operation ...
clock_gettime(CLOCK_MONOTONIC, &end);
long elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + 
                  (end.tv_nsec - start.tv_nsec);

// Sleep
usleep(1000);  // Sleep 1ms
nanosleep(&ts, NULL);  // High precision sleep
```

**System Calls:**
- `time()` - Get current time
- `gettimeofday()` - Get time with microsecond precision
- `clock_gettime()` - Get time (preferred, nanosecond precision)
- `sleep()` - Sleep seconds
- `usleep()` - Sleep microseconds
- `nanosleep()` - Sleep nanoseconds

---

## 🔟 Advanced OS Features (Week 2-4)

### A. Atomic Operations
```c
#include <stdatomic.h>

atomic_int message_count = 0;
atomic_fetch_add(&message_count, 1);  // Thread-safe increment
```

### B. Zero-Copy Transfers
```c
// sendfile() - Direct kernel-to-socket transfer (no userspace copy)
sendfile(socket_fd, file_fd, &offset, count);

// splice() - Move data between file descriptors
splice(file_fd, NULL, socket_fd, NULL, count, SPLICE_F_MOVE);
```

### C. Async I/O (AIO)
```c
struct aiocb cb;
memset(&cb, 0, sizeof(cb));
cb.aio_fildes = fd;
cb.aio_buf = buffer;
cb.aio_nbytes = size;

aio_write(&cb);  // Non-blocking write

// Check later
while (aio_error(&cb) == EINPROGRESS) {
    // Do other work
}
```

---

# 🔥 Part 2: Implementation Challenges & Solutions

## Challenge 1: Race Conditions

### Problem:
```c
// WRONG: Race condition!
if (topic->message_count < MAX_MESSAGES) {
    // Thread A gets here, then Thread B gets here
    topic->messages[topic->message_count] = msg;  // Both write to same index!
    topic->message_count++;
}
```

### Solution:
```c
// CORRECT: Use mutex
pthread_mutex_lock(&topic->mutex);
if (topic->message_count < MAX_MESSAGES) {
    topic->messages[topic->message_count] = msg;
    topic->message_count++;
}
pthread_mutex_unlock(&topic->mutex);
```

### Impact:
- **Symptoms:** Corrupt data, crashes, lost messages
- **Debug Time:** 2-4 hours finding the bug
- **Prevention:** Always protect shared data with locks

---

## Challenge 2: Deadlocks

### Problem:
```c
// Thread 1
pthread_mutex_lock(&mutex_A);
pthread_mutex_lock(&mutex_B);  // Waits for Thread 2
// ...

// Thread 2
pthread_mutex_lock(&mutex_B);
pthread_mutex_lock(&mutex_A);  // Waits for Thread 1 → DEADLOCK!
```

### Solution:
```c
// ALWAYS acquire locks in same order
// Both threads do:
pthread_mutex_lock(&mutex_A);
pthread_mutex_lock(&mutex_B);
// ...
pthread_mutex_unlock(&mutex_B);
pthread_mutex_unlock(&mutex_A);
```

### Detection:
```bash
# Use deadlock detection tools
valgrind --tool=helgrind ./edgemq-broker
```

---

## Challenge 3: Memory Leaks

### Problem:
```c
void handle_message() {
    Message *msg = malloc(sizeof(Message));
    
    if (error_condition) {
        return;  // LEAK! Forgot to free
    }
    
    process(msg);
    free(msg);  // Only freed in success case
}
```

### Solution:
```c
void handle_message() {
    Message *msg = malloc(sizeof(Message));
    if (msg == NULL) return;
    
    int result = process(msg);
    
    // Always free before returning
    free(msg);
    return result;
}

// Or use cleanup attribute (GCC)
void cleanup(Message **msg) {
    if (*msg) free(*msg);
}

void handle_message() {
    Message *msg __attribute__((cleanup(cleanup))) = malloc(sizeof(Message));
    // Automatically freed when going out of scope
}
```

### Detection:
```bash
valgrind --leak-check=full ./edgemq-broker
```

---

## Challenge 4: File Descriptor Exhaustion

### Problem:
```c
while (1) {
    int fd = open("log.txt", O_WRONLY | O_APPEND);
    write(fd, data, size);
    // Forgot to close! Eventually runs out of FDs
}
```

### Symptoms:
```
accept(): Too many open files
```

### Solution:
```c
// Always close file descriptors
int fd = open("log.txt", O_WRONLY | O_APPEND);
write(fd, data, size);
close(fd);  // Critical!

// Or use RAII pattern with macros
#define SCOPED_FD(name, ...) \
    int name __attribute__((cleanup(close_fd))) = open(__VA_ARGS__)

void close_fd(int *fd) { if (*fd >= 0) close(*fd); }

// Usage
SCOPED_FD(fd, "log.txt", O_WRONLY);  // Auto-closes
```

### Check Limits:
```bash
ulimit -n      # See max FDs
lsof -p $PID   # List open files
```

---

## Challenge 5: Buffer Overflows

### Problem:
```c
char buffer[64];
strcpy(buffer, client_input);  // Client sends 1000 bytes → OVERFLOW!
```

### Solution:
```c
char buffer[64];
strncpy(buffer, client_input, sizeof(buffer) - 1);
buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

// Better: use safe functions
strlcpy(buffer, client_input, sizeof(buffer));  // BSD
snprintf(buffer, sizeof(buffer), "%s", client_input);  // Standard
```

---

## Challenge 6: Thundering Herd

### Problem:
```c
// 100 threads waiting on condition variable
pthread_cond_broadcast(&new_message);  // ALL wake up!
// But only 1 can process → 99 wake up for nothing
```

### Solution:
```c
// Use signal instead of broadcast (wake 1 thread)
pthread_cond_signal(&new_message);

// Or use semaphores
sem_post(&message_available);  // Only wakes 1 thread
```

---

## Challenge 7: Network Byte Order

### Problem:
```c
// Sending on Intel (little-endian), receiving on ARM (could be big-endian)
uint32_t value = 12345;
write(socket, &value, 4);  // Wrong on different architectures!
```

### Solution:
```c
#include <arpa/inet.h>

// Always convert to network byte order
uint32_t value = 12345;
uint32_t network_value = htonl(value);  // Host to network long
write(socket, &network_value, 4);

// Receiver converts back
uint32_t received;
read(socket, &received, 4);
uint32_t host_value = ntohl(received);  // Network to host long
```

---

## Challenge 8: Incomplete Reads/Writes

### Problem:
```c
char buffer[1000];
int n = read(socket, buffer, 1000);
// Assumes all 1000 bytes read → WRONG!
// TCP may split into multiple packets
```

### Solution:
```c
// Read until complete
ssize_t read_full(int fd, void *buf, size_t count) {
    size_t total = 0;
    while (total < count) {
        ssize_t n = read(fd, (char*)buf + total, count - total);
        if (n <= 0) {
            if (n == 0) return total;  // EOF
            if (errno == EINTR) continue;  // Interrupted, retry
            return -1;  // Error
        }
        total += n;
    }
    return total;
}
```

---

## Challenge 9: Signal Interruption

### Problem:
```c
int n = read(fd, buffer, size);
// If signal arrives during read, n == -1 and errno == EINTR
if (n < 0) {
    // Crash! Didn't check for EINTR
    perror("read failed");
}
```

### Solution:
```c
int n;
do {
    n = read(fd, buffer, size);
} while (n < 0 && errno == EINTR);  // Retry on interruption

if (n < 0) {
    perror("read failed");
}
```

---

## Challenge 10: Port Already in Use

### Problem:
```bash
./edgemq-broker
Error: Address already in use

# Broker crashed, OS holds port for 60 seconds (TIME_WAIT)
```

### Solution:
```c
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
// Now can immediately rebind after crash
```

---

## Challenge 11: Disk Full

### Problem:
```c
write(fd, data, size);  // Disk full → silent failure!
```

### Solution:
```c
ssize_t n = write(fd, data, size);
if (n != size) {
    if (errno == ENOSPC) {
        fprintf(stderr, "ERROR: Disk full!\n");
        // Graceful degradation: stop accepting writes
    }
}
```

---

## Challenge 12: Thread Safety of Functions

### Problem:
```c
// These are NOT thread-safe:
char *result = strtok(str, ",");  // Uses static buffer
struct tm *t = localtime(&now);   // Uses static buffer

// Multiple threads → data corruption
```

### Solution:
```c
// Use reentrant versions
char *result;
strtok_r(str, ",", &saveptr);  // Thread-safe

struct tm t;
localtime_r(&now, &t);  // Thread-safe
```

---

## Challenge 13: Performance - Context Switching

### Problem:
```c
// 10,000 threads → excessive context switching → slow
for (int i = 0; i < 10000; i++) {
    pthread_create(&threads[i], NULL, worker, NULL);
}
```

### Solution:
```c
// Use thread pool (fixed number of threads)
ThreadPool *pool = threadpool_create(16);  // Optimal: # of CPU cores

for (int i = 0; i < 10000; i++) {
    threadpool_add_task(pool, worker, tasks[i]);
}
```

---

## Challenge 14: Endianness in Binary Protocol

### Problem:
```c
struct Message {
    uint32_t id;
    uint64_t timestamp;
};

// Sending struct directly → breaks on different architectures
write(fd, &msg, sizeof(msg));
```

### Solution:
```c
// Serialize manually
void serialize_message(Message *msg, char *buffer) {
    uint32_t id_network = htonl(msg->id);
    uint64_t ts_network = htobe64(msg->timestamp);
    
    memcpy(buffer, &id_network, 4);
    memcpy(buffer + 4, &ts_network, 8);
}

// Or use protobuf/flatbuffers
```

---

## Challenge 15: Zombie Processes

### Problem:
```c
fork();
// Parent doesn't call wait() → child becomes zombie
```

### Solution:
```c
// Option 1: Wait for children
pid_t pid = fork();
if (pid > 0) {
    waitpid(pid, NULL, 0);
}

// Option 2: Ignore SIGCHLD
signal(SIGCHLD, SIG_IGN);  // Auto-reap zombies

// Option 3: Non-blocking wait
waitpid(-1, NULL, WNOHANG);  // Reap any zombie
```

---

# 📊 Summary: OS Skills Gained

## Core OS Concepts:
✅ Process & Thread Management  
✅ Synchronization (Mutexes, Semaphores, Condition Variables)  
✅ File I/O & Management  
✅ Inter-Process Communication  
✅ Network Programming  
✅ Memory Management  
✅ Signal Handling  
✅ I/O Multiplexing (epoll)  

## System Calls Mastered:
✅ 50+ system calls used  
✅ POSIX API expertise  
✅ Linux-specific optimizations  

## Debugging Skills:
✅ Valgrind (memory leaks)  
✅ Helgrind (race conditions)  
✅ GDB (debugging)  
✅ strace (system call tracing)  
✅ lsof (file descriptor tracking)  

## Performance Skills:
✅ Lock-free programming basics  
✅ Zero-copy transfers  
✅ Memory pools  
✅ Thread pools  
✅ Async I/O  

---

# 🎯 Top 10 Most Common Bugs You'll Face

1. **Forgetting to unlock mutex** → Deadlock
2. **Not checking return values** → Silent failures
3. **Buffer overflows** → Crashes
4. **Memory leaks** → OOM after hours
5. **Race conditions** → Intermittent bugs
6. **Incomplete network reads** → Protocol errors
7. **Byte order issues** → Data corruption
8. **File descriptor leaks** → "Too many open files"
9. **Signal handling during syscalls** → EINTR errors
10. **Thread-unsafe function usage** → Random crashes

---

# 💡 Pro Tips for Success

## 1. Always Check Return Values
```c
// BAD
malloc(size);
open("file.txt", O_RDWR);

// GOOD
if (malloc(size) == NULL) { /* handle */ }
if (open("file.txt", O_RDWR) < 0) { perror("open"); }
```

## 2. Use Valgrind Religiously
```bash
valgrind --leak-check=full --track-origins=yes ./edgemq-broker
```

## 3. Enable Compiler Warnings
```makefile
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g
```

## 4. Use strace for Debugging
```bash
strace -e trace=open,read,write ./edgemq-broker
```

## 5. Test with Stress Tools
```bash
# 1000 concurrent connections
ab -n 100000 -c 1000 http://localhost:9092/
```

---

**This is real systems programming!** 🔥

Every challenge you overcome makes you a better engineer. Good luck! 💪
