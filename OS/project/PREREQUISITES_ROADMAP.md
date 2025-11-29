# Complete Prerequisites Roadmap for EdgeMQ Project
## What to Learn Before Starting + Learning Resources

---

# 🎯 Learning Structure

```
FOUNDATION (Week -2 to -1)
    ↓
CORE CONCEPTS (Parallel with Project Week 1)
    ↓
ADVANCED TOPICS (As needed in Weeks 2-4)
```

---

# 📚 PART 1: FOUNDATION (Before Starting Project)

## 1️⃣ C Programming Fundamentals

### 1.1 Basic Syntax & Types
**What to Learn:**
- Variables, data types (`int`, `char`, `float`, `double`)
- Operators (arithmetic, logical, bitwise)
- Control flow (`if`, `switch`, `for`, `while`)
- Functions (declaration, definition, calling)
- Arrays and strings

**Practice Exercise:**
```c
// Write a program to reverse a string
#include <stdio.h>
#include <string.h>

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main() {
    char str[] = "hello";
    reverse_string(str);
    printf("%s\n", str);  // Output: olleh
    return 0;
}
```

**Resources:**
- Book: "C Programming Language" by K&R (Chapters 1-4)
- Online: [Learn-C.org](https://www.learn-c.org/) Interactive Tutorial
- Video: CS50 Harvard - Week 1 & 2

**Time Needed:** 3-5 days if new to C, 1 day if reviewing

---

### 1.2 Pointers & Memory

**What to Learn:**
- What is a pointer?
- Pointer arithmetic
- Pointers to pointers (`**`)
- Function pointers
- `NULL` pointers
- Common pointer mistakes

**Practice Exercise:**
```c
// Understand pointer basics
#include <stdio.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 10, y = 20;
    printf("Before: x=%d, y=%d\n", x, y);
    
    swap(&x, &y);  // Pass addresses
    
    printf("After: x=%d, y=%d\n", x, y);
    return 0;
}
```

**Critical Concept - Pointer to Array:**
```c
int arr[5] = {1, 2, 3, 4, 5};
int *ptr = arr;  // Points to first element

printf("%d\n", *(ptr + 2));  // Access arr[2]
printf("%d\n", ptr[3]);      // Same as *(ptr + 3)
```

**Resources:**
- Book: "Pointers in C" by Yashavant Kanetkar
- Video: [Pointers in C - freeCodeCamp](https://www.youtube.com/watch?v=XISnO2YhnsY)
- Practice: [HackerRank C - Pointers](https://www.hackerrank.com/domains/c)

**Time Needed:** 2-3 days

---

### 1.3 Dynamic Memory Allocation

**What to Learn:**
- Stack vs Heap memory
- `malloc()`, `calloc()`, `realloc()`, `free()`
- Memory leaks and how to avoid them
- Valgrind basics

**Practice Exercise:**
```c
// Dynamic array
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    printf("Enter size: ");
    scanf("%d", &n);
    
    // Allocate array dynamically
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Use array
    for (int i = 0; i < n; i++) {
        arr[i] = i * 10;
    }
    
    // Print
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    
    // CRITICAL: Free memory
    free(arr);
    
    return 0;
}
```

**Common Mistakes to Avoid:**
```c
// WRONG: Use after free
int *ptr = malloc(100);
free(ptr);
*ptr = 5;  // ❌ Undefined behavior!

// WRONG: Double free
free(ptr);
free(ptr);  // ❌ Crash!

// WRONG: Memory leak
ptr = malloc(100);
ptr = malloc(200);  // ❌ Lost first allocation!

// CORRECT: Set to NULL after free
free(ptr);
ptr = NULL;  // ✅ Prevents use-after-free
```

**Resources:**
- Tutorial: [Dynamic Memory - GeeksforGeeks](https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/)
- Tool: `valgrind --leak-check=full ./program`

**Time Needed:** 2 days

---

### 1.4 Structures & Unions

**What to Learn:**
- Defining structures
- Accessing members (`.` and `->`)
- Nested structures
- `typedef` for cleaner code
- Structure padding and packing
- Unions (for protocol design)

**Practice Exercise:**
```c
// Message structure (like in EdgeMQ)
#include <stdio.h>
#include <string.h>

typedef struct {
    char topic[64];
    int partition;
    char key[128];
    char value[1024];
    unsigned long offset;
    unsigned long timestamp;
} Message;

void print_message(Message *msg) {
    printf("Topic: %s\n", msg->topic);
    printf("Partition: %d\n", msg->partition);
    printf("Offset: %lu\n", msg->offset);
    printf("Value: %s\n", msg->value);
}

int main() {
    Message msg;
    strcpy(msg.topic, "events");
    msg.partition = 0;
    strcpy(msg.value, "Hello World");
    msg.offset = 12345;
    
    print_message(&msg);
    
    return 0;
}
```

**Advanced - Packed Structures:**
```c
// For network protocols (no padding)
typedef struct __attribute__((packed)) {
    uint8_t version;
    uint8_t command;
    uint16_t flags;
    uint32_t length;
} ProtocolHeader;

printf("Size: %zu\n", sizeof(ProtocolHeader));  // Exactly 8 bytes
```

**Resources:**
- Tutorial: [Structures in C - Programiz](https://www.programiz.com/c-programming/c-structures)
- Advanced: "Expert C Programming" - Chapter on structures

**Time Needed:** 1-2 days

---

### 1.5 File I/O

**What to Learn:**
- `fopen()`, `fclose()`, `fread()`, `fwrite()`
- `fprintf()`, `fscanf()`
- Binary vs Text files
- File positioning (`fseek()`, `ftell()`)
- Low-level I/O: `open()`, `read()`, `write()`, `close()`

**Practice Exercise:**
```c
// Write and read binary file
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

int main() {
    // Write
    FILE *fp = fopen("employees.dat", "wb");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    
    Employee emp = {1, "John Doe", 50000.0};
    fwrite(&emp, sizeof(Employee), 1, fp);
    fclose(fp);
    
    // Read
    fp = fopen("employees.dat", "rb");
    Employee read_emp;
    fread(&read_emp, sizeof(Employee), 1, fp);
    fclose(fp);
    
    printf("ID: %d, Name: %s, Salary: %.2f\n", 
           read_emp.id, read_emp.name, read_emp.salary);
    
    return 0;
}
```

**Low-Level I/O (POSIX):**
```c
#include <fcntl.h>
#include <unistd.h>

int fd = open("data.txt", O_RDWR | O_CREAT, 0644);
char buffer[100];
ssize_t n = read(fd, buffer, 100);
write(fd, "hello", 5);
close(fd);
```

**Resources:**
- Tutorial: [File I/O in C - Tutorialspoint](https://www.tutorialspoint.com/cprogramming/c_file_io.htm)
- Man pages: `man 2 open`, `man 2 read`, `man 2 write`

**Time Needed:** 2 days

---

### 1.6 Preprocessor & Build System

**What to Learn:**
- `#include`, `#define`, `#ifdef`
- Header guards
- Makefiles basics
- Compilation process (preprocessing, compiling, linking)
- GCC flags

**Practice Exercise:**

**message.h:**
```c
#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct {
    char topic[64];
    char value[256];
} Message;

void print_message(Message *msg);

#endif
```

**message.c:**
```c
#include <stdio.h>
#include "message.h"

void print_message(Message *msg) {
    printf("Topic: %s, Value: %s\n", msg->topic, msg->value);
}
```

**main.c:**
```c
#include "message.h"

int main() {
    Message msg = {"events", "hello"};
    print_message(&msg);
    return 0;
}
```

**Makefile:**
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -g

all: program

program: main.o message.o
	$(CC) $(CFLAGS) -o program main.o message.o

main.o: main.c message.h
	$(CC) $(CFLAGS) -c main.c

message.o: message.c message.h
	$(CC) $(CFLAGS) -c message.c

clean:
	rm -f *.o program
```

**Resources:**
- Tutorial: [Makefile Tutorial - CS Colby](https://cs.colby.edu/maxwell/courses/tutorials/maketutor/)
- Book: "Managing Projects with GNU Make"

**Time Needed:** 1 day

---

## 2️⃣ Linux System Programming Basics

### 2.1 File Descriptors & System Calls

**What to Learn:**
- What is a file descriptor?
- STDIN (0), STDOUT (1), STDERR (2)
- `open()`, `read()`, `write()`, `close()`
- Error handling (`errno`, `perror()`)

**Practice Exercise:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    const char *msg = "Hello, File Descriptors!\n";
    ssize_t bytes_written = write(fd, msg, strlen(msg));
    if (bytes_written < 0) {
        perror("write");
        close(fd);
        return 1;
    }
    
    printf("Wrote %zd bytes\n", bytes_written);
    
    close(fd);
    return 0;
}
```

**Resources:**
- Book: "The Linux Programming Interface" by Michael Kerrisk - Chapter 4
- Man pages: `man 2 open`, `man 2 read`

**Time Needed:** 2 days

---

### 2.2 Processes

**What to Learn:**
- Process creation (`fork()`)
- Process termination (`exit()`)
- Waiting for children (`wait()`, `waitpid()`)
- Process IDs (`getpid()`, `getppid()`)
- Zombie and orphan processes
- `exec()` family

**Practice Exercise:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent PID: %d\n", getpid());
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child PID: %d, Parent: %d\n", getpid(), getppid());
        sleep(2);
        printf("Child exiting\n");
        exit(0);
    } else {
        // Parent process
        printf("Parent created child with PID: %d\n", pid);
        
        int status;
        wait(&status);  // Wait for child
        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
    }
    
    return 0;
}
```

**Advanced - exec() example:**
```c
pid_t pid = fork();
if (pid == 0) {
    // Replace child process with 'ls' command
    execlp("ls", "ls", "-la", NULL);
    // If exec succeeds, code below never runs
    perror("execlp");
    exit(1);
}
```

**Resources:**
- Book: "Advanced Programming in the UNIX Environment" (APUE) - Chapter 8
- Video: [Fork() System Call - Neso Academy](https://www.youtube.com/watch?v=cex9XrZCU14)

**Time Needed:** 2-3 days

---

### 2.3 Signals

**What to Learn:**
- What are signals?
- Common signals (SIGINT, SIGTERM, SIGKILL, SIGCHLD)
- Signal handlers (`signal()`, `sigaction()`)
- Sending signals (`kill()`, `raise()`)
- Signal safety

**Practice Exercise:**
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int sig) {
    printf("\nCaught signal %d (Ctrl+C)\n", sig);
    printf("Cleaning up...\n");
    exit(0);
}

int main() {
    // Register signal handler
    signal(SIGINT, handle_sigint);
    
    printf("Press Ctrl+C to trigger signal handler\n");
    printf("PID: %d\n", getpid());
    
    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    
    return 0;
}
```

**Resources:**
- Book: "The Linux Programming Interface" - Chapter 20
- Tutorial: [Signal Handling in C - GeeksforGeeks](https://www.geeksforgeeks.org/signals-c-language/)

**Time Needed:** 1-2 days

---

## 3️⃣ Multithreading Fundamentals

### 3.1 Thread Basics (POSIX Threads)

**What to Learn:**
- Thread vs Process
- Creating threads (`pthread_create()`)
- Joining threads (`pthread_join()`)
- Detaching threads (`pthread_detach()`)
- Thread IDs

**Practice Exercise:**
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void *arg) {
    int id = *(int*)arg;
    
    printf("Thread %d: Starting\n", id);
    sleep(2);
    printf("Thread %d: Finishing\n", id);
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};
    
    // Create threads
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &ids[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    
    // Wait for all threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads completed\n");
    return 0;
}
```

**Compile with:**
```bash
gcc -pthread program.c -o program
```

**Resources:**
- Book: "Programming with POSIX Threads" by David Butenhof
- Tutorial: [POSIX Threads - LLNL](https://hpc-tutorials.llnl.gov/posix/)

**Time Needed:** 2-3 days

---

### 3.2 Thread Synchronization - Mutexes

**What to Learn:**
- Race conditions
- Critical sections
- Mutex initialization
- Locking and unlocking
- Deadlock prevention

**Practice Exercise - Race Condition:**
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;  // Shared variable

void* increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        counter++;  // RACE CONDITION!
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Counter: %d (expected: 2000000)\n", counter);
    // Output will be < 2000000 due to race condition!
    
    return 0;
}
```

**Fixed with Mutex:**
```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    printf("Counter: %d (expected: 2000000)\n", counter);
    // Now output will be exactly 2000000
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
```

**Resources:**
- Video: [Race Conditions - Computerphile](https://www.youtube.com/watch?v=MqnpIwN7dz0)
- Tutorial: [Mutex in C - GeeksforGeeks](https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/)

**Time Needed:** 2-3 days

---

### 3.3 Condition Variables

**What to Learn:**
- Producer-Consumer problem
- `pthread_cond_wait()`
- `pthread_cond_signal()` / `pthread_cond_broadcast()`
- Spurious wakeups

**Practice Exercise - Producer-Consumer:**
```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void* producer(void *arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        
        // Wait if buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }
        
        buffer[count++] = i;
        printf("Produced: %d (count: %d)\n", i, count);
        
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        
        usleep(100000);  // 100ms
    }
    return NULL;
}

void* consumer(void *arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        
        // Wait if buffer is empty
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }
        
        int item = buffer[--count];
        printf("Consumed: %d (count: %d)\n", item, count);
        
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        
        usleep(150000);  // 150ms
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    return 0;
}
```

**Resources:**
- Video: [Condition Variables - Jacob Sorber](https://www.youtube.com/watch?v=P6Z5K8zmEmc)
- Book: "The Little Book of Semaphores" by Allen Downey

**Time Needed:** 2-3 days

---

### 3.4 Semaphores

**What to Learn:**
- Binary vs Counting semaphores
- `sem_init()`, `sem_wait()`, `sem_post()`
- Named vs Unnamed semaphores

**Practice Exercise:**
```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CONCURRENT 3

sem_t semaphore;

void* worker(void *arg) {
    int id = *(int*)arg;
    
    printf("Thread %d: Waiting to enter\n", id);
    sem_wait(&semaphore);  // Decrement
    
    printf("Thread %d: Working...\n", id);
    sleep(2);
    
    printf("Thread %d: Done\n", id);
    sem_post(&semaphore);  // Increment
    
    return NULL;
}

int main() {
    pthread_t threads[10];
    int ids[10];
    
    // Initialize semaphore (max 3 concurrent threads)
    sem_init(&semaphore, 0, MAX_CONCURRENT);
    
    for (int i = 0; i < 10; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&semaphore);
    return 0;
}
```

**Resources:**
- Tutorial: [Semaphores in C - GeeksforGeeks](https://www.geeksforgeeks.org/semaphores-in-process-synchronization/)
- Man page: `man sem_overview`

**Time Needed:** 1-2 days

---

## 4️⃣ Network Programming Basics

### 4.1 TCP Socket Programming

**What to Learn:**
- Client-Server model
- Socket creation (`socket()`)
- Binding (`bind()`)
- Listening (`listen()`)
- Accepting connections (`accept()`)
- Connecting (`connect()`)
- Sending/Receiving data (`send()`, `recv()`)

**Practice Exercise - Simple Echo Server:**

**server.c:**
```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    // Listen
    listen(server_fd, 3);
    printf("Server listening on port %d\n", PORT);
    
    // Accept
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Client connected\n");
    
    // Receive and echo
    while (1) {
        int n = read(client_fd, buffer, 1024);
        if (n <= 0) break;
        
        printf("Received: %s", buffer);
        write(client_fd, buffer, n);
        
        memset(buffer, 0, 1024);
    }
    
    close(client_fd);
    close(server_fd);
    return 0;
}
```

**client.c:**
```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    // Connect
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    // Send message
    char *msg = "Hello from client\n";
    send(sock, msg, strlen(msg), 0);
    
    // Receive echo
    read(sock, buffer, 1024);
    printf("Server echo: %s", buffer);
    
    close(sock);
    return 0;
}
```

**Test:**
```bash
# Terminal 1
gcc server.c -o server && ./server

# Terminal 2
gcc client.c -o client && ./client
```

**Resources:**
- Tutorial: [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- Book: "Unix Network Programming" by W. Richard Stevens
- Video: [Socket Programming - CodeVault](https://www.youtube.com/watch?v=LtXEMwSG5-8)

**Time Needed:** 3-4 days

---

### 4.2 Handling Multiple Clients

**What to Learn:**
- Multi-threaded server
- Thread-per-client model
- Thread pools
- `select()` / `poll()` / `epoll()`

**Practice Exercise - Multi-client Server:**
```c
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void* handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);
    
    char buffer[1024];
    int n;
    
    while ((n = read(client_fd, buffer, 1024)) > 0) {
        write(client_fd, buffer, n);
    }
    
    close(client_fd);
    printf("Client disconnected\n");
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);
    
    printf("Server listening on port 8080\n");
    
    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, NULL, NULL);
        
        printf("Client connected\n");
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_fd);
        pthread_detach(thread);
    }
    
    close(server_fd);
    return 0;
}
```

**Resources:**
- Tutorial: [Multi-threaded Server - CodeVault](https://www.youtube.com/watch?v=Y6pFtgRdUts)

**Time Needed:** 2-3 days

---

### 4.3 Binary Protocols & Serialization

**What to Learn:**
- Network byte order (`htons()`, `htonl()`, `ntohs()`, `ntohl()`)
- Binary message format design
- Structure packing
- Endianness

**Practice Exercise:**
```c
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>

// Message format
typedef struct __attribute__((packed)) {
    uint8_t version;
    uint8_t type;
    uint16_t length;
    uint32_t id;
} MessageHeader;

void serialize_header(MessageHeader *header, char *buffer) {
    buffer[0] = header->version;
    buffer[1] = header->type;
    
    uint16_t length_net = htons(header->length);
    memcpy(buffer + 2, &length_net, 2);
    
    uint32_t id_net = htonl(header->id);
    memcpy(buffer + 4, &id_net, 4);
}

void deserialize_header(const char *buffer, MessageHeader *header) {
    header->version = buffer[0];
    header->type = buffer[1];
    
    uint16_t length_net;
    memcpy(&length_net, buffer + 2, 2);
    header->length = ntohs(length_net);
    
    uint32_t id_net;
    memcpy(&id_net, buffer + 4, 4);
    header->id = ntohl(id_net);
}

int main() {
    MessageHeader msg = {1, 5, 256, 12345};
    char buffer[8];
    
    serialize_header(&msg, buffer);
    printf("Serialized: ");
    for (int i = 0; i < 8; i++) {
        printf("%02x ", (unsigned char)buffer[i]);
    }
    printf("\n");
    
    MessageHeader received;
    deserialize_header(buffer, &received);
    printf("Deserialized: v=%d, type=%d, len=%d, id=%d\n",
           received.version, received.type, received.length, received.id);
    
    return 0;
}
```

**Resources:**
- Tutorial: [Binary Protocols - Stanford CS](https://web.stanford.edu/class/cs144/lectures/)

**Time Needed:** 2 days

---

## 5️⃣ Data Structures for High Performance

### 5.1 Linked Lists

**What to Learn:**
- Singly vs Doubly linked lists
- Insertion, deletion, traversal
- Memory management

**Practice Exercise:**
```c
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* create_node(int data) {
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

void append(Node **head, int data) {
    Node *new_node = create_node(data);
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

void print_list(Node *head) {
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node *head = NULL;
    
    append(&head, 10);
    append(&head, 20);
    append(&head, 30);
    
    print_list(head);
    
    free_list(head);
    return 0;
}
```

**Time Needed:** 1 day

---

### 5.2 Hash Tables

**What to Learn:**
- Hash functions
- Collision resolution (chaining, open addressing)
- Load factor
- Dynamic resizing

**Practice Exercise (Simple Hash Table):**
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry *buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;
    }
    return hash % TABLE_SIZE;
}

void insert(HashTable *table, const char *key, int value) {
    unsigned int index = hash(key);
    
    Entry *entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = table->buckets[index];
    
    table->buckets[index] = entry;
}

int get(HashTable *table, const char *key) {
    unsigned int index = hash(key);
    Entry *entry = table->buckets[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return -1;  // Not found
}

int main() {
    HashTable table = {0};
    
    insert(&table, "events", 100);
    insert(&table, "logs", 200);
    
    printf("events: %d\n", get(&table, "events"));
    printf("logs: %d\n", get(&table, "logs"));
    
    return 0;
}
```

**Time Needed:** 2 days

---

### 5.3 Circular Buffers (Ring Buffer)

**What to Learn:**
- Fixed-size buffer
- Producer-Consumer with circular buffer
- Lock-free implementations (advanced)

**Practice Exercise:**
```c
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

void init_buffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

bool enqueue(CircularBuffer *cb, int value) {
    if (cb->count == BUFFER_SIZE) {
        return false;  // Buffer full
    }
    
    cb->buffer[cb->tail] = value;
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

bool dequeue(CircularBuffer *cb, int *value) {
    if (cb->count == 0) {
        return false;  // Buffer empty
    }
    
    *value = cb->buffer[cb->head];
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

int main() {
    CircularBuffer cb;
    init_buffer(&cb);
    
    for (int i = 0; i < 12; i++) {
        if (enqueue(&cb, i * 10)) {
            printf("Enqueued: %d\n", i * 10);
        } else {
            printf("Buffer full!\n");
        }
    }
    
    int value;
    while (dequeue(&cb, &value)) {
        printf("Dequeued: %d\n", value);
    }
    
    return 0;
}
```

**Time Needed:** 1 day

---

# 📊 LEARNING TIMELINE SUMMARY

## Minimum Prerequisites (Can start project after this)
**Total: 10-14 days**

```
Day 1-3:   C basics + pointers
Day 4-5:   Dynamic memory + structures
Day 6-7:   File I/O + Makefile
Day 8-10:  Threads + mutexes
Day 11-12: Network programming basics
Day 13-14: Practice mini-project (simple chat server)
```

## Recommended Prerequisites (Better preparation)
**Total: 20-25 days**

```
Week 1: C programming (all sections)
Week 2: System programming (processes, signals, file descriptors)
Week 3: Threading + synchronization
Week 4: Network programming + data structures
```

## Can Learn in Parallel with Project
These can be learned as needed during the project:

- Advanced synchronization (Week 2 of project)
- epoll/poll (Week 2 of project)
- Memory-mapped files (Week 2 of project)
- Protocol design details (Week 1-2 of project)
- Performance optimization (Week 3-4 of project)

---

# 🎯 RECOMMENDED LEARNING PATH

## Path A: Fast Track (10-14 days)
**For those with some C experience**

1. ✅ Review C pointers and memory (2 days)
2. ✅ Learn pthreads + mutexes (3 days)
3. ✅ Build simple TCP echo server (2 days)
4. ✅ Build multi-threaded echo server (2 days)
5. ✅ Mini project: Chat server (3 days)
6. 🚀 **START EdgeMQ PROJECT**

## Path B: Solid Foundation (20-25 days)
**For those new to systems programming**

1. ✅ C programming fundamentals (5 days)
2. ✅ Linux system programming (5 days)
3. ✅ Multithreading + synchronization (5 days)
4. ✅ Network programming (5 days)
5. ✅ Mini projects (5 days)
6. 🚀 **START EdgeMQ PROJECT**

## Path C: Learn as You Go (Start immediately)
**For experienced programmers**

1. Have basic C knowledge? → Start Day 1 of project
2. Learn threading concepts while building Week 1
3. Learn advanced topics as needed in Weeks 2-4
4. Reference documentation during implementation

---

# 📚 ESSENTIAL RESOURCES

## Books
1. **"The C Programming Language"** - K&R (Bible of C)
2. **"The Linux Programming Interface"** - Michael Kerrisk (Best Linux reference)
3. **"Unix Network Programming"** - W. Richard Stevens (Network programming)
4. **"Programming with POSIX Threads"** - David Butenhof (Threading)

## Online Courses
1. **CS50** - Harvard (Free, excellent C introduction)
2. **Nand2Tetris** - Computer systems from scratch
3. **Operating Systems: Three Easy Pieces** (Free book)

## Interactive Practice
1. **[Learn-C.org](https://www.learn-c.org/)** - Interactive C tutorials
2. **[HackerRank - C](https://www.hackerrank.com/domains/c)** - Practice problems
3. **[LeetCode](https://leetcode.com/)** - Algorithm practice

## Video Resources
1. **Jacob Sorber** - Systems programming (YouTube)
2. **CodeVault** - C and systems programming (YouTube)
3. **Neso Academy** - OS concepts (YouTube)

## Man Pages (Your best friend)
```bash
man 2 fork      # System calls
man 3 pthread   # Library functions
man 7 signal    # Overview pages
```

---

# ✅ SKILL CHECKLIST

Before starting the project, can you:

## C Programming
- [ ] Write programs with pointers and dynamic memory
- [ ] Create and use structures
- [ ] Handle files (read/write)
- [ ] Use Makefiles
- [ ] Debug with GDB
- [ ] Check for memory leaks with Valgrind

## System Programming
- [ ] Understand file descriptors
- [ ] Use fork() to create processes
- [ ] Handle signals gracefully
- [ ] Understand errno and error handling

## Multithreading
- [ ] Create and join threads
- [ ] Use mutexes to protect shared data
- [ ] Understand race conditions
- [ ] Implement producer-consumer with condition variables

## Networking
- [ ] Create TCP client and server
- [ ] Handle multiple clients with threads
- [ ] Understand network byte order
- [ ] Send/receive binary data

## If you checked 80%+ → You're ready! 🚀
## If you checked 50-79% → Spend 1 week on weak areas
## If you checked <50% → Follow Path B (20-25 days)

---

# 🔥 QUICK START MINI-PROJECT

## Build This Before EdgeMQ (2-3 days)

**Project: Multi-Client Chat Server**

**Features:**
- TCP server on port 8080
- Multiple clients can connect
- Messages broadcast to all clients
- Thread-per-client model
- Graceful shutdown with Ctrl+C

**Skills Covered:**
✅ Socket programming  
✅ Multi-threading  
✅ Mutexes  
✅ Dynamic memory  
✅ Signal handling  

**After completing this, you'll be 100% ready for EdgeMQ!**

---

# 💡 PRO TIPS

1. **Don't try to learn everything perfectly** - Start the project when you're 70% ready
2. **Learn by doing** - Build small programs for each concept
3. **Read man pages** - They're incredibly detailed
4. **Use Valgrind daily** - Catch bugs early
5. **Copy code and modify** - Don't write from scratch initially
6. **Ask questions** - Use ChatGPT, Stack Overflow
7. **Debug systematically** - Use printf, GDB, strace
8. **Commit often** - Use Git from day 1

---

**Ready to start learning?** Pick your path and let's go! 🚀

**Already confident?** Say **"START PROJECT"** and I'll generate Day 1 code! 💪
