#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signo)
{
    switch (signo)
    {
    case SIGINT:
        printf("Caught SIGINT (Interrupt from keyboard).\n");
        break;
    case SIGPIPE:
        printf("Caught SIGPIPE (Broken pipe).\n");
        break;
    case SIGQUIT:
        printf("Caught SIGQUIT (Quit from keyboard).\n");
        break;
    case SIGALRM:
        printf("Caught SIGALRM (Alarm clock termination).\n");
        break;
    default:
        printf("Caught unexpected signal: %d\n", signo);
        break;
    }
}

int main()
{
    struct sigaction act;

    // Set up the signal handler for various signals
    act.sa_handler = signal_handler;
    act.sa_flags = 0;

    // Register for signals: SIGINT, SIGPIPE, SIGQUIT, SIGALRM
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGPIPE, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGALRM, &act, NULL);

    // Print the process ID (PID)
    printf("Process ID: %d\n", getpid());

    // Enter an infinite loop to keep the process alive and handle signals
    for (;;);
    return 0;
}
