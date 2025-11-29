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
        printf("Caught SIGALRM (alarm clock termination) .\n");
        break;
    default:
        printf("Caught unexpected signal: %d\n", signo);
        break;
    }
}
int main()
{
    struct sigaction act;
    act.sa_handler = signal_handler;
    act.sa_flags = 0;
    int signals[4] = {SIGINT, SIGPIPE, SIGQUIT, SIGALRM};
    for (int i = 0; i < 4; i++)
    {
        sigaction(signals[i], &act, NULL);
    }
    printf("Raising SIGINT...\n");
    raise(SIGINT);
    printf("Raising SIGPIPE...\n");
    raise(SIGPIPE);
    printf("Raising SIGQUIT...\n");
    raise(SIGQUIT);
    printf("Raising SIGALRM..\n");
    raise(SIGALRM);
    printf("All signals raised and handled.\n");
    return 0;
}