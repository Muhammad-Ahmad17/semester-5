include<stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

    const char *get_signal_name(int signo)
{
    switch (signo)
    {
    case SIGINT:
        return "SIGINT";
    case SIGQUIT:
        return "SIGQUIT";
    case SIGPIPE:
        return "SIGPIPE";
    case SIGALRM:
        return "SIGALRM";
    default:
        return "UNKNOWN";
    }
}

void signal_handler(int signo)
{
    printf("\n>>> [Handler] Received signal: %s (%d)\n", get_signal_name(signo), signo);

    for (int i = 0; i < 5; i++)
    {
        printf(">>> [Handler] Iteration %d: Signal handler active. Waiting for signals...\n", i + 1);
        sleep(3);
    }

    printf(">>> [Handler] Exiting signal handler for signal: %s (%d)\n", get_signal_name(signo), signo);
}

void main_signal_handler(int signo)
{
    printf("\n>>> [Main] Received signal: %s (%d) in the main process\n", get_signal_name(signo), signo);
}

int main()
{

    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    sigaddset(&act.sa_mask, SIGPIPE);

    int signals[4] = {SIGINT, SIGPIPE, SIGQUIT, SIGALRM};
    for (int i = 0; i < 4; i++)
    {
        sigaction(signals[i], &act, NULL);
    }

    printf(">>> Process ID (PID): %d\n", getpid());
    printf(">>> Signal handler ready and running. Send signals to the process using 'kill -SIGNAL %d'.\n\n", getpid());

    while (1)
    {
        printf(">>> [Main] Main process waiting for signals...\n");
        sleep(10);
    }

    return 0;
}