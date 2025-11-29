/* OS LAB 8 - Final Comparative Program
   Compares FCFS, SJF, SRTF, and RR on the same input set.
*/

#include <stdio.h>
#include <limits.h>

#define MAX 50

typedef struct {
    int pid, at, bt, rt;
} Process;

// ---------- FCFS ----------
void fcfs(Process p[], int n, float *avg_wt, float *avg_tat, float *cpu_util) {
    int wt[MAX], tat[MAX];
    int time = 0;
    float total_wt = 0, total_tat = 0;
    int idle = 0;

    // Sort by arrival
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].at > p[j].at) {
                Process t = p[i]; p[i] = p[j]; p[j] = t;
            }

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) 
        { idle += (p[i].at - time); time = p[i].at; }
        wt[i] = time - p[i].at;
        time += p[i].bt;
        tat[i] = wt[i] + p[i].bt;
        total_wt += wt[i];
        total_tat += tat[i];
    }

    *avg_wt = total_wt / n;
    *avg_tat = total_tat / n;
    *cpu_util = (float)(time - idle) / time * 100.0;
}

// ---------- SJF (Non-preemptive) ----------
void sjf(Process p[], int n, float *avg_wt, float *avg_tat, float *cpu_util) {
    int done[MAX] = {0}, wt[MAX], tat[MAX];
    int time = 0, completed = 0, idle = 0;
    float total_wt = 0, total_tat = 0;

    while (completed < n) {
        int idx = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt; idx = i;
            }
        }
        if (idx == -1) { time++; idle++; continue; }

        wt[idx] = time - p[idx].at;
        time += p[idx].bt;
        tat[idx] = wt[idx] + p[idx].bt;
        done[idx] = 1;
        completed++;
        total_wt += wt[idx];
        total_tat += tat[idx];
    }

    *avg_wt = total_wt / n;
    *avg_tat = total_tat / n;
    *cpu_util = (float)(time - idle) / time * 100.0;
}

// ---------- SRTF ----------
void srtf(Process p[], int n, float *avg_wt, float *avg_tat, float *cpu_util) {
    int rt[MAX], finish[MAX], done[MAX] = {0};
    int time = 0, completed = 0, idle = 0;
    float total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) rt[i] = p[i].bt;

    while (completed < n) {
        int idx = -1, min_rt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && rt[i] > 0 && rt[i] < min_rt) {
                min_rt = rt[i];
                idx = i;
            }
        }
        if (idx == -1) { time++; idle++; continue; }

        rt[idx]--;
        time++;
        if (rt[idx] == 0) {
            completed++;
            finish[idx] = time;
            int tat = finish[idx] - p[idx].at;
            int wt = tat - p[idx].bt;
            total_wt += wt;
            total_tat += tat;
        }
    }

    *avg_wt = total_wt / n;
    *avg_tat = total_tat / n;
    *cpu_util = (float)(time - idle) / time * 100.0;
}

// ---------- ROUND ROBIN ----------
void rr(Process p[], int n, int q, float *avg_wt, float *avg_tat, float *cpu_util) {
    int rt[MAX], finish[MAX];
    for (int i = 0; i < n; i++) rt[i] = p[i].bt;
    int time = 0, completed = 0, idle = 0;
    float total_wt = 0, total_tat = 0;

    while (completed < n) {
        int progress = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && rt[i] > 0) {
                progress = 1;
                if (rt[i] > q) {
                    rt[i] -= q;
                    time += q;
                } else {
                    time += rt[i];
                    rt[i] = 0;
                    completed++;
                    finish[i] = time;
                    int tat = finish[i] - p[i].at;
                    int wt = tat - p[i].bt;
                    total_wt += wt;
                    total_tat += tat;
                }
            }
        }
        if (!progress) { time++; idle++; }
    }

    *avg_wt = total_wt / n;
    *avg_tat = total_tat / n;
    *cpu_util = (float)(time - idle) / time * 100.0;
}

// ---------- MAIN ----------
int main() {
    int n, q;
    Process p[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter Arrival Time and Burst Time for P%d: ", i + 1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].rt = p[i].bt;
    }
    printf("Enter Time Quantum for RR: ");
    scanf("%d", &q);

    float wt1, tat1, cpu1;
    float wt2, tat2, cpu2;
    float wt3, tat3, cpu3;
    float wt4, tat4, cpu4;

    fcfs(p, n, &wt1, &tat1, &cpu1);
    sjf(p, n, &wt2, &tat2, &cpu2);
    srtf(p, n, &wt3, &tat3, &cpu3);
    rr(p, n, q, &wt4, &tat4, &cpu4);

    printf("\n---------------------------------------------------------------\n");
    printf("Algorithm\tAvg Waiting\tAvg Turnaround\tCPU Utilization\n");
    printf("---------------------------------------------------------------\n");
    printf("FCFS\t\t%.2f\t\t%.2f\t\t%.2f%%\n", wt1, tat1, cpu1);
    printf("SJF\t\t%.2f\t\t%.2f\t\t%.2f%%\n", wt2, tat2, cpu2);
    printf("SRTF\t\t%.2f\t\t%.2f\t\t%.2f%%\n", wt3, tat3, cpu3);
    printf("RR\t\t%.2f\t\t%.2f\t\t%.2f%%\n", wt4, tat4, cpu4);
    printf("---------------------------------------------------------------\n");

    // identify best algorithm
    float minWT = wt1, minTAT = tat1;
    char bestWT[10] = "FCFS", bestTAT[10] = "FCFS";
    if (wt2 < minWT) { minWT = wt2; sprintf(bestWT, "SJF"); }
    if (wt3 < minWT) { minWT = wt3; sprintf(bestWT, "SRTF"); }
    if (wt4 < minWT) { minWT = wt4; sprintf(bestWT, "RR"); }

    if (tat2 < minTAT) { minTAT = tat2; sprintf(bestTAT, "SJF"); }
    if (tat3 < minTAT) { minTAT = tat3; sprintf(bestTAT, "SRTF"); }
    if (tat4 < minTAT) { minTAT = tat4; sprintf(bestTAT, "RR"); }

    printf("\nBest Algorithm (Lowest Waiting Time): %s (%.2f)\n", bestWT, minWT);
    printf("Best Algorithm (Lowest Turnaround): %s (%.2f)\n", bestTAT, minTAT);
    printf("---------------------------------------------------------------\n");

    return 0;
}
