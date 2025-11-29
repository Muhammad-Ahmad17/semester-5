#include<stdio.h>
int main ()
{
  int bt[20]; //burst time
  int wt[20]; //waiting time
  int tat[20]; //turnaround time
  int i;//starting loop variable
  int n;//number of processes
  float wtsum=0, tatsum=0; //sum of waiting time and turnaround time

  // Input number of processes and their burst times
  printf ("\nEnter the number of processes -- ");
  scanf ("%d", &n);
  for (i = 0; i < n; i++)
    {
      printf ("\nEnter Burst Time for Process %d -- ", i);
      scanf ("%d", &bt[i]);
    }
  wt[0] = 0; //waiting time for first process is 0
  tat[0]  = bt[0]; //turnaround time for first process is its burst time
  for (i = 1; i < n; i++)
    { 
      wt[i] = wt[i - 1] + bt[i - 1]; //waiting time for current process is sum of waiting time and burst time of previous process
      tat[i] = tat[i - 1] + bt[i]; //turnaround time for current process is sum of turnaround time of previous process and burst time of current process
      wtsum = wtsum + wt[i];
      tatsum = tatsum + tat[i];
    }
  printf ("\t PROCESS \tBURST TIME \t WAITING TIME\t TURNAROUND TIME\n");
  for (i = 0; i < n; i++)
    printf ("\n\t P%d \t\t %d \t\t\t %d \t\t\t %d", i, bt[i], wt[i], tat[i]);
  printf ("\nAverage Waiting Time -- %f", wtsum / n);
  printf ("\nAverage Turnaround Time -- %f/n", tatsum / n);

  return 0;
}
