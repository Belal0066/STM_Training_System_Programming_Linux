#include <stdio.h>
#include <unistd.h>
#include "logger.h"

int main() {
    init_logger(SIGRTMIN, "application.log"); // Log to file

    LOG_DEBUG("Debug message.");
    LOG_INFO("Info message.");
    LOG_WARNING("Warning message.");
    LOG_ERROR("Error message.");

    while (1) {
        sleep(1);
    }

    return 0;
}
