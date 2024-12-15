#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "err: %s\n usage: <pid> <0 or 1>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]);
    int data = atoi(argv[2]);

    union sigval sv;
    sv.sival_int = data;

    if (sigqueue(pid, SIGRTMIN, sv) == -1) {
        perror("sigqueue");
        exit(EXIT_FAILURE);
    }

    printf("Sent data: %d \nto process: %d\n\n", data, pid);

    return 0;
}
