/*
 * Round Robin (RR) CPU Scheduling Algorithm
 * 
 * Algorithm Description:
 * - Preemptive scheduling algorithm
 * - Each process gets a fixed time quantum (time slice)
 * - If process doesn't complete in time quantum, it's moved to end of queue
 * - Fair distribution of CPU time among processes
 * - Suitable for time-sharing systems
 * 
 * Time Complexity: O(n) per context switch
 * Space Complexity: O(n)
 */

#include <stdio.h>

int main(void)
{
    const int MAX = 50; // maximum number of processes
    
    // Array declarations for process scheduling data
    int bt[MAX], rem[MAX], at[MAX], pid[MAX]; // burst time, remaining time, arrival time, process id
    int start[MAX], completion[MAX], wt[MAX], tat[MAX]; // start, completion, waiting, turnaround times
    int in_queue[MAX], completed[MAX]; // flags: in_queue, completed
    int timeline[10000], tl_len = 0; // timeline of executions

    // Circular queue for ready queue
    int queue[MAX]; // queue of process indices
    int head = 0, tail = 0; // circular queue pointers

    int n, tq, i; // n: number of processes, tq: time quantum
    float wtsum = 0.0f, tatsum = 0.0f; // sums for averages

    // Input: number of processes
    printf("Enter the number of processes (max %d): ", MAX);
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) { 
        fprintf(stderr, "Invalid number of processes.\n"); 
        return 1; 
    }

    // Input: arrival time and burst time for each process
    for (i = 0; i < n; i++) {
        pid[i] = i;
        printf("Enter Arrival Time for Process %d: ", i);
        if (scanf("%d", &at[i]) != 1) { fprintf(stderr, "Invalid input.\n"); return 1; }
        printf("Enter Burst Time for Process %d: ", i);
        if (scanf("%d", &bt[i]) != 1 || bt[i] < 0) { fprintf(stderr, "Invalid burst time.\n"); return 1; }
        rem[i] = bt[i]; // remaining time starts equal to burst time
        start[i] = -1; // not started yet
        completion[i] = 0;
        in_queue[i] = 0; // not in queue yet
        completed[i] = 0; // not completed yet
    }

    // Input: time quantum
    printf("Enter Time Quantum (positive integer): ");
    if (scanf("%d", &tq) != 1 || tq <= 0) { 
        fprintf(stderr, "Invalid time quantum.\n"); 
        return 1; 
    }

    // Find earliest arrival time and initialize current time
    const int INF = 1<<30;
    int min_arr = INF;
    for (i = 0; i < n; i++) if (at[i] < min_arr) min_arr = at[i];
    int current_time = (min_arr == INF ? 0 : min_arr);

    // STEP 1: Enqueue all processes that arrive at current_time initially
    for (i = 0; i < n; i++) {
        if (at[i] <= current_time && !in_queue[i] && rem[i] > 0) {
            queue[tail++] = i; 
            if (tail == MAX) tail = 0; // wrap around
            in_queue[i] = 1;
        }
    }

    int completed_count = 0;
    
    // STEP 2: Main scheduling loop - continue until all processes complete
    while (completed_count < n) {
        // STEP 3: If queue is empty, advance time to next arrival
        if (head == tail) {
            int next_arr = INF;
            for (i = 0; i < n; i++) if (!completed[i] && at[i] < next_arr) next_arr = at[i];
            if (next_arr == INF) break; // no more arrivals
            current_time = next_arr; // skip to next arrival
            
            // Enqueue all processes arriving at new current_time
            for (i = 0; i < n; i++) {
                if (at[i] <= current_time && !in_queue[i] && rem[i] > 0) {
                    queue[tail++] = i; 
                    if (tail == MAX) tail = 0;
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        // STEP 4: Dequeue next process from front of queue
        int idx = queue[head++];
        if (head == MAX) head = 0; // wrap around
        in_queue[idx] = 0; // remove from queue

        // Record first start time
        if (start[idx] == -1) start[idx] = current_time;

        // STEP 5: Calculate execution duration (min of time quantum or remaining time)
        int exec = (rem[idx] < tq) ? rem[idx] : tq;
        
        // STEP 6: Execute for 'exec' time units, updating timeline
        for (i = 0; i < exec; i++) {
            timeline[tl_len++] = idx; // record execution
            current_time++; // advance time
            
            // Enqueue any new processes that arrive during execution
            int j;
            for (j = 0; j < n; j++) {
                if (!completed[j] && !in_queue[j] && rem[j] > 0 && 
                    at[j] <= current_time && at[j] > current_time - 1) {
                    queue[tail++] = j; 
                    if (tail == MAX) tail = 0;
                    in_queue[j] = 1;
                }
            }
        }
        
        // STEP 7: Decrement remaining time
        rem[idx] -= exec;

        // STEP 8: Check if process has completed
        if (rem[idx] == 0) {
            // Process completed
            completed[idx] = 1;
            completion[idx] = current_time;
            tat[idx] = completion[idx] - at[idx]; // turnaround = completion - arrival
            wt[idx] = tat[idx] - bt[idx]; // waiting = turnaround - burst
            if (wt[idx] < 0) wt[idx] = 0; // ensure non-negative
            wtsum += wt[idx];
            tatsum += tat[idx];
            completed_count++;
        } else {
            // Process not complete, re-enqueue at tail of queue
            if (!in_queue[idx]) {
                queue[tail++] = idx; 
                if (tail == MAX) tail = 0;
                in_queue[idx] = 1;
            }
        }

        // STEP 9: Also enqueue any processes that arrived exactly at current_time
        for (i = 0; i < n; i++) {
            if (!completed[i] && !in_queue[i] && rem[i] > 0 && at[i] <= current_time) {
                queue[tail++] = i; 
                if (tail == MAX) tail = 0;
                in_queue[i] = 1;
            }
        }
    }

    // Output: Execution order in compressed format (pid(duration))
    printf("\n\tEXECUTION ORDER (pid(duration)):\n\t");
    if (tl_len == 0) {
        printf("No execution\n");
    } else {
        int cur = timeline[0], dur = 1;
        for (i = 1; i < tl_len; i++) {
            if (timeline[i] == cur) {
                dur++; // same process continues
            } else {
                // process switched, output previous execution
                printf("P%d(%d) -> ", cur, dur);
                cur = timeline[i];
                dur = 1;
            }
        }
        // output last execution
        printf("P%d(%d)\n", cur, dur);
    }

    // Output: Scheduling table
    printf("\n\tPID\tARRIVAL\tBURST\tSTART\tCOMPLETION\tWAITING\tTURNAROUND\n");
    for (i = 0; i < n; i++) {
        printf("\tP%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
               pid[i], at[i], bt[i],
               (start[i] == -1 ? 0 : start[i]),
               completion[i], wt[i], tat[i]);
    }

    // Output: Average metrics
    printf("\nAverage Waiting Time -- %.2f\n", wtsum / n);
    printf("Average Turnaround Time -- %.2f\n", tatsum / n);

    return 0;
}