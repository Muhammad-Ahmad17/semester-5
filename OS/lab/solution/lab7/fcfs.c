/*
 * FCFS (First-Come-First-Served) CPU Scheduling Algorithm
 * 
 * Algorithm Description:
 * - Processes are executed in the order they arrive (FIFO)
 * - Non-preemptive: once a process starts, it runs to completion
 * - Simple but may cause "convoy effect" (short jobs wait for long ones)
 * 
 * Time Complexity: O(n log n) for sorting, O(n) for scheduling
 * Space Complexity: O(n)
 */

#include <stdio.h>

void swap(int *a , int *b ){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void)
{
    // Array declarations for process scheduling data
    int bt[50]; // burst time (CPU time needed)
    int wt[50]; // waiting time (time in ready queue)
    int tat[50]; // turnaround time (completion time - arrival time)
    int at[50]; // arrival time (when process enters system)
    int pid[50]; // original process ids
    int i, j, n;
    float wtsum = 0.0f, tatsum = 0.0f; // sums for calculating averages

    // Input: number of processes
    printf("Enter the number of processes (max 50): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 50) {
        fprintf(stderr, "Invalid number of processes.\n");
        return 1;
    }

    // Input: arrival time and burst time for each process
    for (i = 0; i < n; i++) {
        pid[i] = i; // assign process ID
        printf("Enter Arrival Time for Process %d: ", i);
        if (scanf("%d", &at[i]) != 1) { fprintf(stderr, "Invalid input.\n"); return 1; }
        printf("Enter Burst Time for Process %d: ", i);
        if (scanf("%d", &bt[i]) != 1 || bt[i] < 0) { fprintf(stderr, "Invalid burst time.\n"); return 1; }
    }

    /* 
     * STEP 1: Sort processes by arrival time (FCFS order)
     * If two processes have same arrival time, sort by pid
     * Selection sort: O(n^2)
     */
    for (i = 0; i < n - 1; i++) {
        int min = i;
        // Find process with minimum arrival time
        for (j = i + 1; j < n; j++) {
            if (at[j] < at[min] || (at[j] == at[min] && pid[j] < pid[min])) {
                min = j;
            }
        }
        // Swap the process with minimum arrival time to current position
        if (min != i) {
            swap(&at[i], &at[min]);
            swap(&bt[i], &bt[min]);
            swap(&pid[i], &pid[min]);
        }
    }

    /* 
     * STEP 2: Calculate scheduling metrics
     * For each process in FCFS order:
     * - wt[i] = start_time - arrival_time
     * - tat[i] = completion_time - arrival_time
     */
    int current_time = 0;
    for (i = 0; i < n; i++) {
        // If CPU is idle (current_time < arrival), advance to arrival time
        if (current_time < at[i]) {
            current_time = at[i]; // CPU idle until this process arrives
        }
        wt[i] = current_time - at[i]; // waiting time = start - arrival
        tat[i] = wt[i] + bt[i];       // turnaround time = waiting + burst
        current_time += bt[i];        // completion time moves forward by burst time

        wtsum += wt[i];
        tatsum += tat[i];
    }

    // Output: scheduling table
    printf("\n\tPROCESS\t\tARRIVAL\tBURST\tWAITING\tTURNAROUND\n");
    for (i = 0; i < n; i++) {
        printf("\tP%d\t\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);
    }

    // Output: average metrics
    printf("\nAverage Waiting Time -- %.2f\n", wtsum / n);
    printf("Average Turnaround Time -- %.2f\n", tatsum / n);

    return 0;
}