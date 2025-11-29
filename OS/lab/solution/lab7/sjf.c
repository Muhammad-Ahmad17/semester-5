/*
 * SJF (Shortest Job First) CPU Scheduling Algorithm
 * 
 * Algorithm Description:
 * - Non-preemptive scheduling algorithm
 * - Among all arrived processes, the one with shortest burst time executes first
 * - Reduces average waiting time compared to FCFS
 * - Can suffer from starvation if many short jobs keep arriving
 * 
 * Time Complexity: O(n^2) for main scheduling loop
 * Space Complexity: O(n)
 */

#include <stdio.h>

int main(void)
{
    // Array declarations for process scheduling data
    int bt[50];      // burst time (CPU time needed)
    int wt[50];      // waiting time (time in ready queue)
    int tat[50];     // turnaround time (completion - arrival)
    int at[50];      // arrival time (when process enters system)
    int pid[50];     // original process ids
    int start[50];   // start time of each process
    int completion[50]; // completion time
    int completed[50];  // flag: 1 if process has completed, 0 otherwise
    int exec_order[50]; // order in which processes are executed

    int i, n;
    float wtsum = 0.0f, tatsum = 0.0f;

    // Input: number of processes
    printf("Enter the number of processes (max 50): ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 50) {
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
        completed[i] = 0; // Initially, no process has completed
    }

    // STEP 1: Main scheduling loop - repeat until all processes are scheduled
    int completed_count = 0;
    int current_time = 0;
    int order_idx = 0;

    while (completed_count < n) {
        // STEP 2: Find the shortest job among arrived and not yet completed processes
        int idx = -1; // index of process to execute next
        int min_bt = 1<<30; // initialize to large value (infinity)

        // Search all processes for shortest burst time among arrived processes
        for (i = 0; i < n; i++) {
            if (!completed[i] && at[i] <= current_time) {
                // Among processes with same burst time, prefer earlier arrival
                if (bt[i] < min_bt || (bt[i] == min_bt && at[i] < at[idx]) || (bt[i] == min_bt && at[i] == at[idx] && pid[i] < pid[idx])) {
                    min_bt = bt[i];
                    idx = i;
                }
            }
        }

        // If no process has arrived yet, advance time to the next arrival
        if (idx == -1) {
            int next_arrival = 1<<30;
            for (i = 0; i < n; i++) {
                if (!completed[i] && at[i] < next_arrival) next_arrival = at[i];
            }
            current_time = next_arrival;
            continue;
        }

        // STEP 3: Schedule the shortest job
        start[idx] = current_time;
        wt[idx] = start[idx] - at[idx]; // waiting time = start - arrival
        completion[idx] = start[idx] + bt[idx]; // completion = start + burst
        tat[idx] = completion[idx] - at[idx]; // turnaround = completion - arrival

        current_time = completion[idx]; // update current time to completion time
        completed[idx] = 1; // mark process as completed
        exec_order[order_idx++] = idx; // record execution order
        completed_count++;

        wtsum += wt[idx];
        tatsum += tat[idx];
    }

    // Output: Execution order
    printf("\n\tEXECUTION ORDER: ");
    for (i = 0; i < order_idx; i++) {
        printf("P%d%s", pid[exec_order[i]], i == order_idx - 1 ? "" : " -> ");
    }
    
    // Output: Scheduling table
    printf("\n\n\tPID\tARRIVAL\tBURST\tSTART\tCOMPLETION\tWAITING\tTURNAROUND\n");
    for (i = 0; i < order_idx; i++) {
        int k = exec_order[i];
        printf("\tP%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n",
               pid[k], at[k], bt[k], start[k], completion[k], wt[k], tat[k]);
    }

    // Output: Average metrics
    printf("\nAverage Waiting Time -- %.2f\n", wtsum / n);
    printf("Average Turnaround Time -- %.2f\n", tatsum / n);

    return 0;
}