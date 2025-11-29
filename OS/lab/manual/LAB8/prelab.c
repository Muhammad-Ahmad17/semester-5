/* Pre-Lab: FCFS Scheduling (No Arrival Time) */
#include <stdio.h>

int main() {
    int bt[20], wt[20], tat[20], n;
    float wtsum = 0, tatsum = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter Burst Time for Process %d: ", i + 1);
        scanf("%d", &bt[i]);
    }

    wt[0] = 0;
    tat[0] = bt[0];
    wtsum = wt[0];
    tatsum = tat[0];

    for (int i = 1; i < n; i++) {
        wt[i] = wt[i - 1] + bt[i - 1];
        tat[i] = wt[i] + bt[i];
        wtsum += wt[i];
        tatsum += tat[i];
    }

    printf("\nPROCESS\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\n", i + 1, bt[i], wt[i], tat[i]);

    printf("\nAverage Waiting Time = %.2f", wtsum / n);
    printf("\nAverage Turnaround Time = %.2f\n", tatsum / n);

    return 0;
}
