#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "logger.h"
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pid> <log_level>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    LogLevel level;
    int base_signal = SIGRTMIN;

    // Determine log level
    if (strcmp(argv[2], "disable") == 0) {
        level = LOG_LEVEL_DISABLED;
    } else if (strcmp(argv[2], "error") == 0) {
        level = LOG_LEVEL_ERROR;
    } else if (strcmp(argv[2], "warning") == 0) {
        level = LOG_LEVEL_WARNING;
    } else if (strcmp(argv[2], "info") == 0) {
        level = LOG_LEVEL_INFO;
    } else if (strcmp(argv[2], "debug") == 0) {
        level = LOG_LEVEL_DEBUG;
    } else {
        fprintf(stderr, "Invalid log level.\n");
        return 1;
    }

    int sig = base_signal + level;
    if (kill(pid, sig) == -1) {
        perror("kill");
        return 1;
    }

    printf("Log level set to %s for process %ld\n", argv[2], (long)pid);
    return 0;
}
