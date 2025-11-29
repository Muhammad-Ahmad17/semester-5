/*
 * SRTF (Shortest Remaining Time First) CPU Scheduling Algorithm
 * 
 * Algorithm Description:
 * - Preemptive version of SJF
 * - Among all arrived processes, selects the one with shortest REMAINING time
 * - Can preempt a running process if a shorter one arrives
 * - Optimal for minimizing average waiting time
 * - Higher overhead due to frequent context switches
 * 
 * Time Complexity: O(n^2) for scheduling simulation
 * Space Complexity: O(n)
 */

#include <stdio.h>

int main(void)
{
    // Array declarations for process scheduling data
    int bt[50];      // original burst time (never changes)
    int rem[50];     // remaining time (decrements as process executes)
    int wt[50];      // waiting time (time in ready queue)
    int tat[50];     // turnaround time (completion - arrival)
    int at[50];      // arrival time (when process enters system)
    int pid[50];     // process ids
    int start[50];   // first start time (-1 if not started)
    int completion[50]; // completion time
    int completed[50];  // flag array: 1 if finished, 0 otherwise

    int timeline[10000]; // timeline of pid executed each time unit
    int tl_len = 0; // length of timeline

    int i, n;
    const int INF = 1<<30; // large value representing infinity
    float wtsum = 0.0f, tatsum = 0.0f; // sums for averages

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
        rem[i] = bt[i]; // remaining time starts equal to burst time
        completed[i] = 0;
        start[i] = -1;
        completion[i] = 0;
    }

    int completed_count = 0;
    int t = 0; // current time

    // If there are processes with arrival > 0, advance to min arrival
    int min_arr = INF;
    for (i = 0; i < n; i++) if (at[i] < min_arr) min_arr = at[i];
    if (min_arr > 0) t = min_arr;

    // STEP 1: Main scheduling loop - continue until all processes complete
    while (completed_count < n) {
        // STEP 2: Pick the arrived process with smallest REMAINING time (preemptive)
        int idx = -1; // index of process to execute
        int min_rem = INF; // minimum remaining time

        // Search all processes for smallest remaining time among arrived processes
        for (i = 0; i < n; i++) {
            if (!completed[i] && at[i] <= t && rem[i] > 0) {
                // Prefer process with smallest remaining time
                // Tiebreak by arrival time, then by pid
                if (rem[i] < min_rem ||
                    (rem[i] == min_rem && (at[i] < at[idx] || (at[i] == at[idx] && pid[i] < pid[idx])))) {
                    min_rem = rem[i];
                    idx = i;
                }
            }
        }

        // If no process has arrived yet, advance time to the next arrival
        if (idx == -1) {
            int next_arr = INF;
            for (i = 0; i < n; i++) if (!completed[i] && at[i] < next_arr) next_arr = at[i];
            if (next_arr == INF) break;
            // Advance time to next arrival without executing
            t = next_arr;
            continue;
        }

        // STEP 3: Record first start time
        if (start[idx] == -1) start[idx] = t;

        // STEP 4: Execute process for 1 time unit (preemptive SRTF)
        rem[idx]--; // decrement remaining time
        timeline[tl_len++] = idx; // record execution
        t++; // advance time by 1

        // STEP 5: Check if process has completed
        if (rem[idx] == 0) {
            completed[idx] = 1; // mark as complete
            completion[idx] = t; // record completion time
            tat[idx] = completion[idx] - at[idx]; // turnaround = completion - arrival
            wt[idx] = tat[idx] - bt[idx]; // waiting = turnaround - burst
            if (wt[idx] < 0) wt[idx] = 0; // ensure non-negative
            wtsum += wt[idx];
            tatsum += tat[idx];
            completed_count++;
        }
    }

    // Output: Execution order in compressed format (pid(duration))
    printf("\n\tEXECUTION ORDER (pid(duration)):\n\t");
    if (tl_len == 0) {
        printf("No execution\n");
    } else {
        int cur = timeline[0];
        int dur = 1;
        for (i = 1; i < tl_len; i++) {
            if (timeline[i] == cur) {
                dur++; // same process continues
            } else {
                // process switched, output previous execution
                if (cur == -1) printf("IDLE(%d) -> ", dur);
                else printf("P%d(%d) -> ", cur, dur);
                cur = timeline[i];
                dur = 1;
            }
        }
        // output last execution
        if (cur == -1) printf("IDLE(%d)\n", dur);
        else printf("P%d(%d)\n", cur, dur);
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