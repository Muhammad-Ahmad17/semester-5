#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int at[20], bt[20], rt[20], wt[20] = {0}, tat[20], completed = 0;
    int finish[20], time = 0;
    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &at[i], &bt[i]);
        rt[i] = bt[i];
    }

    while (completed < n) {
        int idx = -1, min_rt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (at[i] <= time && rt[i] > 0 && rt[i] < min_rt) {
                min_rt = rt[i];
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }

        rt[idx]--;
        time++;
        if (rt[idx] == 0) {
            completed++;
            finish[idx] = time;
            tat[idx] = finish[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
            total_wt += wt[idx];
            total_tat += tat[idx];
        }
    }

    printf("\nPID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], wt[i], tat[i]);

    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    return 0;
}
