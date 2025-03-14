#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "logger.h"

// Global variables for log level and file descriptor
static volatile sig_atomic_t log_level = LOG_LEVEL_INFO;  // Default log level
static int log_signal_base;                               // Default base signal
static int log_fd = STDERR_FILENO;                        // Default output: stderr

// Safe write function to handle interrupted system calls
static void safe_write(int fd, const char *buf, size_t count) {
    while (count > 0) {
        ssize_t written = write(fd, buf, count);
        if (written == -1) {
            if (errno == EINTR) continue; // Retry if interrupted
            return; // Error
        }
        buf += written;
        count -= written;
    }
}

// Signal handler to change log level dynamically
static void handle_signal(int signo, siginfo_t *si, void *context) {
    int level = signo - log_signal_base;
    if (level >= LOG_LEVEL_DISABLED && level <= LOG_LEVEL_DEBUG) {
        log_level = level;
    }
}

// Initialize the logger and set up signal handlers
void init_logger(int base_signal, const char *log_file) {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;

    // Initialize default base signal
    log_signal_base = SIGRTMIN;

    // Allow custom base signals
    if (base_signal >= SIGRTMIN && base_signal <= SIGRTMAX - LOG_LEVEL_DEBUG) {
        log_signal_base = base_signal;
    }

    // Register signal handlers for all log levels
    for (int i = 0; i <= LOG_LEVEL_DEBUG; i++) {
        if (sigaction(log_signal_base + i, &sa, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }

    // Open log file if specified
    if (log_file) {
        log_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (log_fd == -1) {
            perror("Failed to open log file");
            log_fd = STDERR_FILENO;
        }
    }
}

// Get the current log level
LogLevel get_log_level() {
    return log_level;
}

// Set the log level manually
void set_log_level(LogLevel level) {
    if (level >= LOG_LEVEL_DISABLED && level <= LOG_LEVEL_DEBUG) {
        log_level = level;
    }
}

// Helper function for formatted logging
static void log_message(const char *level_str, const char *format, va_list args) {
    if (log_level == LOG_LEVEL_DISABLED) return;

    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer), "[%s] ", level_str);
    vsnprintf(buffer + len, sizeof(buffer) - len, format, args);
    safe_write(log_fd, buffer, strlen(buffer));
    safe_write(log_fd, "\n", 1);
}

// Logging functions
void LOG_ERROR(const char *format, ...) {
    if (log_level >= LOG_LEVEL_ERROR) {
        va_list args;
        va_start(args, format);
        log_message("ERROR", format, args);
        va_end(args);
    }
}

void LOG_WARNING(const char *format, ...) {
    if (log_level >= LOG_LEVEL_WARNING) {
        va_list args;
        va_start(args, format);
        log_message("WARNING", format, args);
        va_end(args);
    }
}

void LOG_INFO(const char *format, ...) {
    if (log_level >= LOG_LEVEL_INFO) {
        va_list args;
        va_start(args, format);
        log_message("INFO", format, args);
        va_end(args);
    }
}

void LOG_DEBUG(const char *format, ...) {
    if (log_level >= LOG_LEVEL_DEBUG) {
        va_list args;
        va_start(args, format);
        log_message("DEBUG", format, args);
        va_end(args);
    }
}
