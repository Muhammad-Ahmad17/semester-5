# **1. Program**

 * A program is a passive entity.
 * It’s basically a file containing code, like a.out, chrome, or myapp.exe.
 * Not running yet, just instructions stored on disk.
 * no memory , no pid 

## Example:
```
./myapp
```

> myapp is a program until you run it.

✅ Key point: A program becomes a process only when executed.


	

---

# **2. Multiprogramming**

* **Definition:** Running **multiple programs** on a single CPU by keeping them in memory at the same time.
* **Goal:** Maximize **CPU utilization**—while one program is waiting for I/O, another can run.
* **Key idea:** **Programs are in memory, but not all run simultaneously**; CPU switches between them.

**Example:**

* You’re copying a file (I/O-bound) and watching a video (CPU-bound) → OS switches CPU to video while file copy waits.

---


| Term             | What it works on              | Execution                                  | Purpose                       |
| ---------------- | ----------------------------- | ------------------------------------------ | ----------------------------- |
| Multiprogramming | Multiple programs             | Concurrent on 1 CPU                        | Maximize CPU utilization      |

---


# **3. QnA**




### **Your first question:**

> 1. How can multiple programs be placed in memory (code segment)? How to distinguish them?

**Answer:**

In multiprogramming, multiple programs (processes) reside in **RAM at the same time**, but each process has its **own separate memory space**. Modern OSes achieve this using **virtual memory**:

* Each process has its **own address space**:

  * Code segment (text) → the program instructions
  * Data segment → global/static variables
  * Heap → dynamic memory
  * Stack → function calls & local variables

* **Virtual addresses** make each process think it has the **entire memory to itself**.

* The OS **maps virtual addresses to physical memory** so that code/data of one process does not overwrite another process.

✅ **Distinguishing programs in memory:**

* Each process has a **Process Control Block (PCB)** maintained by the OS.
* PCB contains:

  * PID (process ID)
  * Program counter
  * Memory limits (base/limit or page table)
  * CPU registers
  * Open files, etc.

So even if multiple programs are loaded in memory, the **OS keeps them logically separated**.

---

### **Your second question:**

> 2. How does the program counter change the context and get back the context (context switching)?

**Answer:**

* The **program counter (PC)** tells the CPU **which instruction of a process to execute next**.

* In multiprogramming:

  1. The CPU runs **Process A**. Its PC is saved in its PCB.
  2. After a time slice or I/O event, the OS performs a **context switch**:

     * Saves **Process A’s CPU registers, PC, and stack pointer** in its PCB
     * Loads **Process B’s CPU registers, PC, and stack pointer** from its PCB
  3. CPU now executes **Process B** from where it left off.

* **Virtual memory ensures** that even though multiple processes share the same CPU, their **code and data segments are isolated**, so the PC always points to the correct instruction in that process’s memory space.

---

### **Putting it together:**

* **Program → Process**: A program becomes a process when loaded into memory with its own PCB.
* **Multiprogramming**: Multiple processes (from different programs) reside in memory simultaneously.
* **Program Counter + PCB**: The OS uses the PC stored in PCB to switch between processes, allowing the CPU to give the illusion of running many programs at the same time.
* **Virtual memory + paging**: Keeps memory segments of different processes separate and safe.

---

### **Analogy (CPU multitasking)**

* Imagine a **teacher reading multiple books**:

  * Each book = a program
  * The teacher reads a few pages (process executes instructions)
  * Puts the bookmark (program counter saved in PCB)
  * Picks another book (context switch)
  * Later resumes from the bookmark (PC restored)

✅ Multiprogramming relies on **context switching + memory isolation** to execute multiple programs efficiently.

---

