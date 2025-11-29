#include <stdio.h>

typedef struct {
    int pid, at, bt, rt, wt, tat;
} Process;

int main() {
    int n, q;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter Time Quantum: ");
    scanf("%d", &q);

    Process p[20];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
        p[i].wt = p[i].tat = 0;
    }

    int time = 0, completed = 0;
    float total_wt = 0, total_tat = 0;

    while (completed < n) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                done = 0;
                if (p[i].rt > q) {
                    time += q;
                    p[i].rt -= q;
                } else {
                    time += p[i].rt;
                    p[i].wt = time - p[i].bt - p[i].at;
                    p[i].rt = 0;
                    completed++;
                    p[i].tat = p[i].wt + p[i].bt;
                    total_wt += p[i].wt;
                    total_tat += p[i].tat;
                }
            }
        }
        if (done) time++;
    }

    printf("\nPID\tAT\tBT\tWT\tTAT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].wt, p[i].tat);

    printf("\nAverage Waiting Time = %.2f", total_wt / n);
    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    return 0;
}
