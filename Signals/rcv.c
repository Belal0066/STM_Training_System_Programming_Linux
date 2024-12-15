#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int sig, siginfo_t *si, void *context) {
    if (si->si_code == SI_QUEUE) {
        int data = si->si_value.sival_int;
        if (data == 0) {
            printf("Received data 0\n >> Terminating......\n");
            exit(EXIT_SUCCESS);
        } else if (data == 1) {
            printf("Received data 1\n >> Aborting with core dump.....\n");
            abort();
        } else {
            printf("Eh da ya Ghaly O_o? %d\n", data);
        }
    }
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Receiver PID: %d\n", getpid());

    while (1);

    return 0;
}
