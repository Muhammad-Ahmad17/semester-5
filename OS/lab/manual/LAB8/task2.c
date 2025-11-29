#include <stdio.h>

typedef struct {
    int pid, at, bt, wt, tat, done;
} Process;

int main() {
    int n, completed = 0, time = 0;
    float total_wt = 0, total_tat = 0;

    printf("Enter50 number of processes: ");
    scanf("%d", &n);

    Process p[20];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].done = 0;
    }

    while (completed < n) {
        int idx = -1, min_bt = 9999;
        for (int i = 0; i < n; i++) {
            if (!p[i].done && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].wt = time - p[idx].at;
        time += p[idx].bt;
        p[idx].tat = p[idx].wt + p[idx].bt;
        p[idx].done = 1;
        completed++;

        total_wt += p[idx].wt;
        total_tat += p[idx].tat;
    }

    printf("\nPID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat);

    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    return 0;
}
